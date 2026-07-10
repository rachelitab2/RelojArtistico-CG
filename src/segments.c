#include <math.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "segments.h"
#include "utils.h"

static float rotation = 0.0f;

void drawSegment(float angle)
{
glPushMatrix();

glRotatef(angle + rotation,0.0f,0.0f,1.0f);

glColor3f(0.30f,0.30f,0.80f);

drawFilledArc(
    0.42f,
    0.82f,
    -25.0f,
     25.0f
);

glColor3f(1.0f,1.0f,1.0f);

drawArc(
    0.82f,
    -25.0f,
     25.0f
);

drawArc(
    0.42f,
    -25.0f,
     25.0f
);

drawLine(0.42f,0.0f,0.82f,0.0f);

float x1 = cos(degreesToRadians(25))*0.42f;
float y1 = sin(degreesToRadians(25))*0.42f;

float x2 = cos(degreesToRadians(25))*0.82f;
float y2 = sin(degreesToRadians(25))*0.82f;

drawLine(x1,y1,x2,y2);

x1 = cos(degreesToRadians(-25))*0.42f;
y1 = sin(degreesToRadians(-25))*0.42f;

x2 = cos(degreesToRadians(-25))*0.82f;
y2 = sin(degreesToRadians(-25))*0.82f;

drawLine(x1,y1,x2,y2);

glPopMatrix();
}
void drawSegments(void)
{
drawSegment(0.0f);

    drawSegment(60.0f);

    drawSegment(120.0f);

    drawSegment(180.0f);

    drawSegment(240.0f);

    drawSegment(300.0f);
}
void updateSegments(void)
{
    rotation += 1.0f;

    if(rotation >= 360.0f)
        rotation = 0.0f;

    glutPostRedisplay();
}