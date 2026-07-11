#ifndef SEGMENTS_H
#define SEGMENTS_H

/* una obra por sector, en el orden en que initSegments() las asigna */
typedef enum
{
    ART_HOKUSAI,
    ART_VANGOGH,
    ART_KANDINSKY,
    ART_MONDRIAN,
    ART_MONET,
    ART_KLIMT

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

void initSegments(void);

void drawSegment(const Segment *segment);

void drawSegments(void);

void updateSegments(void);

#endif
