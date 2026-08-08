#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>

#include "ui.h"
#include "segments.h"
#include "artwork_catalog.h"
#include "app_config.h"
#include "texture.h"
#include "audio.h"

/*
 * Convierte pixeles a unidades del mundo usando la proyeccion actual.
 *
 * Esta funcion replica el criterio de reshape(): el eje menor conserva el
 * rango -1..1 y el eje mayor se expande segun la relacion de aspecto. Con eso
 * los textos pueden alinearse de forma estable aunque cambie el tamano de la
 * ventana.
 */
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

/*
 * Calcula los limites visibles del mundo en la proyeccion ortografica actual.
 *
 * Mantener este calculo en UI permite posicionar paneles por esquina sin
 * depender de coordenadas fijas que solo funcionen para una relacion de aspecto.
 */
static void getWorldBounds(float *left, float *right, float *bottom, float *top)
{
    GLint viewport[4];
    float aspect;

    glGetIntegerv(GL_VIEWPORT, viewport);

    if(viewport[2] <= 0 || viewport[3] <= 0)
    {
        *left = -1.0f;
        *right = 1.0f;
        *bottom = -1.0f;
        *top = 1.0f;
        return;
    }

    aspect = (float)viewport[2] / (float)viewport[3];

    if(aspect >= 1.0f)
    {
        *left = -aspect;
        *right = aspect;
        *bottom = -1.0f;
        *top = 1.0f;
    }
    else
    {
        *left = -1.0f;
        *right = 1.0f;
        *bottom = -1.0f / aspect;
        *top = 1.0f / aspect;
    }
}

static void drawBitmapText(float x, float y, void *font, const char *text)
{
    int i;

    glRasterPos2f(x, y);

    for(i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(font, text[i]);
}

static void drawTextLine(float x, float y, void *font, const char *text)
{
    drawBitmapText(x, y, font, text);
}

/*
 * Divide una descripcion en lineas que caben dentro del panel.
 *
 * FreeGLUT no ofrece layout de texto, por eso el panel mide cada palabra con
 * glutBitmapWidth() y corta en espacios. Esto evita que la descripcion invada
 * el reloj o se salga por el borde derecho del panel.
 */
static int wrapText(const char *text,
                    void *font,
                    int maxWidthPixels,
                    char lines[][128],
                    int maxLines)
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

        if(length > 127)
            length = 127;

        memcpy(lines[lineCount], text + lineStart, (size_t)length);
        lines[lineCount][length] = '\0';

        lineCount++;
    }

    return lineCount;
}

static void drawPanelBackground(float x, float y, float width, float height)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.02f, 0.02f, 0.025f, 0.68f);

    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 0.10f);

    glLineWidth(1.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();
}

static void drawAccentStrip(float x, float y, float height, ArtworkColor accent)
{
    float stripWidth = pixelsToWorldUnits(4);

    glColor3f(accent.red, accent.green, accent.blue);

    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + stripWidth, y);
        glVertex2f(x + stripWidth, y - height);
        glVertex2f(x, y - height);
    glEnd();
}

/* Dibuja imagePath dentro de la caja (x,y,width,height) preservando su
   proporcion real ("contain": nunca la deforma, la centra y deja
   margen si la caja no tiene el mismo aspecto). Si no hay imagen
   valida (ver ADR-014), no dibuja nada y devuelve 0 -- quien llama
   decide que hacer con ese espacio (ver drawArtworkPanel). */
static int drawArtworkImage(float x, float y, float width, float height, const char *imagePath)
{
    Texture tex = loadTexture(imagePath);
    float imageAspect;
    float boxAspect;
    float drawWidth;
    float drawHeight;
    float drawX;
    float drawY;

    if(tex.textureId == 0)
        return 0;

    imageAspect = (float)tex.width / (float)tex.height;
    boxAspect = width / height;

    if(imageAspect > boxAspect)
    {
        drawWidth = width;
        drawHeight = width / imageAspect;
    }
    else
    {
        drawHeight = height;
        drawWidth = height * imageAspect;
    }

    drawX = x + (width - drawWidth) * 0.5f;
    drawY = y - (height - drawHeight) * 0.5f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex.textureId);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(drawX, drawY);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(drawX + drawWidth, drawY);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(drawX + drawWidth, drawY - drawHeight);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(drawX, drawY - drawHeight);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D); /* critico: si queda encendido, tinta el resto del panel */

    return 1;
}

static void drawPaletteSwatch(float x, float y, const ArtworkColor palette[3])
{
    float size = pixelsToWorldUnits(13);
    float gap = pixelsToWorldUnits(5);
    int i;

    for(i = 0; i < 3; i++)
    {
        float sx = x + i * (size + gap);

        glColor3f(palette[i].red, palette[i].green, palette[i].blue);

        glBegin(GL_QUADS);
            glVertex2f(sx, y);
            glVertex2f(sx + size, y);
            glVertex2f(sx + size, y - size);
            glVertex2f(sx, y - size);
        glEnd();
    }
}

static void drawIntervalOption(float x,
                               float y,
                               const char *label,
                               int isActive,
                               ArtworkColor accent)
{
    float width = pixelsToWorldUnits(58);
    float height = pixelsToWorldUnits(23);
    float textX = x + pixelsToWorldUnits(10);
    float textY = y - pixelsToWorldUnits(15);

    if(isActive)
    {
        glColor4f(accent.red, accent.green, accent.blue, 0.22f);
    }
    else
    {
        glColor4f(0.02f, 0.02f, 0.025f, 0.48f);
    }

    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();

    if(isActive)
        glColor3f(accent.red, accent.green, accent.blue);
    else
        glColor3f(0.58f, 0.61f, 0.64f);

    drawTextLine(textX, textY, GLUT_BITMAP_HELVETICA_10, label);
}

static void drawIntervalSelector(void)
{
    float left;
    float right;
    float bottom;
    float top;
    const ArtworkInfo *artwork = getArtworkInfo(getActiveArtworkType());
    ChangeInterval activeInterval = getChangeInterval();
    float margin = pixelsToWorldUnits(26);
    float x;
    float y;
    float gap = pixelsToWorldUnits(7);
    float optionWidth = pixelsToWorldUnits(58);

    getWorldBounds(&left, &right, &bottom, &top);

    x = left + margin;
    y = top - margin;

    glColor3f(0.72f, 0.75f, 0.78f);
    drawTextLine(
        x,
        y,
        GLUT_BITMAP_HELVETICA_10,
        "cambio de obra"
    );

    y -= pixelsToWorldUnits(15);

    drawIntervalOption(
        x,
        y,
        getChangeIntervalLabel(CHANGE_INTERVAL_15_MINUTES),
        activeInterval == CHANGE_INTERVAL_15_MINUTES,
        artwork->accentColor
    );

    drawIntervalOption(
        x + optionWidth + gap,
        y,
        getChangeIntervalLabel(CHANGE_INTERVAL_30_MINUTES),
        activeInterval == CHANGE_INTERVAL_30_MINUTES,
        artwork->accentColor
    );

    drawIntervalOption(
        x + (optionWidth + gap) * 2.0f,
        y,
        getChangeIntervalLabel(CHANGE_INTERVAL_60_MINUTES),
        activeInterval == CHANGE_INTERVAL_60_MINUTES,
        artwork->accentColor
    );
}

/* Rectangulo (en unidades de mundo) del boton de sonido, recalculado
   cada frame en drawSoundToggle() para que uiHandleClick() sepa donde
   esta sin duplicar el calculo de layout. */
static float soundButtonX;
static float soundButtonY;
static float soundButtonWidth;
static float soundButtonHeight;

static void drawSoundToggle(void)
{
    float left;
    float right;
    float bottom;
    float top;
    const ArtworkInfo *artwork = getArtworkInfo(getActiveArtworkType());
    int muted = isAudioMuted();
    float margin = pixelsToWorldUnits(26);
    float x;
    float y;
    float textX;
    float textY;

    getWorldBounds(&left, &right, &bottom, &top);

    x = left + margin;

    /* debajo del selector de intervalo: margen + etiqueta (15px) +
       fila de opciones (23px) + aire (14px) */
    y = top - margin - pixelsToWorldUnits(15) - pixelsToWorldUnits(23) - pixelsToWorldUnits(14);

    glColor3f(0.72f, 0.75f, 0.78f);
    drawTextLine(x, y, GLUT_BITMAP_HELVETICA_10, "sonido");

    y -= pixelsToWorldUnits(15);

    soundButtonX = x;
    soundButtonY = y;
    soundButtonWidth = pixelsToWorldUnits(140);
    soundButtonHeight = pixelsToWorldUnits(23);

    if(muted)
        glColor4f(0.02f, 0.02f, 0.025f, 0.48f);
    else
        glColor4f(artwork->accentColor.red, artwork->accentColor.green, artwork->accentColor.blue, 0.22f);

    glBegin(GL_QUADS);
        glVertex2f(soundButtonX, soundButtonY);
        glVertex2f(soundButtonX + soundButtonWidth, soundButtonY);
        glVertex2f(soundButtonX + soundButtonWidth, soundButtonY - soundButtonHeight);
        glVertex2f(soundButtonX, soundButtonY - soundButtonHeight);
    glEnd();

    if(muted)
        glColor3f(0.58f, 0.61f, 0.64f);
    else
        glColor3f(artwork->accentColor.red, artwork->accentColor.green, artwork->accentColor.blue);

    textX = soundButtonX + pixelsToWorldUnits(10);
    textY = soundButtonY - pixelsToWorldUnits(15);

    drawTextLine(textX, textY, GLUT_BITMAP_HELVETICA_10, muted ? "Activar musica" : "Silenciar musica");
}

/* Rectangulos (en unidades de mundo) de los botones Anterior/Siguiente,
   recalculados cada frame en drawArtworkNavigator(). */
static float prevButtonX;
static float prevButtonY;
static float navButtonWidth;
static float navButtonHeight;
static float nextButtonX;
static float nextButtonY;

static void drawNavButton(float x, float y, float width, float height,
                          const char *label, ArtworkColor accent)
{
    float textWidth = (float)glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *)label);
    float textX = x + (width - pixelsToWorldUnits((int)textWidth)) * 0.5f;
    float textY = y - height * 0.5f - pixelsToWorldUnits(4);

    glColor4f(0.02f, 0.02f, 0.025f, 0.55f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();

    glColor4f(accent.red, accent.green, accent.blue, 0.35f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();

    glColor3f(0.88f, 0.88f, 0.90f);
    drawTextLine(textX, textY, GLUT_BITMAP_HELVETICA_10, label);
}

/* Barra inferior: botones Anterior/Siguiente para saltar de obra a
   mano dentro de la sala activa, mas el contador de cuanto falta para
   el proximo cambio automatico por hora real (ver ADR pendiente y
   getSecondsUntilNextArtworkChange en segments.c: la navegacion manual
   no altera ese contador, el automatico siempre termina retomando el
   control). */
static void drawArtworkNavigator(void)
{
    float left;
    float right;
    float bottom;
    float top;
    const ArtworkInfo *artwork = getArtworkInfo(getActiveArtworkType());
    float margin = pixelsToWorldUnits(26);
    float gap = pixelsToWorldUnits(10);
    float totalWidth;
    float countdownY;
    int secondsLeft = getSecondsUntilNextArtworkChange();
    int minutesLeft;
    char countdownText[48];

    if(secondsLeft < 0)
        secondsLeft = 0;

    minutesLeft = secondsLeft / 60;
    secondsLeft = secondsLeft % 60;

    getWorldBounds(&left, &right, &bottom, &top);

    navButtonWidth  = pixelsToWorldUnits(96);
    navButtonHeight = pixelsToWorldUnits(26);
    totalWidth = navButtonWidth * 2.0f + gap;

    prevButtonX = -totalWidth * 0.5f;
    nextButtonX = prevButtonX + navButtonWidth + gap;

    countdownY = bottom + margin;
    prevButtonY = countdownY + pixelsToWorldUnits(14) + gap + navButtonHeight;
    nextButtonY = prevButtonY;

    drawNavButton(prevButtonX, prevButtonY, navButtonWidth, navButtonHeight, "< Anterior", artwork->accentColor);
    drawNavButton(nextButtonX, nextButtonY, navButtonWidth, navButtonHeight, "Siguiente >", artwork->accentColor);

    snprintf(countdownText, sizeof(countdownText), "proxima obra en %02d:%02d", minutesLeft, secondsLeft);

    glColor3f(0.52f, 0.55f, 0.58f);
    {
        float textWidth = (float)glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (const unsigned char *)countdownText);
        float textX = 0.0f - pixelsToWorldUnits((int)textWidth) * 0.5f;

        drawTextLine(textX, countdownY, GLUT_BITMAP_HELVETICA_10, countdownText);
    }
}

/* Convierte un click en pixeles (origen arriba-izquierda, como lo
   entrega GLUT) a las mismas unidades de mundo que usa el resto de la
   UI, para poder comparar contra soundButtonX/Y/Width/Height. */
static void pixelToWorld(int pixelX, int pixelY, float *worldX, float *worldY)
{
    GLint viewport[4];
    float left;
    float right;
    float bottom;
    float top;

    glGetIntegerv(GL_VIEWPORT, viewport);
    getWorldBounds(&left, &right, &bottom, &top);

    if(viewport[2] <= 0 || viewport[3] <= 0)
    {
        *worldX = 0.0f;
        *worldY = 0.0f;
        return;
    }

    *worldX = left + ((float)pixelX / (float)viewport[2]) * (right - left);
    *worldY = top - ((float)pixelY / (float)viewport[3]) * (top - bottom);
}

int uiHandleClick(int pixelX, int pixelY)
{
    float worldX;
    float worldY;

    pixelToWorld(pixelX, pixelY, &worldX, &worldY);

    if(worldX >= soundButtonX && worldX <= soundButtonX + soundButtonWidth &&
       worldY <= soundButtonY && worldY >= soundButtonY - soundButtonHeight)
    {
        toggleAudioMute();
        return 1;
    }

    if(worldX >= prevButtonX && worldX <= prevButtonX + navButtonWidth &&
       worldY <= prevButtonY && worldY >= prevButtonY - navButtonHeight)
    {
        goToPreviousArtwork();
        return 1;
    }

    if(worldX >= nextButtonX && worldX <= nextButtonX + navButtonWidth &&
       worldY <= nextButtonY && worldY >= nextButtonY - navButtonHeight)
    {
        goToNextArtwork();
        return 1;
    }

    return 0;
}

static void drawArtworkPanel(void)
{
    float left;
    float right;
    float bottom;
    float top;

    const ArtworkInfo *artwork = getArtworkInfo(getActiveArtworkType());

    /* Se prueba la imagen ANTES de fijar panelHeight: sin archivo real
       todavia (imagePath apunta a un placeholder que no existe), el
       panel debe quedar exactamente del tamano compacto de siempre,
       no dejar un hueco vacio reservado para una imagen que no esta. */
    int hasImage = (loadTexture(artwork->imagePath).textureId != 0);
    int imageBandPx = hasImage ? 140 : 0; /* 10 margen + 120 imagen + 10 aire */

    float margin = pixelsToWorldUnits(26);
    float panelWidth = pixelsToWorldUnits(310);
    float panelHeight = pixelsToWorldUnits(178 + imageBandPx);
    float contentX;
    float textY;
    char metaText[96];
    char sectorText[32];
    char descriptionLines[3][128];
    int descriptionLineCount;
    int i;

    float x;
    float y;

    getWorldBounds(&left, &right, &bottom, &top);

    x = right - margin - panelWidth;
    y = top - margin;

    drawPanelBackground(x, y, panelWidth, panelHeight);
    drawAccentStrip(x, y, panelHeight, artwork->accentColor);

    if(hasImage)
    {
        float imageMargin = pixelsToWorldUnits(10);

        drawArtworkImage(
            x + imageMargin,
            y - imageMargin,
            panelWidth - imageMargin * 2.0f,
            pixelsToWorldUnits(120),
            artwork->imagePath
        );
    }

   contentX = x + pixelsToWorldUnits(16);
textY = y - pixelsToWorldUnits(imageBandPx + 20);

snprintf(
    sectorText,
    sizeof(sectorText),
    "sector %d de 6",
    getActiveArtworkIndex() + 1
);

glColor3f(
    artwork->accentColor.red,
    artwork->accentColor.green,
    artwork->accentColor.blue
);
drawTextLine(
    contentX,
    textY,
    GLUT_BITMAP_HELVETICA_10,
    sectorText
);

    textY -= pixelsToWorldUnits(20);

    glColor3f(0.96f, 0.94f, 0.88f);
    drawTextLine(
    contentX,
    textY,
    GLUT_BITMAP_HELVETICA_18,
    artwork->title
);

    textY -= pixelsToWorldUnits(23);

    glColor3f(artwork->accentColor.red, artwork->accentColor.green, artwork->accentColor.blue);
    drawTextLine(
        contentX,
        textY,
        GLUT_BITMAP_HELVETICA_12,
        artwork->author
    );

    snprintf(
        metaText,
        sizeof(metaText),
        "%s - %s",
        artwork->year,
        artwork->movement
    );

    textY -= pixelsToWorldUnits(17);

    glColor3f(0.62f, 0.66f, 0.70f);
    drawTextLine(
        contentX,
        textY,
        GLUT_BITMAP_HELVETICA_10,
        metaText
    );

    textY -= pixelsToWorldUnits(23);

    glColor4f(1.0f, 1.0f, 1.0f, 0.14f);
    glBegin(GL_LINES);
        glVertex2f(contentX, textY + pixelsToWorldUnits(9));
        glVertex2f(x + panelWidth - pixelsToWorldUnits(14), textY + pixelsToWorldUnits(9));
    glEnd();

    descriptionLineCount = wrapText(
        artwork->description,
        GLUT_BITMAP_HELVETICA_10,
        265,
        descriptionLines,
        3
    );

    glColor3f(0.84f, 0.85f, 0.86f);

    for(i = 0; i < descriptionLineCount; i++)
    {
        drawTextLine(
            contentX,
            textY - pixelsToWorldUnits(i * 16),
            GLUT_BITMAP_HELVETICA_10,
            descriptionLines[i]
        );
    }

    glColor3f(0.48f, 0.52f, 0.56f);
    drawTextLine(
        contentX,
        y - panelHeight + pixelsToWorldUnits(25),
        GLUT_BITMAP_HELVETICA_10,
        "paleta"
    );

    drawPaletteSwatch(
        contentX + pixelsToWorldUnits(47),
        y - panelHeight + pixelsToWorldUnits(29),
        artwork->palette
    );
}

void drawUserInterface(void)
{
    drawIntervalSelector();
    drawSoundToggle();
    drawArtworkPanel();
    drawArtworkNavigator();
}
