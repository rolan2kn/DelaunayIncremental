//
// Created by rkindela on 31-03-19.
//

#include "triangulation.h"
#include <map>

Triangulation::Triangulation(int max) : edgesList(), vertexList(), faceList(), maximum(max*10)
{
}

Triangulation::Triangulation(const Triangulation& t) : edgesList(), vertexList(), faceList(), maximum(t.getMaximum())
{
    map<long, HalfEdge*> edgeMapper;
    map<long, Vertex*> vertexMapper;
    map<long, Face*> faceMapper;

    for(auto edge: t.edgesList)                                 // creation and indexing of new HalfEdges
    {
        long eaddr = (long)edge;
        HalfEdge* newedge = new HalfEdge(*edge);
        edgeMapper.insert(std::make_pair(eaddr, newedge));
        this->edgesList.push_back(newedge);
    }
    for(auto edge: t.edgesList)                                // new HalfEdges correction stage
    {
        long eaddr = (long)edge;
        HalfEdge*& newedge = edgeMapper[eaddr];
        long epaddr = (long)newedge->pair;
        HalfEdge*& newpedge = edgeMapper[epaddr];
        newedge->pair = newpedge;
        newpedge->pair = newedge;

        long enaddr = (long)newedge->next;
        HalfEdge*& newnedge = edgeMapper[enaddr];
        newedge->next = newnedge;
    }

    for(auto vertex: t.vertexList)                              // creation and correction of new vertexes
    {
        long vAddress = (long)vertex;
        Vertex* vertexi = new Vertex(*vertex);
        vertexMapper.insert(std::make_pair(vAddress, vertexi));

        long eaddr = (long)vertexi->edge;
        HalfEdge*& edge = edgeMapper[eaddr];
        vertexi->edge = edge;
        edge->vertex = vertexi;
        vertexList.push_back(vertexi);
    }
    for(auto face: t.faceList)                                  // creation and correction of new faces
    {
        long faceAddress = (long)face;
        Face* facei = new Face(*face);
        faceMapper.insert(std::make_pair(faceAddress, facei));

        long eaddr = (long)facei->edge;
        HalfEdge*& edge = edgeMapper[eaddr];
        facei->edge = edge;
        edge->face = facei;
        faceList.push_back(facei);
    }

    // remove mappers info

    edgeMapper.clear();
    vertexMapper.clear();
    faceMapper.clear();
}

Triangulation::~Triangulation()
{
    for(auto edge: edgesList)
    {
        if(edge != nullptr)
            delete edge;
    }

    for(auto vertex: vertexList)
    {
        if(vertex != nullptr)
            delete vertex;
    }

    for(auto face: faceList)
    {
        if(face != nullptr)
            delete face;
    }
}

void Triangulation::reset()
{
    VertexLocator::instance()->reset();
    for(auto edge: edgesList)
    {
        if(edge != nullptr)
            delete edge;
    }
    edgesList.clear();

    for(auto vertex: vertexList)
    {
        if(vertex != nullptr)
            delete vertex;
    }
    vertexList.clear();

    for(auto face: faceList)
    {
        if(face != nullptr)
            delete face;
    }
    faceList.clear();

}

void Triangulation::initData()
{
    POINT* pp0 = new POINT(-maximum, -maximum);
    POINT* pp1 = new POINT(maximum, -maximum);
    POINT* pp2 = new POINT(0, maximum);

    int size = PointRepository::instance()->getPointCount();
    PointRepository::instance()->add(pp0);
    PointRepository::instance()->add(pp1);
    PointRepository::instance()->add(pp2);

    HalfEdge* epp0 = new HalfEdge();
    HalfEdge* epp1 = new HalfEdge();
    HalfEdge* epp2 = new HalfEdge();

    Vertex* v0 = new Vertex();
    v0->pointIndex = size;
    Vertex* v1 = new Vertex();
    v1->pointIndex = size+1;
    Vertex* v2 = new Vertex();
    v2->pointIndex = size+2;

    Face* f0 = new Face();
    f0->edge = epp0;
    epp0->update(v0, epp1, nullptr, f0);
    epp1->update(v1, epp2, nullptr, f0);
    epp2->update(v2, epp0, nullptr, f0);
    edgesList.push_back(epp0);
    edgesList.push_back(epp1);
    edgesList.push_back(epp2);
    vertexList.push_back(v0);
    vertexList.push_back(v1);
    vertexList.push_back(v2);
    faceList.push_back(f0);

    VertexLocator::instance()->add(v0);
    VertexLocator::instance()->add(v1);
    VertexLocator::instance()->add(v2);
}

void Triangulation::buildFromPointCloud(const vector<int>& pointCloud)
{
   this->initData();


   for(int point: pointCloud)
   {
        addPoint(point);
   }
}

void Triangulation::addPoint(int queryPoint)
{
    Vertex* vertex = new Vertex();
    vertex->pointIndex = queryPoint;
    int inside = -1;

    cout <<endl<<"init find face";
    cout.flush();
    Face* face = VertexLocator::instance()->getFaceWithPointInside(vertex, inside);
    cout <<endl<<"end find face";
    cout.flush();

    if (face == nullptr)
    {
        cout <<endl<<PointRepository::instance()->getPoint(queryPoint)<<" con indice "<<queryPoint<<" quedo fuera de todos los faces";
        cout.flush();
        face = this->getFaceWithPoint(queryPoint, inside);
        if (face == nullptr)
            return;
    }

    int i = vertexList.size();

    if (inside > 0)
    {
        cout <<endl<<"init split";
        cout.flush();
        face->splitFace(queryPoint, faceList, edgesList, vertexList);
        cout <<endl<<"end split";
        cout.flush();
    }
    else if (inside == 0)        // an edge holding query point
    {
        cout <<endl<<"init split adjacent";
        cout.flush();
        face->splitAdjacentFaces(queryPoint, faceList, edgesList, vertexList);
        cout <<endl<<"endl split adjacent";
        cout.flush();
    }

    int size = vertexList.size();
    for (; i < size; i++)
    {
        VertexLocator::instance()->add(vertexList[i]);
    }

}

Face* Triangulation::getFaceWithPoint(int queryPoint, int &inside)
{
    inside = -1;
    for(auto face: faceList)
    {
        inside = face->inside(queryPoint);
        if (inside != -1)
            return face;
    }
    return nullptr;
}

void Triangulation::legalizeALL()
{
    for(auto edge: edgesList)
    {
        edge->legalize();
    }
}

Triangulation* Triangulation::clone()
{
    return new Triangulation(*this);
}
