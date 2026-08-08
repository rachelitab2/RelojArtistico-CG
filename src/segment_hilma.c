#include <GL/freeglut.h>

#include "segment_hilma.h"
#include "utils.h"

/*
 * Hilma af Klint - "El Cisne, No. 17" (serie "El Cisne", c. 1915)
 *
 * Circulo dividido en dos mitades por una linea vertical, cada una con
 * su propio juego de anillos concentricos: izquierda negro/blanco,
 * derecha rosa/amarillo/azul. Composicion simetrica sin una
 * orientacion "correcta" fija -- gira con su sector como
 * Delaunay/Malevich, sin contra-rotacion.
 *
 * El circulo se dibuja en el lienzo local (-1..1), no en las
 * coordenadas reales del sector: drawFilledArc() no le importa en que
 * transform este, asi que sirve igual aca que para el fondo polar de
 * otras obras.
 */

static const float COLOR_BG[3]     = {0.659f, 0.247f, 0.165f};
static const float COLOR_BLACK[3]  = {0.055f, 0.050f, 0.047f};
static const float COLOR_WHITE[3]  = {0.925f, 0.910f, 0.870f};
static const float COLOR_PINK[3]   = {0.851f, 0.549f, 0.478f};
static const float COLOR_YELLOW[3] = {0.878f, 0.718f, 0.204f};
static const float COLOR_BLUE[3]   = {0.310f, 0.459f, 0.678f};

void drawHilmaBackground(float innerRadius, float outerRadius, float halfAngle)
{
    glColor3f(COLOR_BG[0], COLOR_BG[1], COLOR_BG[2]);
    drawFilledArc(innerRadius, outerRadius, -halfAngle, halfAngle);
}

void drawHilma(void)
{
    static const float R_OUTER = 0.92f;
    static const float R_MID   = 0.68f;
    static const float R_INNER = 0.32f;

    /* mitad izquierda (90 a 270 grados): negro, luego un anillo blanco */
    glColor3f(COLOR_BLACK[0], COLOR_BLACK[1], COLOR_BLACK[2]);
    drawFilledArc(0.0f, R_MID, 90.0f, 270.0f);

    glColor3f(COLOR_WHITE[0], COLOR_WHITE[1], COLOR_WHITE[2]);
    drawFilledArc(R_MID, R_OUTER, 90.0f, 270.0f);

    /* mitad derecha (-90 a 90 grados): rosa, anillo amarillo, anillo azul */
    glColor3f(COLOR_PINK[0], COLOR_PINK[1], COLOR_PINK[2]);
    drawFilledArc(0.0f, R_INNER, -90.0f, 90.0f);

    glColor3f(COLOR_YELLOW[0], COLOR_YELLOW[1], COLOR_YELLOW[2]);
    drawFilledArc(R_INNER, R_MID, -90.0f, 90.0f);

    glColor3f(COLOR_BLUE[0], COLOR_BLUE[1], COLOR_BLUE[2]);
    drawFilledArc(R_MID, R_OUTER, -90.0f, 90.0f);
}
