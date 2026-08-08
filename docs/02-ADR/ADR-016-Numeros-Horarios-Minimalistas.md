ADR-016 – Números horarios minimalistas en la carátula
Estado

Aceptado.

Contexto

La carátula del reloj (clock.c) utilizaba únicamente doce marcas de línea para señalar las horas, sin ningún número visible. Durante el pulido visual de la semana se suavizó el contraste y grosor de esas marcas y de las manecillas. Para la entrega final se buscó agregar referencias numéricas que faciliten la lectura de la hora, sin recargar visualmente una carátula que ya había sido deliberadamente simplificada.

Problema

Incorporar números horarios legibles sin saturar la carátula, sin competir visualmente con el panel informativo y el selector de intervalo (ya ubicados en las esquinas superiores), y sin requerir texturas de fuente ni assets adicionales.

Alternativas consideradas
Doce números completos

Colocar un número en cada una de las doce posiciones horarias.

Se descartó porque reintroducía la densidad visual que el pulido de la semana había reducido deliberadamente (ver ADR de suavizado de manecillas y marcas), y competía con el resto de la composición ya ajustada.

Sin números, solo marcas

Mantener el estado actual sin agregar numerales.

Alternativa más simple, pero no resolvía la observación de legibilidad planteada para la entrega final.

Cuatro números cardinales (12, 3, 6, 9)

La alternativa seleccionada consistió en agregar únicamente los cuatro números correspondientes a las marcas mayores ya existentes (12, 3, 6 y 9), en la misma posición donde hoy se dibujan esas marcas más gruesas.

Decisión

Se dibujan los números 12, 3, 6 y 9 con glutBitmapCharacter, reutilizando el mismo criterio de conversión de píxeles a unidades del mundo que ya usan segments.c y ui.c para centrar texto. Los números se posicionan inmediatamente por dentro de las marcas mayores existentes, con el mismo tono de gris suave aplicado a esas marcas durante el pulido visual, de modo que no introducen un nuevo nivel de contraste en la carátula.

Las ocho marcas menores restantes permanecen como líneas simples, sin numeral.

Justificación

Limitar los números a las cuatro posiciones cardinales resuelve el objetivo de legibilidad (ubicar rápidamente arriba, abajo, y los costados) sin repetir la saturación visual que ya se había corregido. Es además la opción de menor riesgo de solapamiento con el panel informativo y el selector, que ocupan las esquinas y no el eje central de la carátula.

Reutilizar glutBitmapCharacter mantiene coherencia técnica con el resto del proyecto, que ya depende de las fuentes de mapa de bits de FreeGLUT para todo el texto (panel, selector, etiqueta central), sin introducir una fuente ni un sistema de renderizado de texto nuevo.

Consecuencias
Ventajas
Mejora la legibilidad de la hora sin recargar la composición.
Reutiliza infraestructura de texto ya existente en el proyecto.
Bajo riesgo de colisión visual con otros elementos de la interfaz.
Cambio acotado a un único archivo (clock.c).
Desventajas
Una carátula con doce números completos sería más convencional y podría considerarse más legible por algunos usuarios.
Si en el futuro se agregan más elementos cerca del centro del reloj, estos cuatro números son un punto adicional a revisar por posibles solapamientos.
Evidencia en el proyecto

Esta decisión se evidencia en clock.c, en el bloque de dibujo de marcas horarias, donde las cuatro posiciones mayores (i % 3 == 0) incorporan además el numeral correspondiente junto a la línea de marca ya existente.

Relación con otros ADR

Este ADR construye sobre el trabajo de suavizado visual de manecillas y marcas realizado durante el pulido de continuidad entre sectores, y es independiente del ADR-010 (resaltado dinámico), ya que no interactúa con el estado de la obra activa.
