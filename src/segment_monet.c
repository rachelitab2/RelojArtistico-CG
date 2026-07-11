#include <GL/freeglut.h>

#include "segment_monet.h"
#include "utils.h"

/* Fondo con la geometria real del sector: bandas suaves en
   verdes y azules, transicion tenue tipo estanque impresionista.
   (variante mas simple de la rejilla radio x angulo de las otras
   obras: aqui solo se subdivide por radio, sin columnas angulares) */
static const float RADIUS_FRACTION[5] = {0.00f, 0.30f, 0.55f, 0.78f, 1.00f};

static const float BAND_COLOR[4][3] =
{
    {0.14f,0.34f,0.30f},
    {0.18f,0.46f,0.38f},
    {0.24f,0.56f,0.50f},
    {0.34f,0.64f,0.56f}
};

static void drawBands(float innerRadius, float outerRadius)
{
    float span = outerRadius - innerRadius;
    int row;

    for(row = 0; row < 4; row++)
    {
        float r0 = innerRadius + span * RADIUS_FRACTION[row];
        float r1 = innerRadius + span * RADIUS_FRACTION[row+1];

        glColor3f(BAND_COLOR[row][0], BAND_COLOR[row][1], BAND_COLOR[row][2]);

        drawFilledArc(r0, r1, -25.0f, 25.0f);
    }
}

void drawMonetBackground(float innerRadius, float outerRadius)
{
    drawBands(innerRadius, outerRadius);
}

/* Puente curvo: aproximado con segmentos de linea siguiendo un arco */
static void drawBridge(void)
{
    static const float points[][2] =
    {
        {-0.70f,-0.10f}, {-0.40f, 0.18f}, {-0.05f, 0.28f},
        { 0.30f, 0.20f}, { 0.60f,-0.05f}
    };

    const int count = sizeof(points) / sizeof(points[0]);
    int i;

    glColor3f(0.55f,0.30f,0.20f);
    glLineWidth(4.5f);

    glBegin(GL_LINE_STRIP);
    for(i = 0; i < count; i++)
        glVertex2f(points[i][0], points[i][1]);
    glEnd();
}

/* Nenufares dispersos sobre el agua: circulo verde (hoja) con un
   circulo mas chico y claro encima (flor) */
static void drawLilyPads(void)
{
    static const float pads[][3] =
    {
        {-0.55f,-0.55f,0.14f},
        {-0.10f,-0.65f,0.11f},
        { 0.35f,-0.50f,0.15f},
        { 0.05f,-0.15f,0.10f},
        {-0.35f,-0.20f,0.09f}
    };

    const int count = sizeof(pads) / sizeof(pads[0]);
    int i;

    for(i = 0; i < count; i++)
    {
        glColor3f(0.20f,0.45f,0.30f);
        drawFilledCircle(pads[i][0], pads[i][1], pads[i][2]);

        glColor3f(0.90f,0.75f,0.85f);
        drawFilledCircle(pads[i][0], pads[i][1], pads[i][2]*0.35f);
    }
}

/* primer plano: nenufares abajo, puente arriba */
void drawMonet(void)
{
    drawLilyPads();

    drawBridge();
}
