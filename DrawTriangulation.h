//
// Created by rkindela on 08-04-19.
//

#ifndef PMTOOL_DRAWTRIANGULATION_H
#define PMTOOL_DRAWTRIANGULATION_H



#include <limits>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "triangulation.h"
#include <GL/glut.h>
#include "utils.h"
#include "PointDataGenerator.h"

using namespace std;


class DrawTriangulation {
public:
    enum MODE {RANDOM, MANUAL, GRID};
private:
    Triangulation* triangulation;
    int width;
    int height;
    float distance;
    float xi;
    float yi;
    float wZ;
    DrawTriangulation::MODE mode;
    PointDataGenerator pdg;
public:

    DrawTriangulation(int maxval, int wX, int wY, int wZ = 5000);
    inline void setDistance(float d = 6) {this->distance = (d < 6) ? 6 : d;}
    inline float getDistance(){return distance;}
    inline float getXi(){return xi;}
    inline float getYi(){return yi;}
    inline float getWZ(){return wZ;}
    inline  int getWidth(){return width;}
    inline int getHeight(){return height;}
    inline int getMaxValue(){ return pdg.getMaximum()*10;}
    inline bool isManualMode() const {return (this->mode == MANUAL);}
    void addRandomPointToTriangulation();
    void addPointGridToTriangulation();
    void reset();
    void drawTriangles();
    void drawPoints();
    void addPointToTriangulation(int x, int y);
    void addPointCloudToTriangulation();
    void render ();
    void keyboard (unsigned char key, int x, int y);
    void arrowKeys (int key, int x, int y);
    void setMode(DrawTriangulation::MODE _mode);
};


#endif //PMTOOL_DRAWTRIANGULATION_H
