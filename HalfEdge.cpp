//
// Created by rkindela on 12-04-2018.
//

#include "HalfEdge.h"
#include "Vector3D.h"


std::ostream& operator<<(std::ostream& os, const HalfEdge& e) {
    os << "HALF_EDGE address: " << long(&e) << endl;
    if (e.vertex != nullptr)
        os << "HalfEdge: { " << *e.vertex << endl;
    else
        os << "HalfEdge:{ vertex: nullptr" << endl;

    /**
     * Vertex* vertex;
    HalfEdge* next;
    HalfEdge* pair;
     Face* face;
     */

    if (e.next != nullptr)
        os << ", NEXT: " << (long)(e.next) << endl;
    else
        os << ", NEXT: nullptr" << endl;

    if (e.pair != nullptr)
        os << ", PAIR: " << (long)(e.pair) << endl;
    else
        os << ", PAIR: nullptr" << endl;

    if (e.face != nullptr)
        os << ", " << (*e.face) << endl;
    else
        os << ", FACE: nullptr" << endl;

    os << "}\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vertex& e)
{
    POINT* point = PointRepository::instance()->getPoint(e.pointIndex);
    if (point != nullptr)
        os<<" VERTEX: { "<<point;
    else
        os << "VERTEX: POINT: nullptr";

    if (e.edge != nullptr)
        os<<", EDGE: "<<(long)(e.edge);
    else
        os<<", EDGE: nullptr";
    os <<"}\n";

    return os;
}

std::ostream& operator<<(std::ostream& os, const Face& e)
{
    if (e.edge != nullptr)
        os<<"FACE: { EDGE: "<<(long)(e.edge);
    else
        os<<"FACE: { EDGE: nullptr";
    os <<"}\n";

    return os;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 * HalfEdge Struct Implementation
 */

HalfEdge::HalfEdge(): vertex(nullptr), next(nullptr), pair(nullptr), face(nullptr)
{

}

HalfEdge::~HalfEdge()
{
    vertex = nullptr;   // Vertexes will be destroyed by somebody else
    next = nullptr;     // Edges will be destroyed by somebody else
    pair = nullptr;     // idem
    face = nullptr;     // Faces will be destroyed by somebody else
}

HalfEdge::HalfEdge(const HalfEdge& half_edge): vertex(half_edge.vertex), next(half_edge.next), pair(half_edge.pair), face(half_edge.face)
{

}

int HalfEdge::getDistance(HalfEdge* otherEdge)
{
    HalfEdge* cursor = this;
    int count = 0;

    if (otherEdge == nullptr)
        return -1;

    if(otherEdge == cursor)
        return 0;

    do{
        ++count;
        cursor = cursor->next;
    }while(cursor != this && cursor != otherEdge);

    if(cursor == otherEdge)
        return count;

    return -1;
}

int HalfEdge::getEdgeSize()
{
    int pa = -1;
    int pb = -1;
    if (next != nullptr && next->vertex != nullptr)
        pb = next->vertex->pointIndex;
    if (vertex != nullptr)
        pa = vertex->pointIndex;
    return PointRepository::instance()->squareMagnitude(pa, pb);
}

Vector3D* HalfEdge::getOrientedVector3D()
{
    int pa = -1;
    int pb = -1;
    if (next != nullptr && next->vertex != nullptr)
        pb = next->vertex->pointIndex;
    if (vertex != nullptr)
        pa = vertex->pointIndex;

    return PointRepository::instance()->getVector3dFromPoints(pa, pb);
}

bool HalfEdge::areIntersected(HalfEdge* otherEdge)
{
    if(otherEdge == nullptr ||
       otherEdge->next == nullptr ||
       this->next == nullptr)
        return false;
    if(otherEdge->vertex == nullptr ||
       otherEdge->next->vertex == nullptr ||
        this->vertex == nullptr ||
        this->next->vertex == nullptr)
        return false;

    int p1 = this->vertex->pointIndex;
    int q1 = this->next->vertex->pointIndex;
    int p2 = otherEdge->vertex->pointIndex;
    int q2 = otherEdge->next->vertex->pointIndex;

    return PointRepository::instance()->areIntersected(p1, q1, p2, q2);

}

void HalfEdge::update(Vertex* vertex, HalfEdge* _next, HalfEdge* _pair, Face* face)
{
    this->vertex = vertex;
    this->vertex->edge = this;
    this->next = _next;
    this->pair = _pair;
    this->face = face;
}

void HalfEdge::legalize()
{
    if (this->pair == nullptr || this->face == nullptr)
        return ;

    HalfEdge* currentEdge = this;
    HalfEdge* next = currentEdge->next;
    HalfEdge* nextnext = next->next;

    HalfEdge* currentPair = currentEdge->pair;
    if (currentPair == nullptr )
        return;

    HalfEdge* pnext = currentPair->next;
    if (pnext == nullptr)
        return;

    HalfEdge* pnextnext = pnext->next;

    Face* f1 = pnext->face;
    if (pnextnext == nullptr || f1 == nullptr)
        return;

    if (!face->inCircumcircle(pnextnext->vertex->pointIndex) && !f1->inCircumcircle(nextnext->vertex->pointIndex))
        return;
//    int currentSize = this->getEdgeSize();
//    int newSize = PointRepository::instance()->squareMagnitude(pnextnext->vertex->pointIndex, nextnext->vertex->pointIndex);
//    if(newSize >= currentSize)
//        return;

    this->update(pnextnext->vertex, nextnext, currentPair, face);
    nextnext->update(nextnext->vertex, pnext, nextnext->pair, face);
    pnext->update(pnext->vertex, this, pnext->pair, face);

    currentPair->update(nextnext->vertex, pnextnext, this, f1);
    pnextnext->update(pnextnext->vertex, next, pnextnext->pair, f1);
    next->update(next->vertex, currentPair, next->pair, f1);
    f1->edge = pnextnext;
    face->edge = nextnext;

    cout << endl << "init legalize netx ";
    cout.flush();
    next->legalize();
    cout << endl << "end legalize netx ";
    cout << endl << "init legalize netnextx ";
    cout.flush();
    nextnext->legalize();
    cout << endl << "end legalize netnextx ";
    cout.flush();

}


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 * Vertex Struct Implementation
 */

Vertex::Vertex():pointIndex(-1), edge(nullptr)
{

}

Vertex::Vertex(const Vertex &vertex): pointIndex(vertex.pointIndex), edge(vertex.edge)
{

}

bool Vertex::operator ==(const Vertex& e)
{
    return (pointIndex == e.pointIndex);
}

vector<Face*>* Vertex::getAdjacentFaces()
{
    vector<Face*>* adjacentFaces = new vector<Face*>();

    if(edge == nullptr)
        return adjacentFaces;

    HalfEdge* cursor = edge;
    do
        {
            adjacentFaces->push_back(cursor->face);
            if (cursor->pair != nullptr)
            cursor = cursor->pair->next;
        }while(cursor != edge && cursor != nullptr);

    return adjacentFaces;
}

vector<Vertex*>* Vertex::getLink()
{
    vector<Vertex*>* oppositeVertexes = new vector<Vertex*>();

    if(edge == nullptr)
        return oppositeVertexes;

    HalfEdge* cursor = edge;
    do
    {
        HalfEdge* temp = cursor;
        do{
            if(temp->vertex->pointIndex != pointIndex)
            {
                oppositeVertexes->push_back(temp->vertex);
            }
            temp = temp->next;
        }
        while(temp != cursor);

        if (cursor->pair != nullptr)
            cursor = cursor->pair->next;

    }while(cursor != edge && cursor != nullptr);

    return oppositeVertexes;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 * Face Struct Implementation
 */

Face::Face(): edge(nullptr){}

Face::Face(const Face& face): edge(face.edge)
{

}

HalfEdge* Face::findVertex(Vertex* v) const
{
    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;
    bool found = false;

    do{
        if (tcursor != nullptr)
        {
            if (tcursor->vertex != nullptr)
            {
                if (tcursor->vertex->pointIndex == v->pointIndex)
                    found = true;
            }

            tcursor = tcursor->next;
        }
    }while(tcursor != tfirst && !found);

    if (found)
        return tcursor;

    return nullptr;
}

bool Face::inCircumcircle(int queryPoint)
{
    int a = -1, b = -1, c = -1;

    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;
    bool found = false;

    do{
        if (tcursor != nullptr)
        {
            if (tcursor->vertex != nullptr)
            {
                if (a == -1)
                    a = tcursor->vertex->pointIndex;
                else if (b == -1)
                    b = tcursor->vertex->pointIndex;
                else if (c == -1)
                {
                    c = tcursor->vertex->pointIndex;
                    found = true;
                }
            }
            tcursor = tcursor->next;
        }
    }while(tcursor != tfirst && !found);

    if (found)
        return PointRepository::instance()->inCircumcircle(a, b, c, queryPoint);

    return false;
}

int Face::inside(int pointIndex) const
{
    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;
    bool found = true;
    int value = -1, hold = -1;
    if (tcursor != nullptr && tcursor->next != nullptr)
    {
        do {
            if (tcursor->vertex != nullptr && tcursor->next->vertex != nullptr) {
                int a = tcursor->vertex->pointIndex;
                int b = tcursor->next->vertex->pointIndex;

                value = PointRepository::instance()->areaSign(a, b, pointIndex);
                if (value >= 0)
                    found &= true;
                else
                    found &= false;

                if (value == 0)
                    hold = 0;
            }
            tcursor = tcursor->next;
        } while (tcursor != tfirst && found);
    }

    if (!found)
        return -1;

    return (hold == 0) ? 0 : 1;
}

void Face::splitAdjacentFaces(int pointIndex, vector<Face*>& faces, vector<HalfEdge*>& edges, vector<Vertex*>& vertexes)
{
    auto er1 = this->getEdgeHoldingPoint(pointIndex); // he = holding edge

    if (er1 == nullptr)
        return;

    Vertex* vpq = new Vertex();  vpq->pointIndex = pointIndex;
    Vertex* vpt = vpq->clone();

    HalfEdge* et = er1->next;
    HalfEdge* eq = et->next;

    auto et1 = er1->pair;
    if (et1 != nullptr && et1->face != nullptr)
    {
        Vertex* vpr = vpq->clone();
        Vertex* vps = vpq->clone();
        vpr->pointIndex = vps->pointIndex = pointIndex;

        HalfEdge* er = et1->next;
        HalfEdge* es = er->next;

        HalfEdge* et2 = new HalfEdge();
        HalfEdge* er2 = new HalfEdge();

        HalfEdge* es1 = new HalfEdge();
        HalfEdge* eps = new HalfEdge();

        HalfEdge* eq1 = new HalfEdge();
        HalfEdge* epq = new HalfEdge();

        Face* f0 = er1->face;
        Face* f1 = er->face;

        Face* f2 = new Face();
        Face* f3 = new Face();

        auto q = eq->vertex->clone();
        auto s = es->vertex->clone();

        er1->update(er1->vertex, epq, er2, f0); // face0
        epq->update(vpq, eq, eq1, f0); // face0
        eq->update(eq->vertex, er1, eq->pair, f0); // face0
        f0->edge = eq;

        er->update(er->vertex, es1, er->pair, f1); // face1
        es1->update(s, er2, eps, f1);
        er2->update(vpr, er, er1, f1);
        f1->edge = er;

        es->update(es->vertex, et1, es->pair, f2); // face2
        et1->update(et1->vertex, eps, er2, f2);
        eps->update(vps, es, es1, f2);
        f2->edge = es;

        et->update(et->vertex, eq1, et->pair, f3); // face3
        eq1->update(q, er2, epq, f3);
        er2->update(vpt, et, et1, f3);
        f3->edge = et;

        edges.push_back(et2);    edges.push_back(es1);    edges.push_back(eps);    edges.push_back(eq1);
        edges.push_back(er2);    edges.push_back(epq);    faces.push_back(f2);    faces.push_back(f3);

        vertexes.push_back(vps);    vertexes.push_back(vpr);    vertexes.push_back(vpq); vertexes.push_back(vpt);
        vertexes.push_back(q);    vertexes.push_back(s);

        eq->legalize();
        es->legalize();
        et->legalize();
        er->legalize();
    }
    else
    {
        Face* f0 = er1->face;
        Face* f1 = new Face();

        auto q = eq->vertex->clone();

        HalfEdge* er2 = new HalfEdge();
        HalfEdge* eq1 = new HalfEdge();
        HalfEdge* epq = new HalfEdge();

        er2->update(vpt, et, nullptr, f1); // is new
        eq1->update(q, er2, epq, f1); // is new
        et->update(et->vertex, eq1, et->pair, f1);                     // only next needs to be changed
        f1->edge = et;

        epq->update(vpq, eq, eq1, f0);
        eq->update(eq->vertex, er1, eq->pair, f0);
        er1->update(er1->vertex, epq, nullptr, f0);
        f0->edge = eq;

        edges.push_back(eq1);
        edges.push_back(er2);
        edges.push_back(epq);
        faces.push_back(f1);

        vertexes.push_back(vpq);
        vertexes.push_back(vpt);
        vertexes.push_back(q);

        eq->legalize();
        et->legalize();
    }

}

void Face::splitFace(int pointIndex, vector<Face*>& faces, vector<HalfEdge*>& edges, vector<Vertex*>& vertexes)
{
    Vertex* vpq = new Vertex();
    vpq->pointIndex = pointIndex;

    Vertex* vpr = vpq->clone();
    Vertex* vps = vpq->clone();

    HalfEdge* eq = this->edge;
    HalfEdge* er = eq->next;
    HalfEdge* es = er->next;

    HalfEdge* er1 = new HalfEdge();    HalfEdge* er2 = new HalfEdge();
    HalfEdge* es1 = new HalfEdge();    HalfEdge* es2 = new HalfEdge();
    HalfEdge* eq1 = new HalfEdge();    HalfEdge* eq2 = new HalfEdge();

    Face* f0 = this;
    Face* f1 = new Face();
    Face* f2 = new Face();


    auto r = er->vertex->clone();


    eq->update(eq->vertex, er2, eq->pair, f0); // face 0
    er2->update(r, eq1, er1, f0);
    eq1->update(vpq, eq, eq2, f0);
    f0->edge = eq;

    auto s = es->vertex->clone();
    er->update(er->vertex, es2, er->pair, f1); // face 1
    es2->update(s, er1, es1, f1);
    er1->update(vpr, er, er2, f1);
    f1->edge = er;

    auto q = eq->vertex->clone();   // face 2
    es->update(es->vertex, eq2, es->pair, f2);
    eq2->update(q, es1, eq1, f2);
    es1->update(vps, es, es2, f2);
    f2->edge = es;

    edges.push_back(er1);    edges.push_back(er2);    edges.push_back(es1);    edges.push_back(es2);
    edges.push_back(eq1);    edges.push_back(eq2);    faces.push_back(f1);    faces.push_back(f2);
    vertexes.push_back(vpq);    vertexes.push_back(vpr);    vertexes.push_back(vps);
    vertexes.push_back(r);  vertexes.push_back(s);  vertexes.push_back(q);

    cout<<endl<<"init legalize";
    cout.flush();
    eq->legalize();
    er->legalize();
    es->legalize();
    cout<<endl<<"end legalize";
    cout.flush();
}

HalfEdge* Face::getEdgeHoldingPoint(int pointIndex) const
{
    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;
    do{
        if (tcursor != nullptr && tcursor->next != nullptr)
        {
            if (tcursor->vertex != nullptr && tcursor->next->vertex != nullptr)
            {
                int a = tcursor->vertex->pointIndex;
                if (a == pointIndex)
                    return nullptr;

                int b = tcursor->next->vertex->pointIndex;
                if (b == pointIndex)
                    return nullptr;

                if (PointRepository::instance()->areaSign(a, b, pointIndex) == 0)
                    return tcursor;
            }

            tcursor = tcursor->next;
        }

    }while(tcursor != nullptr && tcursor != tfirst);

    return nullptr;
}

HalfEdge* Face::buildFromPoints(const std::vector<int>& pointcloud)
{
    int count = pointcloud.size(); // obtengo la cantidad de puntos
    edge = new HalfEdge();     // se crea el half edge
    edge->face = this;             // se modifica la cara del edge
    edge->face->edge = edge;      // se modifica el edge de la cara
    edge->pair = nullptr;                  //se inicializa el pair  a nullptr

    HalfEdge * cursor = edge;          // se crea un cursor para iterar por el halfedge
    for (int i = 0; i < count; i++)         // por cada punto se hace lo siguiente
    {
        Vertex* vertex = new Vertex();          // se crea el vertice
        vertex->pointIndex = pointcloud[i];     // se cambia el punto
        cursor->vertex = vertex;                // se modifica el vertice del halfedge actual
        vertex->edge = cursor;                      // se activa el half edge del vertice
        if (i < count-1)                        // se genera half edge seguiente solo hasta el penultimo half edge
        {
            cursor = new HalfEdge();
            cursor->face = this;
            cursor->pair = nullptr;
            vertex->edge->next = cursor;
        }
    }
    cursor->next = edge;               // y se agrega el siguiente del ultimo como el primero y se termina.
    return edge;
}

bool Face::operator ==(const Face& face)
{
    int count = face.getVertexesCount();
    if (this->getVertexesCount() != count)
        return false;

    HalfEdge* ffirst = face.findVertex(edge->vertex);
    HalfEdge* fcursor = ffirst;

    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;

    do{
        if (tcursor != nullptr && fcursor != nullptr)
        {
            if (tcursor->vertex != nullptr && fcursor->vertex != nullptr)
            {
                if (!((*tcursor->vertex) == (*fcursor->vertex)))
                    return false;
            }
            else
                return false;

            fcursor = fcursor->next;
            tcursor = tcursor->next;
        }
    }while(tcursor != tfirst && fcursor != ffirst);

    if (!(tcursor != tfirst && fcursor != ffirst))
        return false;

    return true;
}

bool Face::intersectWithSegment(int p0, int p1, int SMALL_NUM)
{
    /**
     * Vector    u = S.P1 - S.P0;
    Vector    w = S.P0 - Pn.V0;

    float     D = dot(Pn.n, u);
    float     N = -dot(Pn.n, w);

    if (fabs(D) < SMALL_NUM) {           // segment is parallel to plane
        if (N == 0)                      // segment lies in plane
            return 2;
        else
            return 0;                    // no intersection
    }
    // they are not parallel
    // compute intersect param
    float sI = N / D;
    if (sI < 0 || sI > 1)
        return 0;                        // no intersection

    *I = S.P0 + sI * u;                  // compute segment intersect point
    return 1;
     * */

    Vector3D* u = PointRepository::instance()->getVector3dFromPoints(p0, p1);
    Vector3D* w = PointRepository::instance()->getVector3dFromPoints(edge->vertex->pointIndex, p0);
    Vector3D* n = this->getNormal();
    int D = n->dot(*u);
    int N = n->dot(*w);

    delete n;
    delete w;
    delete u;

    if (SMALL_NUM == -1) {
        vector<int> *pointList = this->getVertexesIndices();

        SMALL_NUM = PointRepository::instance()->generalArea2(*pointList);
    }

    if (abs(D) < SMALL_NUM)
    {
        if (N == 0)
            return true;
        else
            return false;
    }

    float sI = N / D;
    if (sI < 0 || sI > 1)
        return 0;                        // no intersection

    return 1;

}

int Face::getVertexesCount() const
{
    HalfEdge* first = this->edge;
    HalfEdge* cursor = first;
    int count = 0;

    if (first == nullptr)
        return 0;

    do{
        if (cursor != nullptr)
        {
            if (cursor->vertex != nullptr)
            {
                ++count;
            }

            cursor = cursor->next;
        }
    }while(cursor != first);

    return count;
}

vector<int>* Face::getVertexesIndices()
{
    vector<int>* indexes = new vector<int>();
    HalfEdge* first = this->edge;
    HalfEdge* cursor = first;

    if (first == nullptr)
        return indexes;

    do{
        if (cursor != nullptr)
        {
            if (cursor->vertex != nullptr)
            {
                indexes->push_back(cursor->vertex->pointIndex);
            }

            cursor = cursor->next;
        }
    }while(cursor != first);

    return indexes;
}

bool Face::inLeft(Face* nFace)
{
    vector<int>* listV = getVertexesIndices();
    if (listV->size() > 3)
        return false;
    HalfEdge* cursor = nFace->edge;
    int vertexCount = 0, inleftCount = 0, n = listV->size();

    do{
        int a = (*listV)[0];
        int b = (*listV)[1];
        int c = (*listV)[2];
        ++vertexCount;
        Vertex* v = cursor->vertex;
        if (v != nullptr)
        {
            if (PointRepository::instance()->volume6(a, b, c, v->pointIndex) >= 0)
                ++inleftCount;
        }
        cursor = cursor->next;
    }while(cursor != nFace->edge);

    delete listV;

    return vertexCount == inleftCount;
}

bool Face::areIntersected(Face* nFace)
{
    vector<int>* listV = getVertexesIndices();
    if (listV->size() > 3)
        return false;
    HalfEdge* cursor = nFace->edge;
    int vertexCount = 0, inleftCount = 0, n = listV->size();

    do{
        int a = (*listV)[0];
        int b = (*listV)[1];
        int c = (*listV)[2];
         ++vertexCount;
         Vertex* v = cursor->vertex;
         if (v != nullptr)
         {
             if (PointRepository::instance()->volume6(a, b, c, v->pointIndex) >= 0)
                 ++inleftCount;
         }
         cursor = cursor->next;
    }while(cursor != nFace->edge);
    delete listV;
    return (inleftCount > 0 && vertexCount != inleftCount);
}

bool Face::inConterClockWise()
{
    int a = -1, b = -1, c = -1;

    HalfEdge* tfirst = this->edge;
    HalfEdge* tcursor = this->edge;
    bool found = false;

    do{
        if (tcursor != nullptr)
        {
            if (tcursor->vertex != nullptr)
            {
                if (a == -1)
                    a = tcursor->vertex->pointIndex;
                else if (b == -1)
                    b = tcursor->vertex->pointIndex;
                else if (c == -1)
                {
                    c = tcursor->vertex->pointIndex;
                    found = true;
                }
            }
            tcursor = tcursor->next;
        }
    }while(tcursor != tfirst && !found);

    if (found)
        return PointRepository::instance()->areaSign(a, b, c);
}

Vector3D* Face::getNormal()
{
    if(edge == nullptr || edge->next == nullptr)
        return nullptr;
    Vector3D* A = edge->getOrientedVector3D();
    Vector3D* B = edge->next->getOrientedVector3D();

    return &(A->cross(*B));
}
