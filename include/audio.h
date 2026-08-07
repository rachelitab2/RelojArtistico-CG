#ifndef AUDIO_H
#define AUDIO_H

/*
 * Modulo de audio: reproduce la pista de fondo asociada a la obra activa.
 *
 * Responsabilidad unica: decidir que archivo suena y si el usuario lo
 * silencio. No conoce nada de renderizado; solo consulta el catalogo y
 * la obra activa (ver ADR-015).
 */

void initAudio(void);

/* Revisa si el intervalo de cambio de obra (15/30/60 min) cambio desde
   la ultima llamada y, si es asi, cambia la pista de fondo. La musica
   esta ligada a la franja de tiempo, no a la obra individual (ver
   ADR-015 revisado), para que agregar mas obras al catalogo no
   requiera mas audios. Debe llamarse periodicamente (timer de
   display.c). */
void updateAudioState(void);

/* Alterna silencio/sonido. El estado de mute persiste aunque cambie
   la obra activa. */
void toggleAudioMute(void);

int isAudioMuted(void);

#endif
