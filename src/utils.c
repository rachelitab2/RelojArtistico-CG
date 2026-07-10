#include <math.h>
#include <GL/freeglut.h>

#include "utils.h"

float degreesToRadians(float degrees)
{
    return degrees * (3.1415926535f / 180.0f);
}

void drawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);

    glVertex2f(x1, y1);
    glVertex2f(x2, y2);

    glEnd();
}

void drawCircle(float x, float y, float radius)
{
    int i;

    glBegin(GL_LINE_LOOP);

    for(i = 0; i < 64; i++)
    {
        float angle = 2.0f * 3.1415926535f * i / 64.0f;

        float px = x + radius * cos(angle);
        float py = y + radius * sin(angle);

        glVertex2f(px, py);
    }

    glEnd();
}

void drawFilledCircle(float x, float y, float radius)
{
    int i;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y);

    for(i = 0; i <= 64; i++)
    {
        float angle = 2.0f * 3.1415926535f * i / 64.0f;

        float px = x + radius * cos(angle);
        float py = y + radius * sin(angle);

        glVertex2f(px, py);
    }

    glEnd();
}

void drawArc(float radius,
             float startAngle,
             float endAngle)
{
    glBegin(GL_LINE_STRIP);

    for(float angle = startAngle;
        angle <= endAngle;
        angle += 2.0f)
    {
        float rad = degreesToRadians(angle);

        float x = cos(rad) * radius;
        float y = sin(rad) * radius;

        glVertex2f(x, y);
    }

    glEnd();
}

void drawFilledArc(float innerRadius,
                   float outerRadius,
                   float startAngle,
                   float endAngle)
{
    glBegin(GL_TRIANGLE_STRIP);

    for(float angle = startAngle;
        angle <= endAngle;
        angle += 2.0f)
    {
        float rad = degreesToRadians(angle);

        float x1 = cos(rad) * innerRadius;
        float y1 = sin(rad) * innerRadius;

        float x2 = cos(rad) * outerRadius;
        float y2 = sin(rad) * outerRadius;

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }

    glEnd();
}