#ifndef UTILS_H
#define UTILS_H

/* primitivas reutilizables, sin estado, usadas por clock.c y las 6 obras */

float degreesToRadians(float degrees);

void drawCircle(float x,float y,float radius); /* solo contorno */

void drawFilledCircle(float x,float y,float radius);
void drawArc(float radius,          /* arco (linea), paso fijo de 2 grados */
             float startAngle,
             float endAngle);

void drawLine(float x1,float y1,float x2,float y2);
void drawFilledArc(                 /* anillo relleno entre dos radios */
    float innerRadius,
    float outerRadius,
    float startAngle,
    float endAngle
);

#endif
