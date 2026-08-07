#include <GL/freeglut.h>

#include "segment_malevich.h"
#include "utils.h"

/*
 * Segmento Malevich: "Cuadrado negro y cuadrado rojo" (1915)
 * Suprematismo - Estatico, proporcionado y limpio dentro del lienzo local (-1 a 1).
 */

void drawMalevichBackground(float innerRadius, float outerRadius, float halfAngle)
{
    /* Fondo limpio blanco-crema caracteristico del Suprematismo */
    glColor3f(0.950f, 0.945f, 0.930f);
    drawFilledArc(innerRadius, outerRadius, -halfAngle, halfAngle);
}

static void drawSquare(float cx, float cy, float size, float angleDegrees, float r, float g, float b)
{
    float half = size * 0.5f;

    glPushMatrix();

    glTranslatef(cx, cy, 0.0f);
    glRotatef(angleDegrees, 0.0f, 0.0f, 1.0f);

    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(-half, -half);
        glVertex2f( half, -half);
        glVertex2f( half,  half);
        glVertex2f(-half,  half);
    glEnd();

    glPopMatrix();
}

void drawMalevich(void)
{
    /* Fondo del lienzo local */
    glColor3f(0.950f, 0.945f, 0.930f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    /* Cuadrado negro (estatico, superior izquierdo) */
    drawSquare(-0.15f, 0.22f, 0.72f, 0.0f, 0.05f, 0.05f, 0.05f);

    /* Cuadrado rojo (estatico, inclinado ~15 deg, inferior derecho) */
    drawSquare(0.18f, -0.32f, 0.46f, -15.0f, 0.78f, 0.12f, 0.04f);
}
