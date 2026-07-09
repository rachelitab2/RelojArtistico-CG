#include <GL/freeglut.h>

#include "display.h"
#include "clock.h"
#include "segments.h"

void initDisplay(void)
{
    glClearColor(0.05f,0.05f,0.05f,1.0f);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawClock();

    drawSegments();

    glutSwapBuffers();
}