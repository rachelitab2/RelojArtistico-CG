ADR-010 – Resaltado dinámico de la obra artística según la hora
Estado

Aceptado.

Contexto

Uno de los objetivos funcionales del proyecto consistía en establecer una relación entre el reloj y las obras artísticas representadas, de modo que el sistema no solo indicara la hora, sino que también destacara automáticamente una de las seis composiciones según el momento del día.

Era necesario implementar este comportamiento sin alterar la lógica interna de cada obra y manteniendo visibles todos los segmentos durante la ejecución.

Problema

Diseñar un mecanismo que permitiera identificar dinámicamente la obra correspondiente a la hora actual y aplicar efectos visuales diferenciadores sin modificar las implementaciones individuales de cada artista.

Alternativas consideradas
Destacar manualmente una obra fija

La obra resaltada permanecería constante durante toda la ejecución.

Esta alternativa eliminaba la relación entre el reloj y el contenido artístico.

Delegar el resaltado a cada módulo artístico

Cada obra decidiría cuándo debía resaltarse.

Esta solución incrementaba el acoplamiento y duplicaba lógica relacionada con la hora del sistema.

Resaltado administrado por el motor de segmentos

La alternativa seleccionada consistió en que el motor de segmentos determinara cuál obra debía resaltarse utilizando la hora actual y aplicara externamente los efectos visuales correspondientes.

Decisión

Se implementó un mecanismo centralizado que calcula la obra activa utilizando la hora del sistema y aplica efectos visuales exclusivamente al segmento correspondiente.

Entre los efectos implementados se incluyen:

incremento de escala mediante transformaciones geométricas;
borde dorado;
superposición luminosa (overlay);
etiqueta central con la hora y el nombre del artista activo.

Las seis obras permanecen visibles en todo momento; únicamente cambia el tratamiento visual del segmento activo.

Justificación

Esta solución preserva completamente la independencia de los módulos artísticos, ya que ninguno necesita conocer el estado del reloj.

Además, mantiene una separación clara entre la lógica temporal y la representación gráfica, permitiendo incorporar nuevos efectos sin modificar las implementaciones existentes.

Consecuencias
Ventajas
Integración directa entre reloj y contenido artístico.
Mayor dinamismo visual.
Bajo acoplamiento.
Fácil incorporación de nuevos efectos.
Conservación del comportamiento original de las obras.
Desventajas
El motor de segmentos concentra la lógica del resaltado.
Incrementa ligeramente la complejidad del proceso de renderizado.
Evidencia en el proyecto

Esta decisión puede observarse en el motor de segmentos, donde se calcula la obra activa utilizando la hora obtenida del sistema y se aplican de forma centralizada las transformaciones y efectos visuales asociados al segmento destacado, incluyendo la representación de la etiqueta informativa en el centro del reloj.

Relación con otros ADR

Este ADR depende directamente del ADR-004 (Motor de segmentos) y del ADR-009 (Sincronización con la hora del sistema), ya que combina ambas decisiones para implementar una de las funcionalidades distintivas del proyecto.