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

/* Pista de audio de fondo asociada a la franja de tiempo (no a la obra
   individual, ver ADR-015 revisado): asi el numero de audios necesarios
   no crece si se agregan mas obras al catalogo. Puede devolver NULL o
   una ruta a un archivo inexistente; audio.c debe tolerarlo. */
const char *getIntervalMusicPath(ChangeInterval interval);

#endif
