#include <math.h>
#include <GL/freeglut.h>

#include "segment_alma_thomas.h"
#include "utils.h"

/*
 * Segmento Alma Thomas: "Fiesta de primavera" (1968, Color Field)
 *
 * La obra real es un mosaico de anillos concentricos de color, cada uno
 * hecho de pequenas pinceladas rectangulares separadas por el lienzo
 * crudo (el "hueco" entre pinceladas es lo que le da vibracion). Es
 * radialmente simetrica -- a diferencia de Demuth o Delaunay, no hace
 * falta contra-rotacion ni lienzo local: toda la composicion vive en el
 * fondo del sector, en las mismas coordenadas polares reales, siguiendo
 * el mismo criterio validado en segment_riley.c (ancho de celda en
 * grados fijos, no fraccion del sector, para que la densidad de la
 * trama no cambie cuando el sector se activa y se ensancha).
 */

static const float RING_COLORS[7][3] =
{
    {0.42f, 0.65f, 0.30f}, /* verde (centro) */
    {0.18f, 0.28f, 0.58f}, /* azul */
    {0.42f, 0.20f, 0.48f}, /* violeta */
    {0.55f, 0.15f, 0.20f}, /* granate */
    {0.75f, 0.20f, 0.15f}, /* rojo */
    {0.88f, 0.45f, 0.12f}, /* naranja */
    {0.92f, 0.75f, 0.15f}  /* amarillo (borde exterior) */
};

/* Sin primer plano: como en segment_mondrian.c y segment_riley.c, toda
   la obra se resuelve en el fondo para ocupar el sector completo. */
void drawAlmaThomas(void)
{
}

void drawAlmaThomasBackground(float innerRadius, float outerRadius, float halfAngle)
{
    static const int   RINGS       = 7;
    static const int   SUBROWS     = 2;    /* filas de pinceladas por anillo de color */
    static const float CELL_DEG    = 3.2f;
    static const float GAP_DEG     = 0.7f;
    static const float RADIAL_GAP  = 0.05f; /* fraccion del grosor de cada fila */

    float span = outerRadius - innerRadius;
    float ringSpan = span / (float)RINGS;
    float subrowSpan = ringSpan / (float)SUBROWS;

    /* lienzo crudo de fondo: se ve en los huecos entre pinceladas */
    glColor3f(0.95f, 0.92f, 0.84f);
    drawFilledArc(innerRadius, outerRadius, -halfAngle, halfAngle);

    int ring, subrow;

    for(ring = 0; ring < RINGS; ring++)
    {
        float ringR0 = innerRadius + ringSpan * (float)ring;
        const float *color = RING_COLORS[ring];

        glColor3f(color[0], color[1], color[2]);

        /* cada anillo de color se subdivide en SUBROWS filas de
           pinceladas mas chicas, alternadas al estilo aparejo de
           ladrillo, para que la trama se sienta mas densa y no como un
           solo arco liso */
        for(subrow = 0; subrow < SUBROWS; subrow++)
        {
            float r0 = ringR0 + subrowSpan * (float)subrow;
            float r1 = r0 + subrowSpan;

            float r0b = r0 + subrowSpan * RADIAL_GAP;
            float r1b = r1 - subrowSpan * RADIAL_GAP;

            float offset = (subrow % 2) * (CELL_DEG * 0.5f);
            float angle;

            for(angle = -halfAngle - offset; angle < halfAngle; angle += CELL_DEG)
            {
                float a0 = angle + GAP_DEG * 0.5f;
                float a1 = angle + CELL_DEG - GAP_DEG * 0.5f;

                if(a0 < -halfAngle) a0 = -halfAngle;
                if(a1 >  halfAngle) a1 =  halfAngle;

                if(a1 <= a0)
                    continue;

                drawFilledArc(r0b, r1b, a0, a1);
            }
        }
    }
}
