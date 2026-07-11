05 – Bitácora de Desarrollo
1. Introducción

La presente bitácora documenta cronológicamente el desarrollo del proyecto Reloj Artístico, describiendo las principales actividades realizadas durante su construcción, las decisiones tomadas por el equipo, las dificultades encontradas y las soluciones implementadas.

Su propósito es dejar evidencia del proceso de desarrollo seguido durante la asignatura de Computación Gráfica I y servir como referencia para futuras modificaciones del proyecto.

2. Planificación Inicial

Durante la primera etapa del proyecto se definió el objetivo general de construir un reloj analógico utilizando OpenGL e integrar en su periferia representaciones artísticas inspiradas en obras reconocidas de la historia del arte.

En esta fase también se establecieron los requisitos principales del sistema y se realizó la investigación previa sobre OpenGL, FreeGLUT, transformaciones geométricas y organización modular del código.

Actividades realizadas
Investigación de antecedentes.
Definición del alcance.
Elaboración de la propuesta inicial.
Organización del repositorio Git.
Definición de ramas de trabajo.

3. Diseño de la Arquitectura

Una vez definida la propuesta, se diseñó la arquitectura general del proyecto.

Se decidió dividir el sistema en módulos especializados con una única responsabilidad, separando claramente el reloj, los segmentos, las utilidades gráficas y las representaciones artísticas.

Durante esta etapa también se definió la estructura del proyecto y la distribución de responsabilidades entre los integrantes.

Actividades realizadas
Diseño modular.
Definición de estructuras de datos.
Creación del motor de segmentos.
Organización del árbol del proyecto.
Elaboración de los primeros ADR.
4. Implementación del Reloj

En esta etapa se desarrolló el funcionamiento básico del reloj analógico.

Se implementó la obtención de la hora del sistema y el dibujo de los distintos componentes de la carátula utilizando primitivas de OpenGL.

Funcionalidades implementadas
Carátula.
Marcas horarias.
Manecilla de horas.
Manecilla de minutos.
Manecilla de segundos.
Sincronización con la hora del sistema.
5. Desarrollo del Motor de Segmentos

Posteriormente se implementó el motor encargado de administrar los seis segmentos del reloj.

Cada segmento almacena su información mediante una estructura propia y es procesado de forma independiente durante el renderizado.

Esta etapa permitió incorporar transformaciones geométricas reutilizables y preparar la integración de las obras artísticas.

Actividades realizadas
Creación de la estructura Segment.
Inicialización del arreglo de segmentos.
Implementación de la rotación del anillo.
Cálculo del lienzo local para cada obra.
6. Desarrollo de las Obras Artísticas

El equipo desarrolló de forma independiente cada una de las seis obras artísticas.

Cada representación fue implementada en un módulo separado utilizando exclusivamente primitivas de OpenGL.

Las obras incorporadas fueron:

La Gran Ola de Kanagawa.
La Noche Estrellada.
Composición abstracta inspirada en Kandinsky.
Composición geométrica inspirada en Mondrian.
Los Nenúfares.
El Beso.

Durante esta etapa se realizaron múltiples ajustes visuales para mejorar el equilibrio entre composición, color y proporciones.

7. Integración del Proyecto

Una vez desarrolladas las distintas obras, comenzó el proceso de integración utilizando Git.

Cada funcionalidad fue incorporada progresivamente a la rama develop, resolviendo los conflictos surgidos durante el proceso y verificando el correcto funcionamiento del sistema después de cada integración.

Actividades realizadas
Integración de ramas.
Resolución de conflictos.
Revisión del código integrado.
Validación mediante compilaciones completas.
8. Mejoras Funcionales

Con la integración finalizada se incorporaron nuevas funcionalidades destinadas a mejorar la experiencia visual del proyecto.

Entre ellas destacan:

selección automática de la obra destacada según la hora del sistema;
efectos visuales sobre el segmento activo;
borde resaltado;
efecto de iluminación;
escala dinámica;
etiqueta informativa con la hora y el nombre del artista.

Estas mejoras permitieron integrar el funcionamiento del reloj con el contenido artístico del proyecto.

9. Correcciones Técnicas

Durante el desarrollo se presentaron diversos inconvenientes técnicos que fueron corregidos antes de la versión final.

Entre ellos se encuentran:

conflictos de integración mediante Git;
errores durante procesos de rebase;
diferencias entre generadores de CMake;
problemas de compilación asociados a FreeGLUT;
errores de enlace por módulos no registrados;
ajustes en la configuración del entorno de desarrollo;
correcciones visuales sobre algunas representaciones artísticas.

Cada incidencia fue documentada y resuelta antes de continuar con el desarrollo.

10. Documentación del Proyecto

Una vez estabilizada la implementación, el equipo inició la elaboración de la documentación técnica.

Se desarrollaron los siguientes documentos:

Investigación y propuesta.
Registros ADR.
Arquitectura del sistema.
Manual técnico.
Comentarios del código.
Decisiones técnicas.
Pruebas y validación.
Bitácora de desarrollo.

La documentación fue elaborada paralelamente a la revisión del código para garantizar su coherencia con la implementación final.

11. Estado Final del Proyecto

Al concluir el desarrollo, el proyecto presenta las siguientes características:

reloj analógico funcional;
sincronización con la hora del sistema;
seis representaciones artísticas;
arquitectura modular;
sistema de compilación mediante CMake;
control de versiones con Git;
documentación técnica completa;
integración estable entre todos los módulos.

La versión final fue compilada y validada satisfactoriamente antes de la entrega.

12. Lecciones Aprendidas

El desarrollo del proyecto permitió fortalecer conocimientos relacionados con:

programación en lenguaje C;
utilización de OpenGL y FreeGLUT;
transformaciones geométricas;
organización modular del software;
trabajo colaborativo mediante Git;
resolución de conflictos durante procesos de integración;
documentación técnica de proyectos de software;
planificación y mantenimiento de aplicaciones gráficas.

Asimismo, la experiencia permitió comprender la importancia de una arquitectura bien definida y de una documentación consistente para facilitar el desarrollo colaborativo y el mantenimiento futuro del proyecto.

13. Conclusiones

La construcción del Reloj Artístico representó la integración de los conocimientos adquiridos durante la asignatura de Computación Gráfica I, combinando programación gráfica, organización del software y trabajo colaborativo.

El desarrollo se realizó de manera incremental, incorporando nuevas funcionalidades conforme avanzaba el proyecto y validando continuamente la estabilidad de la aplicación.

Como resultado, se obtuvo un sistema modular, funcional y documentado, capaz de representar un reloj analógico acompañado por seis composiciones artísticas inspiradas en obras reconocidas de la historia del arte.

<!-- actualización -->