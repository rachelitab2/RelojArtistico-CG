#ifndef SEGMENTS_H
#define SEGMENTS_H

typedef enum
{
    ART_HOKUSAI,
    ART_VANGOGH,
    ART_KANDINSKY,
    ART_MONDRIAN,
    ART_MONET,
    ART_KLIMT

} ArtworkType;

typedef struct
{
    float angle;

    float innerRadius;

    float outerRadius;

    float animation;

    ArtworkType artwork;

} Segment;

void initSegments(void);

void drawSegment(const Segment *segment);

void drawSegments(void);

void updateSegments(void);

#endif