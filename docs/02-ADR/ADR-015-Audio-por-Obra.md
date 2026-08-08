ADR-015 – Audio por obra con silencio seguro
Estado

Aceptado.

Contexto

Para reforzar la experiencia de "galería" del reloj artístico se buscó incorporar una pista de audio de fondo que cambiara junto con la obra activa, con la posibilidad de silenciarla desde el teclado. El proyecto corre exclusivamente sobre Windows, por lo que se evaluó una solución de audio sin introducir dependencias externas de terceros.

Problema

Reproducir un audio distinto por cada obra activa, sin bloquear el ciclo de renderizado, sin requerir una librería de audio de terceros, y sin que la ausencia de un archivo de audio provoque un fallo o cierre inesperado del programa.

Alternativas consideradas
Librería de audio de terceros (OpenAL, SDL_mixer)

Ofrece control fino sobre mezcla y múltiples canales simultáneos.

Se descartó por el tiempo disponible: exige integrar y enlazar una biblioteca nueva completa, con su propia gestión de contexto, para un requerimiento que solo necesita una pista de fondo a la vez.

No incorporar audio

Alternativa más segura, pero no cumplía el objetivo planteado para la entrega final.

PlaySound de la API de Windows (winmm)

La alternativa seleccionada consistió en usar PlaySound en modo asíncrono y en bucle, reproduciendo un archivo .wav por obra. Al cambiar la obra activa, se reemplaza la pista en reproducción.

Decisión

Se agregó un módulo audio.h/audio.c con responsabilidad única: reproducir, detener y silenciar la pista de audio asociada a la obra activa. Cada entrada de ArtworkInfo incorpora un campo musicPath con la ruta al archivo .wav correspondiente.

El módulo se apoya en PlaySound (winmm.lib) con las banderas SND_ASYNC | SND_LOOP | SND_FILENAME, de modo que la reproducción no bloquea el ciclo de dibujo. Se usa exclusivamente formato .wav, evitando la necesidad de códecs adicionales que .mp3 requeriría.

La tecla m alterna el estado de silencio. El estado de mute se mantiene aunque cambie la obra activa: si el usuario silenció el audio, la siguiente obra no debe sonar sola.

Si musicPath es nulo, está vacío, o el archivo no existe, el módulo de audio no reproduce nada y no genera ningún error visible ni interrumpe la ejecución; el resto del programa continúa funcionando exactamente igual que sin audio.

Justificación

winmm es parte del sistema operativo Windows y ya está disponible en el toolchain de compilación (MinGW la enlaza sin instalar nada adicional), coherente con la decisión de mantener el proyecto sin dependencias de terceros.

Encapsular el audio en su propio módulo, con una función de reproducción por obra y un único punto de mute, mantiene la misma filosofía de responsabilidad única y bajo acoplamiento que ya se aplicó en artwork_catalog.c y ui.c.

La degradación segura ante archivos faltantes es una decisión deliberada: la entrega final no debe depender de que todos los archivos de audio estén presentes y correctos en el momento de la demostración.

Consecuencias
Ventajas
Refuerza la identidad de cada obra sin bloquear el renderizado.
No agrega dependencias de terceros, solo una API nativa de Windows.
Silencio seguro ante archivos faltantes o corruptos.
Módulo aislado, fácil de desactivar o reemplazar en el futuro.
Desventajas
Solución específica de Windows; el proyecto pierde portabilidad a otros sistemas operativos si en algún momento se planteara esa meta.
PlaySound solo permite una pista sonando a la vez, sin mezcla de canales.
El proyecto pasa a depender de seis archivos de audio externos que deben distribuirse junto al ejecutable.
Evidencia en el proyecto

Esta decisión se evidencia en el nuevo módulo audio.c/audio.h, en el campo musicPath agregado a ArtworkInfo, en el manejo de la tecla m dentro de main.c, y en la llamada de reproducción dentro del ciclo de actualización cuando cambia la obra activa.

Relación con otros ADR

Este ADR se apoya en el ADR-005 (organización de obras) para asociar cada pista con su obra mediante el catálogo centralizado, y complementa al ADR-010 (resaltado dinámico), ya que el cambio de obra activa dispara tanto el efecto visual como ahora también el cambio de audio.
