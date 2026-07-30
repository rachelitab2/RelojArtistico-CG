#include <GL/freeglut.h>

#include "display.h"
#include "clock.h"
#include "segments.h"
#include "utils.h"
#include "artwork_catalog.h"
#include "ui.h"
#include <stdio.h>

void initDisplay(void)
{
    glClearColor(0.05f,0.05f,0.05f,1.0f); /* fondo casi negro */
}

/* Se ejecuta cada frame. Dibuja todo en orden: reloj, anillo decorativo,
   segmentos, y al final intercambia buffers. */
void display(void)
{

    const ArtworkInfo *activeArtwork = getArtworkInfo(getActiveArtworkType());
ArtworkColor background = activeArtwork->backgroundColor;
ArtworkColor accent = activeArtwork->accentColor;
const float backgroundBlend = 0.55f;

glClearColor(
    0.04f * (1.0f - backgroundBlend) + background.red * backgroundBlend,
    0.04f * (1.0f - backgroundBlend) + background.green * backgroundBlend,
    0.05f * (1.0f - backgroundBlend) + background.blue * backgroundBlend,
    1.0f
);
    
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

   drawClock();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(accent.red, accent.green, accent.blue, 0.28f);
    glLineWidth(1.2f);
    drawCircle(0.0f, 0.0f, 0.80f);

    /* anillo exterior con color pulsante (verde-amarillo). CUESTIONABLE:
       es independiente del pulso dorado del segmento destacado en
       segments.c; son dos animaciones de "brillo" separadas sin relacion
       entre si. */
    glColor4f(accent.red, accent.green, accent.blue, 0.62f);
    glLineWidth(2.5f);

drawCircle(0.0f, 0.0f, 0.90f);

    drawSegments();

    drawUserInterface();

    glutSwapBuffers();
}


/* Motor de la animacion: se reprograma cada 16ms. "value" no se usa,
   es obligatorio por la firma que exige glutTimerFunc. */
void timer(int value)
{
    updateSegments();

    glutPostRedisplay();

    glutTimerFunc(16,timer,0);
}
