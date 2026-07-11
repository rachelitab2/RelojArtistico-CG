ADR-004 – Motor de Segmentos como núcleo del renderizado
Estado

Aceptado.

Contexto

El objetivo principal del proyecto consiste en representar un reloj artístico formado por seis segmentos rotativos, donde cada uno contiene una interpretación gráfica inspirada en un artista diferente. Todos los segmentos deben mantenerse visibles de manera simultánea, rotar como una única estructura y conservar un comportamiento uniforme durante la ejecución.

Para lograr este comportamiento era necesario definir un componente encargado de administrar la geometría, la rotación, el dibujo y la actualización de todos los segmentos sin duplicar código ni distribuir estas responsabilidades entre los módulos individuales de cada obra. La solución debía además facilitar la incorporación de nuevas funcionalidades, como el resaltado dinámico de la obra correspondiente a la hora actual, sin modificar la implementación de cada artista.

Problema

Diseñar un mecanismo que permitiera controlar de forma centralizada el comportamiento de los seis segmentos del reloj, evitando que cada obra administrara su propia transformación, animación o estado, lo que habría incrementado el acoplamiento y la complejidad del sistema.

Alternativas consideradas
Cada obra controla su propia transformación

En esta alternativa cada módulo artístico sería responsable de calcular su posición, rotación y estado.

Aunque resulta sencilla para una escena pequeña, genera duplicación de lógica, dificulta mantener sincronizadas las seis obras y complica la incorporación de nuevas funcionalidades compartidas.

Dibujar cada segmento manualmente desde display.c

Otra posibilidad consistía en que el módulo encargado del renderizado general invocara directamente el dibujo de cada obra y aplicara individualmente todas las transformaciones.

Esta opción concentraba demasiadas responsabilidades en un único módulo y reducía la reutilización del código.

Motor centralizado de segmentos

La alternativa seleccionada consistió en implementar un módulo especializado (segments.c) encargado de administrar el conjunto completo de segmentos mediante una estructura de datos común, aplicando las transformaciones necesarias y delegando únicamente el dibujo artístico a los módulos correspondientes.

Decisión

Se implementó un motor centralizado de segmentos, cuya responsabilidad es administrar el ciclo completo de renderizado de cada sector del reloj.

Este módulo mantiene un arreglo con los seis segmentos, controla la rotación global de la rueda, determina la obra activa según la hora del sistema y coordina el dibujo del fondo, el borde y la representación artística correspondiente.

Cada módulo artístico permanece completamente desacoplado del comportamiento global del reloj, limitándose únicamente a representar su contenido gráfico cuando el motor lo solicita.

Justificación

La centralización del comportamiento en un único motor reduce significativamente la duplicación de código y facilita el mantenimiento del sistema.

Gracias a esta arquitectura fue posible incorporar funcionalidades globales, como la rotación continua de la rueda, el resaltado automático de la obra correspondiente a la hora actual y la visualización de información en el centro del reloj, sin modificar la implementación individual de las obras.

Asimismo, el motor actúa como punto de coordinación entre el reloj, el sistema de tiempo y los módulos artísticos, manteniendo una clara separación entre la lógica de control y la representación gráfica.

Consecuencias
Ventajas
Centralización del comportamiento de todos los segmentos.
Eliminación de lógica duplicada.
Mayor facilidad para incorporar nuevas funcionalidades globales.
Bajo acoplamiento entre el motor y los módulos artísticos.
Mejor mantenibilidad del sistema.
Coordinación uniforme del proceso de renderizado.
Desventajas
El módulo segments.c concentra una parte importante de la lógica del proyecto.
Nuevas funcionalidades relacionadas con los segmentos generalmente deberán incorporarse en este módulo.
Requiere una correcta organización interna para evitar que el motor crezca excesivamente con el tiempo.
Evidencia en el proyecto

La decisión puede observarse en varios componentes del sistema:

Definición del arreglo global que almacena los seis segmentos.
Inicialización centralizada mediante initSegments().
Renderizado uniforme mediante drawSegments() y drawSegment().
Actualización de la rotación mediante updateSegments().
Coordinación del dibujo artístico a través de drawArtwork() y drawArtworkBackground().
Administración del estado de la obra destacada utilizando la hora del sistema.
Relación con otros ADR

Esta decisión se apoya en la arquitectura modular descrita en el ADR-003 y constituye la base para los ADR-005, ADR-006, ADR-007 y ADR-008, los cuales describen cómo el motor delega el dibujo de cada obra, utiliza un lienzo local para cada segmento y coordina el renderizado mediante el tipo ArtworkType.