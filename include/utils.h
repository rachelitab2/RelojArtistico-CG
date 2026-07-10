#ifndef UTILS_H
#define UTILS_H

float degreesToRadians(float degrees);

void drawCircle(float x,float y,float radius);

void drawFilledCircle(float x,float y,float radius);
void drawArc(float radius,
             float startAngle,
             float endAngle);

void drawLine(float x1,float y1,float x2,float y2);
void drawFilledArc(
    float innerRadius,
    float outerRadius,
    float startAngle,
    float endAngle
);

#endif