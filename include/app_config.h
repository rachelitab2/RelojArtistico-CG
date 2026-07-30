#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/*
 * Intervalos disponibles para la evolucion futura del cambio automatico
 * de obra. Por ahora la configuracion se expone en la interfaz para mostrar
 * la direccion del sistema sin acoplarla todavia al calculo horario actual.
 */
typedef enum
{
    CHANGE_INTERVAL_15_MINUTES,
    CHANGE_INTERVAL_30_MINUTES,
    CHANGE_INTERVAL_60_MINUTES

} ChangeInterval;

void setChangeInterval(ChangeInterval interval);

ChangeInterval getChangeInterval(void);

const char *getChangeIntervalLabel(ChangeInterval interval);

#endif
