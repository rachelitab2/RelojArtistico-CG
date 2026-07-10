#include <GL/freeglut.h>

#include <math.h>

#include "clock.h"
#include "utils.h"

void drawClock(void)
{
    int i;

    glColor3f(0.15f,0.15f,0.15f);

    drawFilledCircle(0.0f,0.0f,0.35f);

    glColor3f(1.0f,1.0f,1.0f);

    drawCircle(0.0f,0.0f,0.35f);


    for(i=0;i<12;i++)
    {
        float angle=degreesToRadians(i*30);

        float x1=0.30f*sin(angle);
        float y1=0.30f*cos(angle);

        float x2=0.34f*sin(angle);
        float y2=0.34f*cos(angle);

        drawLine(x1,y1,x2,y2);
    }
}