#include <math.h>
#include <GL/freeglut.h>

#include "segment_riley.h"
#include "utils.h"

/*
 * Segmento Riley: "Ritmo de color" (inspirado en Bridget Riley, Op Art)
 *
 * La referencia es una trama densa de celdas de color saturado que
 * avanzan en diagonal. Igual que Mondrian, toda la composicion vive en
 * el fondo y se construye con sub-sectores del abanico real
 * (drawFilledArc), no en el lienzo cuadrado local: asi llena el sector
 * de borde a borde en vez de dejar el halo gris que deja un cuadrado
 * inscrito en una cuna.
 *
 * Version estatica: el indice de color depende solo de (fila, columna),
 * sin reloj, por lo que la imagen es identica en cada frame.
 */

static const float COLORS[7][3] = {
    {0.173f, 0.369f, 0.541f}, /* azul */
    {0.831f, 0.290f, 0.161f}, /* rojo */
    {0.886f, 0.675f, 0.200f}, /* amarillo */
    {0.184f, 0.486f, 0.314f}, /* verde */
    {0.729f, 0.325f, 0.510f}, /* magenta */
    {0.929f, 0.890f, 0.780f}, /* crema */
    {0.420f, 0.639f, 0.839f}  /* azul claro */
};

/*
 * ROWS bandas radiales x celdas de ancho angular FIJO (CELL_DEG grados).
 *
 * El ancho de celda es absoluto, no una fraccion del sector, a proposito:
 * el sector activo casi duplica su semiangulo (26 -> 50 grados) y con
 * columnas de fraccion fija la trama se estiraria justo cuando mas se ve.
 * Con grados fijos la densidad es la misma activo o no, y como la celda
 * se indexa por su posicion angular absoluta, al ensancharse el sector
 * solo aparecen celdas nuevas en los bordes: las que ya estaban no se
 * mueven ni cambian de color mientras la animacion de layout corre.
 *
 * Cada banda se desplaza ROW_SHIFT_DEG respecto de la anterior; ese
 * corrimiento acumulado es lo que hace que las franjas se lean en
 * diagonal dentro de una geometria polar.
 */
void drawRileyBackground(float innerRadius, float outerRadius, float halfAngle)
{
    static const int   ROWS          = 11;
    static const float CELL_DEG      = 4.0f;
    static const float ROW_SHIFT_DEG = 1.8f;

    float span = outerRadius - innerRadius;

    /* alcanza para cubrir el sector mas el corrimiento total de la
       ultima banda, con un par de celdas de margen */
    int maxCol = (int)((halfAngle + ROWS * ROW_SHIFT_DEG) / CELL_DEG) + 2;
    int row, col;

    for(row = 0; row < ROWS; row++)
    {
        float r0 = innerRadius + span * ((float)row       / (float)ROWS);
        float r1 = innerRadius + span * ((float)(row + 1) / (float)ROWS);

        float shift = ROW_SHIFT_DEG * (float)row;

        for(col = -maxCol; col <= maxCol; col++)
        {
            float a0 = (float)col * CELL_DEG + shift;
            float a1 = a0 + CELL_DEG;

            const float *color;
            int colorIndex;

            if(a0 < -halfAngle) a0 = -halfAngle;
            if(a1 >  halfAngle) a1 =  halfAngle;

            if(a1 <= a0)
                continue;

            /* +700 mantiene el operando positivo cuando col es negativo:
               el % de C conserva el signo del dividendo. */
            colorIndex = (row * 3 + col + 700) % 7;
            color = COLORS[colorIndex];

            glColor3f(color[0], color[1], color[2]);

            drawFilledArc(r0, r1, a0, a1);
        }
    }
}

/* Sin primer plano: como en segment_mondrian.c, toda la obra se resuelve
   en el fondo para ocupar el sector completo. */
void drawRiley(void)
{
}
