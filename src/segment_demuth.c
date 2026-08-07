#include <GL/freeglut.h>

#include "segment_demuth.h"
#include "utils.h"

/* Fondo con la geometria real del sector: bandas radiales calidas
   (rojo oscuro -> naranja), sin rejilla de columnas -- el cuadro real
   es un campo de color casi plano, mismo criterio que segment_monet.c. */
static const float RADIUS_FRACTION[5] = {0.00f, 0.28f, 0.55f, 0.80f, 1.00f};

static const float BAND_COLOR[4][3] =
{
    {0.32f,0.07f,0.05f},
    {0.50f,0.12f,0.07f},
    {0.68f,0.22f,0.10f},
    {0.80f,0.34f,0.14f}
};

static void drawBands(float innerRadius, float outerRadius, float halfAngle)
{
    float span = outerRadius - innerRadius;
    int row;

    for(row = 0; row < 4; row++)
    {
        float r0 = innerRadius + span * RADIUS_FRACTION[row];
        float r1 = innerRadius + span * RADIUS_FRACTION[row+1];

        glColor3f(BAND_COLOR[row][0], BAND_COLOR[row][1], BAND_COLOR[row][2]);

        drawFilledArc(r0, r1, -halfAngle, halfAngle);
    }
}

void drawDemuthBackground(float innerRadius, float outerRadius, float halfAngle)
{
    drawBands(innerRadius, outerRadius, halfAngle);
}

/* Un "5" hecho con 5 barras rectangulares (mismo criterio que un
   display de siete segmentos, usando 5 de los 7 segmentos: arriba,
   vertical superior izquierda, medio, vertical inferior derecha,
   abajo). Se dibuja en el lienzo local (-1..1), reutilizable a
   distinta escala/posicion para el efecto de varios "5" en
   perspectiva del cuadro original. */
static void drawDigitFive(float scale, float offsetX, float offsetY, float r, float g, float b)
{
    static const float barHalfHeight = 0.06f;
    static const float left = -0.30f;
    static const float right = 0.30f;
    static const float top = 0.45f;
    static const float mid = 0.0f;
    static const float bottom = -0.45f;

    glColor3f(r, g, b);

    glBegin(GL_QUADS);
        /* barra superior */
        glVertex2f(offsetX + left*scale,  offsetY + (top+barHalfHeight)*scale);
        glVertex2f(offsetX + right*scale, offsetY + (top+barHalfHeight)*scale);
        glVertex2f(offsetX + right*scale, offsetY + (top-barHalfHeight)*scale);
        glVertex2f(offsetX + left*scale,  offsetY + (top-barHalfHeight)*scale);

        /* vertical superior izquierda */
        glVertex2f(offsetX + left*scale,               offsetY + top*scale);
        glVertex2f(offsetX + (left+barHalfHeight*2)*scale, offsetY + top*scale);
        glVertex2f(offsetX + (left+barHalfHeight*2)*scale, offsetY + mid*scale);
        glVertex2f(offsetX + left*scale,               offsetY + mid*scale);

        /* barra media */
        glVertex2f(offsetX + left*scale,  offsetY + (mid+barHalfHeight)*scale);
        glVertex2f(offsetX + right*scale, offsetY + (mid+barHalfHeight)*scale);
        glVertex2f(offsetX + right*scale, offsetY + (mid-barHalfHeight)*scale);
        glVertex2f(offsetX + left*scale,  offsetY + (mid-barHalfHeight)*scale);

        /* vertical inferior derecha */
        glVertex2f(offsetX + (right-barHalfHeight*2)*scale, offsetY + mid*scale);
        glVertex2f(offsetX + right*scale,                    offsetY + mid*scale);
        glVertex2f(offsetX + right*scale,                    offsetY + bottom*scale);
        glVertex2f(offsetX + (right-barHalfHeight*2)*scale, offsetY + bottom*scale);

        /* barra inferior */
        glVertex2f(offsetX + left*scale,  offsetY + (bottom+barHalfHeight)*scale);
        glVertex2f(offsetX + right*scale, offsetY + (bottom+barHalfHeight)*scale);
        glVertex2f(offsetX + right*scale, offsetY + (bottom-barHalfHeight)*scale);
        glVertex2f(offsetX + left*scale,  offsetY + (bottom-barHalfHeight)*scale);
    glEnd();
}

/* Circulo pequeno arriba a la izquierda, motivo puntual del cuadro
   original (una luz/farol en la esquina). */
static void drawCornerDot(void)
{
    glColor3f(0.95f,0.92f,0.80f);

    drawFilledCircle(-0.62f, 0.68f, 0.05f);
}

/* Tres "5" anidados a escala decreciente, para la sensacion de
   perspectiva/movimiento del cuadro original (los numeros parecen
   alejarse hacia el fondo). */
void drawDemuth(void)
{
    drawDigitFive(1.00f, 0.00f,  0.02f, 0.93f,0.78f,0.30f);
    drawDigitFive(0.62f, 0.03f, -0.02f, 0.98f,0.90f,0.55f);
    drawDigitFive(0.32f, 0.05f, -0.05f, 0.55f,0.15f,0.10f);

    drawCornerDot();
}
