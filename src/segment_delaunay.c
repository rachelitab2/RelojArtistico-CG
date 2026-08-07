#include <math.h>
#include <GL/freeglut.h>

#include "segment_delaunay.h"
#include "utils.h"

/*
 * Segmento Delaunay: cadena ritmica de aros (inspirado en Sonia Delaunay).
 *
 * Composicion validada con un prototipo HTML/canvas antes de portarla:
 * cinco aros en fila horizontal, alternando tamano (grande / conector
 * negro / grande / conector negro / mediano), todos apoyados sobre un
 * mismo eje horizontal. Cada aro grande tiene la mitad superior como un
 * domo SOLIDO (negro hasta el centro, con un borde de color fino) y la
 * mitad inferior como un anillo HUECO (solo el borde lleva color, el
 * resto deja ver el fondo) -- esa asimetria es lo que hace que el borde
 * plano del domo negro, igual en todos los aros, se lea como un eje
 * comun en vez de cinco circulos sueltos.
 *
 * Sin animacion, a proposito (mismo criterio que segment_riley.c).
 */

static const float COLOR_BLUE        [3] = {0.173f, 0.306f, 0.522f};
static const float COLOR_GREEN       [3] = {0.290f, 0.608f, 0.306f};
static const float COLOR_GREEN_LIGHT [3] = {0.361f, 0.682f, 0.373f};
static const float COLOR_GOLD        [3] = {0.851f, 0.663f, 0.231f};
static const float COLOR_ORANGE_GOLD [3] = {0.851f, 0.604f, 0.227f};
static const float COLOR_RED         [3] = {0.761f, 0.227f, 0.165f};
static const float COLOR_YELLOW      [3] = {0.906f, 0.808f, 0.333f};
static const float COLOR_BLACK       [3] = {0.102f, 0.102f, 0.094f};

void drawDelaunayBackground(float innerRadius, float outerRadius, float halfAngle)
{
    /* Fondo neutral claro, igual que en la referencia real */
    glColor3f(0.937f, 0.918f, 0.878f);
    drawFilledArc(innerRadius, outerRadius, -halfAngle, halfAngle);
}

typedef struct
{
    float cx;
    float radius;
    const float *topColor;
    const float *bottomColor;
    const float *haloTopColor;    /* NULL si no lleva halo */
    const float *haloBottomColor; /* NULL si no lleva halo */
} RingSpec;

/* Posiciones y radios trasladados directamente del prototipo (canvas
   600x600, centro en 300,300, eje de la cadena en y=330) al lienzo
   local -1..1: local = (pixel - 300) / 300. */
static const RingSpec RINGS[5] =
{
    { -0.6500f, 0.2833f, COLOR_BLUE,        COLOR_GREEN_LIGHT, NULL,       NULL         },
    { -0.3267f, 0.1067f, COLOR_BLACK,       COLOR_BLACK,       NULL,       NULL         },
    {  0.0467f, 0.3333f, COLOR_GREEN,       COLOR_RED,         COLOR_GOLD, COLOR_YELLOW },
    {  0.4200f, 0.1067f, COLOR_BLACK,       COLOR_BLACK,       NULL,       NULL         },
    {  0.6867f, 0.2267f, COLOR_ORANGE_GOLD, COLOR_GREEN,       NULL,       NULL         }
};

static const float CHAIN_LINE_Y         = -0.10f;
static const float CHAIN_LINE_THICKNESS =  0.033f;

/* Domo negro solido (mitad superior, angulo 0-180) mas borde de color
   fino sobre el domo; banda de color hueca en la mitad inferior
   (angulo 180-360), con el centro sin dibujar para que se vea el fondo. */
static void drawRing(const RingSpec *ring)
{
    float topRimIn = ring->radius * 0.72f;
    float botRimIn = ring->radius * 0.66f;

    glPushMatrix();
        glTranslatef(ring->cx, CHAIN_LINE_Y, 0.0f);

        if(ring->haloTopColor != NULL)
        {
            glColor3f(ring->haloTopColor[0], ring->haloTopColor[1], ring->haloTopColor[2]);
            drawFilledArc(ring->radius, ring->radius * 1.12f, 0.0f, 180.0f);
        }

        if(ring->haloBottomColor != NULL)
        {
            glColor3f(ring->haloBottomColor[0], ring->haloBottomColor[1], ring->haloBottomColor[2]);
            drawFilledArc(ring->radius, ring->radius * 1.10f, 180.0f, 360.0f);
        }

        glColor3f(COLOR_BLACK[0], COLOR_BLACK[1], COLOR_BLACK[2]);
        drawFilledArc(0.0f, ring->radius, 0.0f, 180.0f);

        glColor3f(ring->topColor[0], ring->topColor[1], ring->topColor[2]);
        drawFilledArc(topRimIn, ring->radius, 0.0f, 180.0f);

        glColor3f(ring->bottomColor[0], ring->bottomColor[1], ring->bottomColor[2]);
        drawFilledArc(botRimIn, ring->radius, 180.0f, 360.0f);

    glPopMatrix();
}

/* La barra que atraviesa toda la composicion en CHAIN_LINE_Y. Se dibuja
   antes que los aros para que quede visible solo en los huecos entre
   ellos -- donde hay un aro, su propio domo negro ya cubre la linea con
   el mismo tono, asi que se percibe como un solo eje continuo. */
static void drawChainLine(void)
{
    glColor3f(COLOR_BLACK[0], COLOR_BLACK[1], COLOR_BLACK[2]);

    glBegin(GL_QUADS);
        glVertex2f(-1.0f, CHAIN_LINE_Y - CHAIN_LINE_THICKNESS * 0.5f);
        glVertex2f( 1.0f, CHAIN_LINE_Y - CHAIN_LINE_THICKNESS * 0.5f);
        glVertex2f( 1.0f, CHAIN_LINE_Y + CHAIN_LINE_THICKNESS * 0.5f);
        glVertex2f(-1.0f, CHAIN_LINE_Y + CHAIN_LINE_THICKNESS * 0.5f);
    glEnd();
}

/*
 * Angulo acumulado en la matriz de modelado (ver nota identica en
 * segment_demuth.c): el lienzo local vive dentro del glRotatef del
 * sector, y esta composicion -- igual que el "5" de Demuth -- tiene que
 * leerse siempre derecha, sin importar en que angulo de la galeria caiga
 * el sector.
 */
static float getCurrentRotationDegrees(void)
{
    float matrix[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

    return atan2f(matrix[1], matrix[0]) * (180.0f / 3.14159265f);
}

void drawDelaunay(void)
{
    static const float COMPOSITION_SCALE = 1.06f;

    float uprightAngle = -getCurrentRotationDegrees();
    int i;

    glPushMatrix();

        glRotatef(uprightAngle, 0.0f, 0.0f, 1.0f);
        glScalef(COMPOSITION_SCALE, COMPOSITION_SCALE, 1.0f);

        drawChainLine();

        for(i = 0; i < 5; i++)
            drawRing(&RINGS[i]);

    glPopMatrix();
}
