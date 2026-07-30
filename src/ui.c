#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>

#include "ui.h"
#include "segments.h"
#include "artwork_catalog.h"
#include "app_config.h"

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

static void drawArtworkPanel(void)
{
    float left;
    float right;
    float bottom;
    float top;

    const ArtworkInfo *artwork = getArtworkInfo(getActiveArtworkType());

    float margin = pixelsToWorldUnits(26);
    float panelWidth = pixelsToWorldUnits(310);
    float panelHeight = pixelsToWorldUnits(178);
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

   contentX = x + pixelsToWorldUnits(16);
textY = y - pixelsToWorldUnits(20);

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
    drawArtworkPanel();
}
