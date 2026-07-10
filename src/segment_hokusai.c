#include <math.h>
#include <GL/freeglut.h>

#include "segment_hokusai.h"
#include "utils.h"

/* Fondo pintado con la geometria real del sector (mismo drawFilledArc
   que usa segments.c para el relleno por defecto), asi que llena el
   sector exactamente, sin huecos entre el arte y el borde curvo. */
void drawHokusaiBackground(float innerRadius, float outerRadius)
{
    float midRadius = (innerRadius + outerRadius) * 0.5f;

    /* cielo: mitad exterior del sector */
    glColor3f(0.92f,0.87f,0.68f);
    drawFilledArc(midRadius, outerRadius, -25.0f, 25.0f);

    /* mar: mitad interior del sector */
    glColor3f(0.18f,0.45f,0.78f);
    drawFilledArc(innerRadius, midRadius, -25.0f, 25.0f);
}

/* pequeno y desaturado a proposito: debe leerse al fondo,
   sin competir con la silueta de la ola */
static void drawMountFuji(void)
{
    glColor3f(0.62f,0.64f,0.70f);

    glBegin(GL_TRIANGLES);
       glVertex2f(-0.92f,-0.80f);
       glVertex2f(-0.74f,-0.80f);
       glVertex2f(-0.83f,-0.66f);
    glEnd();

    /* nieve */
    glColor3f(0.96f,0.96f,0.97f);

    glBegin(GL_TRIANGLES);
       glVertex2f(-0.87f,-0.72f);
       glVertex2f(-0.79f,-0.72f);
       glVertex2f(-0.83f,-0.66f);
    glEnd();
}

/* Ribbon de la ola: una polilinea central con grosor variable,
   suficiente para curvar y engancharse en la cresta sin producir
   un poligono auto-intersectante. Silueta agrandada y con un
   gancho mas cerrado para que se lea de inmediato como "la ola". */
static void drawWave(void)
{
    static const float path[][3] =
    {
        /* x,     y,     semigrosor */
        {-0.95f,-0.52f, 0.05f},
        {-0.78f,-0.28f, 0.14f},
        {-0.54f,-0.02f, 0.20f},
        {-0.20f, 0.26f, 0.26f},
        { 0.15f, 0.52f, 0.28f},
        { 0.44f, 0.74f, 0.24f},
        { 0.60f, 0.88f, 0.16f},
        { 0.56f, 0.95f, 0.09f},
        { 0.37f, 0.90f, 0.04f}
    };

    const int count = sizeof(path) / sizeof(path[0]);
    int i;

    glColor3f(0.16f,0.38f,0.66f);

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

        len = sqrtf(dx*dx + dy*dy);
        if(len < 0.0001f)
            len = 0.0001f;

        nx = -dy / len;
        ny =  dx / len;

        glVertex2f(path[i][0] + nx*path[i][2], path[i][1] + ny*path[i][2]);
        glVertex2f(path[i][0] - nx*path[i][2], path[i][1] - ny*path[i][2]);
    }

    glEnd();
}

/* pequenos triangulos apuntando hacia adentro de la cresta,
   simulando las "garras" de espuma caracteristicas de la ola */
static void drawFoamClaws(void)
{
    static const float claws[][3][2] =
    {
        { {0.28f,0.80f}, {0.19f,0.71f}, {0.25f,0.67f} },
        { {0.39f,0.86f}, {0.31f,0.76f}, {0.37f,0.73f} },
        { {0.49f,0.87f}, {0.42f,0.79f}, {0.47f,0.74f} },
        { {0.17f,0.73f}, {0.08f,0.64f}, {0.15f,0.60f} }
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

        /* espuma en el cuerpo de la ola, cerca de la cresta */
        glVertex2f( 0.10f, 0.66f);
        glVertex2f(-0.02f, 0.56f);
        glVertex2f( 0.12f, 0.52f);

        /* gotas sueltas cayendo desde la cresta */
        glVertex2f(-0.30f, 0.30f);
        glVertex2f(-0.42f, 0.22f);
        glVertex2f(-0.28f, 0.16f);

        glVertex2f(-0.05f, 0.44f);
        glVertex2f(-0.16f, 0.36f);
        glVertex2f(-0.02f, 0.30f);

        /* salpicaduras hacia la base de la ola */
        glVertex2f(-0.60f,-0.10f);
        glVertex2f(-0.70f,-0.18f);
        glVertex2f(-0.56f,-0.22f);

    glEnd();
}

void drawHokusai(void)
{
    drawMountFuji();

    drawWave();

    drawFoam();
}
