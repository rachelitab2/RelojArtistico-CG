#include <GL/freeglut.h>

#include <math.h>

#include "clock.h"
#include "utils.h"
#include <time.h>

/* pixeles (los que reporta GLUT) -> unidades del mundo. Mismo criterio
   que segments.c/ui.c, duplicado aqui porque clock.c no depende de
   esos modulos. */
static float pixelsToWorldUnits(int pixels)
{
    GLint viewport[4];
    float aspect;
    float worldHeight;

    glGetIntegerv(GL_VIEWPORT, viewport);

    if(viewport[2] <= 0 || viewport[3] <= 0)
        return 0.0f;

    aspect = (float)viewport[2] / (float)viewport[3];
    worldHeight = (aspect >= 1.0f) ? 2.0f : (2.0f / aspect);

    return (float)pixels * worldHeight / (float)viewport[3];
}

/* dibuja "text" centrado en (centerX, centerY), no solo en X: baja la
   linea base la mitad de la altura aproximada de la fuente. */
static void drawCenteredBitmapText(float centerX, float centerY, void *font, const char *text)
{
    int totalPixelWidth = 0;
    int i;
    float startX;
    float startY;

    for(i = 0; text[i] != '\0'; i++)
        totalPixelWidth += glutBitmapWidth(font, text[i]);

    startX = centerX - pixelsToWorldUnits(totalPixelWidth) * 0.5f;
    startY = centerY - pixelsToWorldUnits(10) * 0.35f;

    glRasterPos2f(startX, startY);

    for(i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(font, text[i]);
}

/* Dibuja la caratula completa: fondo, marcas, manecillas y pivote.
   Se llama una vez por frame desde display.c. */
void drawClock(void)
{
    int i;

    /* activa antialiasing una sola vez, se queda encendido siempre */
    static int smoothingEnabled = 0;

    if(!smoothingEnabled)
    {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        smoothingEnabled = 1;
    }

    const float faceRadius = 0.30f;

    glColor3f(0.10f,0.10f,0.12f);

    drawFilledCircle(0.0f,0.0f,faceRadius);

    glColor3f(0.85f,0.85f,0.88f);

    glLineWidth(1.2f);

    drawCircle(0.0f,0.0f,faceRadius);

    /* solo las 4 posiciones cardinales llevan numero (ADR-016): mantiene
       la caratula legible sin volver a saturarla con los 12 numeros. */
    static const char *MAJOR_LABELS[4] = {"12", "3", "6", "9"};
    /* claramente por DENTRO de tickInner (faceRadius-0.08=0.22), no
       encima: con faceRadius-0.065=0.235 el numero caia entre
       tickInner y tickOuter y se dibujaba pegado a la propia marca
       (se veia como "-9"). El centro ya quedo libre (se saco la
       etiqueta de hora+obra de segments.c), asi que hay espacio de
       sobra para separarlos bien. */
    const float numberRadius = faceRadius - 0.13f;

    /* 12 marcas; cada 3 (12/3/6/9) se dibuja mas larga y gruesa */
    for(i=0;i<12;i++)
    {
        float angle=degreesToRadians(i*30);
        int isMajor = (i % 3 == 0);

        float tickOuter = faceRadius - 0.01f;
        float tickInner = isMajor ? (faceRadius - 0.08f) : (faceRadius - 0.05f);

        float x1 = tickInner*sin(angle);
        float y1 = tickInner*cos(angle);

        float x2 = tickOuter*sin(angle);
        float y2 = tickOuter*cos(angle);

        if(isMajor)
        {
            glColor3f(0.70f,0.70f,0.74f);
            glLineWidth(1.6f);
        }
        else
        {
            glColor3f(0.42f,0.42f,0.46f);
            glLineWidth(0.8f);
        }

        drawLine(x1,y1,x2,y2);

        if(isMajor)
        {
            float numberX = numberRadius*sin(angle);
            float numberY = numberRadius*cos(angle);

            /* color propio, mas brillante que la marca: a 0.70 se perdia
               contra el fondo oscuro de la caratula (0.10,0.10,0.12) */
            glColor3f(0.88f,0.88f,0.90f);

            drawCenteredBitmapText(numberX, numberY, GLUT_BITMAP_HELVETICA_10, MAJOR_LABELS[i/3]);
        }
    }

    ClockTime current = getCurrentTime();

    drawHourHand(current);

    drawMinuteHand(current);

    drawSecondHand(current);

    glColor3f(0.85f,0.65f,0.25f);

    drawFilledCircle(0.0f,0.0f,0.015f); /* pivote central */
}

/* hora real del sistema, sin cache: se llama una vez por frame */
ClockTime getCurrentTime(void)
{
    time_t now;

    time(&now);

    struct tm *current = localtime(&now);

    ClockTime t;

    t.hour = current->tm_hour;
    t.minute = current->tm_min;
    t.second = current->tm_sec;

    return t;
}

/* CUESTIONABLE: hourAngle/minuteAngle/secondAngle no son "static" pero
   tampoco estan declaradas en clock.h; solo se usan dentro de este
   archivo. Deberian ser static o quedar expuestas en el header. */

/* angulo de la manecilla de hora: 30 grados por hora + arrastre por minuto */
float hourAngle(ClockTime t){
    return (t.hour % 12) * 30.0f
         + t.minute * 0.5f;
}

/* 6 grados por minuto + arrastre por segundo */
float minuteAngle(ClockTime t){
    return t.minute * 6.0f
         + t.second * 0.1f;
}

float secondAngle(ClockTime t){
    return t.second * 6.0f;
}
void drawHourHand(ClockTime t)
{
    float angle = degreesToRadians(hourAngle(t));

    float x = 0.15f * sin(angle);
    float y = 0.15f * cos(angle);

    glColor3f(0.75f,0.75f,0.74f);

    glLineWidth(3.0f);

    drawLine(0.0f,0.0f,x,y);
}
void drawMinuteHand(ClockTime t)
{
    float angle = degreesToRadians(minuteAngle(t));

    float x = 0.23f * sin(angle);
    float y = 0.23f * cos(angle);

    glColor3f(0.60f,0.60f,0.64f);

    glLineWidth(1.8f);

    drawLine(0.0f,0.0f,x,y);
}
void drawSecondHand(ClockTime t)
{
    float angle = degreesToRadians(secondAngle(t));

    float x = 0.27f * sin(angle);
    float y = 0.27f * cos(angle);

    glColor3f(0.85f,0.65f,0.25f);

    glLineWidth(1.0f);

    drawLine(0.0f,0.0f,x,y);
}
