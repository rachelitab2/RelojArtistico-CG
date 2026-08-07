#include <math.h>
#include <GL/freeglut.h>

#include "segment_taeuber_arp.h"
#include "utils.h"

/*
 * Segmento Sophie Taeuber-Arp: "Composicion de circulos y semicirculos" (1930)
 * Arte Concreto - Reticula de circulos y semicirculos rotatorios armonicos.
 */

static const float COLORS[5][3] = {
    {0.902f, 0.318f, 0.000f}, /* naranja */
    {0.000f, 0.675f, 0.757f}, /* turquesa */
    {0.984f, 0.753f, 0.176f}, /* amarillo */
    {1.000f, 1.000f, 1.000f}, /* blanco */
    {0.067f, 0.067f, 0.067f}  /* negro */
};

void drawTaeuberArpBackground(float innerRadius, float outerRadius, float halfAngle)
{
    /* Fondo azul marino / grisaceo */
    glColor3f(0.118f, 0.157f, 0.212f);
    drawFilledArc(innerRadius, outerRadius, -halfAngle, halfAngle);
}

void drawTaeuberArp(void)
{
    float t = glutGet(GLUT_ELAPSED_TIME) * 0.001f;

    const int GRID = 6;
    const float CELL_GL = 2.0f / (float)GRID;
    const float RADIUS  = CELL_GL * 0.43f;
    const float INNER_R = RADIUS * 0.28f;

    int step = ((int)floorf(t / 3.0f)) % 4;
    float baseRotationDeg = (float)step * 90.0f;

    int row, col, i;

    /* Reticula de circulos */
    for(row = 0; row < GRID; row++)
    {
        for(col = 0; col < GRID; col++)
        {
            float cx = -1.0f + ((float)col + 0.5f) * CELL_GL;
            float cy =  1.0f - ((float)row + 0.5f) * CELL_GL;

            /* Fondo del circulo */
            const float *cBg = COLORS[(row + col) % 5];
            glColor3f(cBg[0], cBg[1], cBg[2]);
            drawFilledCircle(cx, cy, RADIUS);

            /* Semicirculo rotatorio */
            glPushMatrix();
            glTranslatef(cx, cy, 0.0f);

            float rotDeg = baseRotationDeg + ((float)(row + col) * 10.31f);
            glRotatef(rotDeg, 0.0f, 0.0f, 1.0f);

            const float *cSemi = COLORS[(row * 2 + col + 2) % 5];
            glColor3f(cSemi[0], cSemi[1], cSemi[2]);
            drawFilledArc(0.0f, RADIUS, 0.0f, 180.0f);

            /* Circulo interior */
            const float *cCore = COLORS[(row * 3 + col + 3) % 5];
            glColor3f(cCore[0], cCore[1], cCore[2]);
            drawFilledCircle(0.0f, 0.0f, INNER_R);

            glPopMatrix();
        }
    }

    /* Lineas sutiles de la reticula */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.08f);
    glLineWidth(1.0f);

    for(i = 0; i <= GRID; i++)
    {
        float pos = -1.0f + (float)i * CELL_GL;
        drawLine(pos, -1.0f, pos, 1.0f);
        drawLine(-1.0f, pos, 1.0f, pos);
    }
}
