ADR-011 – Sistema de salas de tiempo (18 obras, 3 intervalos)
Estado

Aceptado. Reemplaza la restricción de "seis obras fijas" descrita en el ADR-005, el ADR-008 y el ADR-018.

Contexto

El diseño original del proyecto (ver ADR-005, ADR-008) fijaba el catálogo en exactamente seis obras, una por segmento, con un único intervalo de cambio (cada 15 minutos). Esa restricción era intencional en su momento: simplificaba el enum ArtworkType, el dispatcher de segments.c y el layout angular de la galería.

El proyecto evolucionó hacia un alcance mayor: representar 18 obras en total, agrupadas temáticamente en tres "salas" (Color y geometría / Luz, ritmo y color / Símbolo, ornamento y abstracción), cada una con seis obras y su propio intervalo de cambio de obra activa (15, 30 o 60 minutos). El usuario elige la sala activa con las teclas 1/2/3 (alias 4/5/6, ver ADR-014).

Problema

Extender el sistema de seis obras fijas a 18 obras organizadas en tres salas intercambiables en tiempo de ejecución, sin reescribir el motor de segmentos ni duplicar la lógica de layout, resaltado y sincronización con el reloj que ya funcionaba para el caso de seis obras.

Alternativas consideradas
Triplicar segments[] y el motor de segmentos

Mantener tres instancias paralelas de la lógica de segmentos, una por sala, activando solo una a la vez.

Se descartó: triplica el codigo de layout, animación de resaltado y despacho, con el consiguiente riesgo de que las tres copias diverjan.

Un enum ArtworkType de 18 valores sin agrupación

Extender el enum existente a 18 valores y elegir 6 "activos" por algún criterio en tiempo de ejecución.

Funcionalmente equivalente a la opción elegida, pero sin una estructura de datos explícita que documente qué 6 obras corresponden a cada sala, dejando esa relación implícita en el código del dispatcher.

Arrays ROOM_15_MIN / ROOM_30_MIN / ROOM_60_MIN + una sala activa

La alternativa seleccionada: el enum ArtworkType crece a 18 valores (agrupados por comentario en tres bloques de 6), y tres arrays constantes (ROOM_15_MIN, ROOM_30_MIN, ROOM_60_MIN, cada uno de tamaño ROOM_SIZE=6) mapean cada sala a sus 6 ArtworkType. Una variable activeRoom (1, 2 o 3) determina cuál array se copia a segments[] en cada momento.

Decisión

Se agregó ROOM_SIZE=6 y los tres arrays ROOM_15_MIN/30/60_MIN en segments.h/.c. setActiveRoom(int roomKey) valida el rango (1-3), llama a loadRoomIntoSegments(roomKey) para recargar segments[6] con las 6 obras de esa sala, ajusta el intervalo de cambio de obra activa vía setChangeInterval() (ver app_config), y resetea la animación de layout para que la transición entre salas arranque limpia.

computeActiveArtworkIndex() deriva slotMinutes de la sala activa (15/30/60) en vez de un valor fijo, manteniendo una única implementación de la lógica de sincronización con el reloj (ver ADR-009) para las tres salas.

El resto del motor de segmentos (drawSegment, updateSectorLayout, el dispatcher de drawArtwork/drawArtworkBackground) no cambió de forma: sigue operando sobre segments[6] sin saber en qué sala está, porque loadRoomIntoSegments() ya dejó los ArtworkType correctos ahí.

Justificación

Reutilizar segments[6] como buffer único, recargado según la sala activa, evita triplicar el motor de segmentos: todo el código de animación, resaltado y despacho por ArtworkType (ADR-004, ADR-008, ADR-010) sigue funcionando sin cambios porque opera sobre el mismo array de siempre, solo que su contenido ahora depende de activeRoom.

Los arrays ROOM_* documentan explícitamente, en un solo lugar, qué 6 obras componen cada sala — más legible que derivarlo con aritmética sobre un enum de 18 valores.

Consecuencias
Ventajas
Triplica el contenido del catálogo (6 → 18 obras) sin triplicar el motor de segmentos.
El código de layout, resaltado y sincronización con el reloj (ADR-009, ADR-010) se reutiliza sin cambios para las tres salas.
Agregar o mover una obra entre salas es un cambio acotado a los arrays ROOM_*.
Desventajas
El enum ArtworkType ahora tiene 18 valores; el dispatcher de dos switch en segments.c debe extenderse por cada obra nueva, sin importar a qué sala pertenezca.
Cambiar de sala reinicia por completo la animación de layout (resetSectorLayout()); no hay transición animada entre el estado de una sala y la siguiente.
El tamaño de cada sala queda fijo en ROOM_SIZE=6 sectores; una sala con más o menos obras requeriría tocar la geometría angular fija del reloj (ver también ADR-012 sobre el conflicto de cupo detectado con Vasarely en la sala de 30 min).
Evidencia en el proyecto

ROOM_15_MIN, ROOM_30_MIN, ROOM_60_MIN y ROOM_SIZE en segments.h/.c; setActiveRoom()/getActiveRoom()/loadRoomIntoSegments() en segments.c; el manejo de teclas 1/2/3 (y sus alias 4/5/6) en main.c; el enum ArtworkType de 18 valores en segments.h.

Relación con otros ADR

Reemplaza la restricción de seis obras fijas de ADR-005, ADR-008 y ADR-018 — esos tres ADR siguen vigentes en cuanto a la organización modular por archivo y el mecanismo de dispatcher, pero su premisa de "seis obras, un único set" queda superada por este ADR. Depende de ADR-009 (sincronización con el reloj) para el cálculo de slotMinutes por sala. Ver también ADR-014 (alias de teclado) y ADR-015 (rama por obra), ambos consecuencia directa de este cambio de alcance.
