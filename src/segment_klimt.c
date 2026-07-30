#include <math.h>
#include <GL/freeglut.h>

#include "segment_klimt.h"
#include "utils.h"

/* Fondo con la geometria real del sector: mosaico de bandas
   radiales x angulares en tonos dorados con acentos oscuros. */
static const float RADIUS_FRACTION[5] = {0.00f, 0.24f, 0.50f, 0.76f, 1.00f};

/* fracciones del semiancho real del sector, no grados fijos (ver
   nota equivalente en segment_kandinsky.c) */
static const float ANGLE_FRACTION[5]  = {-1.00f,-0.40f, 0.08f, 0.52f, 1.00f};

/* 0 = dorado claro, 1 = dorado oscuro, 2 = negro, 3 = rojo profundo */
static const int CELL_COLOR[4][4] =
{
    { 1, 0, 2, 0 },
    { 0, 3, 0, 1 },
    { 2, 0, 1, 0 },
    { 0, 1, 0, 3 }
};

static const float PALETTE[4][3] =
{
    {0.85f,0.68f,0.28f},
    {0.65f,0.48f,0.14f},
    {0.08f,0.08f,0.08f},
    {0.55f,0.10f,0.12f}
};

/* misma tecnica de rejilla que vangogh.c/kandinsky.c/mondrian.c, copiada
   con distinta paleta (ver nota en segment_vangogh.c) */
static void drawBlocks(float innerRadius, float outerRadius, float halfAngle)
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
            float a0 = ANGLE_FRACTION[col] * halfAngle;
            float a1 = ANGLE_FRACTION[col+1] * halfAngle;

            glColor3f(color[0], color[1], color[2]);

            drawFilledArc(r0, r1, a0, a1);
        }
    }
}

void drawKlimtBackground(float innerRadius, float outerRadius, float halfAngle)
{
    drawBlocks(innerRadius, outerRadius, halfAngle);
}

/* Puntos dorados dispersos, motivo textil recurrente en Klimt */
static void drawGoldDots(void)
{
    static const float dots[][3] =
    {
        {-0.55f, 0.45f, 0.09f},
        {-0.15f, 0.55f, 0.07f},
        { 0.30f, 0.40f, 0.08f},
        {-0.40f,-0.10f, 0.07f},
        { 0.10f,-0.05f, 0.10f},
        { 0.50f,-0.20f, 0.07f},
        {-0.10f,-0.55f, 0.08f}
    };

    const int count = sizeof(dots) / sizeof(dots[0]);
    int i;

    glColor3f(0.96f,0.86f,0.45f);

    for(i = 0; i < count; i++)
        drawFilledCircle(dots[i][0], dots[i][1], dots[i][2]);
}

/* Espiral simple hecha con segmentos, otro motivo de Klimt.
   Parametrica: angulo y radio crecen juntos con "t" de 0 a 1. */
static void drawSpiral(void)
{
    int i;
    const int steps = 40;

    glColor3f(0.08f,0.08f,0.08f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_STRIP);

    for(i = 0; i <= steps; i++)
    {
        float t = (float)i / steps;
        float angle = t * 4.0f * 3.1415926535f;
        float radius = 0.06f + t * 0.45f;

        glVertex2f(cosf(angle) * radius, sinf(angle) * radius - 0.20f);
    }

    glEnd();
}

/* primer plano: espiral debajo, puntos dorados encima */
void drawKlimt(void)
{
    drawSpiral();

    drawGoldDots();
}
