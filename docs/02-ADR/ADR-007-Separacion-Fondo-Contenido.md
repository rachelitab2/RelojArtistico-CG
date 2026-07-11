ADR-007 – Separación entre fondo y contenido artístico
Estado

Aceptado.

Contexto

Cada segmento del reloj debía representar una obra artística completa sin perder la forma del sector circular donde se dibuja. Para ello fue necesario distinguir entre el fondo del segmento y los elementos principales de la composición.

Problema

Evitar que la lógica encargada de pintar el fondo y la encargada de dibujar la obra quedaran mezcladas dentro de una única función, dificultando el mantenimiento y futuras modificaciones.

Alternativas consideradas
Dibujar todo en una única función

Cada módulo artístico sería responsable de pintar simultáneamente el fondo y la composición.

Aunque funcional, esta alternativa dificultaba el mantenimiento y reducía la flexibilidad del motor de segmentos.

Fondo común para todas las obras

Todos los segmentos compartirían un mismo fondo.

Esta opción simplificaba la implementación, pero eliminaba parte de la identidad visual de cada artista.

Separación entre fondo y primer plano

La alternativa seleccionada consistió en implementar funciones independientes para el fondo y para el contenido artístico, permitiendo que el motor coordinara ambos elementos de forma uniforme.

Decisión

Cada módulo artístico implementa dos responsabilidades independientes:

Representación del fondo del segmento.
Representación del contenido principal de la obra.

El motor de segmentos ejecuta ambas etapas en orden, garantizando una composición consistente para todos los artistas.

Justificación

Esta separación facilita modificar el fondo sin alterar el contenido artístico y viceversa. Además, permitió incorporar efectos globales, como el resaltado de la obra activa, sin modificar la implementación interna de cada artista.

Consecuencias
Ventajas
Mayor modularidad.
Código más organizado.
Facilidad para mantener las composiciones.
Mayor flexibilidad para futuras mejoras visuales.
Desventajas
Incremento en el número de funciones por módulo.
Necesidad de mantener sincronizadas ambas etapas del renderizado.
Evidencia en el proyecto

Cada módulo artístico implementa funciones independientes para el dibujo del fondo y del contenido principal, mientras que segments.c coordina ambas mediante funciones específicas del motor de renderizado.

Relación con otros ADR

Complementa el ADR-005 y el ADR-006, ya que utiliza el mismo sistema modular y el mismo lienzo local para construir la representación completa de cada segmento.