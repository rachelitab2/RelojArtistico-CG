#include <GL/freeglut.h>

#include <math.h>

#include "clock.h"
#include "utils.h"
#include <time.h>

void drawClock(void)
{
    int i;

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
            glColor3f(0.90f,0.90f,0.92f);
            glLineWidth(2.2f);
        }
        else
        {
            glColor3f(0.55f,0.55f,0.58f);
            glLineWidth(1.0f);
        }

        drawLine(x1,y1,x2,y2);
    }

    ClockTime current = getCurrentTime();

    drawHourHand(current);

    drawMinuteHand(current);

    drawSecondHand(current);

    glColor3f(0.85f,0.65f,0.25f);

    drawFilledCircle(0.0f,0.0f,0.015f);
}

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

float hourAngle(ClockTime t){
    return (t.hour % 12) * 30.0f
         + t.minute * 0.5f;
}

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

    glColor3f(0.92f,0.92f,0.90f);

    glLineWidth(4.0f);

    drawLine(0.0f,0.0f,x,y);
}
void drawMinuteHand(ClockTime t)
{
    float angle = degreesToRadians(minuteAngle(t));

    float x = 0.23f * sin(angle);
    float y = 0.23f * cos(angle);

    glColor3f(0.75f,0.75f,0.78f);

    glLineWidth(2.4f);

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