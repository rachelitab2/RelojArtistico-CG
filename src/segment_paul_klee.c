#include <math.h>
#include <GL/freeglut.h>

#include "segment_paul_klee.h"
#include "utils.h"

/*
 * Segmento Paul Klee: "Castillo y sol" (1928, Bauhaus)
 *
 * Composicion validada con un prototipo HTML/canvas antes de portarla:
 * silueta de castillo armada con teselas rectangulares de color (mas
 * algunos remates triangulares) y un sol simple arriba a la derecha.
 *
 * A diferencia de Escher (patron sin orientacion fija), esta obra tiene
 * cielo arriba y castillo abajo, asi que usa la misma tecnica de
 * contra-rotacion que Demuth/Seurat: todo se dibuja en el lienzo local
 * -1..1 y se cancela la rotacion acumulada del sector para que siempre
 * se lea derecha. El castillo toca los bordes del lienzo (columnas de
 * -1 a 1), asi que la composicion se escala bien por debajo de 1.0 para
 * que ninguna esquina se salga del cuadrado al rotar a un angulo
 * arbitrario (mismo razonamiento que COMPOSITION_SCALE en Demuth).
 */

static const float COLOR_SUN[3]      = {0.878f, 0.576f, 0.165f};
static const float COLOR_BASE[3]     = {0.227f, 0.141f, 0.086f};
static const float COLOR_INK[3]      = {0.102f, 0.059f, 0.031f};
static const float COLOR_BG_LIGHT[3] = {0.541f, 0.271f, 0.129f};
static const float COLOR_BG_DARK[3]  = {0.322f, 0.145f, 0.067f};

static const float PALETTE[13][3] =
{
    {0.541f, 0.227f, 0.118f},
    {0.710f, 0.329f, 0.122f},
    {0.851f, 0.486f, 0.122f},
    {0.878f, 0.647f, 0.165f},
    {0.788f, 0.290f, 0.169f},
    {0.361f, 0.165f, 0.102f},
    {0.184f, 0.290f, 0.165f},
    {0.227f, 0.420f, 0.227f},
    {0.122f, 0.361f, 0.541f},
    {0.153f, 0.290f, 0.439f},
    {0.478f, 0.122f, 0.122f},
    {0.902f, 0.765f, 0.290f},
    {0.788f, 0.788f, 0.722f}
};
static const int PALETTE_SIZE = 13;

static void fillRectOutlined(float x0, float y0, float x1, float y1, const float color[3])
{
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
        glVertex2f(x0, y0);
        glVertex2f(x1, y0);
        glVertex2f(x1, y1);
        glVertex2f(x0, y1);
    glEnd();

    glColor3f(COLOR_INK[0], COLOR_INK[1], COLOR_INK[2]);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x0, y0);
        glVertex2f(x1, y0);
        glVertex2f(x1, y1);
        glVertex2f(x0, y1);
    glEnd();
}

static void fillTriangleOutlined(float x0, float y0, float x1, float y1, float x2, float y2, const float color[3])
{
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_TRIANGLES);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();

    glColor3f(COLOR_INK[0], COLOR_INK[1], COLOR_INK[2]);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
}

void drawPaulKleeBackground(float innerRadius, float outerRadius, float halfAngle)
{
    static const int BG_BANDS = 6;

    float span = outerRadius - innerRadius;
    int i;

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
}

/* Silueta de castillo: 9 columnas de teselas rectangulares de altura
   variable (algunas mas altas simulan torres), con remates triangulares
   en las columnas pares, sobre una franja de base solida. */
static void drawCastle(void)
{
    static const int   COLS = 9;
    static const float HEIGHTS[9] = {0.55f, 0.85f, 0.45f, 1.35f, 0.65f, 1.10f, 0.50f, 0.95f, 0.60f};

    float colWidth = 2.0f / (float)COLS;
    int colorIndex = 0;
    int c;

    for(c = 0; c < COLS; c++)
    {
        float x0 = -1.0f + (float)c * colWidth;
        float x1 = x0 + colWidth * 0.92f;
        float topY = -1.0f + HEIGHTS[c];
        int blocks = (HEIGHTS[c] > 1.0f) ? 3 : 2;
        int b;

        for(b = 0; b < blocks; b++)
        {
            float y0 = -1.0f + (topY - (-1.0f)) * ((float)b / (float)blocks);
            float y1 = -1.0f + (topY - (-1.0f)) * ((float)(b + 1) / (float)blocks);

            fillRectOutlined(x0, y0, x1, y1, PALETTE[colorIndex % PALETTE_SIZE]);
            colorIndex++;
        }

        if(c % 2 == 0)
        {
            float midX = (x0 + x1) * 0.5f;

            fillTriangleOutlined(x0, topY, x1, topY, midX, topY + colWidth * 0.9f, PALETTE[colorIndex % PALETTE_SIZE]);
            colorIndex++;
        }
    }

    fillRectOutlined(-1.0f, -1.0f, 1.0f, -0.85f, COLOR_BASE);
}

static void drawSun(void)
{
    glColor3f(COLOR_SUN[0], COLOR_SUN[1], COLOR_SUN[2]);
    drawFilledCircle(0.45f, 0.62f, 0.20f);

    glColor3f(COLOR_INK[0], COLOR_INK[1], COLOR_INK[2]);
    glLineWidth(1.5f);
    drawCircle(0.45f, 0.62f, 0.20f);
}

/*
 * Sin contra-rotacion dinamica: igual que Van Gogh/Kandinsky, la obra
 * gira con su sector. La unica correccion es una rotacion FIJA de 90
 * grados en sentido horario, aplicada una sola vez al autor la
 * composicion: la base del castillo pasa de mirar hacia -Y a mirar
 * hacia -X local (hacia el centro del reloj) y el cielo/sol hacia +X
 * local (hacia afuera). Como el eje +X local es el que apunta "hacia
 * arriba" en pantalla cuando el sector esta activo (centrado arriba de
 * la rueda), el castillo queda derecho solo al destacarse, sin tocar
 * la matriz de modelado por fotograma.
 *
 * Al ser un multiplo de 90 grados, esta rotacion no saca ninguna
 * esquina del cuadrado -1..1 (un cuadrado rotado 90 grados sobre su
 * propio centro coincide exactamente consigo mismo), asi que no hace
 * falta el margen de seguridad que si necesita una contra-rotacion a
 * angulo arbitrario.
 */
void drawPaulKlee(void)
{
    static const float COMPOSITION_SCALE = 0.95f;

    glPushMatrix();

        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        glScalef(COMPOSITION_SCALE, COMPOSITION_SCALE, 1.0f);

        drawSun();
        drawCastle();

    glPopMatrix();
}
