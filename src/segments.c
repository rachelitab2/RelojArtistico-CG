#include <math.h>
#include <GL/freeglut.h>

#include "segments.h"
#include "utils.h"
#include "clock.h"
#include "segment_monet.h"
#include "segment_mondrian.h"
#include "segment_kandinsky.h"
#include "segment_vangogh.h"
#include "segment_klimt.h"
#include "segment_demuth.h"
#include "segment_malevich.h"
#include "segment_delaunay.h"
#include "segment_riley.h"
#include "segment_rothko.h"
#include "segment_taeuber_arp.h"
#include "segment_alma_thomas.h"
#include "segment_seurat.h"
#include "segment_escher.h"
#include "segment_paul_klee.h"
#include "segment_kupka.h"
#include "segment_hilma.h"
#include "segment_joan_miro.h"
#include "segment_malevich_suprematist.h"
#include "artwork_catalog.h"
#include "app_config.h"

/* -----------------------------------------------------------------------
 * Arrays de sala.
 *
 * Cada array define las 6 obras (por ArtworkType) que componen esa sala.
 * El orden es el orden de aparicion en el reloj (segmento 0 a 5).
 * ----------------------------------------------------------------------- */
const ArtworkType ROOM_15_MIN[ROOM_SIZE] =
{
    ART_MALEVICH,
    ART_KANDINSKY,
    ART_DELAUNAY,
    ART_DEMUTH,
    ART_MONDRIAN,
    ART_RILEY
};

const ArtworkType ROOM_30_MIN[ROOM_SIZE] =
{
    ART_MONET,
    ART_VANGOGH,
    ART_ESCHER,
    ART_SEURAT,
    ART_ALMA_THOMAS,
    ART_PAUL_KLEE
};

const ArtworkType ROOM_60_MIN[ROOM_SIZE] =
{
    ART_KLIMT,
    ART_HILMA,
    ART_JOAN_MIRO,
    ART_ROTHKO,
    ART_KUPKA,
    ART_MALEVICH_SUPREMATIST
};

Segment segments[6]; /* los 6 sectores del reloj, en orden fijo */

static int activeRoom = 1; /* sala activa: 1, 2 o 3 */

/* -----------------------------------------------------------------------
 * Variables de estado del layout — declaradas aqui para que todas las
 * funciones estaticas del archivo puedan acceder a ellas sin reordenar.
 * ----------------------------------------------------------------------- */
static const float ARTWORK_MARGIN        = 0.85f;
static const float HIGHLIGHT_PULSE_SPEED = 0.025f;
static const float TARGET_ANGLE          = 90.0f;
static const float BASE_HALF_ANGLE       = 26.0f;
static const float ACTIVE_HALF_ANGLE     = 50.0f;
static const float LAYOUT_SMOOTHING      = 0.12f;
static const float SECTOR_GAP_DEG        = 1.0f;

static int   activeArtworkIndex      = 0;
static float highlightPulsePhase     = 0.0f;

/* Navegacion manual (Anterior/Siguiente): activeArtworkIndex vive
   normalmente enganchado a la hora real (computeActiveArtworkIndex).
   lastNaturalArtworkIndex guarda el ultimo valor "natural" visto; si
   goToNextArtwork/Previous lo corren manualmente, se desincroniza de
   proposito -- en cuanto la hora real avanza a un slot distinto del
   ultimo natural registrado, updateSegments() lo vuelve a enganchar
   (el automatico "pisa" la eleccion manual, nunca al reves). */
static int lastNaturalArtworkIndex = -1;

/* Flash breve al cambiar de sala: las 6 obras cambian de golpe (son
   texturas/geometria distintas, no hay forma de interpolarlas), asi
   que en vez de eso se atenua la rueda un instante y se recupera sola
   -- disimula el corte sin necesitar un crossfade real. */
static float roomTransitionAlpha = 0.0f;

static float sectorWidthFactor[6];
static float sectorAlpha[6];
static float sectorCenterAngle[6];
static float sectorHalfAngle[6];
static int   sectorLayoutInitialized = 0;

static void updateSectorLayout(void);
static void loadRoomIntoSegments(int roomKey);
static int  computeActiveArtworkIndex(ClockTime t);

/* -----------------------------------------------------------------------
 * setActiveRoom / getActiveRoom
 *
 * Cambia la sala activa y recarga segments[] con las 6 obras de esa sala.
 * Resetea el layout animado para que la transicion arranque limpia.
 * ----------------------------------------------------------------------- */
void setActiveRoom(int roomKey)
{
    if(roomKey < 1 || roomKey > 3)
        return;

    activeRoom = roomKey;
    loadRoomIntoSegments(roomKey);

    switch(roomKey)
    {
        case 1: setChangeInterval(CHANGE_INTERVAL_15_MINUTES); break;
        case 2: setChangeInterval(CHANGE_INTERVAL_30_MINUTES); break;
        case 3: setChangeInterval(CHANGE_INTERVAL_60_MINUTES); break;
    }

    /* NO se resetea el layout: dejar que sectorWidthFactor/sectorAlpha
       animen desde donde estaban (via LAYOUT_SMOOTHING) hace la
       transicion menos brusca que el snap instantaneo de antes. El
       contenido de las obras SI cambia de golpe (son texturas/geometria
       distintas), por eso se suma un flash breve (ver
       roomTransitionAlpha) que disimula ese corte. */
    roomTransitionAlpha = 0.55f;

    /* la navegacion manual (goToNextArtwork/Previous) no tiene sentido
       "heredada" de la sala anterior -- se vuelve a sincronizar con la
       obra que le toca a la sala nueva segun la hora real */
    lastNaturalArtworkIndex = -1;
}

int getActiveRoom(void)
{
    return activeRoom;
}

/* Carga los 6 ArtworkType de la sala indicada en segments[]. */
static void loadRoomIntoSegments(int roomKey)
{
    const ArtworkType *room;
    int i;

    switch(roomKey)
    {
        case 1:  room = ROOM_15_MIN; break;
        case 3:  room = ROOM_60_MIN; break;
        case 2:
        default: room = ROOM_30_MIN; break;
    }

    for(i = 0; i < 6; i++)
    {
        segments[i].angle       = (float)(i * 60);
        /* 15% mas chico que el original (0.42/0.82) para dejar aire
           arriba y abajo del anillo: sin eso no entraban ni el panel
           de descripcion (arriba) ni los controles Anterior/Siguiente
           (abajo) sin recortarse contra el borde de la ventana. Como
           todas las obras dibujan en su lienzo local -1..1 y
           canvasScale se recalcula cada frame a partir de estos
           radios (ver drawSegment), este cambio no afecta las
           proporciones internas de ninguna obra, solo el tamano total
           de la rueda. */
        segments[i].innerRadius = 0.36f;
        segments[i].outerRadius = 0.70f;
        segments[i].animation   = 0.0f;
        segments[i].artwork     = room[i];
    }
}

static void drawArtwork(ArtworkType artwork);
static void drawArtworkBackground(const Segment *segment, float halfAngle);
static void drawPlaceholderArtwork(ArtworkType type);
static void drawPlaceholderBackground(const Segment *segment, float halfAngle);

/* Minutos por slot de la sala activa: 15/30/60. Compartido por
   computeActiveArtworkIndex() y getSecondsUntilNextArtworkChange(),
   para que ambos coincidan siempre en que cuenta como "un bloque". */
static int getActiveSlotMinutes(void)
{
    switch(activeRoom)
    {
        case 1:  return 15;
        case 3:  return 60;
        case 2:
        default: return 30;
    }
}

/* -----------------------------------------------------------------------
 * computeActiveArtworkIndex
 *
 * El slotMinutes se deriva directamente de la sala activa.
 * Sala 1 → 15 min, sala 2 → 30 min, sala 3 → 60 min.
 * ----------------------------------------------------------------------- */
static int computeActiveArtworkIndex(ClockTime t)
{
    int minutesIntoHalfDay = (t.hour % 12) * 60 + t.minute;
    int slotMinutes = getActiveSlotMinutes();

    return (minutesIntoHalfDay / slotMinutes) % 6;
}

/* Segundos que faltan para que la hora real cruce al proximo bloque
   de la sala activa (para el contador "proxima obra en" de la UI). No
   depende de la navegacion manual: cuenta siempre en base al reloj
   real, tal como se decidio para que Anterior/Siguiente no lo altere. */
int getSecondsUntilNextArtworkChange(void)
{
    ClockTime t = getCurrentTime();
    int slotMinutes = getActiveSlotMinutes();
    int minutesIntoHalfDay = (t.hour % 12) * 60 + t.minute;
    int minutesIntoSlot = minutesIntoHalfDay % slotMinutes;
    int secondsIntoSlot = minutesIntoSlot * 60 + t.second;
    int secondsPerSlot = slotMinutes * 60;

    return secondsPerSlot - secondsIntoSlot;
}

/* Navegacion manual: corre activeArtworkIndex un paso, sin tocar
   lastNaturalArtworkIndex -- asi el proximo cambio de hora real (que
   si actualiza lastNaturalArtworkIndex) vuelve a tomar el control (ver
   nota junto a la declaracion de lastNaturalArtworkIndex). */
void goToNextArtwork(void)
{
    activeArtworkIndex = (activeArtworkIndex + 1) % 6;
}

void goToPreviousArtwork(void)
{
    activeArtworkIndex = (activeArtworkIndex + 5) % 6;
}

/* -----------------------------------------------------------------------
 * initSegments
 * ----------------------------------------------------------------------- */
void initSegments(void)
{
    /* arranca en sala 1 (15 min) por defecto. Usa setActiveRoom() en
       vez de duplicar su logica (loadRoomIntoSegments + activeRoom)
       para que el intervalo de cambio (ver app_config) quede
       sincronizado desde el arranque -- antes se pisaba el valor por
       defecto de app_config.c (30 min) sin avisarle, y el selector de
       intervalo de la UI mostraba "30 min" resaltado mientras la sala
       realmente activa era la de 15 min. */
    setActiveRoom(1);

    /* evita un primer frame degenerado */
    updateSectorLayout();
}

/* -----------------------------------------------------------------------
 * updateSectorLayout
 *
 * Anima y empaqueta los 6 sectores cada frame: el activo crece en ancho
 * angular y los demas se comprimen, atenuados segun su distancia circular
 * al activo.
 * ----------------------------------------------------------------------- */
static void updateSectorLayout(void)
{
    static const float DISTANCE_ALPHA[4] = {1.0f, 0.68f, 0.46f, 0.32f};
    float cursor = 0.0f;
    float shift;
    int i;

    if(!sectorLayoutInitialized)
    {
        for(i = 0; i < 6; i++)
        {
            sectorWidthFactor[i] = (i == activeArtworkIndex) ? 1.0f : 0.0f;
            sectorAlpha[i]       = (i == activeArtworkIndex) ? 1.0f : DISTANCE_ALPHA[1];
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
        sectorAlpha[i]       += (alphaTarget - sectorAlpha[i]) * LAYOUT_SMOOTHING;

        sectorHalfAngle[i]   = BASE_HALF_ANGLE + (ACTIVE_HALF_ANGLE - BASE_HALF_ANGLE) * sectorWidthFactor[i];
        sectorCenterAngle[i] = cursor + sectorHalfAngle[i];
        cursor += 2.0f * sectorHalfAngle[i];
    }

    shift = TARGET_ANGLE - sectorCenterAngle[activeArtworkIndex];

    for(i = 0; i < 6; i++)
        sectorCenterAngle[i] += shift;
}

/* -----------------------------------------------------------------------
 * drawSegment
 * ----------------------------------------------------------------------- */
void drawSegment(const Segment *segment)
{
/* CUESTIONABLE: funciona por aritmetica de punteros; solo es valido
   si "segment" apunta dentro del array global segments[]. */
int segmentIndex = (int)(segment - segments);
int isActive = (segmentIndex == activeArtworkIndex);
Segment renderSegment = *segment;
float halfAngle = sectorHalfAngle[segmentIndex];
float alpha = sectorAlpha[segmentIndex];

float drawHalfAngle = halfAngle - SECTOR_GAP_DEG;
if(drawHalfAngle < 1.0f) drawHalfAngle = 1.0f;

if(isActive)
{
    renderSegment.outerRadius += 0.08f;
}

glPushMatrix();

glRotatef(sectorCenterAngle[segmentIndex], 0.0f, 0.0f, 1.0f);

drawArtworkBackground(&renderSegment, drawHalfAngle);

/* El sector activo toma el acento cromatico de su obra para que el
   destaque pertenezca al mismo sistema visual del panel informativo. */
if(isActive)
{
    ArtworkColor accent = getArtworkInfo(renderSegment.artwork)->accentColor;

    glColor4f(accent.red, accent.green, accent.blue, 0.85f);
    glLineWidth(2.0f);
}
else
{
glColor4f(1.0f,1.0f,1.0f,0.30f);
    glLineWidth(1.0f);
}

drawArc(
    renderSegment.outerRadius,
    -drawHalfAngle,
     drawHalfAngle
);

drawArc(
    renderSegment.innerRadius,
    -drawHalfAngle,
     drawHalfAngle
);

float x1 = cos(degreesToRadians(drawHalfAngle))*renderSegment.innerRadius;
float y1 = sin(degreesToRadians(drawHalfAngle))*renderSegment.innerRadius;

float x2 = cos(degreesToRadians(drawHalfAngle))*renderSegment.outerRadius;
float y2 = sin(degreesToRadians(drawHalfAngle))*renderSegment.outerRadius;

drawLine(x1,y1,x2,y2);

x1 = cos(degreesToRadians(-drawHalfAngle))*renderSegment.innerRadius;
y1 = sin(degreesToRadians(-drawHalfAngle))*renderSegment.innerRadius;

x2 = cos(degreesToRadians(-drawHalfAngle))*renderSegment.outerRadius;
y2 = sin(degreesToRadians(-drawHalfAngle))*renderSegment.outerRadius;

drawLine(x1,y1,x2,y2);

/* halo externo sutil */
if(isActive)
{
    ArtworkColor haloColor = getArtworkInfo(renderSegment.artwork)->accentColor;

    glColor4f(haloColor.red, haloColor.green, haloColor.blue, 0.20f);
    glLineWidth(5.0f);

    drawArc(renderSegment.outerRadius + 0.015f, -drawHalfAngle, drawHalfAngle);
}

{
    float midRadius = (renderSegment.innerRadius + renderSegment.outerRadius) * 0.5f;

    float radialHalfWidth = (renderSegment.outerRadius - renderSegment.innerRadius) * 0.5f;

    float angularHalfWidth = midRadius * tanf(degreesToRadians(halfAngle));

    float canvasScale = (radialHalfWidth < angularHalfWidth ? radialHalfWidth : angularHalfWidth) * ARTWORK_MARGIN;

    glPushMatrix();

        glTranslatef(midRadius, 0.0f, 0.0f);

        glScalef(canvasScale, canvasScale, 1.0f);

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

/* overlay calido y muy transparente sobre el activo */
if(isActive)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ArtworkColor accent = getArtworkInfo(renderSegment.artwork)->accentColor;

    glColor4f(accent.red, accent.green, accent.blue, 0.08f);

    drawFilledArc(renderSegment.innerRadius, renderSegment.outerRadius, -drawHalfAngle, drawHalfAngle);

    glColor4f(accent.red, accent.green, accent.blue, 0.16f);

    drawFilledArc(renderSegment.outerRadius - 0.05f, renderSegment.outerRadius, -drawHalfAngle, drawHalfAngle);
}
else
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, (1.0f - alpha) * 0.75f);

    drawFilledArc(renderSegment.innerRadius, renderSegment.outerRadius, -drawHalfAngle, drawHalfAngle);
}

glPopMatrix();
}

void drawSegments(void)
{
    int i;

    for(i = 0; i < 6; i++)
    {
        drawSegment(&segments[i]);
    }

    /* flash de cambio de sala (ver roomTransitionAlpha): un circulo
       negro translucido que cubre la rueda entera y se desvanece solo
       en updateSegments(). Va DESPUES de los 6 segmentos para tapar
       tambien sus bordes/halo. */
    if(roomTransitionAlpha > 0.0f)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0f, 0.0f, 0.0f, roomTransitionAlpha);

        drawFilledCircle(0.0f, 0.0f, 1.0f);
    }
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
    int naturalIndex = computeActiveArtworkIndex(getCurrentTime());

    /* el automatico solo pisa la navegacion manual cuando la hora real
       realmente cruzo a un slot distinto -- no en cada frame, o
       goToNextArtwork/Previous nunca tendrian efecto visible */
    if(naturalIndex != lastNaturalArtworkIndex)
    {
        activeArtworkIndex = naturalIndex;
        lastNaturalArtworkIndex = naturalIndex;
    }

    updateSectorLayout();

    if(roomTransitionAlpha > 0.0f)
    {
        roomTransitionAlpha -= 0.06f;

        if(roomTransitionAlpha < 0.0f)
            roomTransitionAlpha = 0.0f;
    }

    highlightPulsePhase += HIGHLIGHT_PULSE_SPEED;

    if(highlightPulsePhase >= 2.0f*3.1415926535f)
        highlightPulsePhase -= 2.0f*3.1415926535f;
}

/* -----------------------------------------------------------------------
 * drawPlaceholderArtwork
 *
 * Relleno visual generico para obras sin segment_*.c todavia.
 * Usa el backgroundColor del catalogo como tono base y el accentColor
 * para un circulo central de referencia.
 * ----------------------------------------------------------------------- */
static void drawPlaceholderArtwork(ArtworkType type)
{
    const ArtworkInfo *info = getArtworkInfo(type);
    ArtworkColor bg  = info->backgroundColor;
    ArtworkColor acc = info->accentColor;

    /* fondo solido con el color base de la obra */
    glColor3f(bg.red, bg.green, bg.blue);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    /* circulo de acento: senala que la obra esta pendiente */
    glColor4f(acc.red, acc.green, acc.blue, 0.55f);
    glLineWidth(1.5f);
    drawArc(0.55f, 0.0f, 360.0f);

    /* cruz minima para distinguirlo de un fondo real */
    glColor4f(acc.red, acc.green, acc.blue, 0.35f);
    glLineWidth(1.0f);
    drawLine(-0.55f, 0.0f, 0.55f, 0.0f);
    drawLine(0.0f, -0.55f, 0.0f, 0.55f);
}

/* Fondo de sector para obras sin implementar: color base del catalogo. */
static void drawPlaceholderBackground(const Segment *segment, float halfAngle)
{
    const ArtworkInfo *info = getArtworkInfo(segment->artwork);
    ArtworkColor bg = info->backgroundColor;

    glColor3f(bg.red * 0.6f, bg.green * 0.6f, bg.blue * 0.6f);
    drawFilledArc(segment->innerRadius, segment->outerRadius, -halfAngle, halfAngle);
}

/* -----------------------------------------------------------------------
 * drawArtwork: dispatcher principal
 *
 * Las 6 obras implementadas llaman a su modulo propio.
 * Las 12 obras pendientes usan drawPlaceholderArtwork().
 * ----------------------------------------------------------------------- */
static void drawArtwork(ArtworkType artwork)
{
    switch(artwork)
    {
        /* --- implementadas --- */
        case ART_DEMUTH:    drawDemuth();    break;
        case ART_VANGOGH:   drawVanGogh();   break;
        case ART_KANDINSKY: drawKandinsky(); break;
        case ART_MONDRIAN:  drawMondrian();  break;
        case ART_MONET:     drawMonet();     break;
        case ART_KLIMT:     drawKlimt();     break;
        case ART_MALEVICH:  drawMalevich();  break;
        case ART_DELAUNAY:  drawDelaunay();  break;
        case ART_RILEY:       drawRiley();       break;
        case ART_ROTHKO:      drawRothko();      break;
        case ART_TAEUBER_ARP: drawTaeuberArp();  break;
        case ART_ALMA_THOMAS: drawAlmaThomas();  break;
        case ART_SEURAT:      drawSeurat();      break;
        case ART_ESCHER:      drawEscher();      break;
        case ART_PAUL_KLEE:   drawPaulKlee();    break;
        case ART_KUPKA:       drawKupka();       break;
        case ART_HILMA:       drawHilma();       break;
        case ART_JOAN_MIRO:   drawJoanMiro();    break;
        case ART_MALEVICH_SUPREMATIST: drawMalevichSuprematist(); break;

        /* --- placeholders --- */
        default:
            drawPlaceholderArtwork(artwork);
            break;
    }
}

/* -----------------------------------------------------------------------
 * drawArtworkBackground: dispatcher de fondos de sector
 * ----------------------------------------------------------------------- */
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

        case ART_DEMUTH:
            drawDemuthBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_MALEVICH:
            drawMalevichBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_DELAUNAY:
            drawDelaunayBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_RILEY:
            drawRileyBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_ROTHKO:
            drawRothkoBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_TAEUBER_ARP:
            drawTaeuberArpBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_ALMA_THOMAS:
            drawAlmaThomasBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_SEURAT:
            drawSeuratBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_ESCHER:
            drawEscherBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_PAUL_KLEE:
            drawPaulKleeBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_MALEVICH_SUPREMATIST:
            drawMalevichSuprematistBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_KUPKA:
            drawKupkaBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_HILMA:
            drawHilmaBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        case ART_JOAN_MIRO:
            drawJoanMiroBackground(segment->innerRadius, segment->outerRadius, halfAngle);
            break;

        /* todas las obras sin implementar usan el fondo placeholder */
        default:
            drawPlaceholderBackground(segment, halfAngle);
            break;
    }
}
