#ifndef SEGMENT_DEMUTH_H
#define SEGMENT_DEMUTH_H

/* mismo patron de interfaz que las otras 5 obras: Background pinta
   el fondo del sector, la otra dibuja el primer plano en el lienzo
   local */

void drawDemuth(void);

void drawDemuthBackground(float innerRadius, float outerRadius, float halfAngle);

#endif
