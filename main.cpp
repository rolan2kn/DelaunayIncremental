#include <iostream>
#include "./src/TestCaseGenerator.h"
#include "./src/HalfEdgeTestCaseGenerator.h"
#include "./src/DrawTriangulation.h"
#include <GL/glut.h>


GLfloat xRotated, yRotated, zRotated;
GLdouble radius=1;
GLfloat ratio ;
int ancho, alto;




void display(void);
void reshape(int x, int y);

DrawTriangulation* drawTriangulation = nullptr;

void display(void)
{
    drawTriangulation->render();
}

void reshape(int x, int y)
{
    ancho = x;
    alto = y;

    // No debemos dividir por cero al calcular el ratio
    if(alto == 0) alto = 1;

// Establezco mi view port
    glViewport(0, 0, ancho, alto);

// Inicializo la matriz de proyeccion
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

// Calculo el ratio
    ratio = (float)ancho / (float)alto;
    if (ancho <= alto)
        glOrtho (-100, 100.0, -100 / ratio, 100.0 / ratio, 1.0, -1.0);
    else
        glOrtho (-100.0 * ratio, 100.0 * ratio, -100.0, 100.0, 1.0, -1.0);
}

void keyboard (unsigned char key, int x, int y)
{
    drawTriangulation->keyboard(key, x, y);
//    cout<<endl<<"DISTANCE CHANGED: "<<drawTriangulation->getDistance();
    glutPostRedisplay();
}

void arrowKeys (int key, int x, int y)
{
    drawTriangulation->arrowKeys(key, x, y);
//    cout<<endl<<"CAMERA CHANGED: (xi="<<drawTriangulation->getXi()<<", yi="<<drawTriangulation->getYi()<<");";
    glutPostRedisplay();
}

void mouseControllerFunc(int button, int state, int x, int y)
{
    /**
     * donde los parámetros que la función nos da (automáticamente y sin tener que hacer nada) son los siguientes:

    button, un entero que puede tomar los valores GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON o GLUT_RIGHT_BUTTON
     según el usuario haya pulsado el botón izquierdo, el del medio o el derecho, respectivamente.
     Estas constantes estan predefinidas y podemos usarlas sin problema en nuestro código pues
     GLUT las interpretará correctamente.

    state, puede tomar los valores GLUT_UP o GLUT_DOWN, según si se ha pulsado/soltado el correspondiente botón.

    X e Y, son las coordenadas referidas a la ventana de visualización, no al mundo virtual, en las que se pulsó/soltó el susodicho botón.

     * */
     if (!drawTriangulation->isManualMode())
         return;


     int max = drawTriangulation->getMaxValue();
     int min = -drawTriangulation->getMaxValue();


     int h = alto;
     int w = ancho;
     if(state == GLUT_UP)
     {
         switch (button)
         {
             case GLUT_LEFT_BUTTON:
                 try {
//                     cout<<endl<<"SOLTE BOTON IZQUIERDO en x="<<x<<", y="<<y<<")";

                     float ancho_total_seccion_espacio;
                     float alto_total_seccion_espacio;
                     float relacionX, relacionY;

                     if (ratio<1)
                     {
                         ancho_total_seccion_espacio = max - min; // Coord X
                         alto_total_seccion_espacio = max/ratio - (min/ratio); // Coord Y
                     }else{
                         ancho_total_seccion_espacio = max*ratio - (min*ratio);// Coord X
                         alto_total_seccion_espacio = max - (min); // Coord Y
                     }

                     relacionX = ancho_total_seccion_espacio/w;
                     relacionY = alto_total_seccion_espacio/h;

                     float espX, espY;
                     espX = (x-w/2) * relacionX;
                     espY = (y-h/2) * relacionY;

                     char cadena [350];
                     sprintf (cadena, "Pantalla: %d, %d --- Espacio: %f, %f",x,y,espX,espY);
                     glutSetWindowTitle(cadena);
//                     cout<<endl<<"Pantalla: "<<x<<","<<y<<" --- Espacio: "<<espX<<","<<espY;
                     drawTriangulation->addPointToTriangulation(espX, espY);
                 }
                 catch(exception& e)
                 {
                     cout <<endl<<e.what();
                 }




                 glutPostRedisplay();
                 break;
             case GLUT_MIDDLE_BUTTON:
//                 cout<<endl<<"SOLTE BOTON DEL MEDIO en x="<<x<<", y="<<y<<")";
                 break;
             case GLUT_RIGHT_BUTTON:
//                 cout<<endl<<"SOLTE BOTON DERECHO en x="<<x<<", y="<<y<<")";
                 break;
         }
         glutPostRedisplay();
     }

}

/* Funciones de Control del menú seleccionado */
/* Se ejecutan cuando el usuario utilize los menús */

void menu_nivel_1(int identificador){

    /* Según la opción de 1er nivel activada, ejecutaré una rutina u otra */

    switch( identificador){
        case 0:                 // add random point
            drawTriangulation->addRandomPointToTriangulation();
            glutPostRedisplay();
            break;
        case 1:                 // add point cloud
            drawTriangulation->addPointCloudToTriangulation();
            drawTriangulation->setMode(DrawTriangulation::GRID);
            glutPostRedisplay();
            break;
        case 2:                 // add grid
            drawTriangulation->addPointGridToTriangulation();
            drawTriangulation->setMode(DrawTriangulation::GRID);
            glutPostRedisplay();
            break;
        case 3:                 // set manual mode to use mouse
            drawTriangulation->setMode(DrawTriangulation::MANUAL);
            glutPostRedisplay();
            break;
        case 4:                 // reset
            drawTriangulation->reset();
            glutPostRedisplay();
            break;
        default:
            exit( -1 );
    }
}

void mouseMotionControllerFunc(GLsizei x, GLsizei y)
{
//    glutPostRedisplay();
}

// Ejecución principal
int main(int argc, char **argv) {
    drawTriangulation = new DrawTriangulation(MAX_VALUE, 5000, 5000);

    glutInit(&argc, argv);
    glutInitWindowSize(5000,5000);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("Delaunay Triangulation");

    /* Creación del menú */
    int submenu = glutCreateMenu(menu_nivel_1);

    glutAddMenuEntry("Adicionar Punto Aleatorio", 0);
    glutAddMenuEntry("Adicionar Nube de Puntos Aleatorios", 1);
    glutAddMenuEntry("Adicionar Grilla", 2);
    glutAddMenuEntry("Modo Manual", 3);
    glutAddMenuEntry("Reset", 4);
    glutAddMenuEntry("Salir", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowKeys);
    glutMouseFunc( mouseControllerFunc);
    glutMotionFunc( mouseMotionControllerFunc);
    glutMainLoop();


//    HalfEdgeTestCaseGenerator testCaseGenerator("analysis_half.txt", 100, 100, 100);
//    testCaseGenerator.testALL();

    return 0;
}

