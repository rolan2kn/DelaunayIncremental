//
// Created by rkindela on 08-04-19.
//

#include "TriangulationTestCaseGenerator.h"

TriangulationTestCaseGenerator::TriangulationTestCaseGenerator(const std::string& file_name, int trialsCount, int _maxPointCount, int maxvalue)
:TestCaseGenerator(file_name, trialsCount, _maxPointCount, maxvalue)
{

}

void TriangulationTestCaseGenerator::testALL()
{
    if (maxPointCount >= 214748364) // for avoid int overload
        maxPointCount = 21474836;

    string _filename = currentDirectory() + "/pmtool/Docs/triangulation_" + filename;
    resultfile = ofstream(_filename);

    resultfile<<"*******************************\n * Fichero de Resultados de Pruebas \n*******************************\n";
    resultfile<<"\n Seccion de pruebas del codigo \n";

    testCopyTriangulation();
    testDestructionTriangulation();

    testBuildFromPointCloud();

    testAddPoint();
    testInitData();
    testLegalizeEdges();

    testClone();


    resultfile.flush();
    resultfile.close();
}

void TriangulationTestCaseGenerator::testCopyTriangulation()
{
    //Todo
}

void TriangulationTestCaseGenerator::testDestructionTriangulation()
{
    //Todo
}

void TriangulationTestCaseGenerator::testBuildFromPointCloud()
{
    //Todo
}

void TriangulationTestCaseGenerator::testAddPoint()
{
    //Todo
}

void TriangulationTestCaseGenerator::testInitData()
{
    //Todo
}

void TriangulationTestCaseGenerator::testLegalizeEdges()
{
    //Todo
}

void TriangulationTestCaseGenerator::testClone()
{
    //Todo
}
