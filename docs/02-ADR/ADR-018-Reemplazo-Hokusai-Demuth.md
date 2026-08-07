ADR-018 – Reemplazo de la obra de Hokusai por Demuth
Estado

Aceptado.

Contexto

Entre las seis obras representadas, la de Hokusai (La gran ola de Kanagawa) fue identificada como la de peor lectura visual dentro del tamaño reducido de un sector: el cuerpo curvo y azul de la ola, pensado para leerse como oleaje, se confundía con la silueta de un animal marino en vez de una ola estilizada.

Al mismo tiempo, el catálogo de obras se planteó ampliar en el futuro con nuevas incorporaciones, entre ellas Charles Demuth (I Saw the Figure 5 in Gold), ya con material de referencia disponible.

Problema

Decidir si conservar la obra de Hokusai con sus limitaciones visuales conocidas, o reemplazarla por completo por una obra nueva, considerando que el proyecto mantiene la restricción de exactamente seis obras y que cada obra requiere una implementación completa en primitivas OpenGL, no solo una imagen de referencia.

Alternativas consideradas
Mantener Hokusai y solo ajustar colores o proporciones

Se descartó: el problema no era de color sino de lectura de forma (la silueta general se percibe como otra cosa), un ajuste de paleta no lo resuelve.

Agregar Demuth como séptima obra

Rompía la restricción explícita de mantener el catálogo en seis obras (ver ADR-005), y hubiera exigido rediseñar la disposición angular de los sectores.

Reemplazar Hokusai por Demuth manteniendo el mismo slot

La alternativa seleccionada: se elimina por completo el módulo de Hokusai (segment_hokusai.c/.h) y se implementa segment_demuth.c/.h en su lugar, ocupando el mismo valor del enum ArtworkType (renombrado de ART_HOKUSAI a ART_DEMUTH) y la misma posición angular en initSegments().

Decisión

Se removieron segment_hokusai.c e include/segment_hokusai.h del proyecto (código y de CMakeLists.txt). Se creó segment_demuth.c/.h siguiendo el mismo contrato que las demás obras: drawDemuthBackground(innerRadius, outerRadius, halfAngle) para el fondo con la geometría real del sector, y drawDemuth() para el primer plano en el lienzo local.

La composición de Demuth se resolvió con primitivas simples: fondo en bandas radiales cálidas (rojo oscuro a naranja, mismo criterio que segment_monet.c, sin rejilla de columnas), y en primer plano tres numerales "5" construidos con rectángulos al estilo de un display de siete segmentos, superpuestos a escala decreciente para sugerir la perspectiva/profundidad del cuadro original.

El catálogo (artwork_catalog.c) reemplazó la entrada de Hokusai por la de Demuth en la misma posición del arreglo, con su propia paleta, descripción y ruta de imagen real (assets/images/demuth.jpg).

Justificación

Reemplazar en el mismo slot evita tocar la lógica de distribución angular, el sistema de layout de galería (segments.c) o cualquier otro módulo que dependa del número de obras — el cambio queda acotado a: el enum, el dispatcher de dos switch, un archivo nuevo de obra, y una entrada de catálogo.

Usar el mismo patrón de fondo por bandas radiales que ya se validó en Monet reduce el riesgo de introducir una tecnica nueva sin probar a horas de la entrega.

Consecuencias
Ventajas
Elimina la obra con peor lectura visual del conjunto.
Cambio acotado: no afecta el motor de segmentos, el layout de galería, el catalogo salvo una entrada, ni las otras 5 obras.
Mantiene la restriccion de seis obras.
Desventajas
Demuth es una obra nueva sin el mismo nivel de iteracion visual que las otras cinco (Hokusai tuvo un rediseño completo en una sesion anterior; Demuth se implemento en una sola pasada).
Se pierde la referencia a Hokusai/Ukiyo-e en el catalogo, que cubria un movimiento artistico distinto (grabado japones) al resto de la seleccion.
Evidencia en el proyecto

Esta decision se evidencia en segment_demuth.c/.h (nuevos), la ausencia de segment_hokusai.c/.h, el enum ArtworkType en segments.h (ART_DEMUTH en lugar de ART_HOKUSAI), y la entrada correspondiente en artwork_catalog.c.

Relación con otros ADR

Este ADR depende del ADR-005 (organización de obras, seis obras fijas) y del ADR-008 (despacho por ArtworkType), ya que el reemplazo se apoya en que agregar o quitar una obra es, por diseño, un cambio acotado a esos dos puntos. Bridget Riley (la otra obra nueva evaluada junto con Demuth) queda pendiente para una futura Fase 3, sin reemplazar ninguna obra actual todavia (ver assets/futuras/).
