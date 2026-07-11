#ifndef DISPLAY_H
#define DISPLAY_H

void initDisplay(void); /* color de fondo */

void display(void); /* dibuja un frame completo */

void timer(int value); /* motor de la animacion, cada 16ms */

void reshape(int width, int height); /* ajusta la proyeccion al tamano de ventana */

#endif
