#include <GL/freeglut.h>

#include "segment_malevich_suprematist.h"
#include "utils.h"

/*
 * Kazimir Malevich - "Composicion Suprematista" (1916).
 *
 * Formas geometricas planas que "flotan" sin gravedad ni horizonte sobre
 * fondo blanco. Las posiciones y tamanios de cada pieza estan medidos
 * sobre la reproduccion de referencia (fracciones del lienzo original,
 * ancho:alto ~= 1.79:1) y convertidos al lienzo local -1..1 preservando
 * esa proporcion, para que la composicion se lea igual que el original
 * en vez de una interpretacion libre. Composicion estatica: se dibuja
 * directo en el lienzo local, sin contra-rotacion (el desorden orientado
 * ya es parte del lenguaje visual de la obra).
 */

static const float BLACK[3]  = {0.06f, 0.06f, 0.07f};
static const float GREEN[3]  = {0.10f, 0.55f, 0.30f};
static const float BLUE[3]   = {0.08f, 0.10f, 0.55f};
static const float ORANGE[3] = {0.92f, 0.62f, 0.10f};
static const float RED[3]    = {0.82f, 0.16f, 0.10f};
static const float YELLOW[3] = {0.94f, 0.76f, 0.10f};

static void drawRect(float cx, float cy, float w, float h, float angleDeg,
                      const float color[3])
{
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;

    glPushMatrix();

    glTranslatef(cx, cy, 0.0f);
    glRotatef(angleDeg, 0.0f, 0.0f, 1.0f);

    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
        glVertex2f(-halfW, -halfH);
        glVertex2f( halfW, -halfH);
        glVertex2f( halfW,  halfH);
        glVertex2f(-halfW,  halfH);
    glEnd();

    glPopMatrix();
}

void drawMalevichSuprematistBackground(float innerRadius, float outerRadius, float halfAngle)
{
    glColor3f(0.960f, 0.955f, 0.940f);
    drawFilledArc(innerRadius, outerRadius, -halfAngle, halfAngle);
}

void drawMalevichSuprematist(void)
{
    static const float TILT = -8.0f;

    /* fondo del lienzo local */
    glColor3f(0.960f, 0.955f, 0.940f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    /* --- grupo izquierdo: barras negras + banda verde --- */
    drawRect(-0.81f,  0.341f, 0.22f, 0.436f, TILT, BLACK);   /* barra negra superior */
    drawRect(-0.49f,  0.515f, 0.14f, 0.090f, TILT, BLACK);   /* rectangulo negro chico */
    drawRect(-0.41f,  0.341f, 0.04f, 0.257f, TILT - 2.0f, BLACK); /* barra negra fina */
    drawRect(-0.41f, -0.056f, 0.20f, 1.450f, TILT, GREEN);   /* banda verde diagonal larga */
    drawRect(-0.815f,-0.112f, 0.31f, 0.380f, TILT, BLACK);   /* barra negra inferior */
    drawRect(-0.75f, -0.320f, 0.02f, 0.520f, TILT, ORANGE);  /* linea naranja fina */

    /* --- grupo derecho: cluster naranja/azul/negro + linea roja + amarillos --- */
    drawRect( 0.30f,  0.375f, 0.12f, 0.123f, TILT, ORANGE);  /* rectangulo naranja */
    drawRect( 0.37f,  0.476f, 0.08f, 0.056f, TILT, BLUE);    /* guiones azules arriba */
    drawRect( 0.16f,  0.277f, 0.08f, 0.073f, TILT, BLUE);    /* barra azul 1 */
    drawRect( 0.24f,  0.285f, 0.08f, 0.056f, TILT, BLACK);   /* rectangulo negro chico */
    drawRect( 0.165f, 0.196f, 0.07f, 0.090f, TILT, BLUE);    /* barra azul 2 */
    drawRect( 0.42f,  0.137f, 0.02f, 0.460f, -35.0f, RED);   /* linea roja diagonal larga */

    drawRect( 0.465f, 0.201f, 0.15f, 0.313f, TILT, YELLOW);  /* barra amarilla 1 */
    drawRect( 0.66f,  0.246f, 0.10f, 0.246f, TILT, YELLOW);  /* barra amarilla 2 */
    drawRect( 0.392f, 0.078f, 0.096f,0.090f, TILT, YELLOW);  /* cuadrado amarillo chico */
    drawRect( 0.745f, 0.064f, 0.11f, 0.129f, TILT, YELLOW);  /* rectangulo amarillo */
    drawRect( 0.68f, -0.090f, 0.24f, 0.179f, TILT, YELLOW);  /* rectangulo amarillo grande */

    /* --- gran forma azul, abajo a la derecha (trapecio con borde diagonal) --- */
    glColor3f(BLUE[0], BLUE[1], BLUE[2]);
    glBegin(GL_POLYGON);
        glVertex2f( 0.13f, -0.050f);
        glVertex2f( 0.73f, -0.050f);
        glVertex2f( 0.73f, -0.559f);
        glVertex2f(-0.14f, -0.559f);
    glEnd();
}
