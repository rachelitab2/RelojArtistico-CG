#include "app_config.h"

/*
 * Estado global minimo de configuracion.
 *
 * Se mantiene encapsulado para que la interfaz, el input y el motor del reloj
 * no dependan de variables compartidas. Si luego se agrega un archivo de
 * configuracion, este modulo sera el punto natural de extension.
 */
static ChangeInterval currentInterval = CHANGE_INTERVAL_30_MINUTES;

void setChangeInterval(ChangeInterval interval)
{
    currentInterval = interval;
}

ChangeInterval getChangeInterval(void)
{
    return currentInterval;
}

const char *getChangeIntervalLabel(ChangeInterval interval)
{
    switch(interval)
    {
        case CHANGE_INTERVAL_15_MINUTES:
            return "15 min";

        case CHANGE_INTERVAL_30_MINUTES:
            return "30 min";

        case CHANGE_INTERVAL_60_MINUTES:
            return "1 hr";
    }

    return "30 min";
}
