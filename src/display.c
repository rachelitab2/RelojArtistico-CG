#include <GL/freeglut.h>

#include "display.h"
#include "clock.h"
#include "segments.h"
#include "utils.h"
                #include <stdio.h>

void initDisplay(void)
{
    glClearColor(0.05f,0.05f,0.05f,1.0f); /* fondo casi negro */
}

/* Se ejecuta cada frame. Dibuja todo en orden: reloj, anillo decorativo,
   segmentos, y al final intercambia buffers. */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawClock();
    glColor3f(1.0f,1.0f,0.0f);
    drawArc(0.80f,0.0f,60.0f); /* marca decorativa amarilla, sin funcion real */

    glLineWidth(8.0f);

    drawCircle(0.0f,0.0f,0.80f);

    /* anillo exterior con color pulsante (verde-amarillo). CUESTIONABLE:
       es independiente del pulso dorado del segmento destacado en
       segments.c; son dos animaciones de "brillo" separadas sin relacion
       entre si. */
    static float color = 0.0f;

color += 0.01f;

if(color > 1.0f)
    color = 0.0f;

glColor3f(color,1.0f,0.0f);

drawCircle(0.0f,0.0f,0.90f);

    drawSegments();

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
