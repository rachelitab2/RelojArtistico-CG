ADR-017 – Pantalla de carga breve para la demostración
Estado

Aceptado.

Contexto

Para la entrega final se evaluó una experiencia de varias pantallas antes de llegar al reloj (carga, introducción con explicación de las salas, y finalmente el reloj), inspirada en una referencia visual de tipo galería. El proyecto actual arranca directamente sobre la vista del reloj, sin ninguna pantalla previa.

Problema

Decidir cuánta experiencia de pantallas múltiples es razonable incorporar con el tiempo disponible antes de la entrega, sin convertir el arranque del programa en una máquina de estados compleja que no se pueda probar ni pulir a tiempo.

Alternativas consideradas
Secuencia completa de carga, introducción y selección de sala

Replicar la referencia visual completa: pantalla de carga con barra de progreso, pantalla de introducción con explicación de cada modalidad de cambio de obra y botón para continuar, y luego el reloj.

Se descartó para esta entrega. Requiere una máquina de estados nueva (carga → introducción → reloj), contenido explicativo adicional, y no aporta valor funcional sobre el reloj ya construido; el riesgo de introducir un bug en el flujo de arranque justo antes de la demostración no se justifica frente al beneficio.

Sin pantalla previa

Mantener el arranque directo sobre el reloj, como hasta ahora.

Alternativa más segura, pero no capitalizaba la oportunidad de dar una breve presentación de marca ("Reloj Artístico") al iniciar la demostración.

Pantalla de carga única, con transición automática

La alternativa seleccionada consistió en mostrar una única pantalla breve (nombre del proyecto y una barra de progreso simulada) durante un intervalo corto y fijo al iniciar el programa, transicionando automáticamente al reloj sin requerir interacción del usuario.

Decisión

Se incorpora un estado simple de pantalla (enum AppScreen con los valores SCREEN_LOADING y SCREEN_CLOCK) gestionado en main.c/display.c. Al iniciar, se muestra el nombre del proyecto y una barra de progreso que avanza con el tiempo transcurrido; tras un intervalo fijo (aproximadamente dos segundos) el estado cambia a SCREEN_CLOCK y el programa continúa exactamente como funciona hoy.

No se incorpora pantalla de introducción, selección de sala, ni controles de velocidad de demostración: esas piezas quedan documentadas como parte de la Fase 3 (ver roadmap), condicionadas a la evolución del proyecto a múltiples salas de obras.

Justificación

Una pantalla de carga única y con transición automática (sin depender de un clic del usuario) es de bajo riesgo: no introduce nuevas interacciones de teclado ni mouse, no depende de contenido adicional, y es fácil de remover si llegara a fallar durante la demostración.

Limitar el alcance a esto, en vez de la secuencia completa de la referencia visual, es coherente con la decisión general de esta entrega: priorizar piezas pequeñas y reversibles sobre una reestructuración grande del flujo de arranque.

Consecuencias
Ventajas
Aporta una breve presentación de marca sin arriesgar el flujo principal del programa.
Cambio acotado y reversible: quitar la pantalla de carga no afecta el resto del sistema.
No agrega nuevas dependencias ni interacciones de usuario.
Desventajas
No cubre la experiencia completa de introducción y selección de sala planteada en la referencia visual original.
Introduce el primer estado de pantalla del proyecto, que deberá extenderse con cuidado si en el futuro se agregan más pantallas.
Evidencia en el proyecto

Esta decisión se evidencia en el enum AppScreen y su manejo dentro de main.c y display.c, donde display() verifica el estado actual antes de decidir si dibuja la pantalla de carga o el reloj completo.

Relación con otros ADR

Este ADR queda relacionado con el roadmap de Fase 3 (múltiples salas de obras), donde una máquina de estados de pantallas más completa sí se justificaría. Es independiente del resto de los ADR de esta entrega (imagen referencial, audio, números horarios), ya que no depende de ellos ni ellos dependen de este.
