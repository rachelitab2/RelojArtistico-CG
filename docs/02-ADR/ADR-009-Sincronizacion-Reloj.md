ADR-009 – Sincronización del reloj con la hora del sistema
Estado

Aceptado.

Contexto

El proyecto no debía representar únicamente un reloj decorativo, sino un reloj funcional sincronizado con la hora real del sistema operativo. Esta sincronización serviría además como base para implementar comportamientos dinámicos asociados al paso del tiempo.

Problema

Obtener la hora actual del sistema de forma confiable y reutilizable, evitando duplicar la lógica temporal en diferentes módulos del proyecto.

Alternativas consideradas
Hora simulada

Consistía en incrementar manualmente contadores internos para representar el paso del tiempo.

Aunque sencilla de implementar, no garantizaba sincronización con el reloj del sistema.

Actualización manual

Permitía modificar la hora mediante entradas del usuario.

Esta opción se alejaba del objetivo principal del proyecto y complicaba innecesariamente la interacción.

Consulta directa al sistema operativo

La alternativa seleccionada consistió en obtener la hora mediante las funciones estándar de la biblioteca de C y centralizar esta responsabilidad en un único módulo.

Decisión

Se implementó una función especializada encargada de obtener la hora actual del sistema operativo y devolverla mediante una estructura reutilizable.

Esta función es utilizada tanto por el reloj analógico como por el sistema encargado de determinar la obra artística destacada.

Justificación

Centralizar el acceso a la hora evita duplicar código y garantiza que todos los componentes trabajen sobre la misma información temporal.

Además, esta solución facilita futuras ampliaciones relacionadas con el tiempo sin modificar el resto de la arquitectura.

Consecuencias
Ventajas
Sincronización real con el sistema operativo.
Eliminación de duplicación de código.
Fuente única de información temporal.
Fácil reutilización.
Desventajas
Dependencia del reloj del sistema operativo.
Cambios manuales en la hora del sistema afectan directamente la aplicación.
Evidencia en el proyecto

Esta decisión puede observarse en el módulo encargado del reloj, donde se implementa la función responsable de consultar la hora actual del sistema y proporcionar dicha información a los distintos componentes que la requieren.

Relación con otros ADR

Este ADR constituye la base funcional del ADR-010, ya que el mecanismo de resaltado automático depende directamente de la información temporal obtenida por este módulo.