//
// Created by rkindela on 09-04-19.
//

#ifndef PMTOOL_HALFEDGETESTCASEGENERATOR_H
#define PMTOOL_HALFEDGETESTCASEGENERATOR_H
#include "TestCaseGenerator.h"
#include "HalfEdge.h"
#include "Polygon.h"

class HalfEdgeTestCaseGenerator: public TestCaseGenerator {
public:
    HalfEdgeTestCaseGenerator(const std::string& file_name, int trialsCount, int _maxPointCount, int maxvalue = 1000);
    virtual void testALL();

    void testFaceVertexesRelatedMethods();
    void testFaceInside();
    Face* buildMockedFace(PointDataGenerator pdg, int pointCount, bool fromHull);

    bool checkSplittingTest(int cpp, Face* f0, vector<Vertex*>& vertexes, vector<Face*>& faces, vector<HalfEdge*>& edges);
    void testFaceInCircumcircle();
    void testFaceInLeft();
    void testFaceGetFaceEdgeHoldingPoint();
    void testFaceGetNormal();
    void testFaceSplitFace();
    void testFaceSplitAdjacentFaces();
    void testEdgeGetOrientedVector3D();
    void testEdgeGetEdgeSize();
    void testEdgeGetDistance();
    void testEdgeUpdate();
    void testEdgeAreIntersected();
    void testEdgeLegalize();

};


#endif //PMTOOL_HALFEDGETESTCASEGENERATOR_H
