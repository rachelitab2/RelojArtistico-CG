#include <math.h>
#include <GL/freeglut.h>

#include "segment_demuth.h"
#include "utils.h"

/*
 * Charles Demuth - "I Saw the Figure 5 in Gold" (1928, Precisionismo).
 *
 * La obra original es un 5 gigante crema/dorado que ocupa casi todo el
 * plano, con un 5 mas chico adentro y otro oscuro detras, sobre un fondo
 * de planos rojizos en perspectiva. Esta version conserva esa jerarquia:
 * el numero domina, el fondo aporta profundidad, nada mas compite.
 */

/* ---------------------------------------------------------------- */
/* Fondo: planos diagonales                                          */
/* ---------------------------------------------------------------- */

/* Cuatro zonas de color separadas por fronteras que se DESPLAZAN con el
   angulo. Al recorrer el sector en tiras angulares finas y mover el
   radio de corte en cada tira, las fronteras dejan de ser arcos
   concentricos y se leen como planos diagonales: es la unica forma de
   sugerir la perspectiva del cuadro dentro de geometria polar, sin
   texturas. */
static void drawDiagonalPlanes(float innerRadius, float outerRadius, float halfAngle)
{
    static const int STRIPS = 20;

    static const float ZONE_COLOR[4][3] =
    {
        {0.26f, 0.04f, 0.05f}, /* vino profundo (fondo) */
        {0.52f, 0.11f, 0.07f}, /* rojo oscuro */
        {0.72f, 0.24f, 0.09f}, /* rojo naranja */
        {0.86f, 0.42f, 0.14f}  /* naranja encendido (borde exterior) */
    };

    float span = outerRadius - innerRadius;
    int i;

    for(i = 0; i < STRIPS; i++)
    {
        float t = (float)i / (float)STRIPS;
        float a0 = -halfAngle + 2.0f * halfAngle * t;
        float a1 = -halfAngle + 2.0f * halfAngle * ((float)(i + 1) / (float)STRIPS);

        /* las 3 fronteras avanzan hacia afuera a medida que avanza el
           angulo -> el corte entre colores queda inclinado */
        float split[3];
        float previous = 0.0f;
        int zone;

        split[0] = 0.14f + 0.36f * t;
        split[1] = 0.44f + 0.34f * t;
        split[2] = 0.72f + 0.28f * t;

        for(zone = 0; zone < 4; zone++)
        {
            float limit = (zone < 3) ? split[zone] : 1.0f;

            glColor3f(ZONE_COLOR[zone][0], ZONE_COLOR[zone][1], ZONE_COLOR[zone][2]);

            drawFilledArc(innerRadius + span * previous,
                          innerRadius + span * limit,
                          a0, a1);

            previous = limit;
        }
    }
}

void drawDemuthBackground(float innerRadius, float outerRadius, float halfAngle)
{
    drawDiagonalPlanes(innerRadius, outerRadius, halfAngle);
}

/* ---------------------------------------------------------------- */
/* Primer plano: el numero                                           */
/* ---------------------------------------------------------------- */

/* Cinta de grosor constante sobre un camino: permite trazos curvos
   gruesos sin generar un poligono auto-intersectante. path[i] =
   {x, y, semigrosor}. */
static void drawRibbon(const float path[][3], int count, float r, float g, float b)
{
    int i;

    glColor3f(r, g, b);

    glBegin(GL_TRIANGLE_STRIP);

    for(i = 0; i < count; i++)
    {
        float dx, dy, len, nx, ny;

        if(i == 0)
        {
            dx = path[i+1][0] - path[i][0];
            dy = path[i+1][1] - path[i][1];
        }
        else if(i == count - 1)
        {
            dx = path[i][0] - path[i-1][0];
            dy = path[i][1] - path[i-1][1];
        }
        else
        {
            dx = path[i+1][0] - path[i-1][0];
            dy = path[i+1][1] - path[i-1][1];
        }

        len = sqrtf(dx*dx + dy*dy);
        if(len < 0.0001f)
            len = 0.0001f;

        nx = -dy / len;
        ny =  dx / len;

        glVertex2f(path[i][0] + nx*path[i][2], path[i][1] + ny*path[i][2]);
        glVertex2f(path[i][0] - nx*path[i][2], path[i][1] - ny*path[i][2]);
    }

    glEnd();
}

/*
 * Camino de un "5" tipografico, dimensionado para ocupar casi todo el
 * lienzo local (-1..1):
 *
 *   barra superior  (0.50,0.78) -> (-0.42,0.78)
 *   asta vertical   (-0.42,0.78) -> (-0.42,0.20)
 *   entrada al ojo  (-0.42,0.20) -> (0.03,0.20)
 *   ojo             arco de 90 a -160 grados, centro (0.03,-0.26)
 *
 * El arco arranca exactamente en el final de la entrada (a 90 grados
 * del centro cae (0.03, 0.20)), asi el trazo es continuo y no aparecen
 * dos piezas sueltas.
 */
static int buildFivePath(float path[][3])
{
    static const float thickness = 0.115f;
    static const float bowlCenterX =  0.03f;
    static const float bowlCenterY = -0.26f;
    static const float bowlRadius  =  0.46f;
    static const float startAngleDeg =   90.0f;
    static const float endAngleDeg   = -160.0f;
    static const int   bowlSteps = 20;

    int count = 0;
    int i;

    path[count][0] =  0.50f; path[count][1] = 0.78f; path[count][2] = thickness; count++;
    path[count][0] = -0.42f; path[count][1] = 0.78f; path[count][2] = thickness; count++;
    path[count][0] = -0.42f; path[count][1] = 0.20f; path[count][2] = thickness; count++;

    for(i = 0; i <= bowlSteps; i++)
    {
        float t = (float)i / (float)bowlSteps;
        float angle = degreesToRadians(startAngleDeg + (endAngleDeg - startAngleDeg) * t);

        path[count][0] = bowlCenterX + bowlRadius * cosf(angle);
        path[count][1] = bowlCenterY + bowlRadius * sinf(angle);
        path[count][2] = thickness;
        count++;
    }

    return count;
}

/* Un "5" a la escala, posicion y color pedidos. glScalef escala tambien
   el grosor de la cinta, por eso alcanza con un solo camino base. */
static void drawFive(float scale, float offsetX, float offsetY, float r, float g, float b)
{
    float path[32][3];
    int count = buildFivePath(path);

    glPushMatrix();

        glTranslatef(offsetX, offsetY, 0.0f);
        glScalef(scale, scale, 1.0f);

        drawRibbon(path, count, r, g, b);

    glPopMatrix();
}

/* Lineas finas que cruzan la composicion: en el cuadro original marcan
   la fuga de la calle y el destello de los faros. */
static void drawPerspectiveLines(void)
{
    glColor3f(0.16f, 0.03f, 0.04f);

    glLineWidth(2.0f);
    drawLine(-0.95f,  0.62f, 0.95f, -0.30f);

    glLineWidth(1.4f);
    drawLine(-0.95f,  0.20f, 0.95f, -0.72f);

    glLineWidth(1.4f);
    drawLine(-0.60f,  0.95f, 0.72f, -0.95f);
}

/* Circulo claro en la esquina superior izquierda (el farol del cuadro). */
static void drawCornerLamp(void)
{
    glColor3f(0.96f, 0.93f, 0.80f);

    drawFilledCircle(-0.70f, 0.72f, 0.075f);
}

/*
 * Angulo acumulado en la matriz de modelado.
 *
 * El lienzo local de cada obra vive dentro del glRotatef del sector
 * (segments.c), necesario para que fondo y borde apunten hacia afuera.
 * Para una ola o una espiral da igual, pero un numero tiene que leerse
 * derecho siempre. Se extrae el angulo de la submatriz 2x2 superior
 * izquierda -- valido porque solo se acumulan rotaciones y escalas
 * uniformes, nunca deformaciones -- para poder cancelarlo.
 */
static float getCurrentRotationDegrees(void)
{
    float matrix[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

    return atan2f(matrix[1], matrix[0]) * (180.0f / 3.14159265f);
}

/*
 * Orden de dibujo (de atras hacia adelante), igual que la lectura del
 * cuadro: el 5 oscuro detras da profundidad, el crema domina la
 * composicion, y el amarillo queda como nucleo encendido.
 *
 * COMPOSITION_SCALE mantiene el conjunto dentro del radio 1.0 del
 * lienzo: al contrarrotar, cualquier punto mas lejos que eso se saldria
 * del sector por las esquinas.
 */
void drawDemuth(void)
{
    static const float COMPOSITION_SCALE = 0.92f;

    float uprightAngle = -getCurrentRotationDegrees();

    glPushMatrix();

        glRotatef(uprightAngle, 0.0f, 0.0f, 1.0f);
        glScalef(COMPOSITION_SCALE, COMPOSITION_SCALE, 1.0f);

        drawPerspectiveLines();

        drawFive(1.00f, -0.10f,  0.07f, 0.38f, 0.06f, 0.06f); /* detras, vino */
        drawFive(1.00f,  0.06f,  0.00f, 0.93f, 0.89f, 0.70f); /* dominante, crema */
        drawFive(0.56f,  0.16f, -0.04f, 0.95f, 0.78f, 0.16f); /* nucleo, amarillo */

        drawCornerLamp();

    glPopMatrix();
}
