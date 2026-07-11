ADR-005 – Organización de una obra artística por módulo independiente
Estado

Aceptado.

Contexto

Uno de los principales objetivos del proyecto fue representar seis obras artísticas diferentes dentro de los segmentos del reloj. Cada obra posee una composición, una paleta de colores y un conjunto de primitivas geométricas particulares, lo que implica una lógica de dibujo distinta para cada artista.

Además del aspecto técnico, el proyecto fue desarrollado de manera colaborativa utilizando Git y GitHub, donde cada integrante implementó una obra diferente en una rama independiente. Por esta razón, era necesario definir una organización que permitiera desarrollar cada representación artística de forma aislada, reduciendo los conflictos durante la integración y facilitando el mantenimiento del código.

Problema

Determinar cómo organizar la implementación de las obras artísticas para evitar que el código de dibujo se concentrara en un único archivo, lo que habría dificultado el desarrollo colaborativo, incrementado la complejidad del mantenimiento y aumentado la probabilidad de conflictos durante la integración mediante Git.

Alternativas consideradas
Todas las obras en un único archivo

Una alternativa consistía en implementar las seis obras dentro de un único módulo de dibujo.

Aunque esta opción simplificaba la cantidad de archivos del proyecto, provocaba un crecimiento excesivo del código, dificultaba su comprensión y obligaba a que varios integrantes modificaran simultáneamente el mismo archivo, incrementando los conflictos de integración.

Separar únicamente grupos de obras

Otra posibilidad era agrupar varias obras relacionadas dentro de un mismo módulo.

Esta solución reducía parcialmente el tamaño del código, pero seguía manteniendo responsabilidades compartidas y limitaba la independencia de cada representación artística.

Un módulo independiente para cada obra

La alternativa seleccionada consistió en implementar cada obra en un archivo fuente independiente, acompañado de su correspondiente archivo de cabecera.

Cada módulo es responsable exclusivamente del dibujo del fondo y del contenido artístico de una única obra, mientras que el motor de segmentos coordina cuándo y dónde debe representarse.

Decisión

Se decidió implementar cada obra artística en un módulo independiente, compuesto por un archivo fuente (segment_*.c) y un archivo de cabecera (segment_*.h).

Cada módulo expone únicamente las funciones necesarias para representar la obra y su fondo, manteniendo completamente encapsulada su implementación interna.

El motor de segmentos actúa como único punto de acceso hacia estos módulos, evitando dependencias directas entre las distintas obras.

Justificación

La separación por módulos permitió distribuir el desarrollo entre los integrantes del equipo sin interferencias significativas, ya que cada artista podía implementarse y probarse de forma independiente.

Asimismo, esta organización favorece el mantenimiento del proyecto, ya que cualquier modificación sobre una obra específica queda confinada a su propio módulo, sin afectar el funcionamiento del resto del sistema.

Desde el punto de vista de la escalabilidad, esta decisión facilita incorporar nuevas obras en el futuro siguiendo la misma estructura, reutilizando el motor de segmentos ya existente.

Consecuencias
Ventajas
Separación clara entre las distintas representaciones artísticas.
Reducción de conflictos durante el desarrollo colaborativo.
Mayor facilidad para realizar pruebas individuales de cada obra.
Código más organizado y legible.
Posibilidad de incorporar nuevas obras siguiendo la misma arquitectura.
Bajo acoplamiento entre los módulos artísticos.
Desventajas
Incremento en la cantidad de archivos del proyecto.
Necesidad de mantener sincronizadas las declaraciones entre los archivos fuente y sus respectivos encabezados.
La incorporación de una nueva obra requiere registrar el módulo correspondiente dentro del motor de segmentos.
Evidencia en el proyecto

La decisión se refleja en la estructura del proyecto mediante la existencia de módulos independientes para cada artista:

segment_hokusai.c
segment_vangogh.c
segment_kandinsky.c
segment_mondrian.c
segment_monet.c
segment_klimt.c

Cada uno posee su correspondiente archivo de cabecera y define las funciones responsables del dibujo del fondo y del contenido artístico, mientras que el módulo segments.c coordina su utilización durante el proceso de renderizado.

Relación con otros ADR

Esta decisión complementa el ADR-003 (Arquitectura modular del sistema) y depende del ADR-004 (Motor de segmentos), ya que cada módulo artístico es administrado por dicho motor. Asimismo, sirve de base para el ADR-006, donde se explica cómo todas las obras comparten un sistema de lienzo local que garantiza una representación uniforme dentro de cada segmento.