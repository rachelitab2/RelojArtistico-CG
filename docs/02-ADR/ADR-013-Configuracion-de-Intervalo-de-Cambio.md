ADR-013 - Configuracion del intervalo de cambio de obra

Estado

Aceptado.

Contexto

La primera version del sistema seleccionaba la obra activa mediante una relacion fija con la hora del sistema. Esto permitia vincular el reloj con las obras, pero no ofrecia control sobre la frecuencia de cambio ni permitia experimentar con distintos ritmos visuales.

Durante la evolucion del proyecto se propuso incorporar una configuracion para cambiar la obra activa cada 15 minutos, 30 minutos o 1 hora.

Problema

El proyecto necesitaba permitir distintos intervalos de cambio sin mezclar la configuracion con la logica de dibujo ni con los datos de las obras.

Alternativas consideradas

Mantener el cambio fijo por hora

Era la opcion mas simple, pero limitaba la demostracion y dejaba sin efecto el selector visual de intervalo.

Guardar la configuracion directamente en segments.c

Esta opcion reducia archivos, pero aumentaba el acoplamiento entre configuracion, tiempo y renderizado.

Crear un modulo de configuracion

La alternativa seleccionada fue crear app_config.h y app_config.c para encapsular el intervalo activo.

Decision

Se incorporo un modulo app_config que define los intervalos disponibles y permite consultar o modificar la opcion activa.

La interfaz muestra las opciones 15 min, 30 min y 1 hr. El teclado permite cambiar la configuracion mediante las teclas 1, 2 y 3.

El motor de segmentos consulta esta configuracion para calcular la obra activa segun el intervalo seleccionado.

Justificacion

Esta decision separa la configuracion de la logica grafica y permite extender el sistema posteriormente con mas opciones, almacenamiento persistente o controles visuales mediante mouse.

Consecuencias

Ventajas

Configuracion centralizada.
Selector visible durante la ejecucion.
Mayor control para demostraciones.
Preparacion para futuras interacciones.

Desventajas

Agrega estado global controlado.
Requiere mantener coherencia entre UI, teclado y motor de segmentos.

Evidencia en el proyecto

La decision se refleja en app_config.h, app_config.c, main.c, ui.c y segments.c. La UI muestra el intervalo activo, main.c actualiza la configuracion desde teclado y segments.c utiliza esa configuracion para seleccionar la obra activa.

Relacion con otros ADR

Este ADR complementa al ADR-009 (Sincronizacion con la hora del sistema), ya que mantiene la dependencia temporal, pero permite modificar la granularidad del cambio de obra.