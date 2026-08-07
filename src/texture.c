#include <GL/freeglut.h>
#include <string.h>

#include "texture.h"

/* unico .c del proyecto que compila la implementacion de stb_image;
   el resto solo ve el header sin STB_IMAGE_IMPLEMENTATION (ver
   ADR-014). */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TEXTURE_CACHE_CAPACITY 16

typedef struct
{
    const char *path;
    Texture texture;

} CacheEntry;

static CacheEntry cache[TEXTURE_CACHE_CAPACITY];
static int cacheCount = 0;

static const Texture *findInCache(const char *path)
{
    int i;

    for(i = 0; i < cacheCount; i++)
    {
        if(strcmp(cache[i].path, path) == 0)
            return &cache[i].texture;
    }

    return 0;
}

static void addToCache(const char *path, Texture texture)
{
    if(cacheCount >= TEXTURE_CACHE_CAPACITY)
        return; /* cache llena: no crashea, simplemente no cachea esta */

    cache[cacheCount].path = path;
    cache[cacheCount].texture = texture;
    cacheCount++;
}

Texture loadTexture(const char *path)
{
    Texture failed = {0, 0, 0};
    const Texture *cached;
    int width;
    int height;
    int channels;
    unsigned char *pixels;
    unsigned int textureId;
    GLenum format;

    if(path == 0 || path[0] == '\0')
        return failed;

    cached = findInCache(path);
    if(cached != 0)
        return *cached;

    /* stb_image carga de arriba hacia abajo; OpenGL espera el origen
       de la textura abajo a la izquierda. Sin esto la imagen queda
       invertida verticalmente. */
    stbi_set_flip_vertically_on_load(1);

    pixels = stbi_load(path, &width, &height, &channels, 0);

    if(pixels == 0)
    {
        /* no existe o no se pudo decodificar: se cachea el fallo para
           no reintentar la lectura de disco cada vez que se pide
           (ver ADR-014, "no debe fallar"). */
        addToCache(path, failed);
        return failed;
    }

    format = (channels == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* GL_CLAMP_TO_EDGE (GL 1.2) no esta en el header GL 1.1 de este
       toolchain; GL_CLAMP alcanza para el uso de este proyecto (quads
       enteros, sin tiling). */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);

    {
        Texture result;

        result.textureId = textureId;
        result.width = width;
        result.height = height;

        addToCache(path, result);

        return result;
    }
}
