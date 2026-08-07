#ifndef TEXTURE_H
#define TEXTURE_H

/*
 * Textura OpenGL cargada desde archivo (ver ADR-014).
 *
 * textureId en 0 significa "no hay imagen valida": el archivo no existia,
 * no se pudo decodificar, o la ruta era NULL/vacia. Quien use Texture debe
 * verificar ese caso y simplemente omitir el dibujo, sin tratarlo como error.
 */
typedef struct
{
    unsigned int textureId;
    int width;
    int height;

} Texture;

/*
 * Carga una imagen (JPG/PNG/BMP/...) como textura OpenGL, con cache
 * interno por ruta: llamar varias veces con la misma ruta no vuelve a
 * decodificar el archivo ni a subirlo a la GPU.
 */
Texture loadTexture(const char *path);

#endif
