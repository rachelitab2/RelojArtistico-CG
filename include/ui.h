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

#endif