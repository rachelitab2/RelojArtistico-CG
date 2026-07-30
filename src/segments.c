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
#include "artwork_catalog.h"
#include "app_config.h"

Segment segments[6]; /* los 6 sectores del reloj, en orden fijo */

static void updateSectorLayout(void);

void initSegments(void)
{
    /* angulo, radio interior, radio exterior, animation (sin uso), obra */
    segments[0] = (Segment){0.0f,   0.42f,0.82f,0.0f,ART_HOKUSAI};

    segments[1] = (Segment){60.0f,  0.42f,0.82f,0.0f,ART_VANGOGH};

    segments[2] = (Segment){120.0f, 0.42f,0.82f,0.0f,ART_KANDINSKY};

    segments[3] = (Segment){180.0f, 0.42f,0.82f,0.0f,ART_MONDRIAN};

    segments[4] = (Segment){240.0f, 0.42f,0.82f,0.0f,ART_MONET};

    segments[5] = (Segment){300.0f, 0.42f,0.82f,0.0f,ART_KLIMT};

    /* evita un primer frame degenerado: sin esto, sectorCenterAngle y
       sectorHalfAngle quedarian en 0 hasta el primer updateSegments(). */
    updateSectorLayout();
}

static const float ARTWORK_MARGIN = 0.85f; /* margen de seguridad del lienzo local */
static const float HIGHLIGHT_PULSE_SPEED = 0.025f; /* ciclo completo ~4s */
static const float TARGET_ANGLE = 90.0f; /* posicion protagonista: arriba al centro */
static const float BASE_HALF_ANGLE = 26.0f; /* semiancho de un sector normal (~52 grados) */
static const float ACTIVE_HALF_ANGLE = 50.0f; /* semiancho del sector activo (~100 grados) */
static const float LAYOUT_SMOOTHING = 0.12f; /* velocidad de convergencia por frame */

/* obra destacada segun la hora real */
static int activeArtworkIndex = 0;
static float highlightPulsePhase = 0.0f;

/* estado animado del layout: cuanto "crece" cada sector hacia el activo (0..1),
   su atenuacion visual (1=brillo normal, 0=oscurecido), y el angulo/semiancho
   ya empaquetado que usa drawSegment() para dibujar. */
static float sectorWidthFactor[6];
static float sectorAlpha[6];
static float sectorCenterAngle[6];
static float sectorHalfAngle[6];
static int sectorLayoutInitialized = 0;

static void drawArtwork(ArtworkType artwork);
static void drawArtworkBackground(const Segment *segment, float halfAngle);

/* Hora (24h) -> indice de obra activa, segun el intervalo elegido en
   app_config.c (15/30/60 min). Con 60 min equivale al calculo viejo
   (hour % 12 % 6); con intervalos mas cortos la obra activa cambia
   mas seguido dentro de las mismas 12 horas. */
static int computeActiveArtworkIndex(ClockTime t)
{
    int minutesIntoHalfDay = (t.hour % 12) * 60 + t.minute;
    int slotMinutes;

    switch(getChangeInterval())
    {
        case CHANGE_INTERVAL_15_MINUTES: slotMinutes = 15; break;
        case CHANGE_INTERVAL_60_MINUTES: slotMinutes = 60; break;
        case CHANGE_INTERVAL_30_MINUTES:
        default:                         slotMinutes = 30; break;
    }

    return (minutesIntoHalfDay / slotMinutes) % 6;
}

/* Anima y empaqueta los 6 sectores cada frame: el activo crece en ancho
   angular y los demas se comprimen, atenuados segun su distancia circular
   al activo (mismo criterio que el mockup de referencia). El resultado
   (sectorCenterAngle/sectorHalfAngle) es lo que usa drawSegment() para
   dibujar; ya no depende de segment->angle ni de un giro continuo. */
static void updateSectorLayout(void)
{
    static const float DISTANCE_ALPHA[4] = {1.0f, 0.55f, 0.32f, 0.22f};
    float cursor = 0.0f;
    float shift;
    int i;

    if(!sectorLayoutInitialized)
    {
        for(i = 0; i < 6; i++)
        {
            sectorWidthFactor[i] = (i == activeArtworkIndex) ? 1.0f : 0.0f;
            sectorAlpha[i] = (i == activeArtworkIndex) ? 1.0f : DISTANCE_ALPHA[1];
        }

        sectorLayoutInitialized = 1;
    }

    for(i = 0; i < 6; i++)
    {
        int distance = abs(i - activeArtworkIndex);
        float widthTarget;
        float alphaTarget;

        if(distance > 3) distance = 6 - distance;

        widthTarget = (i == activeArtworkIndex) ? 1.0f : 0.0f;
        alphaTarget = DISTANCE_ALPHA[distance];

        sectorWidthFactor[i] += (widthTarget - sectorWidthFactor[i]) * LAYOUT_SMOOTHING;
        sectorAlpha[i] += (alphaTarget - sectorAlpha[i]) * LAYOUT_SMOOTHING;

        sectorHalfAngle[i] = BASE_HALF_ANGLE + (ACTIVE_HALF_ANGLE - BASE_HALF_ANGLE) * sectorWidthFactor[i];
        sectorCenterAngle[i] = cursor + sectorHalfAngle[i];
        cursor += 2.0f * sectorHalfAngle[i];
    }

    shift = TARGET_ANGLE - sectorCenterAngle[activeArtworkIndex];

    for(i = 0; i < 6; i++)
        sectorCenterAngle[i] += shift;
}

void drawSegment(const Segment *segment)
{
/* CUESTIONABLE: funciona por aritmetica de punteros; solo es valido
   si "segment" apunta dentro del array global segments[]. */
int segmentIndex = (int)(segment - segments);
int isActive = (segmentIndex == activeArtworkIndex);
Segment renderSegment = *segment;
float halfAngle = sectorHalfAngle[segmentIndex];
float alpha = sectorAlpha[segmentIndex];

if(isActive)
{
    renderSegment.outerRadius += 0.08f;
}

glPushMatrix();

glRotatef(sectorCenterAngle[segmentIndex], 0.0f, 0.0f, 1.0f);

drawArtworkBackground(&renderSegment, halfAngle);

/* El sector activo toma el acento cromatico de su obra para que el
   destaque pertenezca al mismo sistema visual del panel informativo. */
if(isActive)
{
    ArtworkColor accent = getArtworkInfo(renderSegment.artwork)->accentColor;

    glColor3f(accent.red, accent.green, accent.blue);
    glLineWidth(2.4f);
}
else
{
glColor3f(1.0f,1.0f,1.0f);
    glLineWidth(1.4f);
}

drawArc(
    renderSegment.outerRadius,
    -halfAngle,
     halfAngle
);

drawArc(
    renderSegment.innerRadius,
    -halfAngle,
     halfAngle
);

float x1 = cos(degreesToRadians(halfAngle))*renderSegment.innerRadius;
float y1 = sin(degreesToRadians(halfAngle))*renderSegment.innerRadius;

float x2 = cos(degreesToRadians(halfAngle))*renderSegment.outerRadius;
float y2 = sin(degreesToRadians(halfAngle))*renderSegment.outerRadius;

drawLine(x1,y1,x2,y2);

x1 = cos(degreesToRadians(-halfAngle))*renderSegment.innerRadius;
y1 = sin(degreesToRadians(-halfAngle))*renderSegment.innerRadius;

x2 = cos(degreesToRadians(-halfAngle))*renderSegment.outerRadius;
y2 = sin(degreesToRadians(-halfAngle))*renderSegment.outerRadius;

drawLine(x1,y1,x2,y2);

{
    /* lienzo local de la obra: centrado en el sector y escalado para
       que quepa completo (radial o angular, el que sea mas chico). */
    float midRadius = (renderSegment.innerRadius + renderSegment.outerRadius) * 0.5f;

    float radialHalfWidth = (renderSegment.outerRadius - renderSegment.innerRadius) * 0.5f;

    float angularHalfWidth = midRadius * tanf(degreesToRadians(halfAngle));

    float canvasScale = (radialHalfWidth < angularHalfWidth ? radialHalfWidth : angularHalfWidth) * ARTWORK_MARGIN;

    glPushMatrix();

        glTranslatef(midRadius, 0.0f, 0.0f);

        glScalef(canvasScale, canvasScale, 1.0f);

        /* El pulso se mantiene sutil: la expansion principal del activo ya
           ocurre en el radio exterior del sector completo. */
        if(isActive)
        {
            float highlightScale = 1.03f + 0.025f*sinf(highlightPulsePhase);

            glPushMatrix();

                glScalef(highlightScale, highlightScale, 1.0f);

        drawArtwork(renderSegment.artwork);

    glPopMatrix();
}
        else
        {
            drawArtwork(renderSegment.artwork);
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

    ArtworkColor accent = getArtworkInfo(renderSegment.artwork)->accentColor;

    glColor4f(accent.red, accent.green, accent.blue, 0.08f);

    drawFilledArc(renderSegment.innerRadius, renderSegment.outerRadius, -halfAngle, halfAngle);
}
else
{
    /* atenuacion de los sectores no activos: mismo mecanismo de overlay,
       oscurece en vez de iluminar, y su fuerza depende de la distancia
       circular al sector activo (calculada en updateSectorLayout). */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 1.0f - alpha);

    drawFilledArc(renderSegment.innerRadius, renderSegment.outerRadius, -halfAngle, halfAngle);
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

ArtworkType getActiveArtworkType(void)
{
    return segments[activeArtworkIndex].artwork;
}

int getActiveArtworkIndex(void)
{
    return activeArtworkIndex;
}

void updateSegments(void)
{
    /*
     * La rueda ya no rota continuamente: funciona como una galeria.
     * activeArtworkIndex decide cual obra crece y se centra arriba;
     * updateSectorLayout() anima ese cambio (ancho y atenuacion) suavemente
     * cada frame, sin necesitar un estado de transicion explicito.
     */
    activeArtworkIndex = computeActiveArtworkIndex(getCurrentTime());

    updateSectorLayout();

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
static void drawArtworkBackground(const Segment *segment, float halfAngle)
{
    switch(segment->artwork)
    {
  case ART_MONET:
    drawMonetBackground(segment->innerRadius, segment->outerRadius, halfAngle);
    break;

case ART_MONDRIAN:
    drawMondrianBackground(segment->innerRadius, segment->outerRadius, halfAngle);
    break;

        case ART_KANDINSKY:
            drawKandinskyBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_VANGOGH:
            drawVanGoghBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_KLIMT:
            drawKlimtBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_HOKUSAI:
            drawHokusaiBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        default:
            glColor3f(0.30f,0.30f,0.80f);
            drawFilledArc(segment->innerRadius, segment->outerRadius, -25.0f, 25.0f);
            break;
    }
}
