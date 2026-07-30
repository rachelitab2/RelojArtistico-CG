#include <math.h>
#include <GL/freeglut.h>

#include "segment_hokusai.h"
#include "utils.h"

/* Fondo pintado con la geometria real del sector (mismo drawFilledArc
   que usa segments.c para el relleno por defecto), asi que llena el
   sector exactamente, sin huecos entre el arte y el borde curvo.
   El cielo queda en un solo tono crema limpio; el mar se divide en
   tres bandas de azul para dar sensacion de profundidad. */
void drawHokusaiBackground(float innerRadius, float outerRadius, float halfAngle)
{
    float midRadius = (innerRadius + outerRadius) * 0.5f;
    float seaSpan = midRadius - innerRadius;

    /* cielo: mitad exterior del sector */
    glColor3f(0.92f,0.87f,0.68f);
    drawFilledArc(midRadius, outerRadius, -halfAngle, halfAngle);

    /* mar: mitad interior, tres tonos de azul de mas oscuro a mas claro */
    glColor3f(0.10f,0.28f,0.52f);
    drawFilledArc(innerRadius, innerRadius + seaSpan*0.35f, -halfAngle, halfAngle);

    glColor3f(0.16f,0.38f,0.62f);
    drawFilledArc(innerRadius + seaSpan*0.35f, innerRadius + seaSpan*0.70f, -halfAngle, halfAngle);

    glColor3f(0.24f,0.48f,0.72f);
    drawFilledArc(innerRadius + seaSpan*0.70f, midRadius, -halfAngle, halfAngle);
}

/* Monte Fuji pequeno al fondo: base azul (como neblina sobre el mar)
   y silueta blanca encima. Se dibuja antes que la ola para que quede
   parcialmente detras de ella, como en la composicion original. */
static void drawMountFuji(void)
{
    glColor3f(0.35f,0.50f,0.68f);

    glBegin(GL_TRIANGLES);
        glVertex2f(-0.95f,-0.72f);
        glVertex2f(-0.68f,-0.72f);
        glVertex2f(-0.815f,-0.58f);
    glEnd();

    glColor3f(0.97f,0.97f,0.98f);

    glBegin(GL_TRIANGLES);
        glVertex2f(-0.90f,-0.66f);
        glVertex2f(-0.73f,-0.66f);
        glVertex2f(-0.815f,-0.50f);
    glEnd();
}

/* Dibuja una polilinea central como cinta de grosor variable (mismo
   metodo ya usado en el proyecto para formas curvas sin generar un
   poligono auto-intersectante). "thicknessScale" permite reutilizar
   el mismo camino para capas mas angostas encima. */
static void drawRibbon(const float path[][3], int count, float thicknessScale,
                        float r, float g, float b)
{
    int i;

    glColor3f(r,g,b);

    glBegin(GL_TRIANGLE_STRIP);

    for(i = 0; i < count; i++)
    {
        float dx, dy, len, nx, ny, half;

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

        len = sqrtf(dx*dx + dy*dy);
        if(len < 0.0001f)
            len = 0.0001f;

        nx = -dy / len;
        ny =  dx / len;

        half = path[i][2] * thicknessScale;

        glVertex2f(path[i][0] + nx*half, path[i][1] + ny*half);
        glVertex2f(path[i][0] - nx*half, path[i][1] - ny*half);
    }

    glEnd();
}

/* La gran ola: un cuerpo dominante que ocupa la mayor parte del
   lienzo local, mas un nucleo mas claro encima para dar sensacion de
   volumen sin complicar la tecnica. */
static void drawWave(void)
{
    static const float path[][3] =
    {
        /* x,     y,     semigrosor */
        {-0.88f,-0.55f, 0.10f},
        {-0.72f,-0.28f, 0.28f},
        {-0.42f, 0.00f, 0.40f},
        {-0.05f, 0.28f, 0.46f},
        { 0.28f, 0.50f, 0.42f},
        { 0.50f, 0.68f, 0.30f},
        { 0.62f, 0.80f, 0.18f},
        { 0.55f, 0.88f, 0.09f},
        { 0.36f, 0.83f, 0.04f}
    };

    const int count = sizeof(path) / sizeof(path[0]);

    drawRibbon(path, count, 1.00f, 0.10f,0.30f,0.58f);

    drawRibbon(path, count, 0.55f, 0.24f,0.50f,0.76f);
}

/* Varias garras de espuma triangulares a lo largo de la cresta y el
   gancho de la ola. */
static void drawFoamClaws(void)
{
    static const float claws[][3][2] =
    {
        { {0.58f,0.92f}, {0.46f,0.82f}, {0.54f,0.78f} },
        { {0.68f,0.86f}, {0.58f,0.76f}, {0.65f,0.72f} },
        { {0.44f,0.90f}, {0.34f,0.81f}, {0.41f,0.76f} },
        { {0.30f,0.83f}, {0.20f,0.74f}, {0.27f,0.69f} },
        { {0.16f,0.72f}, {0.06f,0.64f}, {0.13f,0.59f} },
        { {0.68f,0.72f}, {0.60f,0.63f}, {0.66f,0.60f} }
    };

    const int count = sizeof(claws) / sizeof(claws[0]);
    int i;

    glColor3f(0.96f,0.97f,0.99f);

    glBegin(GL_TRIANGLES);

    for(i = 0; i < count; i++)
    {
        glVertex2f(claws[i][0][0], claws[i][0][1]);
        glVertex2f(claws[i][1][0], claws[i][1][1]);
        glVertex2f(claws[i][2][0], claws[i][2][1]);
    }

    glEnd();
}

static void drawFoam(void)
{
    drawFoamClaws();

    glColor3f(0.95f,0.96f,0.98f);

    glBegin(GL_TRIANGLES);

        /* espuma en el cuerpo de la ola */
        glVertex2f( 0.05f, 0.55f);
        glVertex2f(-0.08f, 0.46f);
        glVertex2f( 0.06f, 0.40f);

        /* gotas sueltas cayendo desde la cresta */
        glVertex2f(-0.20f, 0.35f);
        glVertex2f(-0.32f, 0.27f);
        glVertex2f(-0.18f, 0.20f);

        glVertex2f( 0.10f, 0.20f);
        glVertex2f(-0.02f, 0.12f);
        glVertex2f( 0.12f, 0.06f);

        /* salpicaduras hacia la base de la ola */
        glVertex2f(-0.55f,-0.15f);
        glVertex2f(-0.66f,-0.24f);
        glVertex2f(-0.50f,-0.28f);

    glEnd();
}

/* primer plano, en orden: fondo -> montana -> ola -> espuma encima */
void drawHokusai(void)
{
    drawMountFuji();

    drawWave();

    drawFoam();
}
