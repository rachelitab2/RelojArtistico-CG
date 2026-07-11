#ifndef CLOCK_H
#define CLOCK_H

typedef struct
{
    int hour;
    int minute;
    int second;

} ClockTime;

void drawClock(void); /* caratula + manecillas, un solo llamado por frame */

/* CUESTIONABLE: declaradas pero nunca implementadas en clock.c */
void drawClockFace(void);
void drawHourMarks(void);

void drawHourHand(ClockTime t);
void drawMinuteHand(ClockTime t);
void drawSecondHand(ClockTime t);

void updateSystemTime(void); /* CUESTIONABLE: declarada, nunca implementada */

ClockTime getCurrentTime(void); /* hora real del sistema */

#endif
