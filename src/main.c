/* Punto de entrada: solo arma la ventana y los callbacks. No dibuja nada. */

#include <GL/freeglut.h>

#include "display.h"
#include "segments.h"


/* Ajusta glOrtho al redimensionar la ventana para que circulos y
   sectores no se deformen. Agranda el eje mas largo segun el aspect
   ratio y deja el mas corto fijo en -1..1. */
void reshape(int width, int height)
{
    if(height == 0)
        height = 1; /* evita division por cero */

    float aspect = (float)width / (float)height;

    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(aspect >= 1.0f)
    {
        glOrtho(-aspect, aspect,
                -1.0f, 1.0f,
                -1.0f, 1.0f);
    }
    else
    {
        glOrtho(-1.0f,1.0f,
                -1.0f/aspect,
                 1.0f/aspect,
                -1.0f,1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);

    /* CUESTIONABLE: sin GLUT_ALPHA ni GLUT_DEPTH. El overlay de
       segments.c usa alpha igual, porque el blending solo necesita el
       alpha de origen; el 2D no necesita Z-buffer. */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800,600); /* tamano fijo, no configurable */

    glutCreateWindow("Reloj Artistico");
    /* CUESTIONABLE: no se valida si la ventana se creo bien. */

    /* deben ir antes del loop: display() ya espera segments[] lleno */
    initDisplay();

    initSegments();

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    glutTimerFunc(16,timer,0); /* arranca el reloj de animacion (16ms) */

    glutMainLoop(); /* nunca retorna en uso normal */

    return 0;
}
