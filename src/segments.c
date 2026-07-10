#include <math.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "segments.h"
#include "utils.h"
#include "segment_kandinsky.h"

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
static const float ARTWORK_MARGIN = 0.85f;

static void drawArtwork(ArtworkType artwork);
static void drawArtworkBackground(const Segment *segment);

void drawSegment(const Segment *segment)
{
glPushMatrix();

glRotatef(segment->angle + wheelRotation, 0.0f, 0.0f, 1.0f);

drawArtworkBackground(segment);

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

{
    float midRadius = (segment->innerRadius + segment->outerRadius) * 0.5f;

    float radialHalfWidth = (segment->outerRadius - segment->innerRadius) * 0.5f;

    float angularHalfWidth = midRadius * tanf(degreesToRadians(25.0f));

    float canvasScale = (radialHalfWidth < angularHalfWidth ? radialHalfWidth : angularHalfWidth) * ARTWORK_MARGIN;

    glPushMatrix();

        glTranslatef(midRadius, 0.0f, 0.0f);

        glScalef(canvasScale, canvasScale, 1.0f);

        drawArtwork(segment->artwork);

    glPopMatrix();
}

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
    wheelRotation += 0.01f;

    if(wheelRotation >= 360.0f)
        wheelRotation = 0.0f;

    glutPostRedisplay();
}

static void drawArtwork(ArtworkType artwork)
{
    switch(artwork)
    {
        case ART_HOKUSAI:
            break;

        case ART_VANGOGH:
            break;

        case ART_KANDINSKY:
            drawKandinsky();
            break;

        case ART_MONDRIAN:
            break;

        case ART_MONET:
            break;

        case ART_KLIMT:
            break;
    }
}

/* Pinta el fondo respetando la forma real del sector (arco relleno
   entre innerRadius y outerRadius), evitando huecos entre el arte
   y el borde curvo del segmento. Cada obra puede pintar su propio
   fondo; las que aun no tienen contenido usan el azul de referencia. */
static void drawArtworkBackground(const Segment *segment)
{
    switch(segment->artwork)
    {
        case ART_KANDINSKY:
            drawKandinskyBackground(segment->innerRadius, segment->outerRadius);
            break;

        default:
            glColor3f(0.30f,0.30f,0.80f);
            drawFilledArc(segment->innerRadius, segment->outerRadius, -25.0f, 25.0f);
            break;
    }
}
