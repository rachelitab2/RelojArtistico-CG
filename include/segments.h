#ifndef SEGMENTS_H
#define SEGMENTS_H

/* -----------------------------------------------------------------------
 * ArtworkType: identificador unico de cada obra del catalogo.
 *
 * Las 18 entradas se agrupan logicamente por sala; el orden dentro del
 * enum no implica pertenencia a una sala especifica: eso lo definen los
 * arrays ROOM_* en segments.c.
 * ----------------------------------------------------------------------- */
typedef enum
{
    /* --- sala 15 min: Color y geometria --- */
    ART_MALEVICH,
    ART_DELAUNAY,
    ART_DEMUTH,
    ART_KANDINSKY,
    ART_MONDRIAN,
    ART_RILEY,

    /* --- sala 30 min: Luz, ritmo y color --- */
    ART_MONET,
    ART_VANGOGH,
    ART_HOKUSAI,
    ART_SEURAT,
    ART_ALMA_THOMAS,
    ART_PAUL_KLEE,

    /* --- sala 60 min: Abstraccion, simbolo y atmosfera --- */
    ART_KLIMT,
    ART_HILMA,
    ART_JOAN_MIRO,
    ART_ROTHKO,
    ART_KUPKA,
    ART_TAEUBER_ARP

} ArtworkType;

/* un sector del reloj */
typedef struct
{
    float angle;       /* posicion fija alrededor del reloj (grados) */

    float innerRadius;

    float outerRadius;

    float animation;   /* CUESTIONABLE: declarado, nunca se lee en el codigo */

    ArtworkType artwork;

} Segment;

/* -----------------------------------------------------------------------
 * Salas de tiempo.
 *
 * Cada array contiene los 6 ArtworkType que componen esa sala.
 * El orden dentro del array coincide con el indice de segmento (0-5).
 * ----------------------------------------------------------------------- */
#define ROOM_SIZE 6

extern const ArtworkType ROOM_15_MIN[ROOM_SIZE];
extern const ArtworkType ROOM_30_MIN[ROOM_SIZE];
extern const ArtworkType ROOM_60_MIN[ROOM_SIZE];

/* Cambia la sala activa (1, 2 o 3) y recarga segments[].
   Llama a esta funcion desde el manejador de teclado en main.c. */
void setActiveRoom(int roomKey);

/* Devuelve la sala activa (1, 2 o 3). */
int getActiveRoom(void);

void initSegments(void);

void drawSegment(const Segment *segment);

void drawSegments(void);

void updateSegments(void);

ArtworkType getActiveArtworkType(void);

int getActiveArtworkIndex(void);

#endif
