#ifndef ARTWORK_CATALOG_H
#define ARTWORK_CATALOG_H

#include "segments.h"

/*
 * Representa un color RGB normalizado para OpenGL.
 *
 * Cada componente utiliza el rango 0.0f - 1.0f porque coincide con
 * el formato esperado por funciones como glColor3f(). Mantener el color
 * en una estructura propia evita pasar valores sueltos por todo el codigo.
 */
typedef struct
{
    float red;
    float green;
    float blue;

} ArtworkColor;

/*
 * Agrupa la informacion descriptiva y visual de una obra.
 *
 * El catalogo permite separar los metadatos artisticos de la logica de
 * renderizado. De esta forma, los modulos graficos pueden consultar nombre,
 * autor, descripcion o color base sin conocer detalles internos de cada obra.
 */
typedef struct
{
    ArtworkType type;

    const char *title;
    const char *author;
    const char *year;
    const char *movement;
    const char *description;

    ArtworkColor backgroundColor;
    ArtworkColor accentColor;
    ArtworkColor palette[3];

} ArtworkInfo;

/*
 * Devuelve la informacion asociada a una obra artistica.
 *
 * Si se recibe un tipo no reconocido, la funcion devuelve una obra de respaldo
 * para evitar punteros nulos durante el renderizado.
 */
const ArtworkInfo *getArtworkInfo(ArtworkType type);

#endif