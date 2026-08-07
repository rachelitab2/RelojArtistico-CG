#include <math.h>
#include <GL/freeglut.h>

#include "segment_escher.h"
#include "utils.h"

/*
 * Segmento Escher: "Symmetry Drawing No. 69" (1948, teselacion)
 *
 * Composicion validada con un prototipo HTML/canvas antes de portarla:
 * un motivo de 3 reptiles (rojo/azul/dorado) engranados por simetria
 * rotacional de 120 grados, repetido en teselado sobre toda la cuna del
 * sector -- mismo criterio "polar-nativo" que Riley y Alma Thomas: se
 * dibuja directamente en coordenadas reales del sector (radio/angulo),
 * nunca en el lienzo local cuadrado, y el paso angular de la trama es
 * en grados fijos para que la densidad no cambie cuando el sector se
 * activa y su halfAngle crece.
 *
 * Es un patron sin una orientacion "correcta" (a diferencia de un
 * numero o una figura humana), asi que no hace falta contra-rotacion:
 * drawEscher() queda vacio, todo vive en el fondo.
 */

static const float COLOR_RED[3]      = {0.659f, 0.224f, 0.122f};
static const float COLOR_BLUE[3]     = {0.561f, 0.725f, 0.788f};
static const float COLOR_GOLD[3]     = {0.784f, 0.592f, 0.122f};
static const float COLOR_INK[3]      = {0.086f, 0.051f, 0.020f};
static const float COLOR_BG_LIGHT[3] = {0.227f, 0.114f, 0.055f};
static const float COLOR_BG_DARK[3]  = {0.110f, 0.055f, 0.024f};

static void drawFilledTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

static void drawFilledEllipse(float cx, float cy, float rx, float ry)
{
    static const int STEPS = 16;
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

/* Reptil estilizado armado con primitivas simples (ovalo + triangulos),
   mismo criterio que drawDog() en segment_seurat.c -- evita las curvas
   compuestas que se autointersectan y son dificiles de rellenar bien.
   Mira hacia +X local, centrado en el origen. */
static void drawReptile(float x, float y, float angDeg, float s, const float color[3])
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glRotatef(angDeg, 0.0f, 0.0f, 1.0f);
        glScalef(s, s, 1.0f);

        glColor3f(color[0], color[1], color[2]);

        /* cola, con un segundo triangulo para simular el gancho final */
        drawFilledTriangle(-0.16f, 0.05f,  -0.16f, -0.05f,  -0.50f, 0.00f);
        drawFilledTriangle(-0.44f, 0.03f,  -0.44f, -0.06f,  -0.62f, 0.12f);

        /* 4 patas cortas */
        drawFilledTriangle( 0.05f, 0.12f,   0.15f, 0.12f,   0.08f, 0.24f);
        drawFilledTriangle(-0.15f, 0.12f,  -0.05f, 0.12f,  -0.12f, 0.24f);
        drawFilledTriangle( 0.05f,-0.12f,   0.15f,-0.12f,   0.08f,-0.24f);
        drawFilledTriangle(-0.15f,-0.12f,  -0.05f,-0.12f,  -0.12f,-0.24f);

        /* cuerpo */
        drawFilledEllipse(0.0f, 0.0f, 0.24f, 0.13f);

        /* cabeza */
        drawFilledTriangle(0.16f, 0.09f,  0.16f, -0.09f,  0.42f, 0.00f);

        /* ojo */
        glColor3f(COLOR_INK[0], COLOR_INK[1], COLOR_INK[2]);
        drawFilledCircle(0.24f, -0.025f, 0.02f);

    glPopMatrix();
}

/* motivo: 3 reptiles engranados por simetria rotacional de 120 grados
   alrededor de un punto central, como en la obra original */
static void drawMotif(float x, float y, float scale)
{
    static const float ANGLE_OFFSET = 40.0f;
    int i;

    for(i = 0; i < 3; i++)
    {
        const float *color = (i == 0) ? COLOR_RED : (i == 1) ? COLOR_BLUE : COLOR_GOLD;

        drawReptile(x, y, (float)i * 120.0f + ANGLE_OFFSET, scale, color);
    }
}

void drawEscherBackground(float innerRadius, float outerRadius, float halfAngle)
{
    static const float CELL_DEG            = 19.0f;
    static const int   ROWS                = 5;
    static const float ROW_SHIFT_DEG       = CELL_DEG * 0.5f;
    static const float MOTIF_SCALE_FACTOR  = 0.62f; /* relativo a la altura de fila */
    static const float MOTIF_RADIUS_FACTOR = 0.62f; /* extremo de la cola, en unidades del motivo */
    static const int   BG_BANDS            = 6;

    float span = outerRadius - innerRadius;
    float rowHeight = span / (float)ROWS;
    int maxCol = (int)(halfAngle / CELL_DEG) + 2;
    int row, col, i;

    /* fondo calido en bandas concentricas, de mas claro (radio interno)
       a mas oscuro (radio externo) -- imita el degrade radial del
       prototipo sin usar shaders */
    for(i = 0; i < BG_BANDS; i++)
    {
        float t0 = (float)i / (float)BG_BANDS;
        float t1 = (float)(i + 1) / (float)BG_BANDS;
        float r0 = innerRadius + span * t0;
        float r1 = innerRadius + span * t1;
        float t = (t0 + t1) * 0.5f;

        float color[3];
        color[0] = COLOR_BG_LIGHT[0] + (COLOR_BG_DARK[0] - COLOR_BG_LIGHT[0]) * t;
        color[1] = COLOR_BG_LIGHT[1] + (COLOR_BG_DARK[1] - COLOR_BG_LIGHT[1]) * t;
        color[2] = COLOR_BG_LIGHT[2] + (COLOR_BG_DARK[2] - COLOR_BG_LIGHT[2]) * t;

        glColor3f(color[0], color[1], color[2]);
        drawFilledArc(r0, r1, -halfAngle, halfAngle);
    }

    for(row = 0; row < ROWS; row++)
    {
        float r = innerRadius + rowHeight * ((float)row + 0.5f);
        float shift = (float)(row % 2) * ROW_SHIFT_DEG;
        float motifScale = rowHeight * MOTIF_SCALE_FACTOR;
        float motifRadius = motifScale * MOTIF_RADIUS_FACTOR;

        /* un motivo que se sale del radio interno/externo del sector
           se descarta entero -- no hay forma de recortar un triangulo
           a la mitad en modo inmediato, asi que en vez de dejarlo
           sobresalir se prefiere una fila menos densa en el borde */
        float ratio = motifRadius / r;
        float angularMarginDeg = (ratio < 1.0f)
            ? asinf(ratio) * (180.0f / 3.14159265f)
            : halfAngle;

        if(r - motifRadius < innerRadius || r + motifRadius > outerRadius)
            continue;

        for(col = -maxCol; col <= maxCol; col++)
        {
            float a = (float)col * CELL_DEG + shift;
            float rad, x, y;

            if(fabsf(a) + angularMarginDeg > halfAngle)
                continue;

            rad = degreesToRadians(a);
            x = cosf(rad) * r;
            y = sinf(rad) * r;

            drawMotif(x, y, motifScale);
        }
    }
}

void drawEscher(void) {} /* vacio, todo en el fondo */
