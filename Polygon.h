//
// Created by rkindela on 12-04-2018.
//

#ifndef PMTOOL_POLYGON_H
#define PMTOOL_POLYGON_H

#include <vector>
#include <iostream>
#include "utils.h"

using namespace std;

#include "PointRepository.h"
#include "HalfEdge.h"
#include "PointDataGenerator.h"


class Polygon
{
    private:
        HalfEdge* head_edge;
        bool isBad;
    public:
       Polygon();
       Polygon(HalfEdge* hedge);
       ~Polygon();
       void buildFromPoints(const std::vector<int>& pointcloud);
       void buildAsTriangle(int a, int b, int c);
       void builfFromHalfEdge(HalfEdge* half_edge, HalfEdge* end);
       POINT* getPoint(int pos);
       POINT* getPoint(int pos) const;
        int getPointCount() const;
       bool intersect(const Polygon& newPoly);
       int pointIntersections(int npoint);
       bool isRegular();
       bool isConvex();
       bool inside(int npoint);

       bool contains(const Polygon& p) const;
       bool left(int npoint);
       bool rigth(int npoint);
       float area2();
       inline HalfEdge* getHalfEdge() { return head_edge;};
       Face* mute2Face();
       std::vector<Polygon*> splitInConvexes();
       std::vector<Polygon*> delaunayTriangulation(const vector<int>& pointCloud);
       float perimeter();
       vector<Polygon*>* generateTriangulation(vector<int>* P);
       bool isDiagonal(int initialPoint, int finalPoint, const vector<int>& P);
       bool isInCone(int i, int j, const vector<int>& P);
       bool isDiagonalInteriorExterior(int i, int j, const vector<int>& pointCloud);

       std::vector<int> getEarList();
       Polygon* getConvexHull();
       Polygon* giftWrapping(std::vector<int> pointcloud);
       Polygon* grahamScan(std::vector<int> pointcloud);
       Polygon* quickHull(std::vector<int> pointcloud);

    friend std::ostream& operator<<(std::ostream& os, const Polygon* poly);
    friend std::ostream& operator<<(std::ostream& os, const Polygon& poly);

    bool isEqual(Polygon& polyB);
private:
    vector<int> auxQuickHull(std::vector<int> S, int a, int b);
};

#endif //PMTOOL_POLYGON_H
