//
// Created by rkindela on 12-04-2018.
//


#include "Polygon.h"
#include "Vector3D.h"
#include <stack>
#include <algorithm>
#include <queue>
#include <map>

/**
 * Overloading ostream operator it is for printing purposes, this accept one polygon pointer as argument
 * */
std::ostream& operator<<(std::ostream& os, const Polygon* poly)
{
    return os << *poly;
}

/**
 * Overloading ostream operator it is for printing purposes, this accept one polygon constant reference as argument
 * */
std::ostream& operator<<(std::ostream& os, const Polygon& poly)
{
    int count = poly.getPointCount();
    long address = (long)&poly;
    os<<"Polygon address: "<<address<<endl;

    HalfEdge* first = poly.head_edge;
    HalfEdge* cursor = poly.head_edge;

    do{
        if (cursor != nullptr)
        {
            if (cursor->vertex != nullptr)
            {
                int pointIndex = cursor->vertex->pointIndex;
                os << PointRepository::instance()->getPoint(pointIndex);
            }

            cursor = cursor->next;
        }
    }while(cursor != first);

    return os;
}


/**
 * Overloading equal to operator for compare convex hull algorithms, this find for first point and
 * then check in circular way the remaining points
 * */
 bool Polygon::isEqual(Polygon& polyB)
{
    if (head_edge->face != nullptr && polyB.head_edge->face == nullptr)
        return false;
    if (head_edge->face == nullptr && polyB.head_edge->face != nullptr)
        return false;
    if (head_edge->face == nullptr && polyB.head_edge->face == nullptr)
        return true;

    if (!((*this->head_edge->face) == (*polyB.head_edge->face)))
        return false;

    return true;
}

/**
 * Empty constructor
 * */
Polygon::Polygon(): head_edge(), isBad(false)
{

}

Polygon::Polygon(HalfEdge* hedge): head_edge()
{
    this->builfFromHalfEdge(hedge, hedge);
}

void Polygon::buildAsTriangle(int a, int b, int c)
{
    Face* face = new Face();        // se crea la cara nueva
    head_edge = new HalfEdge();     // se crea el half edge
    head_edge->face = face;             // se modifica la cara del edge
    head_edge->face->edge = head_edge;      // se modifica el edge de la cara
    head_edge->pair = nullptr;                  //se inicializa el pair  a nullptr

    Vertex *va = new Vertex();
    Vertex *vb = new Vertex();
    Vertex *vc = new Vertex();
    va->pointIndex = a;
    head_edge->vertex = va;
    va->edge = head_edge;

    vb->pointIndex = b;
    vb->edge = new HalfEdge();
    vb->edge->vertex = vb;
    vb->edge->face = face;
    vb->edge->pair = nullptr;
    head_edge->next = vb->edge;

    vc->pointIndex = c;
    vc->edge = new HalfEdge();
    vc->edge->vertex = vc;
    vc->edge->face = face;
    vc->edge->pair = nullptr;
    vb->edge->next = vc->edge;
    vc->edge->next = head_edge;
}

/**
 * destructor
 * */
Polygon::~Polygon()
{
    delete head_edge;
}

/**
 * Create the polygon Half Edge from a point list
 * **/
void Polygon::buildFromPoints(const std::vector<int>& pointcloud)
{
    Face* face = new Face();        // se crea la cara nueva
    head_edge = face->buildFromPoints(pointcloud);  // se invoca el metodo que reconstruye el face a partir de los ptos.
}

void Polygon::builfFromHalfEdge(HalfEdge* half_edge, HalfEdge* end)
{
    Face* face = new Face();        // se crea la cara nueva
    head_edge = new HalfEdge();     // se crea el half edge
    head_edge->face = face;             // se modifica la cara del edge
    head_edge->face->edge = head_edge;      // se modifica el edge de la cara
    head_edge->pair = nullptr;                  //se inicializa el pair  a nullptr

    HalfEdge * cursor = head_edge;          // se crea un cursor para iterar por el halfedge
    HalfEdge * arg_cursor = half_edge;          // se crea un cursor para iterar por el halfedge del argumento

    do {
        Vertex *vertex = new Vertex();          // se crea el vertice
        vertex->pointIndex = arg_cursor->vertex->pointIndex;     // se cambia el punto
        cursor->vertex = vertex;                // se modifica el vertice del halfedge actual
        vertex->edge = cursor;                      // se activa el half edge del vertice
        arg_cursor = arg_cursor->next;
        if (arg_cursor != end)
        {
            cursor = new HalfEdge();
            cursor->face = face;
            cursor->pair = nullptr;
            vertex->edge->next = cursor;
        }
    }
    while(arg_cursor != end);

    cursor->next = head_edge;
}

/**
 * Returns a point using one position. Note this position is in fact the index position in pointList,
 * we need to use the real index for access to point.
 * **/
POINT* Polygon::getPoint(int pos)
{
    return PointRepository::instance()->getPoint(pos);
}

Face* Polygon::mute2Face()
{
    if (this->head_edge == nullptr || this->head_edge->face == nullptr)
    {
        return nullptr;
    }

    Face* newFace = new Face();
    vector<int >* indexes = head_edge->face->getVertexesIndices();
    newFace->buildFromPoints(*indexes);
    return newFace;
}

/**
 * Returns a point using one position. Note this position is in fact the index position in pointList,
 * we need to use the real index for access to point.
 * **/
POINT* Polygon::getPoint(int pos) const
{
    return PointRepository::instance()->getPoint(pos);
}

/***
 * Build Delaunay Triangulation
 * */
std::vector<Polygon*> Polygon::delaunayTriangulation(const vector<int>& pointCloud)
{
    return std::vector<Polygon*>();
}

/**
 * We check is one polygon intersects with the This.
 * **/

bool Polygon::intersect(const Polygon& newPoly)
{
    /**
     *
Idea basica:

	Supongamos que los bordes  de los poligonos P y Q
estan orientados ccw y que A y B son arcos
con direccion  de P y Q, respectivamente.

    El algoritmo tiene a A y B "cazandose" uno a otro
respectivamente, ajustando sus velocidades de tal forma
de encontrarse cada vez que @P yb1 @Q se intersectan.

A y B se representan como vectores y lo clave aqui son las
reglas para avanzar A y B.

La estructura basicPina del algoritmo es la siguiente:

if (inFlag == Unknown)
    {
        cout<<endl<<"los poligonos P y Q no intersectan";
        cout<<endl<<"Puede ser que no se intersecten o que uno contiene al otro";
    }
Escoger A y B arbitrario.
repeat
	if( A intersecta a B ){
            Chequear por termino
            Actualizar inside flag
        }
	Avanzar ya sea A o B dependiendo de condiciones geométricas
until  Se acaba P o se acaba Q

Manejar P inter Q = 0           P sub Q           Q sub P
b1

Sea a el indice de Pinla cabeza de A y b el indice de la cabeza
de B.  Si B apunta hacia A (ver figura), deseamos avanzar B para
asi acercarnos a una posible interseccion. Esta idea es la  reglaif (inFlag == Unknown)
    {
        cout<<endl<<"los poligonos P y Q no intersectan";
        cout<<endl<<"Puede ser que no se intersecten o que uno contiene al otro";
    }
fundamental.


Sea H(A) el semiplano cerrado a la izquierda de A.


Si AxB > 0 y b no pertenece a H(A)
Si AxB < 0 y b pertenece a H(A)

        ====>  avanza B

     * */

    HalfEdge* P = head_edge;
    HalfEdge* Q = newPoly.head_edge;

    int a, b;                  /** indices en P y Q */
    int a1, b1;                 /**a-1, b-1 respectivamente*/
    Vector3D *A, *B;            // aristas orientadas en P y Q
    int cross;                  // AxB
    bool bHA, aHB;              // b in H(A), a in H(B)

    /**Inicializacion de variables*/

    do
        {
            /**Calculo de variables claves*/
            a1 = P->vertex->pointIndex;
            b1 = Q->vertex->pointIndex;
            a = P->next->vertex->pointIndex;
            b = Q->next->vertex->pointIndex;

            A = P->getOrientedVector3D();
            B = Q->getOrientedVector3D();

            cross = A->detFromCross(*B); // en 2D AxB = C y C tiene la forma (0,0, Cz), por tanto AxB == Cz
            bHA = PointRepository::instance()->areaSign(a1, a, b) >= 0; // leftOn
            aHB = PointRepository::instance()->areaSign(b1, b, a) >= 0; // leftOn

            if (PointRepository::instance()->areIntersected(a1, a, b1, b))
            {
                return true;
            }

            /**Advances Rules*/
            if (cross >= 0)
            {
                if (bHA) {
                    P = P->next;
                }
                else{
                    Q = Q->next;
                }
            } else
            {
                if (aHB) {
                    Q = Q->next;
                }
                else{
                    P = P->next;
                }
            }
        }
    while((P != head_edge) || (Q != newPoly.head_edge));

    /**Casos especiales*/
    return this->contains(newPoly) || newPoly.contains(*this);
}

/**
 * Returns the polygon points count.
 * **/
int Polygon::getPointCount() const
{
   if (head_edge == nullptr || head_edge->face == nullptr)
       return 0;
    return head_edge->face->getVertexesCount();
}

/**
 * Returns if polygon p is inside in this.
 * **/
bool Polygon::contains(const Polygon& p) const
{
    vector<int>* pointList = head_edge->face->getVertexesIndices();
    vector<int>* ppointList = p.head_edge->face->getVertexesIndices();
    int xmin = PointRepository::instance()->getLowerMostXCoord(*pointList);
    int xmax = PointRepository::instance()->getUpperMostXCoord(*pointList);
    int ymin = PointRepository::instance()->getLowerMostYCoord(*pointList);
    int ymax = PointRepository::instance()->getUpperMostYCoord(*pointList);

    int xmin2 = PointRepository::instance()->getLowerMostXCoord(*ppointList);
    int xmax2 = PointRepository::instance()->getUpperMostXCoord(*ppointList);
    int ymin2 = PointRepository::instance()->getLowerMostYCoord(*ppointList);
    int ymax2 = PointRepository::instance()->getUpperMostYCoord(*ppointList);

    int dx0 = getPoint(xmin)->x - p.getPoint(xmin2)->x;
    int dx1 = getPoint(xmax)->x - p.getPoint(xmax2)->x;
    int dy0 = getPoint(ymin)->y - p.getPoint(ymin2)->y;
    int dy1 = getPoint(ymax)->y - p.getPoint(ymax2)->y;

    if ((dx0 < 0 && dx1 > 0 && dy0 > 0 && dy1 < 0))
        return true;
    return false;
}


/**
 * We check if we have a regular polygon.
 * **/
bool Polygon::isRegular()
{
    HalfEdge* cursor = head_edge;
    int sideSize = cursor->getEdgeSize();
    int old_size = sideSize;

    do
        {
            sideSize = cursor->getEdgeSize();
            if (old_size != sideSize)
                return false;
            old_size = sideSize;
            cursor = cursor->next;
        }
    while(cursor != head_edge);

    return true;
}

/**
 * We check if we have a convex polygon.
 * **/
bool Polygon::isConvex()
{
    /**
     *  1) A polygon is convex if for any edge all remain vertex are in the left side of selected edge

        2) If all angles are inferior to 180 degrees
     */

    vector<int>* pointList = head_edge->face->getVertexesIndices();
    bool inLeft = true;
    int last = pointList->size(), i = 0;

    if (last < 3)       // if polygon vertex count is less than three
        return false;

    while(inLeft && i < last)
    {
        // we check if generalArea2 >= 0 to include the colinear case
        // (i+1)%last and (i+1)%last is for check at the end the last point with first and second = last-1, ((last-1)+1)%last = 0, ((last-1)+2)%last = 1
        inLeft = PointRepository::instance()->areaSign((*pointList)[i], (*pointList)[(i+1)%last], (*pointList)[(i+2)%last]) >= 0;
        ++i;
    }

    delete pointList;

    return inLeft;
}

/**
 * We check if we have one point is inside the polygon.
 * **/
bool Polygon::inside(int npoint)
{
    /**
     *  1) Draw a horizontal line to the right of each point and extend it to infinity

        1) Count the number of times the line intersects with polygon edges.

        2) A point is inside the polygon if either count of intersections is odd or
           point lies on an edge of polygon.  If none of the conditions is true, then
           point lies outside.polygon
     */

    int intersectionsNumber = this->pointIntersections(npoint);

    return intersectionsNumber % 2 != 0;
}

int Polygon::pointIntersections(int npoint)
{
    /**
     *  1) Draw a horizontal line from npoint to infinite

        1) Count the number of times the line intersects with polygon edges.

        2) A point is inside the polygon if either count of intersections is odd or
           point lies on an edge of polygon.  If none of the conditions is true, then
           point lies outside.polygon
     */

    vector<int>* pointList = head_edge->face->getVertexesIndices();
    int lastPoint = getPointCount();
    POINT* point = this->getPoint(npoint);
    PointRepository::instance()->add(new POINT(2147483647, point->y));
    int end = PointRepository::instance()->getPointCount()-1;

    int intersectionsNumber = 0;
    for (int i = 0; i < lastPoint; i++)
    {
        //(i+1)%lastPoint is for capture the last edge E(N-1, 0) too
        if (PointRepository::instance()->areIntersected(npoint, end, (*pointList)[i], (*pointList)[(i+1)%lastPoint]))
            ++intersectionsNumber;
    }

    return intersectionsNumber;
}

/**
 * We check if one point is in left from polygon.
 * **/

bool Polygon::left(int npoint)
{
    int intersect = pointIntersections(npoint);
    return  intersect > 0 && intersect % 2 == 0;
}

/**
 * We check if one point is in rigth from polygon.
 * **/
bool Polygon::rigth(int npoint)
{
    return pointIntersections(npoint) == 0;
}

/**
 * Returns the area of polygon.perimetro
 * **/
float Polygon::area2()
{
    /**
     *              n −1
        2∗area ( P)= ∑ x i y i +1 −x i +1 y i
                    i= 0
     * */

    vector<int>* pointList = head_edge->face->getVertexesIndices();
    int area = PointRepository::instance()->generalArea2(*pointList);
    delete pointList;
    return area;
}

vector<Polygon*>* Polygon::generateTriangulation(vector<int>* P)
{
    int n = P->size();
    vector<Polygon*>* polygons = new vector<Polygon*>();
    if (n <= 3) {
        polygons->push_back(this);
        return polygons;
    }
    int i;
    for (i = 0; i < n; i++) {       //busco una oreja i+1
        if (isDiagonal(i, (i + 2) % n, *P))
            break;
    }

    Polygon* p = new Polygon();     // creo un triangulo
    vector<int>* triangle = new vector<int>();
    triangle->push_back((*P)[i]);
    triangle->push_back((*P)[(i+1)%n]);
    triangle->push_back((*P)[(i+2)%n]);
    p->buildFromPoints(*triangle);
    polygons->push_back(p);              //lo agrego a la lista

    P->erase(P->begin() + (i+1)%n);       // borro la oreja i+1
    vector<Polygon*>* tale = generateTriangulation(P);       //y genero la triangulacion con el resto del poligono
    polygons->insert(polygons->end(), tale->begin(), tale->end()); //adiciono el resto de los triangulos a la lista
    return polygons;
}
/**
 * We split the polygon in convexes parts and Returns a list of convex polygons.
 * **/
std::vector<Polygon*> Polygon::splitInConvexes()
{
// iterar por todos los vertices y si forma un angulo concavo marcarlo para dividir, si queda por dividir unirlo al primero
    HalfEdge* cursor = head_edge;
    HalfEdge* first = head_edge;
    std::vector<Polygon*> convexes;
    std::vector<HalfEdge*> splittedAreas;
    std::queue<HalfEdge*> polygons2Check;

    std::stack<HalfEdge*> split_points;
    int a, b, c;
    bool advance = true;
    do {
        advance = true;
        a = cursor->vertex->pointIndex;
        b = cursor->next->vertex->pointIndex;
        c = cursor->next->next->vertex->pointIndex;

        if (PointRepository::instance()->areaSign(a, b, c) < 0)    // if angle in b is concave we have a lot of work
        {
            if (!split_points.empty()) {
                HalfEdge *prev_split_point = split_points.top();

                int distance = prev_split_point->getDistance(cursor);
                if (distance > 1) {    // if points are not adjacents

                    HalfEdge *ncursor = prev_split_point->next; // we get the last half edge to split
                    Polygon* poly = new Polygon();
                    poly->builfFromHalfEdge(ncursor, cursor->next);
                    vector<Polygon *> polyList = poly->splitInConvexes();        // we try to split again the new polygon
                    int _size = polyList.size();                 // if poly is convex then it wasn't splitted

                    if (_size == 0)                 // if poly wan't splitted
                        convexes.push_back(poly);       // I add to list because its convex
                    else {
                        for (int j = 0; j < _size; j++) {   // if not, we add all is convexes parts into the list
                            convexes.push_back(polyList[j]);
                        }
                    }

                    HalfEdge *nedge = new HalfEdge();       // we prepare a new half edge to split the original polygon
                    nedge->next = cursor->next;
                    nedge->face = cursor->face;
                    nedge->vertex = prev_split_point->next->vertex;
                    nedge->vertex->edge = nedge;
                    HalfEdge *temp = prev_split_point->next;    // we remove the useless half edges because  they are inaccessibles
                    while (temp != cursor->next) {
                        HalfEdge *temp1 = temp->next;
                        delete temp;
                        temp = temp1;
                    }
                    cursor = nedge;                     //we update cursor with nedge because cursor was destroyed
                    prev_split_point->next = nedge;     // we finish to split the polygon

                    advance = false;                    // we control the iteration and go back one step to recheck the new split pointto check again the original polygon for split
                }
            }
            split_points.push(cursor);
        }
        if (advance)
            cursor = cursor->next;
    } while (cursor != first->next);

    if (!split_points.empty())
    {
        HalfEdge *prev_split_point = split_points.top();
        split_points.pop();

        HalfEdge* ncursor = prev_split_point->next; // Se obtiene el half edge a picar
        vector<int>* indexes = new vector<int>();   // se crea una lista de indices
        while(ncursor != head_edge)              // Se obtienen todos los vertices involucrados
        {
            indexes->push_back(ncursor->vertex->pointIndex);
            ncursor = ncursor->next;
        }
        Polygon* poly = new Polygon();
        poly->buildFromPoints(*indexes);    // se permite que el nuevo poligono arme el half edge.
        delete indexes;
        convexes.push_back(poly);           // y se añade a la lista de convexos

        prev_split_point->next = head_edge;  //se corta el poligono
    }

    return std::vector<Polygon*>();
}

/**
 * Returns the polygon perimeter.
 * **/

float Polygon::perimeter()
{
    vector<int>* pointList = head_edge->face->getVertexesIndices();
    int count = pointList->size(), index = 2;

    if (count < 3)
        return false;

    POINT* pointA = this->getPoint((*pointList)[0]);
    POINT* pointB = this->getPoint((*pointList)[1]);
    float total_sum = pointB->squareDistance(*pointA);

    while(index < count)
    {
        pointA = this->getPoint((*pointList)[index++]);
        total_sum += pointA->squareDistance(*pointB);
    }
    delete pointList;

    return total_sum;
}


/**
 * Returns is two points form a Polygon Diagonal.
 * **/
bool Polygon::isDiagonal(int initialPoint, int finalPoint, const vector<int>& P)
{
    return isInCone(initialPoint, finalPoint, P) && isDiagonalInteriorExterior(initialPoint, finalPoint, P);
}

bool Polygon::isInCone(int i, int j, const vector<int>& P)
{
    int n = P.size();
    int i1 = (i + 1) % n;
    int in1 = (i - 1 + n) % n;

    if (PointRepository::instance()->areaSign(P[in1], P[i], P[i1]) >= 0) // P[i] convexo
    {
        return (PointRepository::instance()->areaSign(P[i], P[j], P[in1]) > 0) &&
        (PointRepository::instance()->areaSign(P[j], P[i], P[i1]) > 0);
    }
    return (PointRepository::instance()->areaSign(P[i], P[j], P[i1]) > 0) &&
           (PointRepository::instance()->areaSign(P[j], P[i], P[in1]) > 0);
}

bool Polygon::isDiagonalInteriorExterior(int i,int j, const vector<int>& P)
{
    int k , k1;
    int n = P.size();

    for(k=0; k<n; k++)
    {
        k1 = (k+1) % n;
        // saltar incidentes a (i,j)
        if( !(k==i || k==j || k1 == i || k1 == j) )
        {
            if( PointRepository::instance()->areIntersected(P[i], P[j], P[k], P[k1]) )
                return false;
        }
    }
    return true;
}

/**
 * This method find the convex hull from a point cloud and returns a polygon with any vertex on convex hull.
 * **/
Polygon* Polygon::getConvexHull()
{
    vector<int>* pointList = head_edge->face->getVertexesIndices();
    Polygon* p = this->grahamScan(*pointList);
    delete pointList;

    return p;
}

/**
 * This method excecute the Gift Wrapping algorithm and return the convex hull like a polygon.
 * **/

Polygon* Polygon::giftWrapping(std::vector<int> pointcloud)
{
    /***
    jarvis(S)
        // S is the set of points
        pointOnHull = leftmost point in S // which is guaranteed to be part of the CH(S)
        i = 0
        repeat
            P[i] = pointOnHull
            endpoint = S[0]      // initial endpoint for a candidate edge on the hull
            for j from 1 to |S|
                if (endpoint == pointOnHull) or (S[j] is on left of line from P[i] to endpoint)
                    endpoint = S[j]   // found greater left turn, update endpoint
            i = i+1
            pointOnHull = endpoint
        until endpoint == P[0]      // wrapped around to first hull point
     */

    int cloudSize = pointcloud.size();

    if (cloudSize < 3)
        return nullptr;

    std::vector<int> hull;

    int idx = PointRepository::instance()->getLowerMostXCoord(pointcloud);
    int pointOnHull = pointcloud[idx];

    int i = 0, endpoint = 0;
    Polygon* polygon = new Polygon();
    std::vector<int> inhull;
    inhull.push_back(pointOnHull);
    do
    {
        // Add current point to result
        int hullsize = inhull.size();
        if (pointOnHull != inhull[hullsize-1]) {
            inhull.push_back(pointOnHull);
        }
        endpoint = pointcloud[0];

        for (int j = 0; j < cloudSize; j++)
        {
            if (endpoint == pointOnHull || PointRepository::instance()->areaSign(inhull[i], endpoint, pointcloud[j]) > 0)
            {
                endpoint = pointcloud[j];
            }
        }

        ++i;
        pointOnHull = endpoint;

    } while (i < cloudSize && endpoint != inhull[0]);  // While we don't come to first point

    polygon->buildFromPoints(inhull);

    return polygon;
}

/**
 * This method execute the Graham Scan algorithm for compute the point cloud convex hull and returns a polygon.
 * **/
Polygon* Polygon::grahamScan(std::vector<int> pointcloud)
{

    /**
     * let N be number of points
        let points[N] be the array of points
        swap points[0] with the point with the lowest y-coordinate
        sort points by polar angle with points[0]
        let stack = nullptr
        push points[0] to stack
        push points[1] to stack
        push points[2] to stack
        for i = 3 to N:
            while ccw(next_to_top(stack), top(stack), points[i]) <= 0:
                pop stack
            push points[i] to stack
        end
     * */

    int nextToTop(std::stack<int> S); // return the next to top element from S
    int lowestYcoord = PointRepository::instance()->getLowerMostYCoord(pointcloud); //we get the point with lowest Y coord

    if (lowestYcoord < 0 || pointcloud.size() < 3)
        return nullptr;

    int temp = pointcloud[lowestYcoord];                                         // we swap the 0 point with the lowest Y
    pointcloud[lowestYcoord] = pointcloud[0];
    pointcloud[0] = temp;

    std::vector<int> points = PointRepository::instance()->getSortedPolarAngles(pointcloud); // we sort all points using its angle
    int count = points.size();// If modified array of points has less than 3 points,

    // Create an empty stack and push first three points
    // to it.
    std::stack<int> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);

    // Process remaining n-3 pointscout<<endl<<" GrahamScan: "<<endl<<polygon;
    for (int i = 3; i < count; i++)
    {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and points[i] makes
        // a non-left turn

        while (S.size() > 1 && PointRepository::instance()->areaSign(nextToTop(S), S.top(), points[i]) <= 0)
            S.pop();
        S.push(points[i]);
    }

    // Now stack has the output points, print contents

    Polygon* polygon = new Polygon();
    vector<int> hull;

    while (!S.empty())
    {
        int p = S.top();

        hull.push_back(p);

        S.pop();
    }
    polygon->buildFromPoints(hull);

    return polygon;
}

int nextToTop(std::stack<int> S) // return the next to top element from S
{
    int p = S.top();
    S.pop();
    int res = S.top();
    S.push(p);
    return res;
}

Polygon* Polygon::quickHull(std::vector<int> pointcloud)
{
    int a = PointRepository::instance()->getLowerMostXCoord(pointcloud);
    int b = PointRepository::instance()->getUpperMostXCoord(pointcloud);

    vector<int> hull = auxQuickHull(pointcloud, a, b);
    Polygon* p = new Polygon();
    p->buildFromPoints(hull);

    return p;
}

vector<int> Polygon::auxQuickHull(std::vector<int> S, int a, int b)
{
    int c = PointRepository::instance()->getMaximumAreaCoord(S, a, b);
    int size = S.size();
    vector<int> A, B;
    for(int i =0; i < size; i++)
    {
        if(PointRepository::instance()->areaSign(a, c, S[i]) > 0)
            A.push_back(S[i]);
        if(PointRepository::instance()->areaSign(c, b, S[i]) > 0)
            B.push_back(S[i]);
    }

    vector<int> H1,H2, H3, H4;

    int c1 = PointRepository::instance()->getMaximumAreaCoord(S, b, a);
    vector<int> A1, B1;
    for(int i =0; i < size; i++)
    {
        if(PointRepository::instance()->areaSign(c1, a, S[i]) > 0)
            A1.push_back(S[i]);
        if(PointRepository::instance()->areaSign(b, c1, S[i]) > 0)
            B1.push_back(S[i]);
    }

    H1 = auxQuickHull(A, a, c);
    H2 = auxQuickHull(B, c, b);
    H3 = auxQuickHull(A1, c1, a);
    H4 = auxQuickHull(B1, b, c1);

    H3.insert(H3.end(), H4.begin(), H4.end());
    H3.insert(H3.end(), H2.begin(), H2.end());
    H3.insert(H3.end(), H1.begin(), H1.end());

    return H3;
}