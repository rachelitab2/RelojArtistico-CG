#include <stdio.h>
#include <GL/freeglut.h>

#include "display.h"
#include "segments.h"

initDisplay();
initSegments();

void reshape(int width, int height)
{
    if(height == 0)
        height = 1;

    float aspect = (float)width / (float)height;

    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(aspect >= 1.0f)
    {
        glOrtho(-aspect, aspect,
                -1.0f, 1.0f,
                -1.0f, 1.0f);
    }
    else
    {
        glOrtho(-1.0f,1.0f,
                -1.0f/aspect,
                 1.0f/aspect,
                -1.0f,1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800,600);

    glutCreateWindow("Reloj Artistico");

    initDisplay();

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    glutTimerFunc(16, timer, 0);

    glutMainLoop();

    return 0;
}