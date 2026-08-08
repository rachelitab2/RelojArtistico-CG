#include "artwork_catalog.h"

/*
 * Catalogo centralizado de obras del proyecto.
 *
 * Cada registro concentra la informacion textual y visual necesaria para que
 * la escena, la interfaz y el motor de segmentos compartan una misma identidad
 * artistica sin duplicar datos en diferentes modulos.
 *
 * Las obras marcadas con [PLACEHOLDER] no tienen todavia un segment_*.c
 * propio; segments.c las dibuja con un relleno generico hasta que se
 * implemente su modulo grafico.
 */
static const ArtworkInfo ARTWORK_CATALOG[] =
{
    /* ===== SALA 15 MIN: Color y geometria ===== */

    {
        ART_MALEVICH,                                          /* [PLACEHOLDER] */
        "Cuadrado negro y cuadrado rojo",
        "Kazimir Malevich",
        "1915",
        "Suprematismo",
        "Dos formas puras sobre fondo blanco: el negro recto domina, el rojo inclinado ~15 grados escapa hacia abajo.",
        "assets/images/malevich.jpg",
        {0.04f, 0.04f, 0.04f},
        {0.95f, 0.95f, 0.92f},
        {
            {0.04f, 0.04f, 0.04f},
            {0.95f, 0.95f, 0.92f},
            {0.55f, 0.10f, 0.10f}
        }
    },
    {
        ART_DELAUNAY,                                          /* [PLACEHOLDER] */
        "Formas simultaneas",
        "Robert Delaunay",
        "1912",
        "Orfismo",
        "Circulos de color puro que vibran por contraste simultaneo de tonos.",
        "assets/images/delaunay.jpg",
        {0.08f, 0.05f, 0.12f},
        {0.95f, 0.38f, 0.12f},
        {
            {0.95f, 0.38f, 0.12f},
            {0.10f, 0.55f, 0.85f},
            {0.90f, 0.82f, 0.15f}
        }
    },
    {
        ART_DEMUTH,
        "Vi la cifra 5 en oro",
        "Charles Demuth",
        "1928",
        "Precisionismo",
        "Numeros dorados en perspectiva sobre un campo de rojo intenso, ritmo urbano.",
        "assets/images/demuth.jpg",
        {0.12f, 0.03f, 0.02f},
        {0.93f, 0.78f, 0.30f},
        {
            {0.93f, 0.78f, 0.30f},
            {0.98f, 0.90f, 0.55f},
            {0.55f, 0.15f, 0.10f}
        }
    },
    {
        ART_KANDINSKY,
        "Composicion VIII",
        "Wassily Kandinsky",
        "1923",
        "Abstraccion",
        "Geometria tratada como ritmo visual mediante circulos, angulos y lineas.",
        "assets/images/kandinsky.jpg",
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
        "assets/images/mondrian.jpg",
        {0.08f, 0.08f, 0.09f},
        {0.85f, 0.15f, 0.12f},
        {
            {0.85f, 0.15f, 0.12f},
            {0.10f, 0.28f, 0.70f},
            {0.95f, 0.78f, 0.10f}
        }
    },
    {
        ART_RILEY,                                             /* [PLACEHOLDER] */
        "Ritmo de color",
        "Bridget Riley",
        "1967",
        "Op Art",
        "Trama viva de paralelogramos diagonales y vibracion cromatico-optica.",
        "assets/images/riley.jpg",
        {0.10f, 0.08f, 0.12f},
        {0.85f, 0.35f, 0.20f},
        {
            {0.18f, 0.42f, 0.72f},
            {0.88f, 0.68f, 0.18f},
            {0.82f, 0.22f, 0.25f}
        }
    },

    /* ===== SALA 30 MIN: Luz, ritmo y color ===== */

    {
        ART_MONET,
        "Nenufares",
        "Claude Monet",
        "1906",
        "Impresionismo",
        "Superficie de agua y luz construida con tonos verdes y pinceladas suaves.",
        "assets/images/monet.jpg",
        {0.06f, 0.11f, 0.10f},
        {0.35f, 0.65f, 0.55f},
        {
            {0.12f, 0.45f, 0.36f},
            {0.35f, 0.65f, 0.55f},
            {0.75f, 0.25f, 0.36f}
        }
    },
    {
        ART_VANGOGH,
        "Noche estrellada",
        "Vincent van Gogh",
        "1889",
        "Postimpresionismo",
        "Cielo nocturno en movimiento con contraste intenso entre azul y amarillo.",
        "assets/images/vangogh.jpg",
        {0.04f, 0.05f, 0.12f},
        {0.95f, 0.80f, 0.25f},
        {
            {0.05f, 0.09f, 0.32f},
            {0.18f, 0.36f, 0.72f},
            {0.95f, 0.80f, 0.25f}
        }
    },
    {
        ART_ESCHER,                                            /* [PLACEHOLDER] */
        "Symmetry Drawing No. 69",
        "M. C. Escher",
        "1948",
        "Teselacion / Op Art",
        "Reptiles entrelazados que giran alrededor de un punto central, sin huecos ni superposiciones.",
        "assets/images/escher.jpg",
        {0.10f, 0.06f, 0.03f},
        {0.85f, 0.32f, 0.18f},
        {
            {0.72f, 0.20f, 0.14f},
            {0.55f, 0.70f, 0.80f},
            {0.80f, 0.62f, 0.20f}
        }
    },
    {
        ART_SEURAT,
        "Un domingo en la Grande Jatte",
        "Georges Seurat",
        "1886",
        "Puntillismo",
        "Escena de ocio parisino construida punto a punto con colores complementarios.",
        "assets/images/seurat.jpg",
        {0.30f, 0.45f, 0.28f},
        {0.85f, 0.75f, 0.35f},
        {
            {0.30f, 0.45f, 0.28f},
            {0.55f, 0.70f, 0.50f},
            {0.85f, 0.75f, 0.35f}
        }
    },
    {
        ART_ALMA_THOMAS,                                       /* [PLACEHOLDER] */
        "Fiesta de primavera",
        "Alma Thomas",
        "1968",
        "Color Field",
        "Mosaico vibrante de pinceladas rectangulares que celebran la naturaleza.",
        "assets/images/alma_thomas.jpg",
        {0.10f, 0.18f, 0.08f},
        {0.85f, 0.45f, 0.10f},
        {
            {0.85f, 0.45f, 0.10f},
            {0.30f, 0.65f, 0.20f},
            {0.90f, 0.78f, 0.15f}
        }
    },
    {
        ART_PAUL_KLEE,
        "Castillo y sol",
        "Paul Klee",
        "1928",
        "Bauhaus",
        "Silueta de castillo construida con bloques de color puro; un sol geometrico domina el cielo.",
        "assets/images/paul_klee.jpg",
        {0.18f, 0.08f, 0.03f},
        {0.90f, 0.45f, 0.10f},
        {
            {0.90f, 0.45f, 0.10f},
            {0.15f, 0.30f, 0.15f},
            {0.85f, 0.15f, 0.12f}
        }
    },

    /* ===== SALA 60 MIN: Abstraccion, simbolo y atmosfera ===== */

    {
        ART_KLIMT,
        "El beso",
        "Gustav Klimt",
        "1908",
        "Modernismo vienes",
        "Ornamento dorado y patrones planos que envuelven la figura principal.",
        "assets/images/klimt.jpg",
        {0.10f, 0.08f, 0.05f},
        {0.85f, 0.68f, 0.28f},
        {
            {0.85f, 0.68f, 0.28f},
            {0.55f, 0.38f, 0.10f},
            {0.65f, 0.10f, 0.16f}
        }
    },
    {
        ART_HILMA,                                             /* [PLACEHOLDER] */
        "Las diez mayores, No. 7, Adultez",
        "Hilma af Klint",
        "1907",
        "Abstraccion espiritualista",
        "Formas organicas y espirales que expresan estados del alma sin figuracion.",
        "assets/images/hilma.jpg",
        {0.12f, 0.08f, 0.18f},
        {0.90f, 0.55f, 0.70f},
        {
            {0.90f, 0.55f, 0.70f},
            {0.42f, 0.72f, 0.82f},
            {0.95f, 0.88f, 0.45f}
        }
    },
    {
        ART_JOAN_MIRO,                                         /* [PLACEHOLDER] */
        "El carnaval del arlequin",
        "Joan Miro",
        "1925",
        "Surrealismo",
        "Biomorphias y signos flotantes en un universo onico de color y fantasia.",
        "assets/images/joan_miro.jpg",
        {0.05f, 0.05f, 0.18f},
        {0.92f, 0.22f, 0.18f},
        {
            {0.92f, 0.22f, 0.18f},
            {0.95f, 0.80f, 0.10f},
            {0.10f, 0.35f, 0.80f}
        }
    },
    {
        ART_ROTHKO,
        "Color fields for kids",
        "Mark Rothko",
        "1953",
        "Expresionismo abstracto",
        "Bandas verticales de color puro con pincelada pastosa, casi tactil, que laten una junto a otra.",
        "assets/images/rothko.jpg",
        {0.11f, 0.04f, 0.04f},
        {0.90f, 0.46f, 0.16f},
        {
            {0.90f, 0.46f, 0.16f},
            {0.80f, 0.10f, 0.14f},
            {0.55f, 0.16f, 0.22f}
        }
    },
    {
        ART_KUPKA,                                             /* [PLACEHOLDER] */
        "Amorfa, fuga en dos colores",
        "Frantisek Kupka",
        "1912",
        "Orfismo",
        "Espiral dinamica de rojo y azul que evoca ritmo musical en forma pura.",
        "assets/images/kupka.jpg",
        {0.06f, 0.04f, 0.12f},
        {0.88f, 0.18f, 0.22f},
        {
            {0.88f, 0.18f, 0.22f},
            {0.12f, 0.22f, 0.78f},
            {0.90f, 0.88f, 0.85f}
        }
    },
    {
        ART_TAEUBER_ARP,                                       /* [PLACEHOLDER] */
        "Composicion de circulos y semicirculos superpuestos",
        "Sophie Taeuber-Arp",
        "1930",
        "Arte concreto",
        "Circulos de colores puros que se ordenan con rigor geometrico y alegria.",
        "assets/images/taeuber_arp.jpg",
        {0.08f, 0.10f, 0.18f},
        {0.90f, 0.42f, 0.18f},
        {
            {0.90f, 0.42f, 0.18f},
            {0.15f, 0.55f, 0.80f},
            {0.92f, 0.82f, 0.20f}
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