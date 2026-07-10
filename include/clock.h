#ifndef CLOCK_H
#define CLOCK_H

typedef struct
{
    int hour;
    int minute;
    int second;

} ClockTime;

void drawClock(void);

void drawClockFace(void);
void drawHourMarks(void);

void drawHourHand(ClockTime t);
void drawMinuteHand(ClockTime t);
void drawSecondHand(ClockTime t);

void updateSystemTime(void);

ClockTime getCurrentTime(void);

#endif