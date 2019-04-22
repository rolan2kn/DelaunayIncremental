//
// Created by rkindela on 27-05-18.
//

#include "PointDataGenerator.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Vector3D.h"

int PointDataGenerator::randomInt(int min, int max)
{
    srand((unsigned)clock());  /* Seed the */
    max = max - min + 1;
    max = (max == 0) ? 1 : max;
    return min + (rand() % max);  /* Generate a random integer */
}

POINT* PointDataGenerator::randomPoint(int min, int max)
{
    int x = this->randomInt(min, max);
    int y = this->randomInt(min, max);
    int z = (dimension == 3) ? this->randomInt(min, max) : 0;

    return new POINT(x, y, z);
}

void PointDataGenerator::randomPointCloud(int pointCount)
{
    for (int i = 0; i < pointCount;i++)
    {
        POINT* p = this->randomPoint(-this->maximum, this->maximum);
        PointRepository::instance()->add(p);
    }
}

void PointDataGenerator::randomPointGrid(int pointCountDegree)
{
    POINT origin(-this->maximum, this->maximum);
    int size = pointCountDegree;

    int total_size = pointCountDegree*pointCountDegree;

    for (int i = 0; i < total_size;i++)
    {
        int row = i / pointCountDegree;
        int col = i % pointCountDegree;

        POINT* p = new POINT(origin.x + col*size, origin.y - row*size);
        if (p->x >= -this->maximum and p->x <= this->maximum && p->y >= -maximum && p->y <= maximum)
            PointRepository::instance()->add(p);
    }
}

void PointDataGenerator::randomPointCloudWithPercentInside(int pointCount, float percent)
{
    int radius = this->maximum;
    int insideAmount = (int)(percent * pointCount * 0.01);
    int sizeHull = pointCount - insideAmount;

    std::vector<POINT*> onhull = generateOnHullPoints(radius, sizeHull);
    std::vector<POINT*> insidehull = generateInsideHullPoints(radius, insideAmount);


    /***
     * Distribute inside and on hull points randomly
     * */

    int randomSize = this->randomInt(5, 17);
    int insideIter = 0;
    int onHullIter = 0;

    while(PointRepository::instance()->getPointCount() < pointCount)
    {
        if (onHullIter < sizeHull)
        {
            PointRepository::instance()->add( onhull[onHullIter++] );
        }
        else if (insideIter < insideAmount)
        {
            PointRepository::instance()->add( insidehull[insideIter++] );
        }
    }

}

std::vector<POINT*> PointDataGenerator::generateOnHullPoints(int radius, int hullSize)
{
    /**
     * This method generate a point cloud with pointCount points
     * with a percent of those points belonging hull
     *
     * we use the circunference equation for generate the hull points
     *
     * r = (x - a)*(x - a) + (y - b)*(y - b) + (z - c)*(z - c) and we asume the a, b, c as center of axis coords
     * if any point p has distance r from de center then this point belongs o the hull
     * and inside in otherwise. Note c is used only for 3 dimension
     * */

    Vector3D v0(POINT(radius, 0));
    hullSize = (hullSize > 0) ? hullSize : 1;

    /**
     * Generate the hull points
     * **/
    std::vector<POINT*> onHullPoints;
    onHullPoints.push_back(&v0.asPoint());

    while(onHullPoints.size() < hullSize)
    {
        float radAngle = ((M_PI*2)/hullSize)*onHullPoints.size();
        float cosA = cos(radAngle), sinA = sin(radAngle);
        Vector3D vi = Vector3D(v0.x*cosA + v0.y*sinA, v0.x*sinA + v0.y*cosA);
        onHullPoints.push_back(&vi.asPoint());
    }

    return onHullPoints;
}

std::vector<POINT*> PointDataGenerator::generateInsideHullPoints(int radius, int insideAmount)
{
    /**
     * Generate the points inside hull
     *
     *
     * This method generate a point cloud with insideAount points
     * inside convex hull
     *
     * we use the circunference equation for generate the hull points
     *
     * r = (x - a)*(x - a) + (y - b)*(y - b) and we asume the a, b as center of axis coords
     * if any point p has distance r from de center then this point belongs o the hull
     * and inside in otherwise
     *
     * **/

    std::vector<POINT*> insidePoints;
    POINT center(0, 0);
    int factor = radius/10;
    int squareRadius = (radius-factor)*(radius-factor);

    while(insidePoints.size() < insideAmount)
    {
        POINT *p = this->randomPoint(-radius, radius);
        int distance = p->squareDistance(center);

        if (distance < squareRadius)            // only add point inside hull
        {
            insidePoints.push_back(p);
        }
        else                                    // delete point in otherwise
            delete p;
    }
    return insidePoints;
}

void PointDataGenerator::randomPointCloudWithPercentOnHull(int pointCount, float percent)
{
    int radius = this->maximum;
    int sizeHull = (int)(percent * pointCount * 0.01);

    int insideAmount = pointCount - sizeHull;

    std::vector<POINT*> onhull = generateOnHullPoints(radius, sizeHull);
    std::vector<POINT*> insidehull = generateInsideHullPoints(radius, insideAmount);

    /***
     * Distribute inside and on hull points randomly
     * */

    int insideIter = 0;
    int onHullIter = 0;

    while(PointRepository::instance()->getPointCount() < pointCount)
    {
        if (onHullIter < sizeHull)
        {
            PointRepository::instance()->add( onhull[onHullIter++] );
        }
        else if (insideIter < insideAmount)
        {
            PointRepository::instance()->add( insidehull[insideIter++] );
        }
    }
}

void PointDataGenerator::randomPointCloudWithPercentOn3DHull(int pointCount, float percent)
{
    int radius = this->maximum;
    int sizeHull = (int)(percent * pointCount * 0.01);

    int insideAmount = pointCount - sizeHull;

    std::vector<POINT*> onhull = generateOnHullPoints(radius, sizeHull);
    std::vector<POINT*> insidehull = generateInsideHullPoints(radius, insideAmount);


    /***
     * Distribute inside and on hull points randomly
     * */

    int randomSize = this->randomInt(5, maximum);
    bool* inside = new bool[randomSize];                           // for distribute points
    int insideIter = 0;
    int onHullIter = 0;

    for (int i = 0; i < randomSize; i++)
    {
        inside[i] = this->randomInt(1, 5) % 2 != 0;
    }

    while(PointRepository::instance()->getPointCount() < pointCount)
    {
        int idx = this->randomInt(0, randomSize-1); // we determine randomly if we need a point inside or in hull

        if (inside[idx] && onHullIter < sizeHull)
        {
            PointRepository::instance()->add( onhull[onHullIter++] );
        }
        else if (insideIter < insideAmount)
        {
            PointRepository::instance()->add( insidehull[insideIter++] );
        }
    }

    delete [] inside;
}

vector<int>* PointDataGenerator::getRandomPermutation(const vector<int>& pointCloud, int exceptionPoint)
{
    int size = pointCloud.size()-1;
    int cursor = 0;
    vector<int>* permutation = new vector<int>();
    while(cursor < size)
    {
        int realIdx;
        bool valid = false;
        while(!valid) {
            int randomP = this->randomInt(0, size - 1);
            if (randomP != exceptionPoint)
            {
                int i = 0, psize = permutation->size();
                realIdx = pointCloud[randomP];
                while (randomP == exceptionPoint || (i < psize && realIdx != (*permutation)[i++]));
                valid = (i == psize);
            }
        }
        permutation->push_back(realIdx);
        ++cursor;
    }
    return permutation;
}
