//
// Created by rkindela on 11-06-18.
//

#include <cstring>
#include "TestCaseGenerator.h"
# include <cstdarg>
#include <map>
#include "Matrix.h"


TestCaseGenerator::TestCaseGenerator(const std::string& file_name, int trialsCount, int _maxPointCount, int maxvalue)
:trialsCount(trialsCount), maxPointCount(_maxPointCount), maxValue(maxvalue), filename(file_name)
{
}

void TestCaseGenerator::testALL()
{
    if (maxPointCount >= 214748364) // for avoid int overload
        maxPointCount = 21474836;

    resultfile = ofstream(currentDirectory() + "/Docs/" + filename);

    resultfile<<"*******************************\n * Fichero de Resultados de Pruebas \n*******************************\n";
    resultfile<<"\n Seccion de pruebas del codigo \n";

    int tempTrialsCount = (trialsCount) ? trialsCount : 1;
    trialsCount *= 0.5;
//    this->testPointDataGenerator();
//    this->testPoint();
//    this->testVector();
    this->testPointRepository();

    trialsCount = tempTrialsCount;
    resultfile<<"\n*******************************\n * Fin de Pruebas \n*******************************\n";
    cout<<"\n*******************************\n * Fin de Pruebas \n*******************************\n";
    resultfile.flush();
    resultfile.close();
}


void TestCaseGenerator::testPoint()
{
    PointDataGenerator pdg(maxValue);
    resultfile <<"\n\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n "
                 "Prueba de la clase punto. Se realizan "<<trialsCount<<" trials"
                 "\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n"<<endl;
    for (int i = 0; i < trialsCount; i++)
    {
        cout<<"\nTrial #"<<i;

        POINT* p = pdg.randomPoint(-maxValue, maxValue);
        POINT* p1 = pdg.randomPoint(-maxValue, maxValue);
        resultfile<<endl<<"Se generan dos puntos aleatoriamente con valores x, y en ("<<-maxValue<<", "<<maxValue<<"). ";
        resultfile<<endl<<"p0 = "<<p<<", p1 = "<<p1;
        resultfile<<endl<<"Se procede a probar de la clase Point<int>";

        int dx = p->x - p1->x, dy = p->y - p1->y, dz = p->z - p1->z;
        resultfile<<endl<<p<<".distance("<<p1<<") := {esperado: "<<sqrt(dx*dx + dy*dy + dz*dz) <<", real:"<<p->distance(*p1)<<"}";
        resultfile<<endl<<p<<".squareDistance("<<p1<<") := {esperado: "<<(dx*dx + dy*dy + dz*dz) <<", real:"<<p->squareDistance(*p1)<<"}";
        int sx = p->x + p1->x, sy = p->y + p1->y, sz = p->z + p1->z;
        resultfile<<endl<<p<<".operator +("<<p1<<") := {esperado: "<< POINT(sx, sy, sz) <<", real:"<<((*p) + *p1)<<"}";
        resultfile<<endl<<p<<".operator -("<<p1<<") := {esperado: "<< POINT(dx, dy, dz) <<", real:"<<((*p) - *p1)<<"}";
        resultfile<<endl<<p<<".operator =("<<p<<") := {esperado: "<< POINT(p->x, p->y, p->z) <<", real:"<<((*p) = *p)<<"}";
        resultfile<<endl<<p<<".operator =("<<p1<<") := {esperado: "<< POINT(p1->x, p1->y, p1->z) <<", real:"<<((*p) = *p1)<<"}";

    }
    resultfile.flush();
}

void TestCaseGenerator::testVector()
{
    PointDataGenerator pdg(maxValue);
    resultfile <<"\n\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n "
                 "Prueba de la clase Vector3D. Se realizan "<<trialsCount<<" trials"
                  "\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n"<<endl;
    for (int i = 0; i < trialsCount; i++)
    {
        resultfile<<"\nTrial #"<<i;

        POINT* p0 = pdg.randomPoint(-maxValue, maxValue);
        POINT* p1 = pdg.randomPoint(-maxValue, maxValue);
        POINT* p2 = pdg.randomPoint(-maxValue, maxValue);
        POINT* p3 = pdg.randomPoint(-maxValue, maxValue);

        resultfile<<endl<<"Se generan 4 puntos aleatoriamente con valores x, y en (0, "<<maxPointCount<<"). ";
        resultfile<<endl<<"p0 = "<<p0<<", p1 = "<<p1<<", p2 = "<<p2<<", p3 = "<<p3;
        Vector3D* Va = new Vector3D(*p0, *p1);
        Vector3D* Vb = new Vector3D(*p3, *p2);

        resultfile<<endl<<"Se generan dos vectores Va, Vb tal que Va = p1-p0, Vb = p3-p2. Va = "<<Va<<", Vb = "<<Vb;

        int dx = Va->x - Vb->x, dy = Va->y - Vb->y, dz = Va->z - Vb->z;
        int sx = Va->x + Vb->x, sy = Va->y + Vb->y, sz = Va->z + Vb->z;
        resultfile<<endl<<Va<<".sum("<<Vb<<") := {esperado: "<<Vector3D(sx, sy, sz) <<", real:"<<Va->sum(*Vb)<<"}";
        resultfile<<endl<<Va<<".minus("<<Vb<<") := {esperado: "<<Vector3D(dx, dy, dz) <<", real:"<<Va->minus(*Vb)<<"}";

        int n = pdg.randomInt(0, maxPointCount);
        resultfile<<"\n Se genera un numero aleatorio entre (0,"<<maxPointCount<<"). N = "<<n;
        resultfile<<endl<<Va<<".multi("<<n<<") := {esperado: "<<Vector3D(Va->x*n,Va->y*n,Va->z*n) <<", real:"<<Va->multi(n)<<"}";
        resultfile<<endl<<Vb<<".multi("<<n<<") := {esperado: "<<Vector3D(Vb->x*n,Vb->y*n,Vb->z*n) <<", real:"<<Vb->multi(n)<<"}";

        /**
        * wx = uy*vz − uz*vy
        * wy = uz*vx - ux*vz
        * wz = ux*vy - uy*vx
         * */

        resultfile<<endl<<Va<<".cross("<<Vb<<") := {esperado: "<<Vector3D(Va->y*Vb->z - Va->z*Vb->y,Va->z*Vb->x - Va->x*Vb->z,Va->x*Vb->y - Va->y*Vb->x) <<", real:"<<Va->cross(*Vb)<<"}";

        int dotab = (Va->x*Vb->x + Va->y*Vb->y + Va->z*Vb->z);
        resultfile<<endl<<Va<<".dot("<<Vb<<") := {esperado: "<< dotab <<", real:"<<Va->dot(*Vb)<<"}";

        // sqrt(x*x + y*y + z*z)
        float normA = sqrt(Va->x*Va->x + Va->y*Va->y + Va->z*Va->z);
        float normB = sqrt(Vb->x*Vb->x + Vb->y*Vb->y + Vb->z*Vb->z);
        resultfile<<endl<<Va<<".norm() := {esperado: "<<normA <<", real:"<<Va->norm()<<"}";
        resultfile<<endl<<Vb<<".norm() := {esperado: "<<normB <<", real:"<<Vb->norm()<<"}";
        resultfile<<endl<<Va<<".squareNorm() := {esperado: "<<normA*normA<<", real:"<<Va->squareNorm()<<"}";
        resultfile<<endl<<Vb<<".squareNorm() := {esperado: "<<normB*normB<<", real:"<<Vb->squareNorm()<<"}";

        Vector3D vaa(*Va), vbb(*Vb);

        vaa.normalize();
        vbb.normalize();

        normA = (normA != 0) ? normA : 1;
        normB = (normB != 0) ? normB : 1;

        resultfile<<endl<<Va<<".normalize() := {esperado: "<<Vector3D(Va->x/normA, Va->y/normA, Va->z/normA ) <<", real:"<<vaa<<"}";
        resultfile<<endl<<Vb<<".normalize() := {esperado: "<<Vector3D(Vb->x/normB, Vb->y/normB, Vb->z/normB ) <<", real:"<<vbb<<"}";

        float angleab = acos(dotab/(normA*normB));
        resultfile<<endl<<Va<<".angle("<<Vb<<") := {esperado: "<<angleab <<", real:"<<Va->angle(*Vb)<<"}";
        resultfile<<endl<<Va<<".angleCos("<<Vb<<") := {esperado: "<<(dotab/(normA*normB))<<", real:"<<Va->angle(*Vb)<<"}";

    }
    resultfile.flush();
}

void TestCaseGenerator::helper4SingletonChecking()
{
    resultfile<<"\n Se prueba que efectivamente la clase PointRepository es un singleton";
    try
    {
        resultfile<<"\n Se intenta crear un objeto: PointRepository p(); : {esperado: exception error, real: ";
        PointRepository p();
        resultfile<<"fallo, El singleton no deberia dejar crear el objeto.";
        (void)p;
    }
    catch(const exception& e)
    {
        resultfile<<"\n exception error = "<<e.what();
    }
    resultfile<<"}\n";

    resultfile<<"\n Se comprueba que PointRepository::instance() siempre retorna una misma instancia\n";
    resultfile<<"\n PointRepository *pr0  = PointRepository::instance(), *pr1 = PointRepository::instance()\n";
    PointRepository *pr0  = PointRepository::instance(), *pr1 = PointRepository::instance();
    resultfile<<"\n pr0 ="<<pr0<<", pr1 = "<<pr1<<" \npr0 == pr1: {esperado: true, real: "<<(pr0 == pr1)<<"}";
    resultfile.flush();
}

void TestCaseGenerator::helper4Area2Testing(PointRepository* pr0, POINT* p0, POINT* p1, POINT* p2)
{
    resultfile<<"\n ================================================================================= \n";
    resultfile<<endl<<"se prueba el metodo PointRepository::instance()->area2()";
    resultfile<<"\n ================================================================================= \n";

    POINT A, B;

    A = *(p1) - *(p0); // p0, p1, p2
    B = *(p2) - *(p0);

    int area1 = A.x*B.y - A.y*B.x;
    resultfile<<endl<<"pr0->area2("<<p0<<","<<p1<<", "<<p2<<"): {esperado: "<<area1<<", real: "<<pr0->area2(0, 1, 2)<<"}";

    A = *(p2) - *(p0); //p0, p2, p1
    B = *(p1) - *(p0);
    area1 = A.x*B.y - A.y*B.x;
    resultfile<<endl<<"pr0->area2("<<p0<<","<<p2<<", "<<p1<<"): {esperado: "<<area1<<", real: "<<pr0->area2(0, 2, 1)<<"}";

    A = *(p0) - *(p1); //p1, p0, p2
    B = *(p2) - *(p1);
    area1 = A.x*B.y - A.y*B.x;
    resultfile<<endl<<"pr0->area2("<<p1<<","<<p0<<", "<<p2<<"): {esperado: "<<area1<<", real: "<<pr0->area2(1, 0, 2)<<"}";

    A = *(p2) - *(p1); //p1, p2, p0
    B = *(p0) - *(p1);
    area1 = A.x*B.y - A.y*B.x;
    resultfile<<endl<<"pr0->area2("<<p1<<","<<p2<<", "<<p0<<"): {esperado: "<<area1<<", real: "<<pr0->area2(1, 2, 0)<<"}";

    A = *(p0) - *(p2); //p2, p0, p1
    B = *(p1) - *(p2);
    area1 = A.x*B.y - A.y*B.x;
    resultfile<<endl<<"pr0->area2("<<p2<<","<<p0<<", "<<p1<<"): {esperado: "<<area1<<", real: "<<pr0->area2(2, 0, 1)<<"}";

    A = *(p1) - *(p2); //p2, p1, p0
    B = *(p0) - *(p2);
    area1 = A.x*B.y - A.y*B.x;
    resultfile<<endl<<"pr0->area2("<<p2<<","<<p1<<", "<<p0<<"): {esperado: "<<area1<<", real: "<<pr0->area2(2, 1, 0)<<"}";

    A = *(p0) - *(p0); //p0, p0, p1
    B = *(p1) - *(p0);
    area1 = A.x*B.y - A.y*B.x;
    resultfile<<endl<<"pr0->area2("<<p0<<","<<p0<<", "<<p1<<"): {esperado: "<<area1<<", real: "<<pr0->area2(0, 0, 1)<<"}";

    A = *(p1) - *(p0); //p0, p1, p0
    B = *(p0) - *(p0);
    area1 = A.x*B.y - A.y*B.x;
    resultfile<<endl<<"pr0->area2("<<p0<<","<<p1<<", "<<p0<<"): {esperado: "<<area1<<", real: "<<pr0->area2(0, 1, 0)<<"}";

    A = *(p1) - *(p0); //p0, p1, p1
    B = *(p1) - *(p0);
    area1 = A.x*B.y - A.y*B.x;
    resultfile<<endl<<"pr0->area2("<<p0<<","<<p1<<", "<<p1<<"): {esperado: "<<area1<<", real: "<<pr0->area2(0, 1, 1)<<"}";
    resultfile.flush();
}


void TestCaseGenerator::helper4AreaSignTesting(PointRepository* pr0, POINT* p0, POINT* p1, POINT* p2)
{
    resultfile<<"\n ================================================================================= \n";
    resultfile<<endl<<"se prueba el metodo PointRepository::instance()->areaSign()";
    resultfile<<"\n ================================================================================= \n";

    POINT A, B;
    int value;

    A = *(p1) - *(p0); // p0, p1, p2
    B = *(p2) - *(p0);

    int area1 = A.x*B.y - A.y*B.x;
    value = (area1 == 0) ? 0 : ((area1 > 0) ? 1 : -1);
    resultfile<<endl<<"pr0->areaSign("<<p0<<","<<p1<<", "<<p2<<"): {esperado: "<<value<<", real: "<<pr0->areaSign(0, 1, 2)<<"}";

    A = *(p2) - *(p0); //p0, p2, p1
    B = *(p1) - *(p0);
    area1 = A.x*B.y - A.y*B.x;
    value = (area1 == 0) ? 0 : ((area1 > 0) ? 1 : -1);
    resultfile<<endl<<"pr0->areaSign("<<p0<<","<<p2<<", "<<p1<<"): {esperado: "<<value<<", real: "<<pr0->areaSign(0, 2, 1)<<"}";

    A = *(p0) - *(p1); //p1, p0, p2
    B = *(p2) - *(p1);
    area1 = A.x*B.y - A.y*B.x;
    value = (area1 == 0) ? 0 : ((area1 > 0) ? 1 : -1);
    resultfile<<endl<<"pr0->areaSign("<<p1<<","<<p0<<", "<<p2<<"): {esperado: "<<value<<", real: "<<pr0->areaSign(1, 0, 2)<<"}";

    A = *(p2) - *(p1); //p1, p2, p0
    B = *(p0) - *(p1);
    area1 = A.x*B.y - A.y*B.x;
    value = (area1 == 0) ? 0 : ((area1 > 0) ? 1 : -1);
    resultfile<<endl<<"pr0->areaSign("<<p1<<","<<p2<<", "<<p0<<"): {esperado: "<<value<<", real: "<<pr0->areaSign(1, 2, 0)<<"}";

    A = *(p0) - *(p2); //p2, p0, p1
    B = *(p1) - *(p2);
    area1 = A.x*B.y - A.y*B.x;
    value = (area1 == 0) ? 0 : ((area1 > 0) ? 1 : -1);
    resultfile<<endl<<"pr0->areaSign("<<p2<<","<<p0<<", "<<p1<<"): {esperado: "<<value<<", real: "<<pr0->areaSign(2, 0, 1)<<"}";

    A = *(p1) - *(p2); //p2, p1, p0
    B = *(p0) - *(p2);
    area1 = A.x*B.y - A.y*B.x;
    value = (area1 == 0) ? 0 : ((area1 > 0) ? 1 : -1);
    resultfile<<endl<<"pr0->areaSign("<<p2<<","<<p1<<", "<<p0<<"): {esperado: "<<value<<", real: "<<pr0->areaSign(2, 1, 0)<<"}";

    A = *(p0) - *(p0); //p0, p0, p1
    B = *(p1) - *(p0);
    area1 = A.x*B.y - A.y*B.x;
    value = (area1 == 0) ? 0 : ((area1 > 0) ? 1 : -1);
    resultfile<<endl<<"pr0->areaSign("<<p0<<","<<p0<<", "<<p1<<"): {esperado: "<<value<<", real: "<<pr0->areaSign(0, 0, 1)<<"}";

    A = *(p1) - *(p0); //p0, p1, p0
    B = *(p0) - *(p0);
    area1 = A.x*B.y - A.y*B.x;
    value = (area1 == 0) ? 0 : ((area1 > 0) ? 1 : -1);
    resultfile<<endl<<"pr0->areaSign("<<p0<<","<<p1<<", "<<p0<<"): {esperado: "<<value<<", real: "<<pr0->areaSign(0, 1, 0)<<"}";

    A = *(p1) - *(p0); //p0, p1, p1
    B = *(p1) - *(p0);
    area1 = A.x*B.y - A.y*B.x;
    value = (area1 == 0) ? 0 : ((area1 > 0) ? 1 : -1);
    resultfile<<endl<<"pr0->areaSign("<<p0<<","<<p1<<", "<<p1<<"): {esperado: "<<value<<", real: "<<pr0->areaSign(0, 1, 1)<<"}";
    resultfile.flush();
}

void TestCaseGenerator::helper4InCircleTesting(PointRepository* pr0, POINT* pA, POINT* pB, POINT* pC, POINT* pD)
{
    resultfile<<"\n ================================================================================= \n";
    resultfile<<endl<<"se prueba el metodo PointRepository::instance()->inCircumcircle()";
    resultfile<<"\n ================================================================================= \n";

    bool result;
    float _det;

    //------------------------------------------ combinacion 00
    POINT ad = (*pA) - (*pD); // a, b, c, d
    POINT bd = (*pB) - (*pD);
    POINT cd = (*pC) - (*pD);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pA<<","<<pB<<", "<<pC<<", "<<pD<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(0, 1, 2, 3)<<"}";

    //------------------------------------------ combinacion 01
    ad = (*pA) - (*pC); // a, b, D, C
    bd = (*pB) - (*pC);
    cd = (*pD) - (*pC);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pA<<","<<pB<<", "<<pD<<", "<<pC<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(0, 1, 3, 2)<<"}";

    //------------------------------------------ combinacion 02
    ad = (*pA) - (*pD); // a, C, b, d
    bd = (*pC) - (*pD);
    cd = (*pB) - (*pD);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pA<<","<<pC<<", "<<pB<<", "<<pD<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(0, 2, 1, 3)<<"}";

    //------------------------------------------ combinacion 03
    ad = (*pA) - (*pB); // a, C, D, B
    bd = (*pC) - (*pB);
    cd = (*pD) - (*pB);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pA<<","<<pC<<", "<<pD<<", "<<pB<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(0, 2, 3, 1)<<"}";

    //------------------------------------------ combinacion 04
    ad = (*pA) - (*pB); // a, D, C, B
    bd = (*pD) - (*pB);
    cd = (*pC) - (*pB);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pA<<","<<pD<<", "<<pC<<", "<<pB<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(0, 3, 2, 1)<<"}";

    //------------------------------------------ combinacion 05
    ad = (*pA) - (*pC); // a, D, B, C
    bd = (*pD) - (*pC);
    cd = (*pB) - (*pC);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pA<<","<<pD<<", "<<pB<<", "<<pC<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(0, 3, 1, 2)<<"}";

    //------------------------------------------ combinacion 06
    ad = (*pB) - (*pD); // B, A, C, D
    bd = (*pA) - (*pD);
    cd = (*pC) - (*pD);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pB<<","<<pA<<", "<<pC<<", "<<pD<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(1, 0, 2, 3)<<"}";

    //------------------------------------------ combinacion 07
    ad = (*pB) - (*pC);  // B, A, D, C
    bd = (*pA) - (*pC);
    cd = (*pD) - (*pC);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pB<<","<<pA<<", "<<pD<<", "<<pC<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(1, 0, 3, 2)<<"}";

    //------------------------------------------ combinacion 08
    ad = (*pB) - (*pD); // B, C, A, D
    bd = (*pC) - (*pD);
    cd = (*pA) - (*pD);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pB<<","<<pC<<", "<<pA<<", "<<pD<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(1, 2, 0, 3)<<"}";

    //------------------------------------------ combinacion 09
    ad = (*pB) - (*pA); // B, C, D, A
    bd = (*pC) - (*pA);
    cd = (*pD) - (*pA);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pB<<","<<pC<<", "<<pD<<", "<<pA<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(1, 2, 3, 0)<<"}";

    //------------------------------------------ combinacion 10
    ad = (*pB) - (*pC); // B, D, A, C
    bd = (*pD) - (*pC);
    cd = (*pA) - (*pC);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pB<<","<<pD<<", "<<pA<<", "<<pC<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(1, 3, 0, 2)<<"}";

    //------------------------------------------ combinacion 11
    ad = (*pB) - (*pA); // B, D, C, A
    bd = (*pD) - (*pA);
    cd = (*pC) - (*pA);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pB<<","<<pD<<", "<<pC<<", "<<pA<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(1, 3, 2, 0)<<"}";

    //------------------------------------------ combinacion 13
    ad = (*pC) - (*pD); // C, A, B, D
    bd = (*pA) - (*pD);
    cd = (*pB) - (*pD);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pC<<","<<pA<<", "<<pB<<", "<<pD<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(2, 0, 1, 3)<<"}";

    //------------------------------------------ combinacion 14
    ad = (*pC) - (*pB); // C, A, D, B
    bd = (*pA) - (*pB);
    cd = (*pD) - (*pB);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pC<<","<<pA<<", "<<pD<<", "<<pB<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(2, 0, 3, 1)<<"}";

    //------------------------------------------ combinacion 15
    ad = (*pC) - (*pD); // C, B, A, D
    bd = (*pB) - (*pD);
    cd = (*pA) - (*pD);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pC<<","<<pB<<", "<<pA<<", "<<pD<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(2, 1, 0, 3)<<"}";

    //------------------------------------------ combinacion 16
    ad = (*pC) - (*pA); // C, B, D, A
    bd = (*pB) - (*pA);
    cd = (*pD) - (*pA);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pC<<","<<pB<<", "<<pD<<", "<<pA<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(2, 1, 3, 0)<<"}";

    //------------------------------------------ combinacion 17
    ad = (*pC) - (*pB); // C, D, A, B
    bd = (*pD) - (*pB);
    cd = (*pA) - (*pB);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pC<<","<<pD<<", "<<pA<<", "<<pB<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(2, 3, 0, 1)<<"}";


    //------------------------------------------ combinacion 18
    ad = (*pC) - (*pA); // C, D, B, A
    bd = (*pD) - (*pA);
    cd = (*pB) - (*pA);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pC<<","<<pD<<", "<<pB<<", "<<pA<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(2, 3, 1, 0)<<"}";


    //------------------------------------------ combinacion 19
    ad = (*pD) - (*pC); // D, A, B, C
    bd = (*pA) - (*pC);
    cd = (*pB) - (*pC);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pD<<","<<pA<<", "<<pB<<", "<<pC<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(3, 0, 1, 2)<<"}";


    //------------------------------------------ combinacion 20
    ad = (*pD) - (*pB); // d, a, C, b
    bd = (*pA) - (*pB);
    cd = (*pC) - (*pB);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pD<<","<<pA<<", "<<pC<<", "<<pB<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(3, 0, 2, 1)<<"}";

    //------------------------------------------ combinacion 21
    ad = (*pD) - (*pC); // D, B. A. C.
    bd = (*pB) - (*pC);
    cd = (*pA) - (*pC);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pD<<","<<pB<<", "<<pA<<", "<<pC<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(3, 1, 0, 2)<<"}";

    //------------------------------------------ combinacion 22
    ad = (*pD) - (*pA); // D, B, C, A
    bd = (*pB) - (*pA);
    cd = (*pC) - (*pA);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pD<<","<<pB<<", "<<pC<<", "<<pA<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(3, 1, 2, 0)<<"}";

    //------------------------------------------ combinacion 23
    ad = (*pD) - (*pB); // d, c, a, b
    bd = (*pC) - (*pB);
    cd = (*pA) - (*pB);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pD<<","<<pC<<", "<<pA<<", "<<pB<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(3, 2, 0, 1)<<"}";

    //------------------------------------------ combinacion 24
    ad = (*pD) - (*pA); // d, c, b, a
    bd = (*pC) - (*pA);
    cd = (*pB) - (*pA);
    _det = Matrix::det(ad.x, ad.y, ad.x*ad.x+ad.y*ad.y,
                       bd.x, bd.y, bd.x*bd.x+bd.y*bd.y,
                       cd.x, cd.y, cd.x*cd.x+cd.y*cd.y);

    result = (almost_equal(_det, 0.000f));
    resultfile<<endl<<"pr0->inCircumcircle("<<pD<<","<<pC<<", "<<pB<<", "<<pA<<"): {esperado: "<<result<<", real: "<<pr0->inCircumcircle(3, 2, 1, 0)<<"}";



    resultfile.flush();
}

void TestCaseGenerator::helper4LowerXandYTesting(PointRepository* pr0, POINT* p0, POINT* p1, POINT* p2)
{

    int lowestx = maxPointCount*2, lowesty = maxPointCount*2;

    POINT* pp = p0;
    if ((p0->x < lowestx) || ((p0->x == lowestx) && (p0->y < pp->y)))
    {
        lowestx = p0->x;
        pp = p0;
    }

    if ((p1->x < lowestx) || ((p1->x == lowestx) && (p1->y < pp->y)))
    {
        lowestx = p1->x;
        pp = p1;
    }

    if ((p2->x < lowestx) || ((p2->x == lowestx) && (p2->y < pp->y)))
        pp = p2;

    vector<int> indexes = pr0->getIndexes();
    int idx = pr0->getLowerMostXCoord(indexes);
    resultfile<<endl<<"pr0->getLowerMostXCoord(): {esperado: "<<pp<<", real: "<<pr0->getPoint(idx)<<"}";

    pp = p0;
    if ((p0->y < lowesty) || ((p0->y == lowesty) && (p0->x < pp->x)))
    {
        lowesty = p0->y;
        pp = p0;
    }

    if ((p1->y < lowesty) || ((p1->y == lowesty) && (p1->x < pp->x)))
    {
        lowesty = p1->y;
        pp = p1;
    }

    if ((p2->y < lowesty) || ((p2->y == lowesty) && (p2->x < pp->x)))
        pp = p2;

    idx = pr0->getLowerMostYCoord(indexes);
    resultfile<<endl<<"pr0->getLowerMostYCoord(): {esperado: "<<pp<<", real: "<<pr0->getPoint(idx)<<"}";
    resultfile.flush();
}

void TestCaseGenerator::helper4SortedPolarAngleTesting(PointRepository* pr0, POINT* p0, POINT* p1, POINT* p2)
{
    resultfile<<"\n ================================================================================= \n";
    resultfile<<"\n Se prueba el metodo de ordenamiento de los puntos segun angulos polares con el punto 0\n";
    resultfile<<"\n ================================================================================= \n";

    int area1 = 0;
    POINT A, B;
    POINT* points[3] = {new POINT(*p0), new POINT(*p1), new POINT(*p2)};
    POINT* p00 = points[0];
    bool swap = false;

    for (int j = 1; j < 3; j++)
    {
        POINT *&pp = points[j];
        A = *(pp) - *(p00); //p0, p2, p1
        for(int k = j+1; k < 2; k++)
        {
            POINT* pk = points[k];
            B = *(pk) - *(p00);
            area1 = A.x*B.y - A.y*B.x;

            if (area1 == 0)
                swap = (p0->squareDistance(*pk) < p0->squareDistance(*pp));
            else
                swap = (area1 < 0);

            if(swap)
            {
                POINT* tp = pk;
                pk = pp;
                pp = tp;
            }
        }
    }

    std::ostringstream foo;
    foo.str("[");
    foo<<points[0]<<", "<<points[1]<<", "<<points[2]<<"] ";
    for(int k=0; k<3; k++)
    {
        delete points[k];
    }

    vector<int> indexes = pr0->getIndexes();
    vector<int> sindexes = pr0->getSortedPolarAngles(indexes);
    std::ostringstream baa;
    baa.str("[");
    int count = indexes.size()-1;

    for (int m = 0; m < count; m++)
    {
        baa<<pr0->getPoint(m)<<", ";

    }
    baa<<pr0->getPoint(count)<<"] ";

    resultfile<<endl<<"pr0->getSorteredAngles(indxs): {esperado: "<<foo.str()<<", real: "<<baa.str()<<"}";
    resultfile.flush();
}

void TestCaseGenerator::testPointRepository()
{
    PointDataGenerator pdg(maxValue);
    resultfile <<"\n\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n "
                 "Prueba de la clase PointRepository. Esta clase es un repositorio de puntos que garantiza el control centralizado de los puntos. \nEsta clase indexa los puntos y permite que las demas clases (como Polygon y en un futuro HalfEdge, Polyhedron, Octree y otras) manipulen los puntos a traves de su indice global.";

    resultfile <<"\nBasicamente PointRepository implementa el patron de diseño Singleton. Y es una lista que gestiona todos los puntos que se generan\n";
    resultfile<<"\n Se realizan "<<trialsCount<<"trials\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n";
    for (int i = 0; i < trialsCount; i++)
    {
        resultfile<<"\nTrial #"<<i;

        this->helper4SingletonChecking();


        POINT* p0 = pdg.randomPoint(-maxValue, maxValue);
        POINT* p1 = pdg.randomPoint(-maxValue, maxValue);
        POINT* p2 = pdg.randomPoint(-maxValue, maxValue);

        resultfile<<endl<<"se generan 3 puntos aleatorios p0 = "<<p0<<", p1 = "<<p1<<"p2 = "<<p2<<" sin orden establecido";

        PointRepository *pr0  = PointRepository::instance();
        pr0->add(p0);
        pr0->add(p1);
        pr0->add(p2);

        this->helper4Area2Testing(pr0, p0, p1, p2);
        this->helper4AreaSignTesting(pr0, p0, p1, p2);
        this->helper4LowerXandYTesting(pr0, p0, p1, p2);
        this->helper4SortedPolarAngleTesting(pr0, p0, p1, p2);

        POINT* p3 = pdg.randomPoint(-maxValue, maxValue);
        resultfile<<endl<<"se genera otro punto aleatorio p3 = "<<p3<<" sin orden establecido";
        pr0->add(p3);
        this->helper4InCircleTesting(pr0, p0, p1, p2, p3);


        PointRepository::instance()->clear();
        resultfile.flush();
    }

}

void TestCaseGenerator::helper4RandomIntTesting()
{
    resultfile<<"\n ================================================================================= \n";
    resultfile<<"\nSe Prueba el metodo PointGenerator::randomInt(int min, int max).";
    resultfile<<"\n Este metodo genera un numero entero x, tal que min <= x <= max.";
    resultfile<<"\n Se haran "<<trialsCount<<" trials para evaluar el comportamiento del metodo.\n";

    cout<<"\nSe Prueba el metodo PointGenerator::randomInt(int min, int max).";
    cout<<"\n Este metodo genera un numero entero x, tal que min <= x <= max.";
    cout<<"\n Se haran "<<trialsCount<<" trials para evaluar el comportamiento del metodo. mediante la desviacion estandar y la varianza\n";
    resultfile<<"\n ================================================================================= \n";

    std::map<int, int> values;
    PointDataGenerator pdg(maxValue);
    for (int i = 0; i < trialsCount; i++)
    {
//        resultfile<<"\nTrial #"<<i;
        cout<<"\nTrial #"<<i;

        int n = pdg.randomInt(-maxValue, maxValue);
        resultfile<<endl<<"Se genera 1 numero aleatorio n = "<<n<<" tal que ("<<-maxValue<<"<= n <= "<<maxValue<<"). ";
        cout<<endl<<"Se genera 1 numero aleatorio n = "<<n<<" tal que ("<<-maxValue<<"<= n <= "<<maxValue<<"). ";

        if (values.insert(std::make_pair(n, 1)).second == false)
            values[n] += 1;
    }

    std::map<int, int>::iterator iti = values.begin();

    int sumi = 0;
    float media = 0.0;

    while(iti != values.end())
        sumi += (*(iti++)).first;

    int divisor = (trialsCount) ? trialsCount : 1;
    media = ((float)sumi)/divisor;

    cout<<"\nmedia = "<<media;

    int total = 0;

    iti = values.begin();
    float varianza = 0.0;

    while(iti != values.end())
    {
        int xi = ((*(iti++)).first);
        total += xi * xi;
    }


    varianza = ((float)total)/divisor - media*media;
    resultfile<<endl<<"La dispersion de ("<<-maxValue<<", "<<maxValue<<"). para "<<trialsCount<<" intentos esta dada por una varianza igual a: "<<varianza;
    resultfile<<endl<<"La dispersion de ("<<-maxValue<<", "<<maxValue<<"). para "<<trialsCount<<" intentos tiene una desviacion estandard de "<<sqrt(varianza);

    cout<<endl<<"La dispersion de ("<<-maxValue<<", "<<maxValue<<"). para "<<trialsCount<<" intentos esta dada por una varianza igual a: "<<varianza;
    cout<<endl<<"La dispersion de ("<<-maxValue<<", "<<maxValue<<"). para "<<trialsCount<<" intentos tiene una desviacion estandard de "<<sqrt(varianza);

    resultfile.flush();
}

void TestCaseGenerator::helper4RandomPointTesting()
{
    resultfile<<"\n ================================================================================= \n";
    resultfile<<"\nSe Prueba el metodo PointGenerator::randomPoint(int min, int max).";
    resultfile<<"\n Este metodo genera un punto x,y tal que min <= x <= max y min <= y <= max.";
    resultfile<<"\n Se haran "<<trialsCount<<" trials para evaluar el comportamiento del metodo.\n";
    resultfile<<"\n NOTA: Para hallar el comportamiento del metodo se halla la medida de dispersion de los puntos x, y.";
    resultfile<<"\n Para hallar esta dispersion seria ideal una funcion f(x,y) que pertenezca a los reales, y sobre la imagen de esta funcion se realizarian los calculos de varianza y desviacion estandar. ";
    resultfile<<"\n La funcion podria obtenerse simulando las coordenadas del pto como coeficientes de un polinomio de primer grado f(x,y) = x * Max + y";
    resultfile<<endl<<"pero esto generaria numeros grandes para Max >= 1000 y por tanto la varianza seria ridiculamente grande. ";
    resultfile<<endl<<"Por lo tanto utilizaremos como funcion g(x,y) = x+y, "<<endl<<"que al ser una funcion sobreyectiva deberia disminuir la dispersion al existir una imagen para multiples pares (x,y), ej g(2,3) = g(5,0) = g(4,1) = 5. "<<endl<<"Pero si con esta funcion obtenemos un alto nivel de varianza y desviacion estandar significa \nque la generacion de puntos es lo suficientemente buena "<<endl<<"porque la naturaleza de g(x,y) disminuyo la varianza y desviacion estandar teorica en varios ordenes de magnitud";
    resultfile<<"\n ================================================================================= \n\n";


    std::map<int, int> values;
    PointDataGenerator pdg(maxValue);
    for (int i = 0; i < trialsCount; i++)
    {
        resultfile<<"\nTrial #"<<i;

        POINT* p = pdg.randomPoint(-maxValue, maxValue);
        resultfile<<endl<<"Se genera 1 punto aleatorio p = "<<p<<" tal que x, y pertenecen a ("<<-maxValue<<", "<<maxValue<<"). ";
        int n = p->x + p->y;
        resultfile<<endl<<"g("<<p->x<<", "<< p->y<<" = "<<n<<"tal que g(x,y) pertenece a ("<<-2*maxValue<<", "<<2*maxValue<<"). ";

        if (values.insert(std::make_pair(n, 1)).second == false)
            values[n] += 1;
    }

    std::map<int, int>::iterator iti = values.begin();

    long long sumi = 0;
    long long media = 0;

    while(iti != values.end())
        sumi += (*(iti++)).first;

    int divisor = (trialsCount) ? trialsCount : 1;
    media = sumi/divisor;

    long long total = 0;

    iti = values.begin();
    long long varianza = 0.0;

    while(iti != values.end())
    {
        long long xi = ((*(iti++)).first);
        total += xi * xi;
    }

    varianza = total/divisor - media*media;
    resultfile<<endl<<"La dispersion de ("<<-maxValue<<", "<<maxValue<<"). para "<<trialsCount<<" intentos esta dada por una varianza igual a: "<<varianza;
    resultfile<<endl<<"La dispersion de ("<<-maxValue<<", "<<maxValue<<"). para "<<trialsCount<<" intentos tiene una desviacion estandard de "<<sqrt(varianza);

    resultfile.flush();
}

void TestCaseGenerator::helper4RandomPointCloudTesting()
{
    resultfile<<"\n ================================================================================= \n";
    resultfile<<"\nSe Prueba el metodo PointGenerator::randomPointCloud(N).";
    resultfile<<"\n Este metodo genera una nube de N puntos donde cada punto x,y cumple que min <= x <= max y min <= y <= max.";
    resultfile<<"\n Se haran "<<trialsCount<<" trials para evaluar el comportamiento del metodo.\n";
    resultfile<<"\n Para hallar el comportamiento del metodo se halla la medida de dispersion de los puntos x, y en cada nube de puntos.\n";

    resultfile<<"\n ================================================================================= \n";


    PointDataGenerator pdg(maxValue);

    vector<int> values;

    for (int i = 0; i < trialsCount; i++)
    {
        resultfile<<"\nTrial #"<<i;
                ;
        int N = pdg.randomInt(1, pow(10, (i%6)+1));
        resultfile<<endl<<"Se genera 1 numero aleatorio N tal que 0 <= N <= 10^6";

        pdg.randomPointCloud(N);
        resultfile<<endl<<"Se genera 1 nube de puntos de "<<N<<" puntos";
        try {

            for (int j = 0; j < N;j++)
            {

                POINT* p = PointRepository::instance()->getPoint(j);
                int n = p->x + p->y;

                values.push_back(n);
            }

            std::vector<int>::iterator iti = values.begin();

            long long sumi = 0;
            long long media = 0;

            while(iti != values.end())
                sumi += *(iti++);

            media = sumi/N;

            long long total = 0;

            iti = values.begin();
            long long varianza = 0.0;

            while(iti != values.end())
            {
                long long xi = *(iti++);
                total += xi * xi;
            }

            varianza = total/N - media*media;
            resultfile<<endl<<"Dispersion de Nube de Puntos No."<<i+1<<": {Varianza = "<<varianza<<", Desviacion estandar = "<<sqrt(varianza)<<"}";
        }
        catch (exception e)
        {
            cout <<" ha ocurrido un error grave: "<<e.what()<<endl;
        }
        PointRepository::instance()->clear();
        values.clear();
    }
    resultfile.flush();

}

void TestCaseGenerator::testPointDataGenerator()
{
    PointDataGenerator pdg(100);
    resultfile <<"\n\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n "
                 "Prueba de la clase PointDataGenerator.";
    resultfile<<" Esta clase es la encargada de implementar los algoritmos de generacion de puntos y valores aleatorios\n";
    resultfile<<"\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n"<<endl;

    this->helper4RandomIntTesting();
    this->helper4RandomPointTesting();
    this->helper4RandomPointCloudTesting();
}


TestCaseGenerator::~TestCaseGenerator()
{
    resultfile.close();
}