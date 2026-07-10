#include <math.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "segments.h"
#include "utils.h"
#include "segment_hokusai.h"

Segment segments[6];

void initSegments(void)
{
    segments[0] = (Segment){0.0f,   0.42f,0.82f,0.0f,ART_HOKUSAI};

    segments[1] = (Segment){60.0f,  0.42f,0.82f,0.0f,ART_VANGOGH};

    segments[2] = (Segment){120.0f, 0.42f,0.82f,0.0f,ART_KANDINSKY};

    segments[3] = (Segment){180.0f, 0.42f,0.82f,0.0f,ART_MONDRIAN};

    segments[4] = (Segment){240.0f, 0.42f,0.82f,0.0f,ART_MONET};

    segments[5] = (Segment){300.0f, 0.42f,0.82f,0.0f,ART_KLIMT};
}

static float wheelRotation = 0.0f;

static void drawArtwork(ArtworkType artwork);

void drawSegment(const Segment *segment)
{
glPushMatrix();

glRotatef(segment->angle + wheelRotation, 0.0f, 0.0f, 1.0f);

glColor3f(0.30f,0.30f,0.80f);

drawFilledArc(
   segment->innerRadius
  ,segment->outerRadius,
    -25.0f,
     25.0f
);

glColor3f(1.0f,1.0f,1.0f);

drawArc(
    segment->outerRadius,
    -25.0f,
     25.0f
);

drawArc(
    segment->innerRadius,
    -25.0f,
     25.0f
);

drawLine(segment->innerRadius,0.0f,segment->outerRadius,0.0f);

float x1 = cos(degreesToRadians(25))*segment->innerRadius;
float y1 = sin(degreesToRadians(25))*segment->innerRadius;

float x2 = cos(degreesToRadians(25))*segment->outerRadius;
float y2 = sin(degreesToRadians(25))*segment->outerRadius;

drawLine(x1,y1,x2,y2);

x1 = cos(degreesToRadians(-25))*segment->innerRadius;
y1 = sin(degreesToRadians(-25))*segment->innerRadius;

x2 = cos(degreesToRadians(-25))*segment->outerRadius;
y2 = sin(degreesToRadians(-25))*segment->outerRadius;

drawLine(x1,y1,x2,y2);

drawArtwork(segment->artwork);

glPopMatrix();
}
void drawSegments(void)
{
    int i;

    for(i = 0; i < 6; i++)
    {
        drawSegment(&segments[i]);
    }
}
void updateSegments(void)
{
    wheelRotation += 1.0f;

    if(wheelRotation >= 360.0f)
        wheelRotation = 0.0f;

    glutPostRedisplay();
}

static void drawArtwork(ArtworkType artwork)
{
    switch(artwork)
    {
        case ART_HOKUSAI:
         drawHokusai();
            break;

        case ART_VANGOGH:
            break;

        case ART_KANDINSKY:
            break;

        case ART_MONDRIAN:
            break;

        case ART_MONET:
            break;

        case ART_KLIMT:
            break;
    }
}