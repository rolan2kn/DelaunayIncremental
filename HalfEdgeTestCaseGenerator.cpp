//
// Created by rkindela on 09-04-19.
//

#include "HalfEdgeTestCaseGenerator.h"


HalfEdgeTestCaseGenerator::HalfEdgeTestCaseGenerator(const std::string& file_name, int trialsCount, int _maxPointCount, int maxvalue)
:TestCaseGenerator(file_name, trialsCount, _maxPointCount, maxvalue)
{
}

void HalfEdgeTestCaseGenerator::testALL()
{
    if (maxPointCount >= 214748364) // for avoid int overload
        maxPointCount = 21474836;

    string _filename = currentDirectory() + "/pmtool/Docs/" + filename;
    resultfile = ofstream(_filename);

    resultfile<<"*******************************\n * Fichero de Resultados de Pruebas de HalfEdgeTestGenerator \n*******************************\n";
    resultfile<<"\n Seccion de pruebas del codigo \n";

    testFaceVertexesRelatedMethods();
    testFaceInside();

    testFaceInCircumcircle();
    testFaceInLeft();
    testFaceGetFaceEdgeHoldingPoint();
    testFaceGetNormal();
    testFaceSplitFace();
    testFaceSplitAdjacentFaces();
    testEdgeGetOrientedVector3D();
    testEdgeGetEdgeSize();
    testEdgeGetDistance();
    testEdgeUpdate();
    testEdgeAreIntersected();
    testEdgeLegalize();


    resultfile.flush();
    resultfile.close();
}

void HalfEdgeTestCaseGenerator::testFaceVertexesRelatedMethods()
{
    PointDataGenerator pdg(maxValue);
    resultfile <<"\n\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n "
                 "Prueba de Face::getVertexexCount. Se realizan "<<trialsCount<<" trials"
                                                                        "\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n"<<endl;
    for (int i = 0; i < trialsCount; i++)
    {
        cout<<"\nTrial #"<<i;

        int pointCount = pdg.randomInt(0, maxValue);

        Face* f = this->buildMockedFace(pdg, pointCount, false);

        vector<int>* idxes = f->getVertexesIndices();
        int it = 0;
        bool resp = false;
        for(auto ind: *idxes)
        {
            if(ind!= it++)
            {
                resultfile<<endl<<"face::getvertexes test FAIL";
                cout<<endl<<"face::getvertexes test FAIL";
                resp = true;
            }
        }

        if (resp)
        {
            resultfile<<endl<<"face::getvertexes test Successfully";
            cout<<endl<<"face::getvertexes test Successfully";
        }


        int count = f->getVertexesCount();

        if (count != pointCount)
        {
            resultfile<<endl<<"face::getvertexesCount test fail";
            cout<<endl<<"face::getvertexesCount test fail";
        }
        else
        {
            resultfile<<endl<<"face::getvertexesCount test Successfully";
            cout<<endl<<"face::getvertexesCount test Successfully";
        }

        int randv = pdg.randomInt(0, pointCount-1);
        Vertex* v = new Vertex();
        v->pointIndex = randv;
        resp = f->findVertex(v);
        if (resp)
        {
            cout <<endl<<"face:findVertex text Successfully";
            resultfile <<endl<<"face:findVertex text Successfully";
        }
        else
        {
            cout <<endl<<"face:findVertex text FAIL";
            resultfile <<endl<<"face:findVertex text FAIL";
        }
        PointRepository::instance()->clear();

    }
    resultfile.flush();
}

Face* HalfEdgeTestCaseGenerator::buildMockedFace(PointDataGenerator pdg, int pointCount, bool fromHull) {

    pdg.randomPointCloud(pointCount);
    cout.flush();

    Face* f = nullptr;
    if(!fromHull)
    {
        f = new Face();
        f->buildFromPoints(PointRepository::instance()->getIndexes());
        return f;
    }

    if (fromHull)
    {
        Polygon* p = new Polygon();
        p->buildFromPoints(PointRepository::instance()->getIndexes());
        Polygon* ch = p->getConvexHull();

        if (ch != nullptr)
        {
            f = ch->mute2Face();
            delete p;
            delete ch;

            return f;
        }
    }
    return nullptr;
}

void HalfEdgeTestCaseGenerator::testFaceInside()
{
    PointDataGenerator pdg(maxValue);
    resultfile <<"\n\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n "
                 "Prueba de Face::getVertexexCount. Se realizan "<<trialsCount<<" trials"
              "\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n"<<endl;

    Face* f = nullptr;
    for (int i = 0; i < trialsCount; i++)
    {
        cout<<"\nTrial #"<<i;

        cout<<endl<<"start to pdg.randomInt(1, maxValue)";
        int pointCount = pdg.randomInt(1, maxValue);
        cout<<endl<<"end to pdg.randomInt(1, maxValue)";
        cout.flush();


        try{
            f = this->buildMockedFace(pdg, pointCount, true);

            if (f == nullptr)
                continue;

            Vertex* v = new Vertex;
            int success = 0, failed = 0;
            for(int pos = 0; pos < pointCount; pos++ )
            {
                v->pointIndex = pos;
                bool resp = (f->findVertex(v) != nullptr);
                int _inside = f->inside(pos);

                if(!resp)
                {
                    if(_inside == 1)
                    {
                        ++success;
                    }
                    else
                    {
                        ++failed;
                    }
                }
                else
                {
                    if(_inside < 1)
                    {
                        ++success;
                    }
                    else
                    {
                        ++failed;
                    }
                }
            }

            cout <<endl<<"face:inside trivial test have been "<<(success*100)/pointCount<<"% Successfully";
            resultfile <<endl<<"face:inside trivial test have been "<<(success*100)/pointCount<<"% Successfully";
            cout <<endl<<"face:inside trivial test have been "<<(failed*100)/pointCount<<"% Failed";
            resultfile <<endl<<"face:inside trivial test have been "<<(failed*100)/pointCount<<"% Failed";

            vector<int> pointCloud = PointRepository::instance()->getIndexes();
            int lowX = PointRepository::instance()->getLowerMostXCoord(pointCloud);
            int lowY = PointRepository::instance()->getLowerMostYCoord(pointCloud);
            int upperX = PointRepository::instance()->getUpperMostXCoord(pointCloud);
            int upperY = PointRepository::instance()->getUpperMostYCoord(pointCloud);

            POINT& p0 = *PointRepository::instance()->getPoint(lowX);
            POINT& p1 = *PointRepository::instance()->getPoint(lowY);
            POINT& p2 = *PointRepository::instance()->getPoint(upperX);
            POINT& p3 = *PointRepository::instance()->getPoint(upperY);

            int point = PointRepository::instance()->getCentralPointPosition(pointCloud, pointCount);

            POINT* pp0 = new POINT(p0.x - 100, p1.y-100);
            POINT* pp1 = new POINT(p2.x + 100, p1.y);

            PointRepository::instance()->add(pp0);
            PointRepository::instance()->add(pp1);


            int inside1 = f->inside(point);
            int outside2 = f->inside(point+1);
            int outside3 = f->inside(point+2);

            bool resp = true;
            if (inside1 == 1)
                resp = true;
            else
                resp = false;

            if(outside2 == -1)
                resp = resp & true;
            else
                resp = false;

            if (outside3 == -1)
                resp &= true;
            else resp = false;

            if (resp)
            {
                cout <<endl<<"face:inside Successfully";
                resultfile <<endl<<"face:inside Successfully";
            }
            else
            {
                cout <<endl<<"face:inside  FAIL";
                resultfile <<endl<<"face:inside FAIL";
            }

            delete f;
            PointRepository::instance()->clear();
            delete PointRepository::instance();
        }
        catch(exception& e)
        {
            cout << endl<<e.what();
        }

    }
    resultfile.flush();
}

void HalfEdgeTestCaseGenerator::testFaceInCircumcircle()
{
    PointDataGenerator pdg(maxValue);
    resultfile <<"\n\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n "
                 "Prueba de Face::getVertexexCount. Se realizan "<<trialsCount<<" trials"
                                                                                "\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n"<<endl;

    Face* f = nullptr;
    for (int i = 0; i < trialsCount; i++)
    {
        cout<<"\nTrial #"<<i;

        int pointCount = 3;

        try{
            f = this->buildMockedFace(pdg, pointCount, true);

            if (f == nullptr)
                continue;

            vector<int> pointCloud = PointRepository::instance()->getIndexes();

            int lowX = PointRepository::instance()->getLowerMostXCoord(pointCloud);
            int lowY = PointRepository::instance()->getLowerMostYCoord(pointCloud);
            int upperX = PointRepository::instance()->getUpperMostXCoord(pointCloud);
            int upperY = PointRepository::instance()->getUpperMostYCoord(pointCloud);

            POINT& p0 = *PointRepository::instance()->getPoint(lowX);
            POINT& p1 = *PointRepository::instance()->getPoint(lowY);
            POINT& p2 = *PointRepository::instance()->getPoint(upperX);
            POINT& p3 = *PointRepository::instance()->getPoint(upperY);

            POINT* pp0 = new POINT(p0.x - 100, p1.y-100);
            POINT* pp1 = new POINT(p2.x + 100, p1.y-100);
            POINT* half = new POINT((p0+p2).multiply(0.5f));

            int point = PointRepository::instance()->getCentralPointPosition(pointCloud, pointCount);
            PointRepository::instance()->add(pp0);
            PointRepository::instance()->add(pp1);
//            PointRepository::instance()->add(p0);


            int ccw = f->inConterClockWise();
            cout<<endl<<"el face esta orientado en ccw ssi f->inConterClockWise() > 0 else cw, el valor es: "<<ccw;
            cout.flush();
            bool op1 = f->inCircumcircle(point);
            bool op2 = f->inCircumcircle(point+1);
            bool op3 = f->inCircumcircle(point+2);
            bool op4 = f->inCircumcircle(lowX);

            bool resp = true;
            if (op1)
                resp = true;
            else
                resp = false;

            if(!op2)
                resp = resp & true;
            else
                resp = false;

            if (!op3)
                resp &= true;
            else resp = false;
            if (op4)
                resp &= true;
            else resp = false;

            if (resp)
            {
                cout <<endl<<"face:inCircumcircle Successfully";
                resultfile <<endl<<"face:inCircumcircle Successfully";
            }
            else
            {
                cout <<endl<<"face:inCircumcircle FAIL";
                resultfile <<endl<<"face:inCircumcircle FAIL";
            }

            delete f;
            PointRepository::instance()->clear();
            delete PointRepository::instance();
        }
        catch(exception& e)
        {
            cout << endl<<e.what();
        }

    }
    resultfile.flush();
}

void HalfEdgeTestCaseGenerator::testFaceInLeft()
{

}

void HalfEdgeTestCaseGenerator::testFaceGetFaceEdgeHoldingPoint()
{

}

void HalfEdgeTestCaseGenerator::testFaceGetNormal()
{

}

bool HalfEdgeTestCaseGenerator::checkSplittingTest(int cpp, Face* f0, vector<Vertex*>& vertexes, vector<Face*>& faces, vector<HalfEdge*>& edges)
{
    int vsize = vertexes.size();
    int fsize = faces.size();
    int esize = edges.size();
    int ins = f0->inside(cpp);
    bool resp = false;

    if (ins > 0)
    {
        f0->splitFace(cpp, faces, edges, vertexes);
        int vsize2 = vertexes.size();
        int fsize2 = faces.size();
        int esize2 = edges.size();

        resp = ((vsize2-vsize == 6) &&
                (fsize2-fsize == 2) &&
                (esize2-esize == 6));
        for(int i = fsize; i < fsize2; i++)
        {
            HalfEdge* he = faces[i]->edge;
            HalfEdge* hep = faces[i]->edge;
            int countfaces = 0;
            for (int j = 0; j < 3; j++)
            {
                if(he->face == faces[i])
                {
                    ++countfaces;
                }
                he = he->next;
                if(hep == he && j == 2)
                    resp = true;
            }
        }
    }

    else
    {
        HalfEdge* he = f0->getEdgeHoldingPoint(cpp);

        f0->splitAdjacentFaces(cpp, faces, edges, vertexes);
        int vsize2 = vertexes.size();
        int fsize2 = faces.size();
        int esize2 = edges.size();

        if(he != nullptr && he->pair != nullptr && he->pair->face != nullptr)
            resp = ((vsize2-vsize == 6) &&
                    (fsize2-fsize == 2) &&
                    (esize2-esize == 6));
        else if (he != nullptr && he->pair == nullptr)
            resp = ((vsize2-vsize == 3) &&
                    (fsize2-fsize == 1) &&
                    (esize2-esize == 3));

        for(int i = fsize; i < fsize2; i++)
        {
            HalfEdge* he = faces[i]->edge;
            HalfEdge* hep = faces[i]->edge;
            int countfaces = 0;
            for (int j = 0; j < 3; j++)
            {
                if(he->face == faces[i])
                {
                    ++countfaces;
                }
                he = he->next;
                if(hep == he && j == 2)
                    resp &= true;
            }
        }
    }

    return resp;
}

void HalfEdgeTestCaseGenerator::testFaceSplitFace()
{
    PointDataGenerator pdg(maxValue);
    resultfile <<"\n\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n "
                 "Prueba de Face::getVertexexCount. Se realizan "<<trialsCount<<" trials";

    Face* f = nullptr;
    for (int i = 0; i < trialsCount; i++)
    {
        cout<<"\nTrial #"<<i;

        POINT* p0 = pdg.randomPoint(-maxValue, maxValue);
        POINT* p1 = new POINT(p0->x, p0->y-maxValue);
        POINT* p2 = new POINT(p0->x+maxValue, p0->y-maxValue);
        POINT* p3 = new POINT(p2->x, p0->y);

        int size = PointRepository::instance()->getPointCount();
        PointRepository::instance()->add(p0);
        PointRepository::instance()->add(p1);
        PointRepository::instance()->add(p2);
        PointRepository::instance()->add(p3);

        HalfEdge* eq = new HalfEdge();
        HalfEdge* eqp = new HalfEdge();

        HalfEdge* er = new HalfEdge();
        HalfEdge* erp = new HalfEdge();

        HalfEdge* es = new HalfEdge();
        HalfEdge* esp = new HalfEdge();

        HalfEdge* et = new HalfEdge();
        HalfEdge* etp = new HalfEdge();

        HalfEdge* er1 = new HalfEdge();
        HalfEdge* et1 = new HalfEdge();

        Face* f0 = new Face();
        Face* f1 = new Face();

        Vertex* vq = new Vertex();
        vq->pointIndex = 0;
        Vertex* vtp = vq->clone();

        Vertex* vr = new Vertex();
        vr->pointIndex = 1;
        Vertex* vr1 =vr->clone();
        Vertex* vqp =vr->clone();

        Vertex* vs = new Vertex();
        vs->pointIndex = 2;
        Vertex* vrp = vs->clone();

        Vertex* vt = new Vertex();
        vt->pointIndex = 3;
        Vertex* vt1 = vt->clone();
        Vertex* vsp = vt->clone();

        eq->update(vq, er1, nullptr, f0);
        er1->update(vr1, et, et1, f0);
        et->update(vt, eq, nullptr, f0);

        er->update(vr, es, nullptr, f1);
        es->update(vs, et1, nullptr, f1);
        et1->update(vt1, er, er1, f1);

        f0->edge = eq;
        f1->edge = er;

        vector<Face*> faces;
        vector<Vertex*> vertexes;
        vector<HalfEdge*> edges;

        faces.push_back(f0);
        faces.push_back(f1);

        vertexes.push_back(vq);
        vertexes.push_back(vtp);
        vertexes.push_back(vr);
        vertexes.push_back(vr1);
        vertexes.push_back(vqp);
        vertexes.push_back(vs);
        vertexes.push_back(vrp);
        vertexes.push_back(vt);
        vertexes.push_back(vt1);
        vertexes.push_back(vsp);

        edges.push_back(eq);
        edges.push_back(er1);
        edges.push_back(et);
        edges.push_back(er);
        edges.push_back(es);
        edges.push_back(et1);
        edges.push_back(eqp);
        edges.push_back(etp);
        edges.push_back(esp);
        edges.push_back(erp);

        vector<int>* points = new vector<int>();
        points->push_back(0);
        points->push_back(1);
        points->push_back(2);

        int sgn = PointRepository::instance()->area2(0, 1, 2);
        int cpp = PointRepository::instance()->getCentralPointPosition(*points, 3);
        delete points;


        int newy = p2->y + (int)(maxValue/4);
        POINT* p4 = new POINT(p2->x, newy);
        int p4pos = PointRepository::instance()->getPointCount();
        PointRepository::instance()->add(p4);

        bool resp = this->checkSplittingTest(cpp, f0, vertexes, faces, edges);

        bool resp2 = this->checkSplittingTest(p4pos, f1, vertexes, faces, edges);

        if (resp && resp2)
        {
            cout<<endl<<"face::split was successful";
            resultfile<<endl<<"face::split was successful";
        }
        else
        {
            cout<<endl<<"face::split was failed";
            resultfile<<endl<<"face::split was failed";
        }

        PointRepository::instance()->clear();

        delete eq;
        delete eqp;

        delete er;
        delete  erp;

        delete es;
        delete esp;

        delete et;
        delete etp;

        delete er1;
        delete et1;

        delete f0;
        delete f1;

        delete vq;
        delete vtp;

        delete  vr;
        delete  vr1;
        delete  vqp;

        delete vs;
        delete vrp;

        delete vt; delete vt1; delete vsp;
        faces.clear();
        vertexes.clear();
        edges.clear();
    }
    resultfile.flush();
}

void HalfEdgeTestCaseGenerator::testFaceSplitAdjacentFaces()
{

}

void HalfEdgeTestCaseGenerator::testEdgeGetOrientedVector3D()
{

}

void HalfEdgeTestCaseGenerator::testEdgeGetEdgeSize()
{

}

void HalfEdgeTestCaseGenerator::testEdgeGetDistance()
{

}

void HalfEdgeTestCaseGenerator::testEdgeUpdate()
{

}

void HalfEdgeTestCaseGenerator::testEdgeAreIntersected()
{

}

void HalfEdgeTestCaseGenerator::testEdgeLegalize()
{

}