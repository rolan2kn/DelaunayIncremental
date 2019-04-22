//
// Created by rkindela on 08-04-19.
//

#include "DrawTriangulation.h"
#include <cmath>
#include "utils.h"


DrawTriangulation::DrawTriangulation(int maxval, int wX, int wY, int _wZ)
: width(wX), height(wY), distance(300), xi (0), yi(0), wZ(_wZ), pdg(maxval)
{
    triangulation = new Triangulation(maxval);

    triangulation->initData();
}

//**********************************************************************

void DrawTriangulation::render ()
{
    glPolygonMode(GL_FRONT, GL_POLYGON);
    glPolygonMode(GL_BACK, GL_POINT);
    glDisable( GL_DEPTH_TEST ) ;


    // fijar viewport y transformación de proyección:

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity() ;

//    glViewport(0, 0, this->width, this->height) ;
//    glOrtho( -100.0f, 100.0f, -100.0f, +100.0f, 1.0, -1);
    glFrustum( -100.0f, 100.0f, -100.0f, +100.0f, 30.0, wZ);


    // fijar transformación de vista:
    glMatrixMode( GL_MODELVIEW ) ;
    glLoadIdentity() ;
    glTranslatef(xi, yi,  -this->distance); // alejar para que el observador
    // quede fuera del objeto

//    glRotatef(0,1,0,0);
//    glRotatef(0,0,1,0);

    // limpiar imagen

    glClearColor( 1.0, 1.0, 1.0, 1.0 ) ;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  ) ;
    // fijar modo de visualización en alambre:

    this->drawTriangles();
    this->drawPoints();

    glutSwapBuffers() ;


} // Practica::Redibujar

void DrawTriangulation::drawTriangles()
{

    glBegin(GL_TRIANGLES);

    int size = triangulation->getFacesCount();
    float r, g, b;

    for( unsigned long i = 0 ; i < size ; i++ )
    {
        Face* tri = triangulation->getFace(i);

        if( !tri->color.wasAssigned)
        {
            r = pdg.randomInt(0, 255)/255.0f;
            g = pdg.randomInt(0, 255)/255.0f;
            b = pdg.randomInt(0, 255)/255.0f;
            tri->setColor(FaceColor(r, g, b));
        }
        else
        {
            r = tri->color.r;
            g = tri->color.g;
            b = tri->color.b;
        }

        glColor3f(r, g, b) ; // yellow

        vector<int>* idxs = tri->getVertexesIndices();
        int vsize = idxs->size();

        for( int j = 0 ; j < vsize ; j++ )
        {
            int pointidx = (*idxs)[j];
            POINT* v = PointRepository::instance()->getPoint(pointidx);
            glVertex3d(v->x,v->y,v->z);
        }
    }

    glEnd() ;

    // mostrar en pantalla el array de pixels actualizado


}

void DrawTriangulation::drawPoints()
{

    int size = PointRepository::instance()->getPointCount();
    int rad = 3;
    int angle = 30;
    int times = 360 / angle;

    for( unsigned long i = 0 ; i < size ; i++ ) {
        glBegin(GL_POLYGON);
            POINT *v = PointRepository::instance()->getPoint(i);
            glColor3f(1, 0, 0); // red

            // Dibujamos el primer vértice

            POINT p(-rad, rad, 0);
            glVertex3d(v->x+p.x, v->y+p.y, v->z+p.z);

            /* Dibujamos los 5 vértices restantes haciendo rotar
            * el anterior 60º, utilizaremos el equivalente en
            * radianes ya que las funciones sin y cos del
            * paquete Math de Java trabaja con estos últimos */

            for (int j = 0; j< times;j++) {

                float x = p.x * cos(toRadians(angle)) - p.y * sin(toRadians(angle));

                float y = p.x * sin(toRadians(angle)) + p.y * cos(toRadians(angle));

                // Dibujamos el nuevo vértice obtenido
                glVertex3d(v->x+x, v->y+y, v->z+p.z);
                p.x  = x;
                p.y = y;
            }
        glEnd() ;
    }



}

void DrawTriangulation::keyboard (unsigned char key, int x, int y)
{
        switch (key)
        {
            case 'f':
                this->distance += 10;
                break;
            case 'p':
                this->addRandomPointToTriangulation();
                break;

            case 'n':
                this->distance -= 10;
                break;
                case 27: case 'q':
                exit(0);
        }
}

void DrawTriangulation::arrowKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_RIGHT:
            xi += 20.5;
            break;

        case GLUT_KEY_LEFT:
            xi -= 20.5;
            break;

        case GLUT_KEY_UP:
            yi += 20.5;
            break;

        case GLUT_KEY_DOWN:
            yi -= 20.5;
            break;
    }
}

void DrawTriangulation::setMode(DrawTriangulation::MODE _mode)
{
    this->mode = _mode;
}

void DrawTriangulation::addRandomPointToTriangulation()
{
    try
    {
        int last = PointRepository::instance()->getPointCount();
        POINT* newp = pdg.randomPoint(-pdg.getMaximum(), pdg.getMaximum());
        PointRepository::instance()->add(newp);
        triangulation->addPoint(last);
    }
    catch (exception& e)
    {
        cout<<endl<<e.what();
    }

}

void DrawTriangulation::addPointToTriangulation(int x, int y)
{
    if (!this->isManualMode())
        return;

    int last = PointRepository::instance()->getPointCount();
    POINT* newp = new POINT(x, y);
    PointRepository::instance()->add(newp);
    triangulation->addPoint(last);
}

void DrawTriangulation::addPointCloudToTriangulation()
{
    if(mode == GRID)
        return;
    int pointCount = pdg.randomInt(1, pdg.getMaximum());

    pdg.randomPointCloud(pointCount);
    for(int i = 0; i < pointCount; i++)
        triangulation->addPoint(i);
}

void DrawTriangulation::addPointGridToTriangulation()
{
    if(mode == GRID)
        return;
    int degree = pdg.randomInt(5, pdg.getMaximum());

    pdg.randomPointGrid(degree);

    int pointCount = PointRepository::instance()->getPointCount();
    for(int i = 0; i < pointCount; i++)
        triangulation->addPoint(i);
    triangulation->legalizeALL();
}

void DrawTriangulation::reset()
{
    triangulation->reset();
    PointRepository::instance()->clear();
    triangulation->initData();
}
