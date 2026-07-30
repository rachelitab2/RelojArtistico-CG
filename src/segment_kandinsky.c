#include <GL/freeglut.h>

#include "segment_kandinsky.h"
#include "utils.h"

/* Fondo con la geometria real del sector: rejilla de bandas
   radiales x angulares con paleta abstracta y contrastante. */
static const float RADIUS_FRACTION[5] = {0.00f, 0.22f, 0.50f, 0.78f, 1.00f};

/* fracciones del semiancho real del sector, no grados fijos: el sector
   activo puede ser mas ancho que uno normal (ver sectorHalfAngle en
   segments.c), y la rejilla debe cubrirlo completo en vez de dejar
   franjas vacias a los lados. */
static const float ANGLE_FRACTION[5]  = {-1.00f,-0.52f, 0.04f, 0.56f, 1.00f};

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

/* misma tecnica de rejilla que vangogh.c/mondrian.c/klimt.c, copiada
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

void drawKandinskyBackground(float innerRadius, float outerRadius, float halfAngle)
{
    drawBlocks(innerRadius, outerRadius, halfAngle);
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

/* Lineas rectas que cruzan la composicion, otro motivo tipico.
   CUESTIONABLE: el color (0.08,0.08,0.10) casi coincide pero no es
   identico al negro "unificado" (0.08,0.08,0.08) que usan las demas
   obras (mondrian, klimt) para sus lineas/contornos. */
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

/* primer plano: lineas debajo, circulos encima */
void drawKandinsky(void)
{
    drawLines();

    drawCircles();
}
