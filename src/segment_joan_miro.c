#include <math.h>
#include <GL/freeglut.h>

#include "segment_joan_miro.h"
#include "utils.h"

/*
 * Joan Miro - "El Jardin" (The Garden)
 *
 * Version estilizada, no copia literal (el original tiene demasiado
 * detalle para reproducir con precision): linea negra serpenteante
 * conectando puntos de color, un par de dianas/ojos concentricos, una
 * mancha negra tipo llama, una estrella y manchas biomorficas de
 * color -- los motivos mas reconocibles del cuadro.
 *
 * Validada con un prototipo HTML/canvas antes de portarla. Tiene una
 * orientacion clara (a diferencia de Escher/Alma Thomas), asi que usa
 * la misma contra-rotacion que Demuth/Seurat/Paul Klee.
 */

static const float COLOR_RED[3]    = {0.784f, 0.153f, 0.122f};
static const float COLOR_GREEN[3]  = {0.180f, 0.561f, 0.243f};
static const float COLOR_YELLOW[3] = {0.910f, 0.761f, 0.165f};
static const float COLOR_BLUE[3]   = {0.133f, 0.333f, 0.643f};
static const float COLOR_INK[3]    = {0.039f, 0.039f, 0.039f};
static const float COLOR_WHITE[3]  = {0.949f, 0.929f, 0.878f};
static const float COLOR_PINK[3]   = {0.878f, 0.561f, 0.627f};

static const float COLOR_BG_LIGHT[3] = {0.663f, 0.776f, 0.910f};
static const float COLOR_BG_DARK[3]  = {0.110f, 0.227f, 0.388f};

/* ---------------------------------------------------------------- */
/* Fondo: degrade azul aproximado en bandas radiales                 */
/* ---------------------------------------------------------------- */

void drawJoanMiroBackground(float innerRadius, float outerRadius, float halfAngle)
{
    static const int BANDS = 6;

    float span = outerRadius - innerRadius;
    int i;

    for(i = 0; i < BANDS; i++)
    {
        float t0 = (float)i / (float)BANDS;
        float t1 = (float)(i + 1) / (float)BANDS;
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

/* ---------------------------------------------------------------- */
/* Primer plano: los motivos de Miro                                 */
/* ---------------------------------------------------------------- */

static void fillEllipse(float cx, float cy, float rx, float ry, float rotDeg, const float color[3])
{
    static const int STEPS = 20;
    float rad = degreesToRadians(rotDeg);
    float cosr = cosf(rad);
    float sinr = sinf(rad);
    int i;

    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);

        for(i = 0; i <= STEPS; i++)
        {
            float a = degreesToRadians((float)i * (360.0f / (float)STEPS));
            float ex = rx * cosf(a);
            float ey = ry * sinf(a);

            glVertex2f(cx + ex * cosr - ey * sinr, cy + ex * sinr + ey * cosr);
        }
    glEnd();
}

static void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const float color[3])
{
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

/* cinta de grosor constante sobre un camino, igual tecnica que
   drawRibbon() en segment_demuth.c */
static void strokeRibbon(const float path[][2], int count, float thickness, const float color[3])
{
    int i;

    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_TRIANGLE_STRIP);

    for(i = 0; i < count; i++)
    {
        float dx, dy, len, nx, ny;

        if(i == 0)
        {
            dx = path[i+1][0] - path[i][0];
            dy = path[i+1][1] - path[i][1];
        }
        else if(i == count - 1)
        {
            dx = path[i][0] - path[i-1][0];
            dy = path[i][1] - path[i-1][1];
        }
        else
        {
            dx = path[i+1][0] - path[i-1][0];
            dy = path[i+1][1] - path[i-1][1];
        }

        len = sqrtf(dx * dx + dy * dy);
        if(len < 0.0001f)
            len = 0.0001f;

        nx = -dy / len;
        ny =  dx / len;

        glVertex2f(path[i][0] + nx * thickness, path[i][1] + ny * thickness);
        glVertex2f(path[i][0] - nx * thickness, path[i][1] - ny * thickness);
    }

    glEnd();
}

static void fillStar(float cx, float cy, float outerR, float innerR, float rotDeg, const float color[3])
{
    int i;

    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);

        for(i = 0; i <= 10; i++)
        {
            float r = (i % 2 == 0) ? outerR : innerR;
            float a = degreesToRadians(-90.0f + rotDeg + (float)i * 36.0f);

            glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
        }
    glEnd();
}

/* suma a `pts` los puntos muestreados de una curva Bezier cuadratica,
   sin repetir el punto inicial (ya esta en pts) */
static int addQuadCurve(float pts[][2], int count,
                         float x0, float y0, float cx, float cy, float x1, float y1,
                         int steps)
{
    int i;

    for(i = 1; i <= steps; i++)
    {
        float t = (float)i / (float)steps;
        float mt = 1.0f - t;

        pts[count][0] = mt * mt * x0 + 2.0f * mt * t * cx + t * t * x1;
        pts[count][1] = mt * mt * y0 + 2.0f * mt * t * cy + t * t * y1;
        count++;
    }

    return count;
}

/* mancha negra tipo "llama/planta", armada con 4 curvas Bezier
   muestreadas y cerradas en un poligono relleno */
static void fillFlameShape(void)
{
    float pts[32][2];
    int n = 0;
    int i;

    pts[n][0] = -0.20f; pts[n][1] = -0.35f; n++;
    n = addQuadCurve(pts, n, -0.20f, -0.35f, -0.05f, -0.20f,  0.05f, -0.45f, 6);
    n = addQuadCurve(pts, n,  0.05f, -0.45f,  0.15f, -0.65f,  0.35f, -0.55f, 6);
    n = addQuadCurve(pts, n,  0.35f, -0.55f,  0.15f, -0.75f, -0.05f, -0.70f, 6);
    n = addQuadCurve(pts, n, -0.05f, -0.70f, -0.30f, -0.60f, -0.20f, -0.35f, 6);

    glColor3f(COLOR_INK[0], COLOR_INK[1], COLOR_INK[2]);

    glBegin(GL_POLYGON);
        for(i = 0; i < n; i++)
            glVertex2f(pts[i][0], pts[i][1]);
    glEnd();
}

static void drawDot(float x, float y, float r, const float color[3])
{
    glColor3f(color[0], color[1], color[2]);
    drawFilledCircle(x, y, r);
}

static void drawTarget(float x, float y, float outerR, const float outerColor[3], const float innerColor[3])
{
    drawDot(x, y, outerR, outerColor);
    drawDot(x, y, outerR * 0.56f, innerColor);
    drawDot(x, y, outerR * 0.22f, COLOR_INK);
}

/*
 * Angulo acumulado en la matriz de modelado (misma nota que en
 * segment_demuth.c / segment_seurat.c / segment_paul_klee.c): los
 * elementos figurativos (diana, estrella) tienen que leerse siempre
 * derechos.
 */
static float getCurrentRotationDegrees(void)
{
    float matrix[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

    return atan2f(matrix[1], matrix[0]) * (180.0f / 3.14159265f);
}

void drawJoanMiro(void)
{
    static const float COMPOSITION_SCALE = 0.92f;

    float path1[6][2] =
    {
        {-0.80f, 0.15f}, {-0.55f, 0.35f}, {-0.30f, 0.20f},
        {-0.05f, 0.40f}, { 0.15f, 0.15f}, { 0.35f, 0.05f}
    };
    const float *path1Colors[6] =
    {
        COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_RED, COLOR_YELLOW, COLOR_GREEN
    };

    float path2[5][2] =
    {
        {-0.55f, -0.05f}, {-0.30f, -0.20f}, {-0.10f, -0.10f},
        { 0.10f, -0.30f}, { 0.30f, -0.20f}
    };
    const float *path2Colors[5] =
    {
        COLOR_YELLOW, COLOR_RED, COLOR_GREEN, COLOR_RED, COLOR_YELLOW
    };

    float path3[4][2] =
    {
        {0.15f, -0.55f}, {0.35f, -0.45f}, {0.55f, -0.60f}, {0.70f, -0.45f}
    };

    float uprightAngle = -getCurrentRotationDegrees();
    int i;

    glPushMatrix();

        glRotatef(uprightAngle, 0.0f, 0.0f, 1.0f);
        glScalef(COMPOSITION_SCALE, COMPOSITION_SCALE, 1.0f);

        /* manchas biomorficas grandes, detras de todo */
        fillEllipse(0.55f, 0.55f, 0.30f, 0.22f,  20.0f, COLOR_GREEN);
        fillEllipse(0.72f, 0.30f, 0.22f, 0.30f, -10.0f, COLOR_RED);

        /* linea 1: camino negro + nodos de color */
        strokeRibbon(path1, 6, 0.022f, COLOR_INK);
        for(i = 0; i < 6; i++)
            drawDot(path1[i][0], path1[i][1], (i % 2 == 0) ? 0.035f : 0.022f, path1Colors[i]);

        /* linea 2 */
        strokeRibbon(path2, 5, 0.018f, COLOR_INK);
        for(i = 0; i < 5; i++)
            drawDot(path2[i][0], path2[i][1], 0.022f, path2Colors[i]);

        fillFlameShape();

        drawTarget(-0.15f, -0.10f, 0.09f, COLOR_WHITE, COLOR_GREEN);
        drawTarget( 0.68f,  0.55f, 0.08f, COLOR_YELLOW, COLOR_RED);

        fillStar(-0.55f, -0.55f, 0.16f, 0.07f, 8.0f, COLOR_BLUE);

        fillTriangle(-0.85f, -0.15f, -0.72f, -0.15f, -0.80f, -0.02f, COLOR_PINK);

        fillEllipse(-0.68f, 0.68f, 0.14f, 0.10f, -20.0f, COLOR_INK);
        drawDot(-0.62f, 0.72f, 0.02f, COLOR_RED);

        strokeRibbon(path3, 4, 0.020f, COLOR_INK);
        drawDot(path3[0][0], path3[0][1], 0.025f, COLOR_RED);
        drawDot(path3[3][0], path3[3][1], 0.025f, COLOR_GREEN);

    glPopMatrix();
}
