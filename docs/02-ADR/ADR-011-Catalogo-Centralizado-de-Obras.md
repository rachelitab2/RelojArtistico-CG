ADR-011 - Catalogo centralizado de obras artisticas

Estado

Aceptado.

Contexto

La evolucion visual del proyecto requirio que la informacion de cada obra estuviera disponible para distintos modulos del sistema.

Antes de esta decision, datos como el nombre de la obra, autor, descripcion y colores asociados estaban repartidos o dependian indirectamente de la logica de renderizado. Esto dificultaba construir una interfaz informativa, aplicar fondos dinamicos y mantener una identidad visual coherente por obra.

Problema

El sistema necesitaba consultar informacion textual y visual de cada obra sin acoplar la interfaz, el motor de segmentos y la escena principal a las funciones internas de dibujo de cada modulo artistico.

Alternativas consideradas

Mantener los datos dentro de segments.c

Esta alternativa era rapida, pero aumentaba la responsabilidad del motor de segmentos y mezclaba datos artisticos con logica de animacion y renderizado.

Duplicar datos en ui.c y display.c

Esta opcion permitia avanzar visualmente, pero introducia inconsistencias futuras si una obra cambiaba de nombre, descripcion o paleta.

Crear un catalogo centralizado de obras

La alternativa seleccionada fue crear un modulo especifico para almacenar los metadatos y colores de cada obra.

Decision

Se incorporaron los archivos artwork_catalog.h y artwork_catalog.c.

El catalogo define una estructura ArtworkInfo que almacena:

tipo de obra;
titulo;
autor;
anio;
movimiento artistico;
descripcion;
color de fondo;
color de acento;
paleta de tres colores.

Los modulos de interfaz y escena consultan esta informacion mediante getArtworkInfo().

Justificacion

Esta decision reduce duplicacion, mejora la separacion de responsabilidades y permite que nuevas funcionalidades visuales reutilicen los mismos datos.

Tambien prepara el proyecto para futuras extensiones como imagenes de referencia, musica por obra, fichas artisticas ampliadas o configuracion externa.

Consecuencias

Ventajas

Fuente unica de informacion por obra.
Menor acoplamiento entre UI, escena y segmentos.
Facil mantenimiento de textos y colores.
Base escalable para nuevas experiencias visuales.

Desventajas

Agrega un modulo adicional.
Requiere mantener sincronizado el enum ArtworkType con el catalogo.

Evidencia en el proyecto

La decision se refleja en artwork_catalog.h y artwork_catalog.c. La interfaz utiliza los datos del catalogo para mostrar la ficha informativa, mientras que display.c utiliza los colores de fondo y acento para adaptar la escena a la obra activa.

Relacion con otros ADR

Este ADR complementa al ADR-005 (Organizacion de obras) y al ADR-008 (ArtworkType), ya que extiende la representacion de cada obra con informacion descriptiva y visual centralizada.