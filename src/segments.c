#include <math.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "segments.h"
#include "utils.h"
#include "clock.h"
#include "segment_monet.h"
#include "segment_mondrian.h"
#include "segment_kandinsky.h"
#include "segment_vangogh.h"
#include "segment_klimt.h"
#include "segment_hokusai.h"

Segment segments[6]; /* los 6 sectores del reloj, en orden fijo */

void initSegments(void)
{
    /* angulo, radio interior, radio exterior, animation (sin uso), obra */
    segments[0] = (Segment){0.0f,   0.42f,0.82f,0.0f,ART_HOKUSAI};

    segments[1] = (Segment){60.0f,  0.42f,0.82f,0.0f,ART_VANGOGH};

    segments[2] = (Segment){120.0f, 0.42f,0.82f,0.0f,ART_KANDINSKY};

    segments[3] = (Segment){180.0f, 0.42f,0.82f,0.0f,ART_MONDRIAN};

    segments[4] = (Segment){240.0f, 0.42f,0.82f,0.0f,ART_MONET};

    segments[5] = (Segment){300.0f, 0.42f,0.82f,0.0f,ART_KLIMT};
}

static float wheelRotation = 0.0f; /* grados, avanza en updateSegments() */
static const float ARTWORK_MARGIN = 0.85f; /* margen de seguridad del lienzo local */
static const float HIGHLIGHT_PULSE_SPEED = 0.025f; /* ciclo completo ~4s */

/* obra destacada segun la hora real */
static int activeArtworkIndex = 0;
static float highlightPulsePhase = 0.0f;

static void drawArtwork(ArtworkType artwork);
static void drawArtworkBackground(const Segment *segment);

/* Hora (24h) -> indice de obra activa. Reloj de 12h, la secuencia de
   6 obras se repite dos veces al dia (12=Hokusai(0) ... 5=Klimt(5),
   6=Hokusai(0) otra vez). El indice coincide con el orden de
   ArtworkType y con el de initSegments(). */
static int computeActiveArtworkIndex(ClockTime t)
{
    int hour12 = t.hour % 12;

    return hour12 % 6;
}

void drawSegment(const Segment *segment)
{
/* CUESTIONABLE: funciona por aritmetica de punteros; solo es valido
   si "segment" apunta dentro del array global segments[]. */
int segmentIndex = (int)(segment - segments);
int isActive = (segmentIndex == activeArtworkIndex);

glPushMatrix();

glRotatef(segment->angle + wheelRotation, 0.0f, 0.0f, 1.0f);

drawArtworkBackground(segment);

/* borde dorado y mas grueso solo si es el segmento activo */
if(isActive)
{
    glColor3f(0.82f,0.68f,0.28f);
    glLineWidth(2.0f);
}
else
{
    glColor3f(1.0f,1.0f,1.0f);
    glLineWidth(1.4f);
}

drawArc(
    segment->outerRadius,
    -25.0f,
     25.0f
);

drawArc(
    segment->innerRadius,
    -25.0f,
     25.0f
);

float x1 = cos(degreesToRadians(25))*segment->innerRadius;
float y1 = sin(degreesToRadians(25))*segment->innerRadius;

float x2 = cos(degreesToRadians(25))*segment->outerRadius;
float y2 = sin(degreesToRadians(25))*segment->outerRadius;

drawLine(x1,y1,x2,y2);

x1 = cos(degreesToRadians(-25))*segment->innerRadius;
y1 = sin(degreesToRadians(-25))*segment->innerRadius;

x2 = cos(degreesToRadians(-25))*segment->outerRadius;
y2 = sin(degreesToRadians(-25))*segment->outerRadius;

drawLine(x1,y1,x2,y2);

{
    /* lienzo local de la obra: centrado en el sector y escalado para
       que quepa completo (radial o angular, el que sea mas chico). */
    float midRadius = (segment->innerRadius + segment->outerRadius) * 0.5f;

    float radialHalfWidth = (segment->outerRadius - segment->innerRadius) * 0.5f;

    float angularHalfWidth = midRadius * tanf(degreesToRadians(25.0f));

    float canvasScale = (radialHalfWidth < angularHalfWidth ? radialHalfWidth : angularHalfWidth) * ARTWORK_MARGIN;

    glPushMatrix();

        glTranslatef(midRadius, 0.0f, 0.0f);

        glScalef(canvasScale, canvasScale, 1.0f);

        /* pulso: solo escala el contenido de drawArtwork() del activo,
           no el fondo ni el borde (esos ya se dibujaron arriba). */
        if(isActive)
        {
            float highlightScale = 1.04f + 0.04f*sinf(highlightPulsePhase);

            glPushMatrix();

                glScalef(highlightScale, highlightScale, 1.0f);

                drawArtwork(segment->artwork);

            glPopMatrix();
        }
        else
        {
            drawArtwork(segment->artwork);
        }

    glPopMatrix();
}

/* overlay calido y muy transparente sobre el activo, solo iluminacion */
if(isActive)
{
    /* se activa el blend aqui mismo por si acaso: si no estuviera
       encendido, este color se veria opaco y taparia la obra entera
       (paso una vez con Monet). No se desactiva porque clock.c lo
       necesita encendido siempre para el antialiasing de sus lineas. */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f,0.92f,0.75f,0.05f);

    drawFilledArc(segment->innerRadius, segment->outerRadius, -25.0f, 25.0f);
}

glPopMatrix();
}

/* nombre de cada obra, para la etiqueta del centro */
static const char* getArtworkName(ArtworkType artwork)
{
    switch(artwork)
    {
        case ART_HOKUSAI:   return "Hokusai";
        case ART_VANGOGH:   return "Van Gogh";
        case ART_KANDINSKY: return "Kandinsky";
        case ART_MONDRIAN:  return "Mondrian";
        case ART_MONET:     return "Monet";
        case ART_KLIMT:     return "Klimt";
    }

    return "";
}

/* pixeles (los que reporta GLUT) -> unidades del mundo, usando el mismo
   calculo de aspect ratio que reshape() en main.c. Sirve para centrar
   texto sin importar el tamano de la ventana. */
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

/* dibuja "text" centrado en centerX, linea base en y */
static void drawCenteredBitmapText(float centerX, float y, void *font, const char *text)
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

/* hora + nombre de la obra activa, dentro de la carátula, debajo del
   pivote para no chocar con las manecillas ni las marcas de hora */
static void drawActiveArtworkLabel(void)
{
    ClockTime t = getCurrentTime();
    int hour12 = t.hour % 12;
    int displayHour = (hour12 == 0) ? 12 : hour12;
    char timeText[16]; /* "12:15" son 6 bytes; sobra espacio a proposito */
    const char *artworkName;

    sprintf(timeText, "%02d:%02d", displayHour, t.minute);

    artworkName = getArtworkName(segments[activeArtworkIndex].artwork);

    glColor3f(0.75f,0.75f,0.75f);

    drawCenteredBitmapText(0.0f, -0.09f, GLUT_BITMAP_HELVETICA_10, timeText);

    drawCenteredBitmapText(0.0f, -0.15f, GLUT_BITMAP_HELVETICA_10, artworkName);
}

void drawSegments(void)
{
    int i;

    for(i = 0; i < 6; i++)
    {
        drawSegment(&segments[i]);
    }

    drawActiveArtworkLabel();
}
void updateSegments(void)
{
    wheelRotation += 0.01f;

    if(wheelRotation >= 360.0f)
        wheelRotation = 0.0f;

    activeArtworkIndex = computeActiveArtworkIndex(getCurrentTime());

    highlightPulsePhase += HIGHLIGHT_PULSE_SPEED;

    if(highlightPulsePhase >= 2.0f*3.1415926535f)
        highlightPulsePhase -= 2.0f*3.1415926535f;

    glutPostRedisplay();
}

/* dispatcher: obra en el lienzo local, segun ArtworkType */
static void drawArtwork(ArtworkType artwork)
{
    switch(artwork)
    {
        case ART_HOKUSAI:
            drawHokusai();
            break;

        case ART_VANGOGH:
            drawVanGogh();
            break;

        case ART_KANDINSKY:
            drawKandinsky();
            break;

        case ART_MONDRIAN:
            drawMondrian();
            break;

        case ART_MONET:
            drawMonet();
            break;

        case ART_KLIMT:
            drawKlimt();
            break;
    }
}

/* dispatcher: fondo real del sector, segun ArtworkType. El default ya
   es inalcanzable (las 6 obras tienen su propio case), se deja como
   respaldo. */
static void drawArtworkBackground(const Segment *segment)
{
    switch(segment->artwork)
    {
        case ART_MONET:
            drawMonetBackground(segment->innerRadius, segment->outerRadius);
            break;

        case ART_MONDRIAN:
            drawMondrianBackground(segment->innerRadius, segment->outerRadius);
            break;

        case ART_KANDINSKY:
            drawKandinskyBackground(segment->innerRadius, segment->outerRadius);
            break;

        case ART_VANGOGH:
            drawVanGoghBackground(segment->innerRadius, segment->outerRadius);
            break;

        case ART_KLIMT:
            drawKlimtBackground(segment->innerRadius, segment->outerRadius);
            break;

        case ART_HOKUSAI:
            drawHokusaiBackground(segment->innerRadius, segment->outerRadius);
            break;

        default:
            glColor3f(0.30f,0.30f,0.80f);
            drawFilledArc(segment->innerRadius, segment->outerRadius, -25.0f, 25.0f);
            break;
    }
}
