//
// Created by rkindela on 31-03-19.
//

#ifndef PMTOOL_TRIANGULATION_H
#define PMTOOL_TRIANGULATION_H

#include "vertex_avl_tree.h"
#include "HalfEdge.h"
#include <vector>


using namespace std;

class Triangulation {

private:
    vector<HalfEdge*> edgesList;
    vector<Vertex*> vertexList;
    vector<Face*> faceList;
    int maximum;
public:
    Triangulation(int max_value);
    Triangulation(const Triangulation& t);
    ~Triangulation();

    void buildFromPointCloud(const vector<int>& pointCloud);

    void addPoint(int queryPoint);
    void initData();

    inline int getFacesCount() const { return faceList.size(); }
    inline int getHalfEdgesCount() const {return edgesList.size();}
    inline int getVertexesCount() const {return vertexList.size();}
    inline int getMaximum() const {return maximum;}
    Face* getFace(int pos) const { return faceList[pos]; }
    Face* getFaceWithPoint(int queryPoint, int &inside);
    HalfEdge* getHalfEdge(int pos) const {return edgesList[pos]; }
    Vertex* getVertex(int pos) const {return vertexList[pos];}
    void legalizeALL();
    void reset();

    Triangulation* clone();
};


#endif //PMTOOL_TRIANGULATION_H
