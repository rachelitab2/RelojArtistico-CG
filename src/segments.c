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

Segment segments[6];

void initSegments(void)
{
    segments[0] = (Segment){0.0f,   0.42f,0.82f,0.0f,ART_HOKUSAI};

    segments[1] = (Segment){60.0f,  0.42f,0.82f,0.0f,ART_VANGOGH};

    segments[2] = (Segment){120.0f, 0.42f,0.82f,0.0f,ART_KANDINSKY};

    segments[3] = (Segment){180.0f, 0.42f,0.82f,0.0f,ART_MONDRIAN};

    segments[4] = (Segment){240.0f, 0.42f,0.82f,0.0f,ART_MONET};

    segments[5] = (Segment){300.0f, 0.42f,0.82f,0.0f,ART_KLIMT};
}

static float wheelRotation = 0.0f;
static const float ARTWORK_MARGIN = 0.85f;
static const float HIGHLIGHT_PULSE_SPEED = 0.025f; /* ciclo completo ~4s a 16ms/tick */

/* Fase 2: obra destacada segun la hora real. */
static int activeArtworkIndex = 0;
static float highlightPulsePhase = 0.0f;

static void drawArtwork(ArtworkType artwork);
static void drawArtworkBackground(const Segment *segment);

/* Convierte la hora real (24h, vía getCurrentTime()) al indice de la
   obra que corresponde destacar, siguiendo un reloj de 12 horas donde
   la secuencia de las 6 obras se repite dos veces por dia:
       12 -> Hokusai(0)   1 -> VanGogh(1)   2 -> Kandinsky(2)
        3 -> Mondrian(3)  4 -> Monet(4)     5 -> Klimt(5)
        6 -> Hokusai(0)   7 -> VanGogh(1)   8 -> Kandinsky(2)
        9 -> Mondrian(3) 10 -> Monet(4)    11 -> Klimt(5)
   El indice resultante coincide con el orden de ArtworkType y con el
   orden en que initSegments() asigna las obras a los 6 segmentos. */
static int computeActiveArtworkIndex(ClockTime t)
{
    int hour12 = t.hour % 12; /* reloj de 12 horas: 0 representa las 12 */

    return hour12 % 6; /* la secuencia de 6 obras se repite cada 6 horas */
}

void drawSegment(const Segment *segment)
{
int segmentIndex = (int)(segment - segments);
int isActive = (segmentIndex == activeArtworkIndex);

glPushMatrix();

glRotatef(segment->angle + wheelRotation, 0.0f, 0.0f, 1.0f);

drawArtworkBackground(segment);

/* Borde dorado, un poco mas grueso, solo para el segmento activo;
   el resto conserva el borde blanco de siempre. */
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
    float midRadius = (segment->innerRadius + segment->outerRadius) * 0.5f;

    float radialHalfWidth = (segment->outerRadius - segment->innerRadius) * 0.5f;

    float angularHalfWidth = midRadius * tanf(degreesToRadians(25.0f));

    float canvasScale = (radialHalfWidth < angularHalfWidth ? radialHalfWidth : angularHalfWidth) * ARTWORK_MARGIN;

    glPushMatrix();

        glTranslatef(midRadius, 0.0f, 0.0f);

        glScalef(canvasScale, canvasScale, 1.0f);

        /* Escala adicional muy sutil (hasta ~1.08x) con pulso lento,
           unicamente para el contenido de drawArtwork() del segmento
           activo. El fondo, los arcos y las lineas radiales, ya
           dibujados antes de este bloque, no se ven afectados. */
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

/* Overlay calido de muy baja opacidad sobre el segmento activo,
   para dar sensacion de iluminacion extra sin alterar sus colores.
   Se asegura el blending aqui mismo (glEnable es idempotente, no
   pasa nada si clock.c ya lo habia activado) en vez de asumir que
   sigue encendido: si por algun motivo no lo estaba, el alpha se
   ignoraba y este color quedaba opaco, tapando la obra entera (bug
   ya observado: cubria a Monet cuando era el destacado). No se
   desactiva despues porque clock.c lo deja encendido de forma
   permanente para el antialiasing de sus lineas. */
if(isActive)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f,0.92f,0.75f,0.05f);

    drawFilledArc(segment->innerRadius, segment->outerRadius, -25.0f, 25.0f);
}

glPopMatrix();
}

/* Nombre visible de cada obra, para la etiqueta del centro del reloj. */
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

/* Convierte un ancho en pixeles (tal como lo reporta GLUT) a unidades
   del mundo, usando la misma logica de aspect ratio que reshape() en
   main.c: si la ventana es mas ancha que alta el rango vertical del
   mundo es fijo (2.0), si es mas alta que ancha el que es fijo es el
   horizontal. Asi el texto se centra correctamente sin importar el
   tamano de la ventana. */
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

/* Dibuja "text" centrado horizontalmente en centerX, con la linea base
   en y. Usa glutBitmapWidth() para medir el texto antes de dibujarlo. */
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

/* Etiqueta discreta dentro del circulo central: hora actual (reloj de
   12 horas, reutilizando getCurrentTime()) y nombre de la obra
   destacada (segments[activeArtworkIndex]). Se dibuja desplazada
   hacia abajo del pivote para no competir con las manecillas ni las
   marcas de hora. */
static void drawActiveArtworkLabel(void)
{
    ClockTime t = getCurrentTime();
    int hour12 = t.hour % 12;
    int displayHour = (hour12 == 0) ? 12 : hour12;
    char timeText[16]; /* "12:15" ocupa 6 bytes; se deja margen para que gcc no advierta sobre el peor caso teorico de %02d */
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

/* Pinta el fondo respetando la forma real del sector (arco relleno
   entre innerRadius y outerRadius), evitando huecos entre el arte
   y el borde curvo del segmento. Cada obra puede pintar su propio
   fondo; las que aun no tienen contenido usan el azul de referencia. */
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
