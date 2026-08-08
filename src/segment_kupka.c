#include <math.h>
#include <GL/freeglut.h>

#include "segment_kupka.h"
#include "utils.h"

/*
 * Frantisek Kupka - "Amorfa, fuga en dos colores" (1912, Orfismo)
 *
 * Espiral abstracta de manchas de color sin una figura reconocible ni
 * una orientacion "correcta" fija -- igual criterio que Riley: se
 * dibuja polar-nativo (coordenadas reales del sector) por zonas
 * radiales con una textura moteada pseudo-aleatoria, sin contra-
 * rotacion. La progresion de zonas (gris oscuro -> rojo/azul ->
 * azul/negro -> claro con manchas negras -> rosa/azul -> gris fino)
 * estiliza el barrido de izquierda a derecha del cuadro original como
 * un barrido de adentro hacia afuera del sector.
 */

static const float COLOR_GREY_DARK[3] = {0.318f, 0.302f, 0.263f};
static const float COLOR_BLACK[3]     = {0.078f, 0.075f, 0.086f};
static const float COLOR_RED[3]       = {0.706f, 0.145f, 0.169f};
static const float COLOR_BLUE[3]      = {0.243f, 0.278f, 0.596f};
static const float COLOR_CREAM[3]     = {0.937f, 0.910f, 0.827f};
static const float COLOR_LAVENDER[3]  = {0.706f, 0.718f, 0.827f};
static const float COLOR_PINK[3]      = {0.878f, 0.616f, 0.616f};
static const float COLOR_GREY_LIGHT[3] = {0.788f, 0.792f, 0.827f};

static float hashf(float x, float y)
{
    float s = sinf(x * 127.1f + y * 311.7f) * 43758.5453f;

    return s - floorf(s);
}

static void mixColor(float out[3], const float a[3], const float b[3], float t)
{
    out[0] = a[0] + (b[0] - a[0]) * t;
    out[1] = a[1] + (b[1] - a[1]) * t;
    out[2] = a[2] + (b[2] - a[2]) * t;
}

/* Zona radial (t en 0..1) -> par de colores que se alternan en ese
   anillo, moteados via hashf. */
static void pickZoneColors(float t, const float **colorA, const float **colorB)
{
    if(t < 0.12f)
    {
        *colorA = COLOR_GREY_DARK; *colorB = COLOR_BLACK;
    }
    else if(t < 0.30f)
    {
        *colorA = COLOR_RED; *colorB = COLOR_BLUE;
    }
    else if(t < 0.48f)
    {
        *colorA = COLOR_BLUE; *colorB = COLOR_BLACK;
    }
    else if(t < 0.74f)
    {
        *colorA = COLOR_CREAM; *colorB = COLOR_LAVENDER;
    }
    else if(t < 0.90f)
    {
        *colorA = COLOR_PINK; *colorB = COLOR_BLUE;
    }
    else
    {
        *colorA = COLOR_LAVENDER; *colorB = COLOR_GREY_LIGHT;
    }
}

static void drawDot(float x, float y, float r, const float color[3])
{
    static const int STEPS = 8;
    int i;

    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);

        for(i = 0; i <= STEPS; i++)
        {
            float a = 2.0f * 3.1415926535f * (float)i / (float)STEPS;

            glVertex2f(x + r * cosf(a), y + r * sinf(a));
        }
    glEnd();
}

/* pasada de manchas oscuras finas en la banda clara del medio, como
   los trazos negros del original */
static void drawDashAccents(float innerRadius, float outerRadius, float halfAngle)
{
    static const float RADIAL_STEP  = 0.03f;
    static const float ANGULAR_STEP = 4.0f;

    float span = outerRadius - innerRadius;
    float r;

    for(r = innerRadius + span * 0.46f; r < innerRadius + span * 0.76f; r += RADIAL_STEP)
    {
        float a;

        for(a = -halfAngle; a < halfAngle; a += ANGULAR_STEP)
        {
            float n = hashf(r * 5.0f, a * 3.0f);
            float jr, ja, rad, x, y;

            if(n > 0.6f)
                continue; /* deja huecos, no cubre toda la banda */

            jr = r + (hashf(r, a) - 0.5f) * RADIAL_STEP;
            ja = a + (hashf(a, r + 1.0f) - 0.5f) * ANGULAR_STEP;
            rad = degreesToRadians(ja);
            x = cosf(rad) * jr;
            y = sinf(rad) * jr;

            drawDot(x, y, 0.010f + hashf(r, a + 2.0f) * 0.006f, COLOR_BLACK);
        }
    }
}

void drawKupkaBackground(float innerRadius, float outerRadius, float halfAngle)
{
    static const float CELL_DEG = 5.0f;
    static const int   ROWS     = 20;

    float span = outerRadius - innerRadius;
    int maxCol = (int)(halfAngle / CELL_DEG) + 2;
    int row, col;

    for(row = 0; row < ROWS; row++)
    {
        float t0 = (float)row / (float)ROWS;
        float t1 = (float)(row + 1) / (float)ROWS;
        float tMid = (t0 + t1) * 0.5f;
        float r0 = innerRadius + span * t0;
        float r1 = innerRadius + span * t1;

        const float *colorA;
        const float *colorB;

        pickZoneColors(tMid, &colorA, &colorB);

        for(col = -maxCol; col <= maxCol; col++)
        {
            float a0 = (float)col * CELL_DEG;
            float a1 = a0 + CELL_DEG;
            float n;
            float blend[3];

            if(a0 < -halfAngle) a0 = -halfAngle;
            if(a1 >  halfAngle) a1 =  halfAngle;
            if(a1 <= a0) continue;

            n = hashf(tMid * 37.0f + (float)row, (a0 + a1) * 0.5f * 0.7f);

            /* mezcla en vez de elegir un color puro: da la sensacion
               de mancha de acuarela en lugar de mosaico duro */
            mixColor(blend, colorA, colorB, (n > 0.55f) ? 1.0f : 0.0f + n * 0.3f);

            glColor3f(blend[0], blend[1], blend[2]);
            drawFilledArc(r0, r1, a0, a1);
        }
    }

    drawDashAccents(innerRadius, outerRadius, halfAngle);
}

void drawKupka(void) {} /* vacio, todo en el fondo */
