ADR-012 – Dos técnicas de dibujo por obra: lienzo local vs. polar-nativo
Estado
Aceptado.

Contexto
El ADR-006 establece que cada obra dibuja su contenido dentro de un lienzo local cuadrado (-1..1), centrado en el punto medio radial del sector y escalado a canvasScale (el mayor cuadrado que entra en el sector, limitado por el más chico entre el ancho radial y el ancho angular disponibles).

Esa técnica funciona bien para obras "de marco": una composición discreta con fondo propio, pensada para leerse como un cuadro dentro de un cuadro (Van Gogh, Kandinsky, Mondrian, Demuth, Malevich, Klimt, Paul Klee). Pero al implementar obras de campo de color o patrón continuo sin un centro compositivo fijo (Riley, Alma Thomas, Escher, Kupka, y la primera versión de Rothko basada en "Color fields for kids"), usar el lienzo cuadrado dejaba la obra minúscula: canvasScale queda acotado por el ancho radial del sector (fijo en innerRadius=0.42/outerRadius=0.82 para todas las salas), muy por debajo del ancho angular disponible, sobre todo en el sector activo.

Problema
Decidir, para cada obra nueva, si debe dibujarse dentro del lienzo cuadrado local (con su límite de tamaño heredado de canvasScale) o directamente en las coordenadas reales del sector (innerRadius, outerRadius, halfAngle), y dejar ese criterio explícito para que no haya que redescubrirlo obra por obra.

Alternativas consideradas

Usar siempre el lienzo cuadrado local — Mantener una única técnica para todas las obras, sin excepciones. Se descartó tras el caso de Rothko: la primera versión (bandas verticales de color en un lienzo cuadrado con contra-rotación) se veía correcta en aislamiento pero "muy chica dentro del sector" en contexto — el usuario la rechazó explícitamente por ese motivo.

Aumentar canvasScale global o los radios del sector — Agrandar innerRadius/outerRadius o relajar ARTWORK_MARGIN para que el cuadrado local tenga más espacio. Se descartó: es un cambio global que afecta a las 18 obras y a la geometría del reloj (arcos, halo, overlay de sectors.c), con riesgo de romper el ajuste ya validado de las obras "de marco".

Dibujar polar-nativo (coordenadas reales del sector) para obras de campo/patrón continuo — La alternativa seleccionada: para obras sin un marco compositivo fijo, el fondo (Background) recibe innerRadius, outerRadius y halfAngle directamente (como ya hacían todos los fondos, ver ADR-007) y dibuja ahí mismo el contenido completo con drawFilledArc/drawFilledCircle, ocupando el wedge entero. La función de primer plano (drawX()) queda vacía, ya que no hay nada que dibujar en el lienzo local recortado.

Decisión
Se adoptó como criterio: si la obra tiene una composición central discreta con marco propio, usa el lienzo local (Background pinta un fondo simple con drawFilledArc; drawX() dibuja la composición en -1..1). Si la obra es un patrón, campo de color o textura continua sin un "centro" fijo, se dibuja polar-nativo dentro de Background(), y drawX() queda vacía ({}). Rothko fue reescrita bajo este segundo criterio tras el rechazo de la primera versión; Riley, Alma Thomas, Escher y Kupka ya lo usaban.

Cuando el patrón polar-nativo necesita subdividir el wedge en celdas (radiales o angulares) usando drawFilledArc por celda, se debe solapar levemente cada celda con la siguiente (ver Rothko: OVERLAP_DEG) porque el loop interno de drawFilledArc avanza el ángulo en pasos fijos de 2° y puede no alcanzar el límite exacto pedido, dejando una costura oscura visible entre celdas contiguas.

Justificación
Elegir la técnica según el contenido (marco fijo vs. patrón continuo) en vez de aplicar una regla única evita el problema de tamaño detectado en Rothko sin tener que tocar la geometría compartida del reloj (canvasScale, ARTWORK_MARGIN, radios de sector), que ya está validada por 13 obras existentes.

Documentar el criterio explícitamente (en vez de dejarlo como una decisión ad-hoc por obra) evita repetir el mismo ciclo de "se ve chica, hay que arreglarla" en cada obra nueva de campo de color — el propio usuario señaló que este ajuste "se hizo una y otra vez" antes de que quedara claro el patrón.

Consecuencias

Ventajas: Las obras de campo de color/patrón ocupan el sector completo, igual de vistosas activas o no. No requiere tocar la geometría compartida del reloj (canvasScale, radios, ARTWORK_MARGIN). El criterio de elección (marco fijo vs. patrón continuo) es explícito y reutilizable para obras futuras.

Desventajas: Dos técnicas distintas conviven en el mismo dispatcher; alguien que lea segments.c por primera vez debe saber que drawX() vacía es intencional para ciertas obras, no un olvido. El solapamiento de celdas (OVERLAP_DEG) es un parche puntual al comportamiento de drawFilledArc, no una corrección de la función compartida — cada obra polar-nativo con celdas discretas debe recordarlo por separado.

Evidencia en el proyecto
drawRiley()/drawEscher()/drawKupka()/drawRothko() vacías ({}), con toda la composición en sus respectivas funciones *Background(); comparar con drawDemuth()/drawKandinsky()/drawPaulKlee(), que sí dibujan en el lienzo local. El comentario y la constante OVERLAP_DEG en segment_rothko.c documentan el ajuste de costuras.

Relación con otros ADR
Refina el ADR-006 (sistema de lienzo local) y el ADR-007 (separación fondo/contenido), que seguían vigentes pero no contemplaban el caso de obras sin lienzo local. Relacionado con ADR-011 (sistema de salas), ya que la necesidad de que las obras "llenen" el sector se hizo más evidente al ampliar el catálogo a 18 obras con estilos muy distintos entre sí.
