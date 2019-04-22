//
// Created by rkindela on 12-04-2018.
//

#ifndef PMTOOL_EDGE_H
#define PMTOOL_EDGE_H

#include "PointRepository.h"
#include "Vector3D.h"

struct  Vertex;
struct Face;



struct HalfEdge {
    Vertex* vertex;
    HalfEdge* next;
    HalfEdge* pair;
    Face* face;

    HalfEdge();
    ~HalfEdge();
    HalfEdge(const HalfEdge& half_edge);
    Vector3D* getOrientedVector3D();
    int getEdgeSize();
    int getDistance(HalfEdge* otherEdge);
    void update(Vertex* vertex, HalfEdge* _next, HalfEdge* _pair, Face* face);
    bool areIntersected(HalfEdge* otherEdge);
    void legalize();
    friend std::ostream& operator<<(std::ostream& os, const HalfEdge& e);
};

struct Vertex
{
    int pointIndex; // Represent the point index in PointRepository
    HalfEdge* edge;

    Vertex();
    Vertex(const Vertex& vertex);
    friend std::ostream& operator<<(std::ostream& os, const Vertex& e);
    bool operator ==(const Vertex& e);
    vector<Face*>* getAdjacentFaces();
    vector<Vertex*>* getLink();
    inline ~Vertex(){ this->edge = nullptr;} // Edges will be destroyed by somebody else
    Vertex* clone() { return new Vertex(*this);}
};
struct FaceColor
{
    float r;
    float g;
    float b;
    bool wasAssigned;
    FaceColor(float _r= 0.0, float _g= 0.0, float _b= 0.0): r(_r), g(_g), b(_b), wasAssigned(false){}
    void operator=(const FaceColor& c)
    {
        r = c.r;
        g = c.g;
        b = c.b;
        wasAssigned = true;
    }
};

struct Face
{
    HalfEdge* edge;
    FaceColor color;

    Face();
    Face(const Face& face);
    friend std::ostream& operator<<(std::ostream& os, const Face& e);
    bool operator ==(const Face& face);
    int getVertexesCount() const;
    vector<int>* getVertexesIndices();
    HalfEdge* findVertex(Vertex* v) const;
    int inside(int pointIndex) const;
    bool inCircumcircle(int queryPoint);
    HalfEdge* buildFromPoints(const vector<int>& pointCloud);
    bool inLeft(Face* nFace);
    bool areIntersected(Face* nFace);
    bool intersectWithSegment(int p0, int p1, int SMALL_NUM = -1);
    bool inConterClockWise();
    HalfEdge* getEdgeHoldingPoint(int pointIndex) const;
    Vector3D* getNormal();
    void splitFace(int pointIndex, vector<Face*>& faces, vector<HalfEdge*>& edges, vector<Vertex*>& vertexes);
    void splitAdjacentFaces(int pointIndex, vector<Face*>& faces, vector<HalfEdge*>& edges, vector<Vertex*>& vertexes);
    inline ~Face() { edge = nullptr;}   // Edges will be destroyed by somebody else
    void setColor(const FaceColor& color) {this->color = color;}
};

#endif //PMTOOL_EDGE_H
