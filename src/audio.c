#include "audio.h"
#include "app_config.h"

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif

/* -1 no es un ChangeInterval valido: fuerza que la primera llamada a
   updateAudioState() siempre dispare una reproduccion. */
static int lastInterval = -1;
static int muted = 0;

/* Reproduce (o detiene) la pista de una franja de tiempo. Si la ruta es
   NULL, esta vacia, o el archivo no existe, PlaySound simplemente no
   suena nada; SND_NODEFAULT evita que Windows reproduzca el sonido de
   sistema por defecto como reemplazo (ver ADR-015: "no debe fallar"). */
static void playTrackForInterval(ChangeInterval interval)
{
#ifdef _WIN32
    const char *path = getIntervalMusicPath(interval);

    if(path == 0 || path[0] == '\0')
    {
        PlaySoundA(0, 0, 0);
        return;
    }

    PlaySoundA(path, 0, SND_ASYNC | SND_LOOP | SND_FILENAME | SND_NODEFAULT);
#else
    (void)interval;
#endif
}

static void stopPlayback(void)
{
#ifdef _WIN32
    PlaySoundA(0, 0, 0);
#endif
}

void initAudio(void)
{
    lastInterval = -1;
    muted = 0;
}

void updateAudioState(void)
{
    ChangeInterval currentInterval = getChangeInterval();

    if((int)currentInterval == lastInterval)
        return;

    lastInterval = (int)currentInterval;

    if(!muted)
        playTrackForInterval(currentInterval);
}

void toggleAudioMute(void)
{
    muted = !muted;

    if(muted)
    {
        stopPlayback();
    }
    else
    {
        /* fuerza replay de la pista actual al desilenciar */
        lastInterval = -1;
        updateAudioState();
    }
}

int isAudioMuted(void)
{
    return muted;
}
