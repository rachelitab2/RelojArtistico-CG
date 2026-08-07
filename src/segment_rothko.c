#include <math.h>
#include <GL/freeglut.h>

#include "segment_rothko.h"
#include "utils.h"

/*
 * Segmento Rothko: "No. 61 (Oxido y azul)" (1953)
 * Expresionismo Abstracto / Color Field - Campos de color difuminados flotantes.
 */

void drawRothkoBackground(float innerRadius, float outerRadius, float halfAngle)
{
    /* Fondo borgona / marron oscuro profundo */
    glColor3f(0.110f, 0.043f, 0.039f);
    drawFilledArc(innerRadius, outerRadius, -halfAngle, halfAngle);
}

static void drawSoftBlockGL(float cx, float cy, float halfW, float halfH,
                            const float darkColor[3], const float lightColor[3],
                            float scale, float alpha)
{
    int i;
    const int LAYERS = 14;

    glPushMatrix();

    glTranslatef(cx, cy, 0.0f);
    glScalef(scale, scale, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Capas difuminadas externas (soft fuzzy edges) */
    for(i = LAYERS; i >= 0; i--)
    {
        float pad = (float)i * 0.015f;
        float layerAlpha = alpha * (0.025f + (float)i * 0.018f);

        glColor4f(lightColor[0], lightColor[1], lightColor[2], layerAlpha);

        glBegin(GL_QUADS);
            glVertex2f(-halfW - pad, -halfH - pad);
            glVertex2f( halfW + pad, -halfH - pad);
            glVertex2f( halfW + pad,  halfH + pad);
            glVertex2f(-halfW - pad,  halfH + pad);
        glEnd();
    }

    /* Nucleo principal con gradiente vertical */
    glBegin(GL_QUADS);
        glColor4f(darkColor[0],  darkColor[1],  darkColor[2],  alpha);
        glVertex2f(-halfW, -halfH);
        glVertex2f( halfW, -halfH);

        glColor4f(lightColor[0], lightColor[1], lightColor[2], alpha);
        glVertex2f( halfW,  halfH);
        glVertex2f(-halfW,  halfH);
    glEnd();

    glPopMatrix();
}

void drawRothko(void)
{
    float t = glutGet(GLUT_ELAPSED_TIME) * 0.001f;

    float breath = 0.5f + 0.5f * sinf((2.0f * 3.1415926535f * t) / 10.0f);
    float scale  = 1.0f + breath * 0.025f;
    float alpha  = 0.85f + breath * 0.10f;

    /* Bloque superior (Oxido) */
    const float TOP_DARK[3]  = {0.549f, 0.227f, 0.153f};
    const float TOP_LIGHT[3] = {0.722f, 0.290f, 0.196f};

    /* Bloque inferior (Azul) */
    const float BOTTOM_DARK[3]  = {0.106f, 0.286f, 0.396f};
    const float BOTTOM_LIGHT[3] = {0.169f, 0.424f, 0.690f};

    /* Dibujar Bloque Oxido */
    drawSoftBlockGL(0.0f, 0.4286f, 0.5143f, 0.3286f, TOP_DARK, TOP_LIGHT, scale, alpha);

    /* Dibujar Bloque Azul */
    drawSoftBlockGL(0.0f, -0.3786f, 0.5000f, 0.2929f, BOTTOM_DARK, BOTTOM_LIGHT, scale, alpha);
}
