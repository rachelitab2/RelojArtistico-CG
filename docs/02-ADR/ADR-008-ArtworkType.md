ADR-008 – Despacho de obras mediante ArtworkType
Estado

Aceptado.

Contexto

El proyecto implementa seis obras artísticas diferentes, pero el motor de segmentos debía ser capaz de renderizar cualquiera de ellas utilizando un único flujo de dibujo.

Era necesario establecer un mecanismo que permitiera seleccionar automáticamente la obra correspondiente a cada segmento sin acoplar el motor a un módulo artístico específico.

Problema

Determinar cómo asociar cada segmento con su obra artística evitando múltiples estructuras condicionales dispersas por el código y manteniendo una arquitectura extensible.

Alternativas consideradas
Un módulo específico para cada segmento

Cada segmento invocaría directamente la obra correspondiente.

Esta alternativa generaba una fuerte dependencia entre el motor y cada implementación artística.

Uso de punteros a funciones

Se evaluó almacenar punteros a funciones dentro de cada segmento.

Aunque esta solución ofrecía flexibilidad, incrementaba la complejidad del proyecto y dificultaba su comprensión dentro del contexto académico.

Enumeración (ArtworkType) y dispatcher central

La alternativa seleccionada consistió en definir una enumeración que identifica cada obra y utilizar un dispatcher central encargado de invocar la implementación correspondiente.

Decisión

Se implementó un tipo enumerado denominado ArtworkType, asociado a cada segmento durante su inicialización.

El motor de segmentos utiliza dicho valor para seleccionar la función de dibujo correspondiente mediante una estructura switch, centralizando toda la lógica de despacho en un único punto del sistema.

Justificación

Esta solución mantiene desacoplado el motor respecto a las implementaciones individuales de cada artista y facilita incorporar nuevas obras sin modificar el flujo general del renderizado.

Además, la utilización de una enumeración mejora la legibilidad del código y reduce la posibilidad de errores al identificar cada representación artística.

Consecuencias
Ventajas
Bajo acoplamiento entre el motor y las obras.
Código más legible.
Fácil incorporación de nuevos artistas.
Punto único de mantenimiento.
Desventajas
Cada nueva obra requiere agregar un nuevo valor al enum y extender el switch.
El dispatcher debe mantenerse actualizado conforme evolucione el proyecto.
Evidencia en el proyecto

La decisión se evidencia mediante la definición del tipo ArtworkType, la asignación de dicho valor durante la inicialización de los segmentos y el uso de funciones de despacho encargadas de seleccionar automáticamente la representación gráfica correspondiente.

Relación con otros ADR

Este ADR complementa el ADR-004, ya que el motor de segmentos utiliza el dispatcher para coordinar el dibujo de todas las obras, y se apoya en la organización modular descrita en el ADR-005.