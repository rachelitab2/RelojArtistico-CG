#ifndef UI_H
#define UI_H

/*
 * Dibuja los elementos informativos de la interfaz.
 *
 * Esta capa contiene informacion visual auxiliar que no pertenece al reloj ni
 * a las obras individuales: paneles, textos, indicadores y controles. Separar
 * la interfaz permite evolucionar la experiencia sin cargar display.c con
 * detalles de composicion.
 */
void drawUserInterface(void);

/* Convierte un click de mouse (coordenadas en pixeles, origen arriba a la
   izquierda, como las entrega GLUT) y, si cae sobre el boton de sonido,
   alterna el mute. Devuelve 1 si el click fue consumido por la UI, 0 si
   no (para que quien llama sepa si debe procesarlo de otra forma). */
int uiHandleClick(int pixelX, int pixelY);

#endif