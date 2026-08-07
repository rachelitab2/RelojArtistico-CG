/* Punto de entrada: solo arma la ventana y los callbacks. No dibuja nada. */

#include <GL/freeglut.h>

#include "display.h"
#include "segments.h"
#include "app_config.h"
#include "audio.h"

#ifdef _WIN32
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif


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

/*
 * Entrada minima para la configuracion de la demo.
 *
 * Las teclas 1, 2 y 3 actualizan el intervalo elegido sin mezclar esa logica
 * con el renderizado. En una etapa posterior este bloque puede migrar a un
 * modulo input.c cuando existan mas interacciones.
 */
void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    /* cualquier tecla saca de la pantalla de intro; en cualquier otra
       pantalla no hace nada (ver display.c) */
    advanceFromIntroScreen();

    switch(key)
    {
        case '1':
            setChangeInterval(CHANGE_INTERVAL_15_MINUTES);
            break;

        case '2':
            setChangeInterval(CHANGE_INTERVAL_30_MINUTES);
            break;

        case '3':
            setChangeInterval(CHANGE_INTERVAL_60_MINUTES);
            break;

        case 'm':
        case 'M':
            toggleAudioMute();
            break;
    }

    glutPostRedisplay();
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

    initAudio();

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);

    glutTimerFunc(33,timer,0); /* arranca el reloj de animacion (~30 FPS) */

    glutMainLoop(); /* nunca retorna en uso normal */

    return 0;
}
