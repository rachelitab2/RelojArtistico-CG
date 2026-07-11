ADR-003 – Arquitectura modular del sistema
Estado

Aceptado.

Contexto

Desde el inicio del proyecto se estableció como objetivo desarrollar una aplicación gráfica que integrara múltiples responsabilidades: inicialización del entorno gráfico, renderizado del reloj analógico, representación de seis obras artísticas, administración del tiempo, animación y actualización continua de la escena.

Debido a que cada componente posee funciones claramente diferenciadas, resultaba necesario definir una arquitectura que favoreciera la organización del código, facilitara el trabajo colaborativo mediante Git y permitiera incorporar nuevas funcionalidades sin afectar el resto del sistema. Durante el desarrollo, cada integrante trabajó en ramas independientes implementando una obra específica, por lo que la estructura del proyecto debía minimizar los conflictos de integración.

Problema

Diseñar una estructura de software que permitiera distribuir las responsabilidades del proyecto en módulos independientes, manteniendo un bajo acoplamiento entre ellos y facilitando tanto el desarrollo colaborativo como el mantenimiento posterior.

Alternativas consideradas
Todo el proyecto en un único archivo

Consistía en concentrar toda la lógica del programa en un solo archivo fuente.

Aunque esta alternativa simplificaba inicialmente la compilación, generaba un código difícil de mantener, incrementaba el riesgo de conflictos durante el trabajo en equipo y complicaba la localización de errores.

Separación parcial por funcionalidades

Otra posibilidad era dividir únicamente algunos componentes principales, como el reloj y el renderizado, manteniendo todas las obras artísticas en un mismo módulo.

Esta opción reducía parcialmente el tamaño del código, pero seguía provocando conflictos frecuentes cuando varios integrantes modificaban simultáneamente el mismo archivo.

Arquitectura completamente modular

La alternativa seleccionada consistió en dividir el proyecto en módulos especializados, donde cada archivo posee una única responsabilidad claramente definida. Esta organización facilita la comprensión del código, permite desarrollar funcionalidades de manera independiente y reduce significativamente el acoplamiento entre componentes.

Decisión

Se adoptó una arquitectura modular basada en responsabilidades, organizando el proyecto en archivos fuente independientes según la funcionalidad que implementan.

Cada módulo encapsula una responsabilidad específica:

main.c: inicialización del programa y registro de callbacks.
display.c: renderizado general de la escena.
clock.c: dibujo del reloj y obtención de la hora del sistema.
segments.c: administración del motor de segmentos y coordinación de las obras.
utils.c: primitivas geométricas reutilizables.
segment_hokusai.c, segment_vangogh.c, segment_kandinsky.c, segment_mondrian.c, segment_monet.c y segment_klimt.c: implementación individual de cada obra artística.

Esta estructura se complementa con archivos de cabecera (.h) que definen únicamente las interfaces públicas necesarias para la comunicación entre módulos.

Justificación

La arquitectura modular permitió distribuir el trabajo entre los integrantes del equipo mediante ramas independientes, reduciendo considerablemente los conflictos durante la integración del proyecto.

Asimismo, esta organización facilita el mantenimiento del código, ya que cada módulo puede modificarse o ampliarse sin afectar directamente el funcionamiento de los demás componentes.

Desde el punto de vista académico, la separación por responsabilidades también mejora la comprensión del flujo de ejecución y favorece la reutilización de funciones auxiliares, especialmente las relacionadas con el dibujo de primitivas y el motor de segmentos.

Consecuencias
Ventajas
Separación clara de responsabilidades.
Bajo acoplamiento entre módulos.
Mayor facilidad para mantener el código.
Integración sencilla mediante Git.
Reducción de conflictos durante el desarrollo colaborativo.
Posibilidad de extender el proyecto incorporando nuevas obras o funcionalidades sin modificar la estructura general.
Desventajas
Incremento en el número de archivos que conforman el proyecto.
Necesidad de mantener correctamente sincronizadas las declaraciones entre archivos .h y .c.
Requiere una organización más cuidadosa de las dependencias entre módulos.
Evidencia en el proyecto

La arquitectura modular puede observarse en diversos elementos del proyecto:

Organización del código fuente en módulos especializados dentro de la carpeta src.
Declaración de interfaces públicas mediante archivos de cabecera en include.
Configuración de todos los módulos en el archivo CMakeLists.txt.
Integración independiente de cada obra artística mediante archivos específicos (segment_*.c).
Uso de segments.c como coordinador central del motor de renderizado de los segmentos.
Relación con otros ADR

Esta decisión se apoya en las tecnologías seleccionadas en los ADR-001 y ADR-002, y sirve como fundamento para los siguientes registros de arquitectura, especialmente el ADR-004 (Motor de segmentos) y el ADR-005 (Una obra artística por módulo), que profundizan en la organización interna del sistema.