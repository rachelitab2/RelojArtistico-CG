ADR-013 – Mitigación del crash del driver gráfico al cambiar de sala
Estado

Aceptado (mitigado, causa raíz no resuelta — ver Consecuencias).

Contexto

Al ampliar el proyecto al sistema de tres salas (ADR-011), se detectó un cierre abrupto reproducible de la aplicación al cambiar de sala (tecla 1/2/3), con código de excepción 0xc0000005 en el Visor de eventos de Windows. Un diagnóstico detallado (ver docs/09-Diagnostico-Pantallazo-VIDEO-SCHEDULER.md) usando adjunte en vivo de gdb al proceso confirmó que el crash ocurre enteramente dentro de nvoglv64.dll (el driver de NVIDIA), en DrvPresentBuffers, sin ningún cuadro de pila perteneciente al código de la aplicación.

Problema

Decidir qué hacer ante un crash reproducible cuya causa raíz está confirmada como interna al driver gráfico, no al código propio, sabiendo que no existe una corrección de código que garantice eliminarlo, pero que sí hay cambios razonables que reducen la probabilidad de dispararlo.

Alternativas consideradas
No hacer nada (esperar a que el usuario actualice el driver)

Documentar el hallazgo y no tocar el código, ya que la causa está fuera de la aplicación.

Se descartó como única acción: aunque la causa raíz es externa, el patrón de uso de la aplicación (mucha geometría nueva de golpe al cambiar de sala, doble solicitud de repintado) coincide con el tipo de carga que expone bugs de este tipo en el pipeline de present del driver, así que reducir esa carga es una mitigación razonable aunque no sea una corrección definitiva.

Rehacer el pipeline de renderizado (VBOs, geometría cacheada)

Migrar de OpenGL 1.x en modo inmediato a buffers de vértices para reducir drásticamente las llamadas de dibujo por frame.

Se descartó por alcance: es una reescritura mayor del motor gráfico completo, desproporcionada para un síntoma que ya se identificó como un bug de driver, no de rendimiento de la aplicación.

Mitigaciones puntuales de bajo riesgo

La alternativa seleccionada: aplicar cambios acotados y de bajo riesgo que reducen la carga de presentación en el momento del cambio de sala, sin rehacer el pipeline de renderizado.

Decisión

Se aplicaron tres mitigaciones independientes:

1. Reducción de draw calls en las obras más densas (segment_seurat.c, segment_escher.c): reemplazo de drawFilledCircle() (64 segmentos, pensado para círculos grandes) por un drawDot() local de 8 segmentos para los miles de puntos pequeños de esas obras, y ajuste de los pasos de muestreo (RADIAL_STEP, ANGULAR_STEP en Seurat) para reducir la cantidad total de primitivas por frame.

2. Eliminación de glutPostRedisplay() duplicado en keyboard() (main.c): el timer de 33ms ya solicita un repintado por ciclo; pedir uno adicional justo en el instante de cambio de sala reproducía el mismo patrón de doble-redisplay ya documentado como causa de un bugcheck previo del driver (ver docs/09).

3. Activación explícita de VSync vía wglSwapIntervalEXT (activateVsync() en main.c, llamada tras glutCreateWindow()): sincroniza cada present con el refresco real del monitor, evitando que se encolen presents más rápido de lo que el driver los puede procesar.

Justificación

Las tres mitigaciones atacan la misma hipótesis (sobrecarga de la cola de presentación del driver en el instante de cambio de sala) desde ángulos distintos, sin requerir rehacer el pipeline de renderizado ni introducir dependencias nuevas.

Se optó por aplicarlas y aceptar que el crash pueda seguir ocurriendo ocasionalmente, en vez de bloquear el desarrollo de las 18 obras a la espera de una corrección de código que, según la evidencia de gdb, no existe del lado de la aplicación.

Consecuencias
Ventajas
Las tres mitigaciones son mejoras válidas de rendimiento e higiene de código independientemente de si resuelven el crash (menos draw calls innecesarios, sin llamadas de repintado redundantes, VSync explícito en vez de implícito).
El comportamiento del crash quedó documentado con evidencia reproducible (docs/09), evitando que una futura sesión repita la misma investigación desde cero.
Desventajas
El crash sigue siendo reproducible de forma no determinística tras las tres mitigaciones (confirmado con pruebas automatizadas de cambio de sala repetido, que no lo reprodujeron de forma consistente, pero el usuario sí lo vio en uso interactivo real).
La causa raíz solo puede resolverse actualizando el driver de NVIDIA del equipo, algo fuera del control del código del proyecto.
Se agregaron alias de teclado 4/5/6 (ver ADR-014) específicamente para descartar que el crash estuviera atado a las teclas 1/2/3, lo que confirmó que el disparador es el cambio de sala en sí, no la tecla usada.
Evidencia en el proyecto

docs/09-Diagnostico-Pantallazo-VIDEO-SCHEDULER.md (diagnóstico completo con backtraces de gdb); drawDot() en segment_seurat.c y segment_escher.c; ausencia de glutPostRedisplay() en keyboard() (main.c, con comentario explicando el motivo); activateVsync() en main.c.

Relación con otros ADR

Consecuencia directa de ADR-011 (sistema de salas), que introdujo el patrón de uso (cambio de sala con recarga completa de 6 obras) donde el crash se hizo evidente. Relacionado con ADR-014 (alias de teclado), usado como herramienta de diagnóstico para este mismo problema.
