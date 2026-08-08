/* Punto de entrada: solo arma la ventana y los callbacks. No dibuja nada. */

#include <string.h>

#include <GL/freeglut.h>

#include "display.h"
#include "segments.h"
#include "audio.h"

#ifdef _WIN32
#include <windows.h>

__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

/* wglext.h no viene en este toolchain minimo; se declara a mano lo
   justo para pedir VSync explicito (ver activateVsync() mas abajo). */
typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);

/*
 * El crash observado en produccion queda dentro de nvoglv64.dll,
 * en DrvPresentBuffers (ver docs/09-Diagnostico-Pantallazo-VIDEO-SCHEDULER.md
 * y su seguimiento). Sin VSync explicito, glutPostRedisplay() puede pedir
 * un nuevo present antes de que el driver termine de procesar el
 * anterior, sobrecargando la cola de presentacion -- exactamente la
 * funcion donde crashea. Forzar el intervalo de swap a 1 sincroniza
 * cada present con el refresco real del monitor.
 *
 * Debe llamarse DESPUES de glutCreateWindow() (necesita un contexto GL
 * activo para resolver la extension WGL).
 */
static void activateVsync(void)
{
    /* PROC (el tipo de retorno de wglGetProcAddress) no coincide en la
       firma con PFNWGLSWAPINTERVALEXTPROC; el cast intermedio por
       void* es la forma estandar de cargar extensiones WGL sin que
       -Wcast-function-type se queje del cast directo entre punteros a
       funcion incompatibles. */
    void *proc = (void *)wglGetProcAddress("wglSwapIntervalEXT");
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)proc;

    if(wglSwapIntervalEXT != 0)
        wglSwapIntervalEXT(1);
}

/*
 * Todas las rutas de assets del catalogo son relativas ("assets/images/...",
 * "assets/audio/..."), asi que dependen de cual sea el directorio de
 * trabajo al arrancar -- no de donde este el .exe. Si algo lanza el
 * proceso con el CWD en otro lado (ej. "Run" del IDE, un acceso directo,
 * o abrirlo desde build/ en vez de la raiz del proyecto), todas las
 * imagenes y el audio quedan en blanco sin ningun aviso.
 *
 * Se ubica la carpeta del ejecutable y se sube hasta 2 niveles buscando
 * "assets/". Si no aparece, se deja el directorio de trabajo como
 * estaba -- degradacion segura, mismo criterio que loadTexture() y
 * PlaySoundA(): nunca falla, en el peor caso el catalogo queda como
 * estaba antes de este fix.
 */
static void resolveAssetsWorkingDirectory(void)
{
    char exePath[MAX_PATH];
    char *lastSlash;
    int attempt;

    if(GetModuleFileNameA(NULL, exePath, MAX_PATH) == 0)
        return;

    lastSlash = strrchr(exePath, '\\');
    if(lastSlash != NULL)
        *lastSlash = '\0';

    SetCurrentDirectoryA(exePath);

    for(attempt = 0; attempt < 3; attempt++)
    {
        DWORD attrs = GetFileAttributesA("assets");

        if(attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY))
            return;

        SetCurrentDirectoryA("..");
    }
}
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
 * Las teclas 1, 2 y 3 cambian la sala activa (ver segments.c):
 *   1 -> Sala 15 min: Color y geometria
 *   2 -> Sala 30 min: Luz, ritmo y color
 *   3 -> Sala 60 min: Abstraccion, simbolo y atmosfera
 *
 * En una etapa posterior este bloque puede migrar a un modulo input.c
 * cuando existan mas interacciones.
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
        case '4':
            setActiveRoom(1); /* Sala 15 min: Color y geometria */
            break;

        case '2':
        case '5':
            setActiveRoom(2); /* Sala 30 min: Luz, ritmo y color */
            break;

        case '3':
        case '6':
            setActiveRoom(3); /* Sala 60 min: Abstraccion, simbolo y atmosfera */
            break;

        case 'm':
        case 'M':
            toggleAudioMute();
            break;
    }

    /* sin glutPostRedisplay() aca a proposito: el timer de 33ms (ver
       display.c) ya pide un repintado por ciclo. Duplicarlo justo en
       el instante de un cambio de sala (mucha geometria nueva de
       golpe) reproduce el mismo patron de doble-redisplay que causo
       el bugcheck original del driver grafico (ver
       docs/09-Diagnostico-Pantallazo-VIDEO-SCHEDULER.md) -- el retraso
       de hasta 33ms para que se refleje la tecla es imperceptible. */
}

int main(int argc,char** argv)
{
#ifdef _WIN32
    resolveAssetsWorkingDirectory();
#endif

    glutInit(&argc,argv);

    /* CUESTIONABLE: sin GLUT_ALPHA ni GLUT_DEPTH. El overlay de
       segments.c usa alpha igual, porque el blending solo necesita el
       alpha de origen; el 2D no necesita Z-buffer. */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800,600); /* tamano fijo, no configurable */

    glutCreateWindow("Reloj Artistico");
    /* CUESTIONABLE: no se valida si la ventana se creo bien. */

#ifdef _WIN32
    activateVsync();
#endif

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
