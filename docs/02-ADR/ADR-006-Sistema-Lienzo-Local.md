ADR-006 – Uso de un sistema de lienzo local para cada obra artística
Estado

Aceptado.

Contexto

Cada una de las seis obras representadas en el reloj posee una composición gráfica diferente. Sin embargo, todas debían ocupar un espacio visual equivalente dentro de su segmento correspondiente, independientemente de su complejidad o del número de primitivas utilizadas.

Era necesario establecer un mecanismo que permitiera diseñar cada obra en un mismo sistema de coordenadas, evitando recalcular manualmente posiciones y escalas para cada segmento del reloj.

Problema

Definir un sistema de representación que permitiera reutilizar la misma lógica de dibujo para todas las obras sin depender de las dimensiones reales del segmento donde serían renderizadas.

Alternativas consideradas
Dibujar directamente sobre el segmento

Cada obra calcularía sus coordenadas utilizando el radio y el ángulo reales del segmento.

Esta alternativa incrementaba considerablemente la complejidad del código y dificultaba el mantenimiento de las composiciones.

Coordenadas independientes para cada artista

Cada módulo definiría su propio sistema de coordenadas.

Aunque permitía mayor libertad de diseño, hacía imposible mantener una escala uniforme entre las distintas obras.

Sistema de lienzo local normalizado

La alternativa seleccionada consistió en utilizar un lienzo local común para todas las obras, centrado en el origen y posteriormente transformado mediante traslación y escalado antes de ser dibujado dentro del segmento correspondiente.

Decisión

Se implementó un sistema de lienzo local normalizado, donde todas las obras se diseñan utilizando un mismo espacio de coordenadas.

Durante el proceso de renderizado, el motor de segmentos calcula la posición y escala adecuadas para el segmento actual y aplica las transformaciones necesarias antes de invocar el dibujo de la obra.

Justificación

Esta decisión permitió desarrollar las seis obras utilizando un mismo sistema de referencia, facilitando el trabajo individual de cada integrante y garantizando una apariencia visual consistente dentro del reloj.

Además, el mecanismo desacopla completamente el contenido artístico de la geometría del segmento, favoreciendo la reutilización del código y simplificando futuras modificaciones.

Consecuencias
Ventajas
Escala uniforme para todas las obras.
Independencia entre la geometría del segmento y el dibujo artístico.
Mayor facilidad para diseñar y ajustar las composiciones.
Reutilización del mismo mecanismo para cualquier obra futura.
Desventajas
Requiere aplicar transformaciones adicionales antes del dibujo.
Obliga a mantener un sistema de coordenadas coherente entre todos los módulos.
Evidencia en el proyecto

Esta decisión puede observarse en el módulo segments.c, donde antes de invocar el dibujo artístico se aplican transformaciones mediante glTranslatef() y glScalef() para posicionar el lienzo local dentro del segmento correspondiente. El tamaño del lienzo se calcula dinámicamente utilizando el margen definido por ARTWORK_MARGIN.

Relación con otros ADR

Este ADR depende del ADR-004, ya que el motor de segmentos es el responsable de posicionar el lienzo local, y complementa el ADR-005, donde se establece que cada obra se implementa de forma independiente.