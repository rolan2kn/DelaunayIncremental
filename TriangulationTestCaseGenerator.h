//
// Created by rkindela on 08-04-19.
//

#ifndef PMTOOL_TRIANGULATIONTESTCASEGENERATOR_H
#define PMTOOL_TRIANGULATIONTESTCASEGENERATOR_H
#include "TestCaseGenerator.h"

class TriangulationTestCaseGenerator: public TestCaseGenerator
{
public:
    TriangulationTestCaseGenerator(const std::string& file_name, int trialsCount, int _maxPointCount, int maxvalue = 1000);
    virtual void testALL();

    void testCopyTriangulation();
    void testDestructionTriangulation();

    void testBuildFromPointCloud();

    void testAddPoint();
    void testInitData();
    void testLegalizeEdges();

    void testClone();

};


#endif //PMTOOL_TRIANGULATIONTESTCASEGENERATOR_H
