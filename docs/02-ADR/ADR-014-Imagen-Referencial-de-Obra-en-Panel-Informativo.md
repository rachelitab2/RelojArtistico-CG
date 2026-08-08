ADR-014 – Imagen referencial de la obra en el panel informativo
Estado

Aceptado.

Contexto

El panel informativo (ui.c) mostraba hasta ahora un swatch de tres colores como referencia visual de la paleta de cada obra, sin ninguna imagen real de la pintura representada. Para la entrega final se buscó incorporar una miniatura real de cada obra dentro del panel, manteniendo el reloj en modo inmediato de OpenGL sin introducir un pipeline de texturizado sobre geometría curva.

Problema

Determinar cómo incorporar imágenes reales de las seis obras sin comprometer la estabilidad del proyecto ni requerir un sistema de carga de assets complejo, y sin modificar la representación artística de cada sector (que sigue dibujándose con primitivas, no con texturas).

Alternativas consideradas
Textura mapeada sobre la geometría curva del sector

La imagen real reemplazaría el fondo dibujado de cada sector (drawArtworkBackground).

Esta alternativa fue descartada por su alto riesgo: requiere coordenadas UV sobre arcos generados con drawFilledArc, y obligaría a reescribir las seis funciones de fondo ya validadas, con muy poco margen de tiempo para probarlo.

No incorporar imagen real

Mantener únicamente el swatch de paleta como referencia visual.

Esta alternativa era la más segura, pero no cumplía con el objetivo visual planteado para la entrega final.

Imagen real como textura plana dentro del panel informativo

La alternativa seleccionada consistió en cargar la imagen de la obra activa como una textura OpenGL y dibujarla como un rectángulo (quad) dentro del panel que ya existía en ui.c, sin tocar la representación de los sectores.

Decisión

Se incorporó stb_image.h, una biblioteca de un solo archivo de cabecera y dominio público, para decodificar archivos JPG/PNG en memoria. La textura resultante se sube una única vez por obra (con caché en memoria) y se dibuja como un quad con glTexCoord2f dentro del panel informativo, encima de la posición donde antes solo estaba el swatch de paleta.

Cada entrada de ArtworkInfo incorpora una ruta de archivo de imagen. Si el archivo no existe o no puede decodificarse, el panel no falla: se omite la imagen y se conserva el resto del panel (título, autor, descripción, paleta) exactamente como funcionaba antes.

Justificación

stb_image.h no es una dependencia externa en el sentido tradicional: no requiere instalación, gestor de paquetes ni enlazado dinámico; es un único archivo de cabecera que se compila junto con el proyecto, un patrón ampliamente usado en C para evitar dependencias pesadas.

Limitar el uso de la imagen real a un quad plano dentro del panel, en vez de mapearla sobre la geometría curva del sector, mantiene acotado el riesgo técnico y preserva intacta la arquitectura de dibujo por primitivas que ya estaba validada en las seis obras.

Consecuencias
Ventajas
Referencia visual real de cada obra, sin comprometer la estabilidad del proyecto.
Ninguna modificación a los módulos artísticos existentes (segment_*.c).
Degradación segura: si falta un archivo de imagen, el programa sigue funcionando.
Dependencia mínima y de bajo riesgo (un solo archivo de cabecera).
Desventajas
El proyecto pasa a depender de archivos externos (imágenes) que deben distribuirse junto al ejecutable.
Introduce el primer uso de texturas en un proyecto que hasta ahora era enteramente de geometría inmediata.
Requiere gestionar la carga y liberación de texturas para evitar fugas de memoria de video.
Evidencia en el proyecto

Esta decisión se evidencia en la carga de imágenes mediante stb_image dentro del módulo de recursos, en el campo de ruta de imagen agregado a ArtworkInfo, y en el bloque de dibujo del quad de imagen dentro de drawArtworkPanel() en ui.c.

Relación con otros ADR

Este ADR se apoya en el ADR-005 (organización de obras) y en el ADR-008 (ArtworkType), ya que reutiliza el mismo catálogo centralizado para asociar cada obra con su archivo de imagen. Es independiente del ADR-006 (lienzo local), dado que no afecta el dibujo de los sectores.
