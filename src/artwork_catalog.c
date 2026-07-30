#include "artwork_catalog.h"

/*
 * Catalogo centralizado de obras del proyecto.
 *
 * Cada registro concentra la informacion textual y visual necesaria para que
 * la escena, la interfaz y el motor de segmentos compartan una misma identidad
 * artistica sin duplicar datos en diferentes modulos.
 */
static const ArtworkInfo ARTWORK_CATALOG[] =
{
    {
        ART_HOKUSAI,
        "La gran ola de Kanagawa",
        "Katsushika Hokusai",
        "1831",
        "Ukiyo-e",
        "Grabado japones con azul profundo, espuma blanca y movimiento estructural.",
        {0.05f, 0.09f, 0.14f},
        {0.30f, 0.55f, 0.85f},
        {
            {0.05f, 0.16f, 0.32f},
            {0.30f, 0.55f, 0.85f},
            {0.92f, 0.86f, 0.68f}
        }
    },
    {
        ART_VANGOGH,
        "Noche estrellada",
        "Vincent van Gogh",
        "1889",
        "Postimpresionismo",
        "Cielo nocturno en movimiento con contraste intenso entre azul y amarillo.",
        {0.04f, 0.05f, 0.12f},
        {0.95f, 0.80f, 0.25f},
        {
            {0.05f, 0.09f, 0.32f},
            {0.18f, 0.36f, 0.72f},
            {0.95f, 0.80f, 0.25f}
        }
    },
    {
        ART_KANDINSKY,
        "Composicion VIII",
        "Wassily Kandinsky",
        "1923",
        "Abstraccion",
        "Geometria tratada como ritmo visual mediante circulos, angulos y lineas.",
        {0.10f, 0.08f, 0.12f},
        {0.85f, 0.20f, 0.20f},
        {
            {0.85f, 0.20f, 0.20f},
            {0.20f, 0.42f, 0.78f},
            {0.95f, 0.66f, 0.12f}
        }
    },
    {
        ART_MONDRIAN,
        "Composicion con rojo, azul y amarillo",
        "Piet Mondrian",
        "1930",
        "Neoplasticismo",
        "Reticula ortogonal de lineas negras y bloques de color primario.",
        {0.08f, 0.08f, 0.09f},
        {0.85f, 0.15f, 0.12f},
        {
            {0.85f, 0.15f, 0.12f},
            {0.10f, 0.28f, 0.70f},
            {0.95f, 0.78f, 0.10f}
        }
    },
    {
        ART_MONET,
        "Nenufares",
        "Claude Monet",
        "1906",
        "Impresionismo",
        "Superficie de agua y luz construida con tonos verdes y pinceladas suaves.",
        {0.06f, 0.11f, 0.10f},
        {0.35f, 0.65f, 0.55f},
        {
            {0.12f, 0.45f, 0.36f},
            {0.35f, 0.65f, 0.55f},
            {0.75f, 0.25f, 0.36f}
        }
    },
    {
        ART_KLIMT,
        "El beso",
        "Gustav Klimt",
        "1908",
        "Modernismo vienes",
        "Ornamento dorado y patrones planos que envuelven la figura principal.",
        {0.10f, 0.08f, 0.05f},
        {0.85f, 0.68f, 0.28f},
        {
            {0.85f, 0.68f, 0.28f},
            {0.55f, 0.38f, 0.10f},
            {0.65f, 0.10f, 0.16f}
        }
    }
};

static const int ARTWORK_CATALOG_SIZE =
    sizeof(ARTWORK_CATALOG) / sizeof(ARTWORK_CATALOG[0]);

const ArtworkInfo *getArtworkInfo(ArtworkType type)
{
    int i;

    for(i = 0; i < ARTWORK_CATALOG_SIZE; i++)
    {
        if(ARTWORK_CATALOG[i].type == type)
            return &ARTWORK_CATALOG[i];
    }

    return &ARTWORK_CATALOG[0];
}