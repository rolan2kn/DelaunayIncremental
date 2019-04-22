//
// Created by rkindela on 12-04-2018.
//

#ifndef PMTOOL_POINTREPOSITORY_H
#define PMTOOL_POINTREPOSITORY_H

#include "Point.h"
#include <iostream>
#include <vector>
#include "Vector3D.h"


using namespace std;

class PointRepository {
private:
    static PointRepository* pointRepository;
    std::vector<POINT*> pointList;

    PointRepository();
public:
    static PointRepository* instance();
    void add(POINT* p);
    POINT* getPoint(int pointPosition);
    Vector3D* getVector3dFromPoints(int pointA, int pointB);
    float area2(int pointA, int pointB, int pointC);
    int areaSign(int pointA, int pointB, int pointC);
    int generalArea2(const vector<int> &pointcloud);
    int volume6(int pointA, int pointB, int pointC, int pointD);
    int middleAngleCos(int pointA, int pointB, int pointC);
    int squareMagnitude(int pointA, int pointB);
    float angleCos(int pointA, int pointB, int pointC, int pointD);
    bool areIntersected(int pointA, int pointB, int pointC, int pointD);
    bool inCircumcircle(int pointA, int pointB, int pointC, int pointD);
    bool onSegment(int p, int q, int r);
    int computeLexicographicOrder(int a, int b);
    int getPointCount() const;
    int getLowerMostXCoord(const std::vector<int> &pointCloud);
    int getLowerMostYCoord(const std::vector<int> &pointCloud);
    int getLowerMostZCoord(const std::vector<int> &pointCloud);
    int getUpperMostXCoord(const std::vector<int> &pointCloud);
    int getUpperMostYCoord(const std::vector<int> &pointCloud);
    int getUpperMostZCoord(const std::vector<int> &pointCloud);
    int getMaximumAreaCoord(const std::vector<int> &pointCloud, int a, int b);
    int getCentralPointPosition(const std::vector<int> &pointCloud, int n);
    std::vector<int> getSortedPolarAngles(std::vector<int>& pointCloud);
    std::vector<int> getSortedPoints(std::vector<int>& pointCloud);

    std::vector<int> getIndexes();
    void remove(int pos);
    void clear();
    int pivotePosition;

    ~PointRepository();
    friend std::ostream& operator<<(std::ostream& os, const PointRepository* pr);
};


#endif //PMTOOL_POINTREPOSITORY_H
