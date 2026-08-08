#include <GL/freeglut.h>

#include "display.h"
#include "clock.h"
#include "segments.h"
#include "utils.h"
#include "artwork_catalog.h"
#include "ui.h"
#include "audio.h"
#include "texture.h"
#include <stdio.h>
#include <string.h>

#define FRAME_INTERVAL_MS 33

/* pequeno corrimiento hacia abajo (deja un poco mas de aire arriba,
   donde estan los paneles de UI, que abajo, donde esta el navegador de
   obras); el offset y los radios de segments.c (ver loadRoomIntoSegments)
   se ajustaron juntos para que el anillo completo (incluido el halo del
   sector activo) entre sin recortarse ni chocar con ningun panel. */
static const float WHEEL_VERTICAL_OFFSET = 0.02f;

/* Secuencia de arranque (ver ADR-017): carga breve -> intro de las 3
   franjas de tiempo -> reloj. La intro avanza con cualquier tecla, no
   con mouse, para no sumar un callback nuevo la vispera de la entrega. */
typedef enum
{
    SCREEN_LOADING,
    SCREEN_INTRO,
    SCREEN_CLOCK

} AppScreen;

static AppScreen currentScreen = SCREEN_LOADING;
static float loadingElapsedSeconds = 0.0f;
static const float LOADING_DURATION_SECONDS = 2.0f;

/* Precarga de texturas durante la pantalla de carga (ver
 * preloadNextTexture() y su llamada en timer()).
 *
 * Crear una textura de OpenGL (glGenTextures + glTexImage2D) mientras
 * el usuario interactua -- por ejemplo al clickear Siguiente y pasar a
 * una obra que todavia no cargo su imagen -- crashea de forma
 * reproducible en varias maquinas (ver
 * docs/09-Diagnostico-Pantallazo-VIDEO-SCHEDULER.md). Se descarto que
 * sea corrupcion de imagen o mal manejo de memoria (las 19 imagenes
 * cargan perfecto con stb_image puro, sin OpenGL); el problema aparece
 * especificamente al crear texturas GL durante el uso interactivo.
 *
 * La mitigacion: cargar las 19 texturas (18 obras + logo) UNA POR
 * FRAME durante la pantalla de carga, antes de que el usuario pueda
 * tocar nada -- cada carga queda separada por un ciclo real de
 * timer()/display()/glutSwapBuffers(), a diferencia de un loop
 * apretado. Para cuando se puede interactuar, loadTexture() siempre
 * pega en cache (ver texture.c) y no vuelve a crear una textura GL. */
static int preloadIndex = 0;

static const char *getPreloadImagePath(int index)
{
    static const char *paths[1 + ROOM_SIZE * 3];
    static int initialized = 0;
    int i;

    if(!initialized)
    {
        int n = 0;

        paths[n++] = "assets/logo.png";

        for(i = 0; i < ROOM_SIZE; i++) paths[n++] = getArtworkInfo(ROOM_15_MIN[i])->imagePath;
        for(i = 0; i < ROOM_SIZE; i++) paths[n++] = getArtworkInfo(ROOM_30_MIN[i])->imagePath;
        for(i = 0; i < ROOM_SIZE; i++) paths[n++] = getArtworkInfo(ROOM_60_MIN[i])->imagePath;

        initialized = 1;
    }

    if(index < 0 || index >= (int)(sizeof(paths) / sizeof(paths[0])))
        return 0;

    return paths[index];
}

#define PRELOAD_TOTAL (1 + ROOM_SIZE * 3)

/* Un paso de precarga por llamada (ver preloadIndex). Se llama una vez
   por tick del timer mientras dure SCREEN_LOADING. */
static void preloadNextTexture(void)
{
    if(preloadIndex >= PRELOAD_TOTAL)
        return;

    loadTexture(getPreloadImagePath(preloadIndex));
    preloadIndex++;
}

static const float GOLD[3] = {0.85f, 0.65f, 0.25f};

/* pixeles -> unidades del mundo y texto centrado: mismo criterio
   duplicado en segments.c/ui.c/clock.c (ver nota en clock.c). */
static float pixelsToWorldUnits(int pixels)
{
    GLint viewport[4];
    float aspect;
    float worldHeight;

    glGetIntegerv(GL_VIEWPORT, viewport);

    if(viewport[2] <= 0 || viewport[3] <= 0)
        return 0.0f;

    aspect = (float)viewport[2] / (float)viewport[3];
    worldHeight = (aspect >= 1.0f) ? 2.0f : (2.0f / aspect);

    return (float)pixels * worldHeight / (float)viewport[3];
}

static void drawCenteredText(float centerX, float y, void *font, const char *text)
{
    int totalPixelWidth = 0;
    int i;
    float startX;

    for(i = 0; text[i] != '\0'; i++)
        totalPixelWidth += glutBitmapWidth(font, text[i]);

    startX = centerX - pixelsToWorldUnits(totalPixelWidth) * 0.5f;

    glRasterPos2f(startX, y);

    for(i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(font, text[i]);
}

/* dibuja "RELOJ ARTISTIC" en un color y la ultima "O" en dorado, como
   acento de marca (eco del logotipo, sin depender de cargar la imagen
   real todavia). Centrado como una sola pieza. */
static void drawWordmark(float centerY, void *font)
{
    static const char *base = "RELOJ ARTISTIC";
    static const char *lastLetter = "O";
    int baseWidthPx = 0;
    int lastWidthPx = 0;
    int i;
    float startX;

    for(i = 0; base[i] != '\0'; i++)
        baseWidthPx += glutBitmapWidth(font, base[i]);

    for(i = 0; lastLetter[i] != '\0'; i++)
        lastWidthPx += glutBitmapWidth(font, lastLetter[i]);

    startX = -pixelsToWorldUnits(baseWidthPx + lastWidthPx) * 0.5f;

    glColor3f(0.94f, 0.94f, 0.96f);
    glRasterPos2f(startX, centerY);
    for(i = 0; base[i] != '\0'; i++)
        glutBitmapCharacter(font, base[i]);

    glColor3f(GOLD[0], GOLD[1], GOLD[2]);
    glRasterPos2f(startX + pixelsToWorldUnits(baseWidthPx), centerY);
    for(i = 0; lastLetter[i] != '\0'; i++)
        glutBitmapCharacter(font, lastLetter[i]);
}

/* linea corta de acento, el mismo recurso que usa el mockup de
   referencia arriba de cada titular (un "filete" dorado). */
static void drawAccentRule(float centerX, float y, float halfWidth)
{
    glColor3f(GOLD[0], GOLD[1], GOLD[2]);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
        glVertex2f(centerX - halfWidth, y);
        glVertex2f(centerX + halfWidth, y);
    glEnd();
}

/* nombre del proyecto, subtitulo y una barra de progreso simulada que
   avanza con el tiempo transcurrido (no representa carga real de
   archivos, es una presentacion de marca breve, ver ADR-017). */
static void drawLoadingScreen(void)
{
    Texture logo = loadTexture("assets/logo.png");
    float progress = loadingElapsedSeconds / LOADING_DURATION_SECONDS;
    const float barWidth = 0.5f;
    const float barHeight = 0.016f;
    const float barY = -0.68f;

    if(progress > 1.0f) progress = 1.0f;

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(logo.textureId != 0)
    {
        /* el logo ya trae el nombre del proyecto dibujado adentro, asi
           que reemplaza al wordmark de texto, no lo acompana. */
        const float logoWidth = 1.00f;
        const float logoHeight = logoWidth * (float)logo.height / (float)logo.width;
        const float centerY = 0.10f;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, logo.textureId);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-logoWidth * 0.5f, centerY + logoHeight * 0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( logoWidth * 0.5f, centerY + logoHeight * 0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( logoWidth * 0.5f, centerY - logoHeight * 0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-logoWidth * 0.5f, centerY - logoHeight * 0.5f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D); /* critico: si queda encendido, tinta el resto de la UI */
    }
    else
    {
        /* degradacion segura (ver ADR-014): sin logo.png, se dibuja
           el wordmark de texto que ya existia antes de esta imagen. */
        drawAccentRule(0.0f, 0.10f, 0.045f);

        drawWordmark(0.0f, GLUT_BITMAP_TIMES_ROMAN_24);

        glColor3f(0.52f, 0.52f, 0.56f);
        drawCenteredText(0.0f, -0.10f, GLUT_BITMAP_HELVETICA_10, "ARTE  -  TIEMPO  -  COMPUTACION GRAFICA");
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, 0.14f);
    glBegin(GL_QUADS);
        glVertex2f(-barWidth * 0.5f, barY + barHeight);
        glVertex2f( barWidth * 0.5f, barY + barHeight);
        glVertex2f( barWidth * 0.5f, barY);
        glVertex2f(-barWidth * 0.5f, barY);
    glEnd();

    glColor3f(GOLD[0], GOLD[1], GOLD[2]);
    glBegin(GL_QUADS);
        glVertex2f(-barWidth * 0.5f, barY + barHeight);
        glVertex2f(-barWidth * 0.5f + barWidth * progress, barY + barHeight);
        glVertex2f(-barWidth * 0.5f + barWidth * progress, barY);
        glVertex2f(-barWidth * 0.5f, barY);
    glEnd();

    glutSwapBuffers();
}

/* Tarjeta de una franja de tiempo en la pantalla de intro: borde fino,
   etiqueta dorada, titulo, y dos lineas de descripcion cortas (fijas,
   sin wrap generico, porque el contenido de cada tarjeta es conocido
   y breve). */
static void drawIntroCard(float x, float y, float width, float height,
                           const char *label, const char *title,
                           const char *desc1, const char *desc2)
{
    float textX = x + pixelsToWorldUnits(16);

    glColor4f(1.0f, 1.0f, 1.0f, 0.05f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 0.16f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();

    glColor3f(GOLD[0], GOLD[1], GOLD[2]);
    glRasterPos2f(textX, y - pixelsToWorldUnits(22));
    { int i; for(i = 0; label[i] != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, label[i]); }

    glColor3f(0.94f, 0.94f, 0.96f);
    glRasterPos2f(textX, y - pixelsToWorldUnits(42));
    { int i; for(i = 0; title[i] != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, title[i]); }

    glColor3f(0.60f, 0.60f, 0.64f);
    glRasterPos2f(textX, y - pixelsToWorldUnits(62));
    { int i; for(i = 0; desc1[i] != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, desc1[i]); }

    glRasterPos2f(textX, y - pixelsToWorldUnits(76));
    { int i; for(i = 0; desc2[i] != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, desc2[i]); }
}

/* Divide "text" en lineas que entran en maxWidthPixels, cortando en
   espacios. Mismo criterio que wrapText() en ui.c (duplicado aqui
   porque display.c no depende de ui.c) -- FreeGLUT no da layout de
   texto, asi que hay que medir palabra por palabra con
   glutBitmapWidth(). */
static int wrapTextForIntro(const char *text, void *font, int maxWidthPixels,
                             char lines[][160], int maxLines)
{
    int lineCount = 0;
    int position = 0;

    while(text[position] != '\0' && lineCount < maxLines)
    {
        int lineStart;
        int lastSpace = -1;
        int width = 0;
        int length;

        while(text[position] == ' ')
            position++;

        lineStart = position;

        while(text[position] != '\0')
        {
            width += glutBitmapWidth(font, text[position]);

            if(text[position] == ' ')
                lastSpace = position;

            if(width > maxWidthPixels && lastSpace >= lineStart)
                break;

            position++;
        }

        if(text[position] == '\0')
        {
            length = position - lineStart;
        }
        else if(lastSpace >= lineStart)
        {
            length = lastSpace - lineStart;
            position = lastSpace + 1;
        }
        else
        {
            length = position - lineStart;
        }

        if(length > 159)
            length = 159;

        memcpy(lines[lineCount], text + lineStart, (size_t)length);
        lines[lineCount][length] = '\0';

        lineCount++;
    }

    return lineCount;
}

/* Parrafo de presentacion del proyecto, en una tarjeta propia entre el
   subtitulo y las 3 franjas de tiempo: mismo tratamiento visual
   (borde fino, fondo casi transparente) que drawIntroCard(), para que
   se lea como parte organizada de la pantalla y no como texto plano
   suelto. */
static void drawProjectDescription(float x, float y, float width, float height)
{
    static const char *description =
        "El Reloj Artistico es un proyecto de Computacion Grafica que busca "
        "acercar el arte al publico de una manera interactiva. Cada reloj "
        "contiene diferentes obras de autores distintos, recreadas mediante "
        "figuras geometricas que conservan la esencia visual de la pieza "
        "original.";
    char lines[5][160];
    int lineCount;
    int i;
    float lineHeight = pixelsToWorldUnits(20);
    float textY;

    glColor4f(1.0f, 1.0f, 1.0f, 0.04f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();

    glColor4f(GOLD[0], GOLD[1], GOLD[2], 0.30f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();

    lineCount = wrapTextForIntro(description, GLUT_BITMAP_HELVETICA_12,
                                  (int)(width / pixelsToWorldUnits(1)) - 40,
                                  lines, 5);

    textY = y - (height - (float)lineCount * lineHeight) * 0.5f - pixelsToWorldUnits(14);

    glColor3f(0.78f, 0.78f, 0.82f);

    for(i = 0; i < lineCount; i++)
        drawCenteredText(x + width * 0.5f, textY - (float)i * lineHeight, GLUT_BITMAP_HELVETICA_12, lines[i]);
}

/* Pantalla intermedia: explica las 3 franjas de tiempo antes de entrar
   al reloj. Avanza con cualquier tecla (ver advanceFromIntroScreen),
   no con mouse, para no sumar un callback nuevo (ver ADR-017). */
static void drawIntroScreen(void)
{
    const float cardWidth = 0.5f;
    const float cardHeight = 0.42f;
    const float cardGap = 0.05f;
    const float cardsTotalWidth = cardWidth * 3.0f + cardGap * 2.0f;
    const float cardsLeft = -cardsTotalWidth * 0.5f;
    const float cardsTop = -0.05f;
    const float buttonWidth = 0.34f;
    const float buttonHeight = 0.11f;
    const float buttonY = -0.62f;

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawAccentRule(-cardsTotalWidth * 0.5f, 0.62f, 0.045f);

    glColor3f(0.94f, 0.94f, 0.96f);
    glRasterPos2f(cardsLeft, 0.50f);
    {
        static const char *headline = "Tres franjas, un mismo reloj";
        int i;
        for(i = 0; headline[i] != '\0'; i++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, headline[i]);
    }

    glColor3f(0.66f, 0.66f, 0.70f);
    drawCenteredText(0.0f, 0.34f, GLUT_BITMAP_HELVETICA_12,
        "Elegis cada cuanto cambia la obra activa: eso define el ritmo del reloj.");

    drawProjectDescription(cardsLeft, 0.25f, cardsTotalWidth, 0.20f);

    drawIntroCard(cardsLeft, cardsTop, cardWidth, cardHeight,
        "CADA 15 MIN", "Ritmo rapido",
        "La obra activa", "rota mas seguido.");

    drawIntroCard(cardsLeft + cardWidth + cardGap, cardsTop, cardWidth, cardHeight,
        "CADA 30 MIN", "Ritmo intermedio",
        "Configuracion", "por defecto.");

    drawIntroCard(cardsLeft + (cardWidth + cardGap) * 2.0f, cardsTop, cardWidth, cardHeight,
        "CADA 1 HORA", "Ritmo pausado",
        "Ideal para mirar", "cada obra con calma.");

    glColor4f(GOLD[0], GOLD[1], GOLD[2], 0.90f);
    glBegin(GL_QUADS);
        glVertex2f(-buttonWidth * 0.5f, buttonY + buttonHeight);
        glVertex2f( buttonWidth * 0.5f, buttonY + buttonHeight);
        glVertex2f( buttonWidth * 0.5f, buttonY);
        glVertex2f(-buttonWidth * 0.5f, buttonY);
    glEnd();

    glColor3f(0.10f, 0.09f, 0.06f);
    drawCenteredText(0.0f, buttonY + buttonHeight * 0.5f - pixelsToWorldUnits(4),
        GLUT_BITMAP_HELVETICA_12, "Entrar al reloj");

    glColor3f(0.46f, 0.46f, 0.50f);
    drawCenteredText(0.0f, buttonY - pixelsToWorldUnits(20),
        GLUT_BITMAP_HELVETICA_10, "presiona cualquier tecla para continuar");

    glutSwapBuffers();
}

void initDisplay(void)
{
    glClearColor(0.05f,0.05f,0.05f,1.0f); /* fondo casi negro */

    printf("OpenGL vendor: %s\n", glGetString(GL_VENDOR));
    printf("OpenGL renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
}

/* Se ejecuta cada frame. El reloj completo solo se dibuja en
   SCREEN_CLOCK; las otras dos pantallas son autocontenidas (ver
   ADR-017). */
void display(void)
{
    if(currentScreen == SCREEN_LOADING)
    {
        drawLoadingScreen();
        return;
    }

    if(currentScreen == SCREEN_INTRO)
    {
        drawIntroScreen();
        return;
    }

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

   glPushMatrix();
   glTranslatef(0.0f, WHEEL_VERTICAL_OFFSET, 0.0f);

   drawClock();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(accent.red, accent.green, accent.blue, 0.28f);
    glLineWidth(1.2f);
    drawCircle(0.0f, 0.0f, 0.68f); /* 15% mas chico, ver nota junto a WHEEL_VERTICAL_OFFSET */

    /* anillo exterior con color pulsante (verde-amarillo). CUESTIONABLE:
       es independiente del pulso dorado del segmento destacado en
       segments.c; son dos animaciones de "brillo" separadas sin relacion
       entre si. */
    glColor4f(accent.red, accent.green, accent.blue, 0.62f);
    glLineWidth(2.5f);

drawCircle(0.0f, 0.0f, 0.76f); /* 15% mas chico, ver nota junto a WHEEL_VERTICAL_OFFSET */

    drawSegments();

   glPopMatrix();

    /* fuera del translate: la UI usa coordenadas de pantalla via
       getWorldBounds(), no debe moverse con la rueda. */
    drawUserInterface();

    glutSwapBuffers();
}


/* Motor de la animacion: se reprograma cada 16ms. "value" no se usa,
   es obligatorio por la firma que exige glutTimerFunc. */
void timer(int value)
{
    (void)value;

    if(currentScreen == SCREEN_LOADING)
    {
        loadingElapsedSeconds += FRAME_INTERVAL_MS / 1000.0f;

        preloadNextTexture();

        /* no pasa a la intro hasta cumplir el tiempo minimo de marca
           Y terminar de precargar -- lo que tarde mas. Con 19
           texturas a un ritmo de una por frame (33ms) la precarga
           sola tarda bastante menos que LOADING_DURATION_SECONDS, asi
           que en la practica el tiempo minimo de marca es el que
           manda y la precarga ya esta lista de sobra. */
        if(loadingElapsedSeconds >= LOADING_DURATION_SECONDS && preloadIndex >= PRELOAD_TOTAL)
            currentScreen = SCREEN_INTRO;
    }
    else if(currentScreen == SCREEN_CLOCK)
    {
        updateSegments();

        updateAudioState();
    }

    glutPostRedisplay();

    glutTimerFunc(FRAME_INTERVAL_MS,timer,0);
}

/* Unico punto de salida de SCREEN_INTRO. Se llama desde el manejador
   de teclado en main.c ante CUALQUIER tecla; si la pantalla actual no
   es la intro, no hace nada (evita que una tecla cualquiera interfiera
   con el reloj o la carga). */
void advanceFromIntroScreen(void)
{
    if(currentScreen == SCREEN_INTRO)
        currentScreen = SCREEN_CLOCK;
}
