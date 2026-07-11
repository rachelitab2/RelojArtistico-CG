Empecemos con el primero
ADR-001 – Selección del lenguaje de programación C99
Estado

Aceptado.

Contexto

El proyecto Reloj Artístico con Segmentos Rotativos fue desarrollado como parte de la asignatura Computación Gráfica I, cuyo propósito principal es aplicar los fundamentos de la representación gráfica mediante programación. Debido a la naturaleza del proyecto, era necesario utilizar un lenguaje que permitiera trabajar directamente con la API de OpenGL, controlar explícitamente las transformaciones geométricas y comprender el funcionamiento del pipeline gráfico clásico.

Asimismo, la solución debía ser compatible con FreeGLUT y con la herramienta de compilación CMake, utilizadas durante el desarrollo del proyecto.

Problema

Seleccionar un lenguaje que permitiera implementar el proyecto de forma eficiente, con acceso directo a las funciones gráficas de OpenGL, sin introducir capas adicionales de abstracción que dificultaran el aprendizaje de los conceptos fundamentales de Computación Gráfica.

Alternativas consideradas
C++

Ofrece características avanzadas como programación orientada a objetos y una biblioteca estándar más amplia. Sin embargo, para este proyecto dichas ventajas no eran indispensables y habrían añadido un nivel de complejidad que no aportaba beneficios significativos al objetivo académico.

C# con OpenTK

Permite desarrollar aplicaciones gráficas modernas con mayor nivel de abstracción. No obstante, se aparta del enfoque utilizado en la asignatura y oculta parte del funcionamiento interno del pipeline gráfico clásico.

C99

Proporciona acceso directo a la API de OpenGL, posee una sintaxis sencilla, amplia compatibilidad con FreeGLUT y CMake, y permite comprender de forma explícita el proceso de renderizado y las transformaciones geométricas.

Decisión

Se adoptó el estándar C99 como lenguaje principal para el desarrollo del proyecto.

La implementación utiliza exclusivamente código en C, organizado en módulos independientes y compilado mediante CMake con el compilador GCC (MinGW). Esta decisión permitió integrar de forma directa OpenGL y FreeGLUT, manteniendo un control explícito sobre cada etapa del proceso de renderizado.

Justificación

El uso de C99 facilita el aprendizaje de los conceptos fundamentales de Computación Gráfica al eliminar capas intermedias entre el programa y la biblioteca gráfica. Esto permitió implementar directamente transformaciones mediante matrices, primitivas geométricas, animaciones, coordenadas cartesianas y el manejo del estado gráfico de OpenGL.

Además, la simplicidad del lenguaje favoreció la organización modular del proyecto, separando claramente la inicialización del sistema, el motor de renderizado, el reloj analógico, el sistema de segmentos y cada una de las obras artísticas implementadas. Esta estructura también facilitó el trabajo colaborativo mediante ramas independientes en Git.

Consecuencias
Ventajas
Integración directa con OpenGL y FreeGLUT.
Compatibilidad con CMake y MinGW.
Mayor comprensión del pipeline gráfico clásico.
Código ligero y con pocas dependencias externas.
Organización modular sencilla y mantenible.
Facilidad para trabajar mediante ramas independientes en Git.
Desventajas
El lenguaje no proporciona mecanismos de orientación a objetos de forma nativa.
Requiere una mayor organización manual del proyecto.
La gestión de estructuras y responsabilidades depende completamente del diseño realizado por el equipo.
Evidencia en el proyecto

La decisión puede observarse en:

Configuración del estándar C99 en CMakeLists.txt.
Implementación completa mediante archivos .c y .h.
Uso directo de OpenGL y FreeGLUT en todos los módulos gráficos.
Compilación mediante GCC (MinGW) y CMake durante todo el desarrollo del proyecto.