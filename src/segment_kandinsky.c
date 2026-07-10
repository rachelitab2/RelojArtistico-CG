#include <GL/freeglut.h>

#include "segment_kandinsky.h"
#include "utils.h"

/* Fondo con la geometria real del sector: rejilla de bandas
   radiales x angulares con paleta abstracta y contrastante. */
static const float RADIUS_FRACTION[5] = {0.00f, 0.22f, 0.50f, 0.78f, 1.00f};
static const float ANGLE_DEG[5]       = {-25.0f,-13.0f,  1.0f, 14.0f, 25.0f};

/* 0 = crema, 1 = rojo, 2 = azul, 3 = amarillo, 4 = negro, 5 = teal */
static const int CELL_COLOR[4][4] =
{
    { 4, 1, 0, 2 },
    { 3, 0, 5, 1 },
    { 0, 2, 4, 0 },
    { 5, 0, 1, 3 }
};

static const float PALETTE[6][3] =
{
    {0.92f,0.88f,0.78f},
    {0.82f,0.18f,0.16f},
    {0.15f,0.30f,0.75f},
    {0.93f,0.75f,0.15f},
    {0.08f,0.08f,0.08f},
    {0.10f,0.55f,0.55f}
};

static void drawBlocks(float innerRadius, float outerRadius)
{
    float span = outerRadius - innerRadius;
    int row, col;

    for(row = 0; row < 4; row++)
    {
        float r0 = innerRadius + span * RADIUS_FRACTION[row];
        float r1 = innerRadius + span * RADIUS_FRACTION[row+1];

        for(col = 0; col < 4; col++)
        {
            const float *color = PALETTE[CELL_COLOR[row][col]];

            glColor3f(color[0], color[1], color[2]);

            drawFilledArc(r0, r1, ANGLE_DEG[col], ANGLE_DEG[col+1]);
        }
    }
}

void drawKandinskyBackground(float innerRadius, float outerRadius)
{
    drawBlocks(innerRadius, outerRadius);
}

/* Circulos concentricos superpuestos, motivo recurrente de Kandinsky */
static void drawCircles(void)
{
    glColor3f(0.95f,0.95f,0.92f);
    drawFilledCircle(0.10f, 0.15f, 0.32f);

    glColor3f(0.82f,0.18f,0.16f);
    drawFilledCircle(0.10f, 0.15f, 0.21f);

    glColor3f(0.15f,0.30f,0.75f);
    drawFilledCircle(0.10f, 0.15f, 0.11f);

    glColor3f(0.93f,0.75f,0.15f);
    drawFilledCircle(-0.45f,-0.40f, 0.16f);
}

/* Lineas rectas que cruzan la composicion, otro motivo tipico */
static void drawLines(void)
{
    glColor3f(0.08f,0.08f,0.10f);

    glLineWidth(3.0f);
    drawLine(-0.90f,-0.70f, 0.85f, 0.60f);

    glLineWidth(2.0f);
    drawLine(-0.80f, 0.75f, 0.70f,-0.55f);

    glLineWidth(4.0f);
    drawLine(-0.95f, 0.10f, 0.95f, 0.30f);
}

void drawKandinsky(void)
{
    drawLines();

    drawCircles();
}
