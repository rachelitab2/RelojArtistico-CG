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

    float rotation;

    ArtworkType artwork;

} Segment;

void initSegments(void);

void updateSegments(void);

void drawSegments(void)
{
    int i;

    for(i = 0; i < 6; i++)
    {
        drawSegment(segments[i].angle);
    }
}

#endif