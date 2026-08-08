#include <math.h>
#include <GL/freeglut.h>

#include "segment_rothko.h"
#include "utils.h"

/*
 * Mark Rothko - "Color fields for kids".
 *
 * Cinco bandas de color con textura pastosa (papel arrugado, pinceladas
 * visibles). A diferencia de Demuth/Van Gogh, esta pieza no tiene un
 * arriba/abajo que deba leerse "derecho": es un campo de color puro, asi
 * que se dibuja polar-nativo (coordenadas reales del sector, igual que
 * Monet/Riley/Kupka) para ocupar el wedge completo en vez de quedar
 * encerrada en el cuadrado chico inscripto que usan las obras con
 * contra-rotacion. Las bandas se reparten en el angulo del sector y la
 * textura corre en direccion radial (equivalente polar de "vertical").
 */

static const float BAND_COLOR[5][3] =
{
    {0.85f, 0.42f, 0.53f}, /* rosa */
    {0.80f, 0.10f, 0.14f}, /* rojo */
    {0.90f, 0.46f, 0.16f}, /* naranja */
    {0.85f, 0.27f, 0.32f}, /* rojo coral */
    {0.55f, 0.16f, 0.22f}  /* granate */
};

static float hashf(float x, float y)
{
    float s = sinf(x * 127.1f + y * 311.7f) * 43758.5453f;

    return s - floorf(s);
}

static void mixColor(float out[3], const float a[3], const float b[3], float t)
{
    out[0] = a[0] + (b[0] - a[0]) * t;
    out[1] = a[1] + (b[1] - a[1]) * t;
    out[2] = a[2] + (b[2] - a[2]) * t;
}

/* Pinceladas radiales alternando tonos mas claros/oscuros que la banda
   base, dentro de la cuna angular (a0..a1) y el rango de radio dado.
   Simula el papel arrugado y las vetas del original. */
static void drawBandTexture(int bandIndex, float a0, float a1,
                             float innerRadius, float outerRadius,
                             const float base[3])
{
    static const int STREAKS  = 10;
    static const int SEGMENTS = 6;
    float span = outerRadius - innerRadius;
    int s;

    glLineWidth(2.0f);

    for(s = 0; s < STREAKS; s++)
    {
        float fa = hashf((float)bandIndex * 13.1f, (float)s * 7.7f);
        float streakAngle = a0 + fa * (a1 - a0);
        int isLight = hashf((float)s, (float)bandIndex * 3.3f) > 0.5f;
        float tone[3];
        float white[3] = {1.0f, 1.0f, 1.0f};
        float black[3] = {0.0f, 0.0f, 0.0f};
        float alpha = 0.18f + hashf((float)s, (float)bandIndex + 5.0f) * 0.22f;
        float angle = streakAngle;
        int k;

        if(isLight)
            mixColor(tone, base, white, 0.35f + hashf((float)s, (float)bandIndex) * 0.25f);
        else
            mixColor(tone, base, black, 0.20f + hashf((float)s + 1.0f, (float)bandIndex) * 0.25f);

        glColor4f(tone[0], tone[1], tone[2], alpha);

        glBegin(GL_LINE_STRIP);

        for(k = 0; k <= SEGMENTS; k++)
        {
            float r = innerRadius + span * ((float)k / (float)SEGMENTS);
            float rad;

            if(k > 0)
                angle += (hashf((float)(s + k), (float)(bandIndex + k) * 1.7f) - 0.5f) * (a1 - a0) * 0.12f;

            rad = degreesToRadians(angle);
            glVertex2f(r * cosf(rad), r * sinf(rad));
        }

        glEnd();
    }
}

void drawRothkoBackground(float innerRadius, float outerRadius, float halfAngle)
{
    /* drawFilledArc avanza el angulo en pasos de 2 grados y puede no
       llegar exacto al limite pedido, dejando una costura oscura entre
       bandas contiguas. Se dibuja cada banda un poco mas ancha de lo
       que le toca (solapando a su vecina) para tapar ese hueco. */
    static const int   BAND_COUNT  = 5;
    static const float OVERLAP_DEG = 2.5f;

    float bandWidthDeg = (2.0f * halfAngle) / (float)BAND_COUNT;
    int i;

    for(i = 0; i < BAND_COUNT; i++)
    {
        float a0 = -halfAngle + (float)i * bandWidthDeg;
        float a1 = a0 + bandWidthDeg;
        float drawA0 = a0 - ((i > 0) ? OVERLAP_DEG : 0.0f);
        float drawA1 = a1 + ((i < BAND_COUNT - 1) ? OVERLAP_DEG : 0.0f);

        glColor3f(BAND_COLOR[i][0], BAND_COLOR[i][1], BAND_COLOR[i][2]);
        drawFilledArc(innerRadius, outerRadius, drawA0, drawA1);

        drawBandTexture(i, a0, a1, innerRadius, outerRadius, BAND_COLOR[i]);
    }
}

void drawRothko(void) {} /* vacio, todo en el fondo (polar-nativo) */
