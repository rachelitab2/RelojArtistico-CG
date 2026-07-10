#include <math.h>
#include <GL/freeglut.h>

#include "segment_vangogh.h"
#include "utils.h"

/* Fondo con la geometria real del sector (igual tecnica que Mondrian):
   bandas radiales x angulares en tonos de azul, con un par de celdas
   amarillas sueltas simulando estrellas dentro del remolino. */
static const float RADIUS_FRACTION[5] = {0.00f, 0.25f, 0.50f, 0.75f, 1.00f};
static const float ANGLE_DEG[5]       = {-25.0f,-12.0f,  0.0f, 12.0f, 25.0f};

/* 0 = azul profundo, 1 = azul medio, 2 = azul remolino, 3 = estrella */
static const int CELL_COLOR[4][4] =
{
    { 0, 0, 1, 0 },
    { 1, 2, 0, 1 },
    { 0, 1, 3, 2 },
    { 2, 3, 1, 0 }
};

static const float PALETTE[4][3] =
{
    {0.05f,0.10f,0.35f},
    {0.10f,0.22f,0.55f},
    {0.20f,0.38f,0.72f},
    {0.95f,0.85f,0.35f}
};

static void drawBlocks(float innerRadius, float outerRadius)
{
    float span = outerRadius - innerRadius;
    int row, col;

    for(row = 0; row < 4; row++)
    {
        float r0 = innerRadius + span * RADIUS_FRACTION[row];
        float r1 = innerRadius + span * RADIUS_FRACTION[row+1];

        for(col = 0; col < 4; col++)
        {
            const float *color = PALETTE[CELL_COLOR[row][col]];

            glColor3f(color[0], color[1], color[2]);

            drawFilledArc(r0, r1, ANGLE_DEG[col], ANGLE_DEG[col+1]);
        }
    }
}

void drawVanGoghBackground(float innerRadius, float outerRadius)
{
    drawBlocks(innerRadius, outerRadius);
}

/* Ciprés: ribbon central con grosor variable, mismo patron que
   la ola de Hokusai, pero angosto y alargado. */
static void drawCypress(void)
{
    static const float path[][3] =
    {
        {-0.55f,-0.90f, 0.10f},
        {-0.60f,-0.55f, 0.14f},
        {-0.52f,-0.15f, 0.16f},
        {-0.60f, 0.25f, 0.13f},
        {-0.50f, 0.60f, 0.09f},
        {-0.58f, 0.90f, 0.03f}
    };

    const int count = sizeof(path) / sizeof(path[0]);
    int i;

    glColor3f(0.05f,0.14f,0.10f);

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

static void drawMoon(void)
{
    glColor3f(0.97f,0.92f,0.65f);

    drawFilledCircle(0.35f, 0.55f, 0.22f);
}

void drawVanGogh(void)
{
    drawMoon();

    drawCypress();
}
