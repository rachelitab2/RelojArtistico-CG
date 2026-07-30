#include <math.h>
#include <GL/freeglut.h>

#include "segment_mondrian.h"
#include "utils.h"

/* Bandas radiales (filas) y angulares (columnas) irregulares que
   subdividen el sector real -- 4x4 = 16 celdas. Cada celda es un
   sub-sector del abanico (no un rectangulo recto), asi que la
   composicion llena el sector de borde a borde, sin el halo de
   fondo que dejaba el lienzo cuadrado local. */
static const float RADIUS_FRACTION[5] = {0.00f, 0.24f, 0.48f, 0.76f, 1.00f};

/* fracciones del semiancho real del sector, no grados fijos (ver
   nota equivalente en segment_kandinsky.c) */
static const float ANGLE_FRACTION[5]  = {-1.00f,-0.44f,-0.08f, 0.40f, 1.00f};

/* 0 = blanco, 1 = rojo, 2 = azul, 3 = amarillo */
static const int CELL_COLOR[4][4] =
{
    /* col0 col1 col2 col3 */
    { 2, 0, 0, 3 },  /* fila 0 (radio interior) */
    { 0, 0, 1, 0 },  /* fila 1 */
    { 1, 0, 0, 2 },  /* fila 2 */
    { 0, 3, 0, 0 }   /* fila 3 (radio exterior) */
};

static const float PALETTE[4][3] =
{
    {0.95f,0.95f,0.92f}, /* blanco (ligeramente calido, igual que el resto de la paleta) */
    {0.85f,0.15f,0.12f}, /* rojo */
    {0.12f,0.32f,0.82f}, /* azul */
    {0.95f,0.82f,0.12f}  /* amarillo */
};

/* misma tecnica de rejilla que vangogh.c/kandinsky.c/klimt.c, copiada
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

/* Grosor variable por linea para que la rejilla se sienta
   dibujada a mano, no perfectamente uniforme. */
static void drawGridLines(float innerRadius, float outerRadius, float halfAngle)
{
    static const float radialLineWidth[5]  = {4.5f, 3.0f, 4.0f, 2.5f, 4.5f};
    static const float arcLineWidth[5]     = {4.5f, 2.5f, 4.0f, 3.0f, 4.5f};

    float span = outerRadius - innerRadius;
    int i;

    glColor3f(0.08f,0.08f,0.08f);

    /* lineas radiales (una por cada division angular) */
    for(i = 0; i < 5; i++)
    {
        float angle = degreesToRadians(ANGLE_FRACTION[i] * halfAngle);

        float x1 = cosf(angle) * innerRadius;
        float y1 = sinf(angle) * innerRadius;
        float x2 = cosf(angle) * outerRadius;
        float y2 = sinf(angle) * outerRadius;

        glLineWidth(radialLineWidth[i]);

        drawLine(x1, y1, x2, y2);
    }

    /* arcos (uno por cada division radial) */
    for(i = 0; i < 5; i++)
    {
        float r = innerRadius + span * RADIUS_FRACTION[i];

        glLineWidth(arcLineWidth[i]);

        drawArc(r, ANGLE_FRACTION[0] * halfAngle, ANGLE_FRACTION[4] * halfAngle);
    }
}

void drawMondrianBackground(float innerRadius,
                            float outerRadius,
                            float halfAngle)
{
    drawBlocks(innerRadius, outerRadius, halfAngle);

    drawGridLines(innerRadius, outerRadius, halfAngle);
}

/* CUESTIONABLE: sin primer plano; a diferencia de las otras 5 obras,
   toda la composicion de Mondrian vive en drawMondrianBackground()
   (es intencional: es solo bloques y lineas, fiel al estilo real). */
void drawMondrian(void)
{
}
