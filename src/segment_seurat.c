#include <math.h>
#include <GL/freeglut.h>

#include "segment_seurat.h"
#include "utils.h"

/*
 * Segmento Seurat: "Un domingo en la Grande Jatte" (1886, Puntillismo)
 *
 * Composicion validada con un prototipo HTML/canvas antes de portarla:
 * fondo puntillista (agua + cesped, a base de muchos puntitos de color,
 * nunca un color solido) y tres siluetas PLANAS reconocibles encima
 * (mujer con sombrilla, pareja sentada, perro, mas dos veleros chicos)
 * -- las siluetas se dejan lisas a proposito para que se lean claras
 * contra el fondo texturado, en vez de competir con el.
 *
 * Sin animacion: el hash pseudo-aleatorio depende solo de la posicion
 * de cada punto, no del tiempo (mismo criterio que Riley/Alma Thomas).
 */

static const float COLOR_WATER1[3]       = {0.243f, 0.431f, 0.557f};
static const float COLOR_WATER2[3]       = {0.361f, 0.576f, 0.659f};
static const float COLOR_WATER3[3]       = {0.561f, 0.749f, 0.788f};
static const float COLOR_GRASS1[3]       = {0.361f, 0.541f, 0.235f};
static const float COLOR_GRASS2[3]       = {0.490f, 0.651f, 0.282f};
static const float COLOR_GRASS3[3]       = {0.651f, 0.757f, 0.408f};
static const float COLOR_GRASS4[3]       = {0.290f, 0.459f, 0.188f};
static const float COLOR_SKIN[3]         = {0.890f, 0.725f, 0.549f};
static const float COLOR_NAVY[3]         = {0.110f, 0.153f, 0.251f};
static const float COLOR_NAVY_LIGHT[3]   = {0.180f, 0.239f, 0.361f};
static const float COLOR_BROWN[3]        = {0.478f, 0.231f, 0.180f};
static const float COLOR_PARASOL[3]      = {0.769f, 0.337f, 0.165f};
static const float COLOR_BLACK[3]        = {0.067f, 0.067f, 0.067f};
static const float COLOR_WHITE[3]        = {0.949f, 0.937f, 0.902f};

/* ---------------------------------------------------------------- */
/* Fondo: puntillismo sobre la geometria real del sector             */
/* ---------------------------------------------------------------- */

/* pseudo-aleatorio determinista, sin estado -- misma pareja (x,y)
   siempre da el mismo resultado, para que la trama no cambie de frame
   a frame */
static float hashf(float x, float y)
{
    float s = sinf(x * 127.1f + y * 311.7f) * 43758.5453f;

    return s - floorf(s);
}

/* drawFilledCircle() de utils.c usa 64 segmentos, pensado para circulos
   grandes; para un punto de radio ~0.01 eso es miles de vertices
   desperdiciados por nada (el poligono es indistinguible de un circulo
   a ese tamano). Con cientos de puntos por fotograma en esta trama, la
   diferencia es real: un octogono aca corta el costo por punto ~8x sin
   cambio visible. */
static void drawDot(float x, float y, float radius)
{
    static const int STEPS = 8;
    int i;

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);

        for(i = 0; i <= STEPS; i++)
        {
            float angle = 2.0f * 3.1415926535f * (float)i / (float)STEPS;

            glVertex2f(x + radius * cosf(angle), y + radius * sinf(angle));
        }
    glEnd();
}

void drawSeuratBackground(float innerRadius, float outerRadius, float halfAngle)
{
    /* la version anterior (0.014 / 1.6) se veia bien pero generaba del
       orden de 2000 puntos en el sector activo -- de a 64 segmentos por
       drawFilledCircle(), eso es mas de 100000 vertices por fotograma
       solo para esta obra, y se cruzo con un crash del driver grafico
       (ver docs/09-Diagnostico-Pantallazo-VIDEO-SCHEDULER.md) al
       cambiar de sala. Este paso mas grande + drawDot() de 8 lados baja
       el costo por fotograma sin perder la base solida de abajo. */
    static const float RADIAL_STEP  = 0.026f;
    static const float ANGULAR_STEP = 3.0f;   /* grados */
    static const float WATER_FRACTION = 0.32f; /* del span, desde innerRadius */

    const float *WATER_PALETTE[3] = { COLOR_WATER1, COLOR_WATER2, COLOR_WATER3 };
    const float *GRASS_PALETTE[4] = { COLOR_GRASS1, COLOR_GRASS2, COLOR_GRASS3, COLOR_GRASS4 };

    float span = outerRadius - innerRadius;
    float waterLimit = innerRadius + span * WATER_FRACTION;
    float r;

    /* base solida por zona: los puntos van ENCIMA, no reemplazan el
       relleno -- garantiza que no se vea negro por mas ralos que
       queden los puntos en algun punto de la trama */
    glColor3f(COLOR_WATER1[0], COLOR_WATER1[1], COLOR_WATER1[2]);
    drawFilledArc(innerRadius, waterLimit, -halfAngle, halfAngle);

    glColor3f(COLOR_GRASS4[0], COLOR_GRASS4[1], COLOR_GRASS4[2]);
    drawFilledArc(waterLimit, outerRadius, -halfAngle, halfAngle);

    for(r = innerRadius; r < outerRadius; r += RADIAL_STEP)
    {
        float a;

        for(a = -halfAngle; a < halfAngle; a += ANGULAR_STEP)
        {
            float jr = r + (hashf(r, a) - 0.5f) * RADIAL_STEP * 0.9f;
            float ja = a + (hashf(a, r) - 0.5f) * ANGULAR_STEP * 0.9f;
            float rad = degreesToRadians(ja);

            float x = cosf(rad) * jr;
            float y = sinf(rad) * jr;

            float dotRadius = 0.007f + hashf(r, a + 1.0f) * 0.005f;

            const float *color;

            if(jr < waterLimit)
                color = WATER_PALETTE[(int)(hashf(r * 0.5f, a * 0.5f) * 3.0f) % 3];
            else
                color = GRASS_PALETTE[(int)(hashf(r * 0.5f, a * 0.5f) * 4.0f) % 4];

            glColor3f(color[0], color[1], color[2]);
            drawDot(x, y, dotRadius);
        }
    }
}

/* ---------------------------------------------------------------- */
/* Primer plano: siluetas planas                                     */
/* ---------------------------------------------------------------- */

static void drawFilledTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

static void drawFilledQuad(float x1, float y1, float x2, float y2,
                            float x3, float y3, float x4, float y4)
{
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
        glVertex2f(x4, y4);
    glEnd();
}

static void drawFilledEllipse(float cx, float cy, float rx, float ry)
{
    static const int STEPS = 24;
    int i;

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);

        for(i = 0; i <= STEPS; i++)
        {
            float rad = degreesToRadians((float)i * (360.0f / (float)STEPS));

            glVertex2f(cx + rx * cosf(rad), cy + ry * sinf(rad));
        }
    glEnd();
}

/* Velero: casco (trapecio) + vela (triangulo). Todas las coordenadas
   internas estan en "pixeles de prototipo" (canvas de referencia
   600x600); glScalef(scale/300) las lleva al lienzo local -1..1. */
static void drawSailboat(float x, float y, float scale)
{
    float s = scale / 300.0f;

    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(s, s, 1.0f);

        glColor3f(COLOR_WHITE[0], COLOR_WHITE[1], COLOR_WHITE[2]);
        drawFilledQuad(-14.0f, 0.0f,  14.0f, 0.0f,  9.0f, -8.0f,  -9.0f, -8.0f);

        glColor3f(COLOR_NAVY_LIGHT[0], COLOR_NAVY_LIGHT[1], COLOR_NAVY_LIGHT[2]);
        drawFilledTriangle(0.0f, 26.0f,  0.0f, 0.0f,  -11.0f, 0.0f);

    glPopMatrix();
}

/* La figura mas iconica del cuadro: vestido largo, cabeza con sombrero
   chico, sombrilla como abanico detras de la cabeza. Todo plano, sin
   detalle interno -- se reconoce por silueta. */
static void drawParasolWoman(float x, float y, float scale)
{
    static const int FAN_STEPS = 14;
    static const float FAN_START_DEG = 1.08f * 180.0f;
    static const float FAN_END_DEG   = 1.62f * 180.0f;

    float s = scale / 300.0f;
    int i;

    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(s, s, 1.0f);

        /* vestido */
        glColor3f(COLOR_NAVY[0], COLOR_NAVY[1], COLOR_NAVY[2]);
        drawFilledTriangle(0.0f, 70.0f,  34.0f, -90.0f,  -34.0f, -90.0f);

        /* cabeza */
        glColor3f(COLOR_SKIN[0], COLOR_SKIN[1], COLOR_SKIN[2]);
        drawFilledCircle(0.0f, 84.0f, 11.0f);

        /* sombrero */
        glColor3f(COLOR_BLACK[0], COLOR_BLACK[1], COLOR_BLACK[2]);
        drawFilledEllipse(0.0f, 92.0f, 15.0f, 5.0f);

        /* sombrilla: abanico detras y arriba de la cabeza (angulos
           tomados del prototipo canvas, con Y invertido) */
        glColor3f(COLOR_PARASOL[0], COLOR_PARASOL[1], COLOR_PARASOL[2]);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(6.0f, 96.0f);

            for(i = 0; i <= FAN_STEPS; i++)
            {
                float deg = FAN_START_DEG + (FAN_END_DEG - FAN_START_DEG) * ((float)i / (float)FAN_STEPS);
                float rad = degreesToRadians(deg);

                glVertex2f(6.0f + 46.0f * cosf(rad), 96.0f - 46.0f * sinf(rad));
            }
        glEnd();

        glColor3f(COLOR_BLACK[0], COLOR_BLACK[1], COLOR_BLACK[2]);
        glLineWidth(2.0f);
        drawLine(6.0f, 96.0f, 6.0f, 20.0f);

    glPopMatrix();
}

/* Pareja sentada: dos manchas ovaladas con cabeza y sombrero, sin
   detalle -- el grupo de la izquierda del cuadro original. */
static void drawSeatedFigure(float dx, const float bodyColor[3])
{
    glColor3f(bodyColor[0], bodyColor[1], bodyColor[2]);
    drawFilledEllipse(dx, 0.0f, 26.0f, 16.0f);

    glColor3f(COLOR_SKIN[0], COLOR_SKIN[1], COLOR_SKIN[2]);
    drawFilledCircle(dx, 18.0f, 9.0f);

    glColor3f(COLOR_BLACK[0], COLOR_BLACK[1], COLOR_BLACK[2]);
    drawFilledEllipse(dx, 26.0f, 12.0f, 4.0f);
}

static void drawSeatedCouple(float x, float y, float scale)
{
    float s = scale / 300.0f;

    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(s, s, 1.0f);

        drawSeatedFigure(-16.0f, COLOR_NAVY);
        drawSeatedFigure( 16.0f, COLOR_BROWN);

    glPopMatrix();
}

/* Perro: silueta minima en negro solido, como aparece en primer plano
   en el cuadro original. */
static void drawDog(float x, float y, float scale)
{
    float s = scale / 300.0f;

    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(s, s, 1.0f);

        glColor3f(COLOR_BLACK[0], COLOR_BLACK[1], COLOR_BLACK[2]);
        drawFilledEllipse(0.0f, 0.0f, 22.0f, 9.0f);
        drawFilledEllipse(20.0f, 6.0f, 8.0f, 7.0f);

        drawFilledQuad(-14.0f, -6.0f,  -10.0f, -6.0f,  -10.0f, -16.0f,  -14.0f, -16.0f);
        drawFilledQuad(  6.0f, -6.0f,   10.0f, -6.0f,   10.0f, -16.0f,    6.0f, -16.0f);

    glPopMatrix();
}

/*
 * Angulo acumulado en la matriz de modelado (ver nota identica en
 * segment_demuth.c / segment_delaunay.c): las siluetas tienen que
 * leerse siempre derechas, sin importar en que angulo de la galeria
 * caiga el sector.
 */
static float getCurrentRotationDegrees(void)
{
    float matrix[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

    return atan2f(matrix[1], matrix[0]) * (180.0f / 3.14159265f);
}

void drawSeurat(void)
{
    static const float COMPOSITION_SCALE = 1.0f;

    float uprightAngle = -getCurrentRotationDegrees();

    glPushMatrix();

        glRotatef(uprightAngle, 0.0f, 0.0f, 1.0f);
        glScalef(COMPOSITION_SCALE, COMPOSITION_SCALE, 1.0f);

        /* posiciones trasladadas del prototipo (canvas 600x600) al
           lienzo local -1..1: local = (px-300)/300, con Y invertido
           (el canvas crece hacia abajo, el lienzo local hacia arriba) */
        drawSailboat(-0.700f, 0.5667f, 1.0f);
        drawSailboat(-0.500f, 0.5333f, 0.7f);

        drawSeatedCouple(-0.4667f, -0.4333f, 1.0f);
        drawDog(-0.1333f, -0.5667f, 1.1f);
        drawParasolWoman(0.5667f, -0.4333f, 1.05f);

    glPopMatrix();
}
