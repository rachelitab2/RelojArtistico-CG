ADR-012 - Evolucion visual tipo galeria artistica

Estado

Aceptado.

Contexto

El proyecto evoluciono desde un reloj analogico con seis segmentos artisticos hacia una experiencia visual mas cercana a una galeria interactiva.

La version anterior mantenia la rueda en rotacion continua y destacaba una obra mediante efectos simples. Sin embargo, este comportamiento no comunicaba con suficiente claridad la relacion entre obra activa, informacion artistica y composicion visual.

Problema

Era necesario mejorar la experiencia visual sin reescribir las obras existentes ni introducir tecnologias nuevas como texturas, audio o renderizado 3D.

El desafio consistia en lograr que la obra activa tuviera mayor protagonismo, que las obras secundarias pasaran a segundo plano y que la escena se percibiera como una composicion artistica organizada.

Alternativas consideradas

Mantener la rotacion continua

La rotacion permanente generaba movimiento, pero reducia la lectura visual de las obras y hacia que la composicion pareciera decorativa.

Migrar a una version 3D

La migracion a 3D permitiria mayor profundidad visual, pero aumentaba demasiado la complejidad para el alcance inmediato del proyecto.

Implementar una galeria 2D con acordeon visual

La alternativa seleccionada fue mantener el renderizado 2D con OpenGL inmediato, pero reorganizar el comportamiento visual mediante una obra activa ampliada, centrada y acompanada por atenuacion de las obras secundarias.

Decision

Se transformo el comportamiento del motor de segmentos para que la rueda deje de girar continuamente.

La obra activa se centra en una posicion protagonista, aumenta su ancho angular y mantiene un tratamiento visual destacado. Las obras secundarias se oscurecen de acuerdo con su distancia respecto a la obra activa.

La escena conserva el reloj analogico central, pero ahora la composicion visual prioriza la obra correspondiente al intervalo activo.

Justificacion

Esta solucion mejora el impacto visual sin abandonar la arquitectura existente.

El cambio mantiene las seis obras en sus modulos individuales, evita una reescritura completa y demuestra conceptos importantes de computacion grafica como transformaciones, interpolacion, composicion jerarquica, transparencia y animacion.

Consecuencias

Ventajas

Mayor claridad visual.
Mejor jerarquia entre obra activa y obras secundarias.
Experiencia mas cercana a una galeria artistica.
Menor riesgo que una migracion a 3D.
Base preparada para transiciones, musica e imagenes de referencia.

Desventajas

El motor de segmentos aumenta su responsabilidad visual.
Las obras deben soportar sectores de ancho variable.
Requiere parametrizar los fondos para evitar valores angulares fijos.

Evidencia en el proyecto

La decision se refleja en segments.c, donde se administra el layout dinamico de sectores, el crecimiento de la obra activa, la atenuacion de obras secundarias y la orientacion visual de la rueda.

Relacion con otros ADR

Este ADR evoluciona al ADR-004 (Motor de segmentos), ADR-006 (Sistema de lienzo local) y ADR-010 (Resaltado dinamico), ampliando el resaltado simple hacia una composicion tipo galeria.