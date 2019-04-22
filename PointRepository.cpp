//
// Created by rkindela on 12-04-2018.
//

#include "PointRepository.h"
#include "Vector3D.h"
#include "utils.h"
#include "Matrix.h"


PointRepository* PointRepository::pointRepository = nullptr;

std::ostream& operator<<(std::ostream& os, const PointRepository* pr)
{
    int count = pr->getPointCount();
    long address = (long)pr;
    os<<"POINT_REPOSITORY address: "<<address<<endl;
    os<<"point count: "<<count<<endl;

    for (auto it : pr->pointList)
    {
        os<<"POINT: "<<*it<<endl;
    }

    return os;
}

PointRepository::PointRepository() : pointList()
{

}

void PointRepository::clear() {
    if (pointRepository == nullptr)
        return;

    int count = pointList.size();
    for (int i = 0; i < count; i++)
    {
        if (pointList[i] != nullptr)
            delete pointList[i];
        pointList[i] = nullptr;
    }
    pointList.clear();
    pointRepository = nullptr;
}

PointRepository::~PointRepository()
{
    if (pointRepository == nullptr)
        return;

    int count = pointList.size();
    for (int i = 0; i < count;i++)
        delete pointList[i];
    pointList.clear();
    pointRepository = nullptr;
}

PointRepository* PointRepository::instance()
{
    if (pointRepository == nullptr)
        pointRepository = new PointRepository();
    return pointRepository;
}

void PointRepository::add(POINT* p)
{
    pointList.push_back(p);
}

POINT* PointRepository::getPoint(int pointPosition)
{
    if (pointPosition >= 0 && pointPosition < pointList.size())
        return pointList[pointPosition];
    return nullptr;
}

float PointRepository::area2(int pointA, int pointB, int pointC)
{
    POINT* pA = this->getPoint(pointA);
    POINT* pB = this->getPoint(pointB);
    POINT* pC = this->getPoint(pointC);

    if (pA == nullptr || pB == nullptr || pC == nullptr)
        return 0;

    Vector3D A(*pA, *pB);
    Vector3D B(*pA, *pC);

    return (A.x*B.y - A.y*B.x);
}

int PointRepository::areaSign(int pointA, int pointB, int pointC)
{
    float area = this->area2(pointA, pointB, pointC);

    if(almost_equal(area, 0.00f))
        return 0;
    return (area > 0) ? 1 : -1;
}

int PointRepository::volume6(int pointA, int pointB, int pointC, int pointD)
{
    /**
     *          |pAx pAy pAz 1|
     * (1/3!) * |pBx pBy pBz 1| = (pB-pA)x(pC-pA)*(pD-pA)
     *          |pCx pCy pCz 1|
     *          |pDx pDy pDz 1|*/

    POINT* pA = this->getPoint(pointA);
    POINT* pB = this->getPoint(pointB);
    POINT* pC = this->getPoint(pointC);
    POINT* pD = this->getPoint(pointD);

    if (pA == nullptr || pB == nullptr || pC == nullptr || pD == nullptr)
        return 0;

    Vector3D v1(*pA, *pB);
    Vector3D v2(*pA, *pC);
    Vector3D v3(*pA, *pD);

    return v1.cross(v2).dot(v3);
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool PointRepository::onSegment(int _p, int _q, int _r)
{
    POINT& p = *this->getPoint(_p);
    POINT& q = *this->getPoint(_q);
    POINT& r = *this->getPoint(_r);

    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;

    return false;
}

bool PointRepository::inCircumcircle(int pointA, int pointB, int pointC, int pointD)
{
    /**
     * |Ax Ay Ax^2+Ay^2 1|
     * |Bx By Bx^2+By^2 1| > 0
     * |Cx Cy Cx^2+Cy^2 1|
     * |Dx Dy Dx^2+Dy^2 1|*/

    POINT* pA = this->getPoint(pointA);
    POINT* pB = this->getPoint(pointB);
    POINT* pC = this->getPoint(pointC);
    POINT* pD = this->getPoint(pointD);

    if (pA == nullptr || pB == nullptr || pC == nullptr || pD == nullptr)
        return false;

    POINT& ad = (*pA) - (*pD);
    POINT& bd = (*pB) - (*pD);
    POINT& cd = (*pC) - (*pD);
    float _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                             bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                             cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    if (almost_equal(_det, 0.000f))
        return true;

    return (_det > 0);
}

bool PointRepository::areIntersected(int p1, int q1, int p2, int q2)
{
    int o1 = areaSign(p1, q1, p2);
    int o2 = areaSign(p1, q1, q2);
    int o3 = areaSign(p2, q2, p1);
    int o4 = areaSign(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

int PointRepository::squareMagnitude(int pointA, int pointB)
{
    POINT* pA = this->getPoint(pointA);
    POINT* pB = this->getPoint(pointB);

    if (pA == nullptr || pB == nullptr)
        return 0;

    return pA->squareDistance(*pB);
}

int PointRepository::middleAngleCos(int pointA, int pointB, int pointC)
{
    POINT* pA = this->getPoint(pointA);
    POINT* pB = this->getPoint(pointB);
    POINT* pC = this->getPoint(pointC);

    if (pA == nullptr || pB == nullptr || pC == nullptr)
        return 0;

    Vector3D v1(*pA, *pB);
    Vector3D v2(*pA, *pC);

    return v1.angleCos(v2);
}

float PointRepository::angleCos(int pointA, int pointB, int pointC, int pointD)
{
    POINT* pA = this->getPoint(pointA);
    POINT* pB = this->getPoint(pointB);
    POINT* pC = this->getPoint(pointC);
    POINT* pD = this->getPoint(pointD);

    if (pA == nullptr || pB == nullptr || pC == nullptr || pD == nullptr)
        return 0;

    Vector3D v1(*pA, *pB);
    Vector3D v2(*pC, *pD);

    return v1.angleCos(v2);
}

int PointRepository::getPointCount() const
{
    return pointList.size();
}

int PointRepository::getCentralPointPosition(const std::vector<int> &pointCloud, int p)
{
    int count = pointList.size();
    if (p < count)
        count = p;

    int x = 0, y = 0, z = 0;
    for (int i = 0; i < count; i++)
    {
        int idx = pointCloud[i];
        POINT *initialPoint = this->getPoint(idx);

        x += initialPoint->x;
        y += initialPoint->y;
        z += initialPoint->z;
    }
    if (count == 0) // for avoid division by zero exception
        count = 1;

    POINT *center = new POINT(x / count, y / count, z / count);

    PointRepository::instance()->add(center); // we add the center point to repository for guarantee indexing

    int pointPosition = PointRepository::instance()->getPointCount()-1;

    return pointPosition;
}

int PointRepository::getLowerMostXCoord(const std::vector<int> &pointCloud)
{
    int x = 214748364, count = pointCloud.size(), idxleftPoint = 0;

    for (int idx = 0; idx < count; idx++)
    {
        int i = pointCloud[idx];
        POINT* tempPoint = this->getPoint(i);

        if (x > tempPoint->x)
        {
            idxleftPoint = i;
            x = tempPoint->x;
        }
        else if(x == tempPoint->x)
        {
            POINT* currentPoint = this->getPoint(idxleftPoint);
            if (currentPoint != nullptr)
            {
                if (tempPoint->y > currentPoint->y)
                    idxleftPoint = idx;
                if (tempPoint->z > currentPoint->z)
                    idxleftPoint = idx;
            }
        }
    }

    return idxleftPoint;
}

int PointRepository::getLowerMostYCoord(const std::vector<int> &pointCloud)
{
    int y = 214748364, count = pointCloud.size(), idxleftPoint = 0;

    for (int i = 0; i < count; i++)
    {
        int idx = pointCloud[i];
        POINT* tempPoint = this->getPoint(idx);

        if (y > tempPoint->y)
        {
            idxleftPoint = idx;
            y = tempPoint->y;
        }
        else if(y == tempPoint->y)
        {
            POINT* currentPoint = this->getPoint(idxleftPoint);
            if (currentPoint != nullptr)
            {
                if (tempPoint->x > currentPoint->x)
                    idxleftPoint = idx;
                if (tempPoint->z > currentPoint->z)
                    idxleftPoint = idx;
            }
        }
    }

    return idxleftPoint;
}

int PointRepository::getLowerMostZCoord(const std::vector<int> &pointCloud)
{
    int z = 214748364, count = pointCloud.size(), idxleftPoint = 0;

    for (int i = 0; i < count; i++)
    {
        int idx = pointCloud[i];
        POINT* tempPoint = this->getPoint(idx);

        if (z > tempPoint->z)
        {
            idxleftPoint = idx;
            z = tempPoint->z;
        }
        else if(z == tempPoint->z)
        {
            POINT* currentPoint = this->getPoint(idxleftPoint);
            if (currentPoint != nullptr)
            {
                if (tempPoint->x > currentPoint->x)
                    idxleftPoint = idx;
                if (tempPoint->y > currentPoint->y)
                    idxleftPoint = idx;
            }
        }
    }

    return idxleftPoint;
}

int PointRepository::getUpperMostXCoord(const std::vector<int> &pointCloud)
{
    int x = -214748364, count = pointCloud.size(), idxleftPoint = 0;

    for (int i = 0; i < count; i++)
    {
        int idx = pointCloud[i];
        POINT* tempPoint = this->getPoint(idx);
        if (x < tempPoint->x)
        {
            idxleftPoint = idx;
            x = tempPoint->x;
        }
        else if(x == tempPoint->x)
        {
            POINT* currentPoint = this->getPoint(idxleftPoint);
            if (currentPoint != nullptr)
            {
                if (tempPoint->y < currentPoint->y)
                    idxleftPoint = idx;
                if (tempPoint->z < currentPoint->z)
                    idxleftPoint = idx;
            }
        }
    }

    return idxleftPoint;
}

int PointRepository::getUpperMostYCoord(const std::vector<int> &pointCloud)
{
    int y = -214748364, count = pointCloud.size(), idxleftPoint = 0;

    for (int i = 0; i < count; i++)
    {
        int idx = pointCloud[i];
        POINT* tempPoint = this->getPoint(idx);

        if (y < tempPoint->y)
        {
            idxleftPoint = idx;
            y = tempPoint->y;
        }
        else if(y == tempPoint->y)
        {
            POINT* currentPoint = this->getPoint(idxleftPoint);
            if (currentPoint != nullptr)
            {
                if (tempPoint->x < currentPoint->x)
                    idxleftPoint = idx;
                if (tempPoint->z < currentPoint->z)
                    idxleftPoint = idx;
            }
        }
    }

    return idxleftPoint;
}

int PointRepository::getUpperMostZCoord(const std::vector<int> &pointCloud)
{
    int z = -214748364, count = pointCloud.size(), idxleftPoint = 0;

    for (int i = 0; i < count; i++)
    {
        int idx = pointCloud[i];
        POINT* tempPoint = this->getPoint(idx);

        if (z < tempPoint->z)
        {
            idxleftPoint = idx;
            z = tempPoint->z;
        }
        else if(z == tempPoint->z)
        {
            POINT* currentPoint = this->getPoint(idxleftPoint);
            if (currentPoint != nullptr)
            {
                if (tempPoint->x < currentPoint->x)
                    idxleftPoint = idx;
                if (tempPoint->y < currentPoint->y)
                    idxleftPoint = idx;
            }
        }
    }

    return idxleftPoint;
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void *vp1, const void *vp2)
{
    int p0 = PointRepository::instance()->pivotePosition;
    int p1 = *(int*)vp1;
    int p2 = *(int*)vp2;

    // Find orientation
    int o = PointRepository::instance()->areaSign(p0, p1, p2);
    POINT pp0 = *PointRepository::instance()->getPoint(p0);
    POINT pp1 = *PointRepository::instance()->getPoint(p1);
    POINT pp2 = *PointRepository::instance()->getPoint(p2);

    if (o == 0) {
        return (pp0.squareDistance(pp2) >= pp0.squareDistance(pp1)) ? -1 : 1;
    }

    return (o > 0) ? -1 : 1;
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare2(const void *vp1, const void *vp2)
{
    int p1 = *(int*)vp1;
    int p2 = *(int*)vp2;

    // Find orientation
    return PointRepository::instance()->computeLexicographicOrder(p1, p2);
}

int PointRepository::computeLexicographicOrder( int a, int b)
{
    POINT* p1 = PointRepository::instance()->getPoint(a);
    POINT* p2 = PointRepository::instance()->getPoint(b);
    // test the x-coord first
    if (p1->x > p2->x) return 1;
    if (p1->x < p2->x) return (-1);
    // and test the y-coord second
    if (p1->y > p2->y) return 1;
    if (p1->y < p2->y) return (-1);

    if (p1->z > p2->z) return 1;
    if (p1->z < p2->z) return (-1);
    // when you exclude all other possibilities, what remains is...
    return 0;  // they are the same point
}


std::vector<int> PointRepository::getSortedPolarAngles(std::vector<int>& pointCloud)
{
    /**
     *
     * This method sort all points according its polar angles with point P0.
     * */

    int count = pointCloud.size() - 1;

    pivotePosition = pointCloud[0];

    qsort(&pointCloud[1], count, sizeof(int), compare);

    return pointCloud;
}

std::vector<int> PointRepository::getSortedPoints(std::vector<int>& pointCloud)
{
    /**
     *
     * This method sort all points according its polar angles with point P0.
     * */

    int count = pointCloud.size();


    qsort(&pointCloud[0], count, sizeof(int), compare2);

    return pointCloud;
}

std::vector<int> PointRepository::getIndexes()
{
    std::vector<int>* indexes = new std::vector<int>();

    int count = pointList.size();

    for (int i = 0; i < count; i++)
    {
        indexes->push_back(i);
    }
    return *indexes;
}

int PointRepository::generalArea2(const vector<int>& pointcloud)
{
    /**
     * The general formulation for compute the area of any simple polygon is using the Gauss Determinant
     *
     * **/
    bool inLeft = true;
    int last = pointcloud.size(), i = 0;
    int area = 0;

    if (last < 3)       // if polygon vertex count is less than three
        return 0;

    while(inLeft && i < last)
    {
        // we check if generalArea2 >= 0 to include the colinear case

        int pos = pointcloud[i];
        int pos2 = pointcloud[(i+1)%last];
        POINT* pi = this->getPoint(pos);
        POINT* pi1 = this->getPoint(pos2); // (i+1)%last is for check at the end the last point with first ((last-1)+1)%last = 0

        area += pi->x*pi1->y - pi1->x*pi->y;
        ++i;
    }
    return area;
}
Vector3D* PointRepository::getVector3dFromPoints(int pointA, int pointB)
{
    POINT *a = new POINT(), *b = new POINT();


  if (pointA != -1)
      a = getPoint(pointA);
  if (pointB != -1)
      b = getPoint(pointB);

  return new Vector3D(*a, *b);
}

int PointRepository::getMaximumAreaCoord(const std::vector<int> &pointCloud, int a, int b)
{
    int max_area = -214748364, count = pointCloud.size(), idxmaxPoint = 0;

    for (int i = 0; i < count; i++)
    {
        int idx = pointCloud[i];

        int area = this->area2(a, b, idx);

        if (max_area < area)
        {
            idxmaxPoint = idx;
            max_area = area;
        }
    }

    return idxmaxPoint;
}

void PointRepository::remove(int pos)
{
    if (pos < 0 || pos > pointList.size())
        return;
    pointList.erase(pointList.begin() + pos);
}