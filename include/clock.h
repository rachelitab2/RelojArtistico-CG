#ifndef CLOCK_H
#define CLOCK_H

void drawClock(void);

void drawClockFace(void);
void drawHourMarks(void);

void drawHourHand(void);
void drawMinuteHand(void);
void drawSecondHand(void);

void updateSystemTime(void);
typedef struct
{
    int hour;
    int minute;
    int second;

} ClockTime;

ClockTime getCurrentTime(void);

#endif