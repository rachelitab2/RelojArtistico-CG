#include <GL/freeglut.h>

#include "display.h"

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800,600);

    glutCreateWindow("Reloj Artistico");

    initDisplay();

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}