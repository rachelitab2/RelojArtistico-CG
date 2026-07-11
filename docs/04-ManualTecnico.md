1. Introducción
Propósito del documento

El presente Manual Técnico tiene como finalidad documentar la estructura interna del proyecto Reloj Artístico, proporcionando la información necesaria para que cualquier desarrollador pueda comprender, compilar, mantener y extender la aplicación.

Este documento describe la organización del código fuente, las tecnologías empleadas, la función de cada módulo, el proceso de compilación y las recomendaciones para incorporar nuevas funcionalidades sin alterar la arquitectura existente.

El manual está dirigido principalmente a desarrolladores, docentes y futuros mantenedores del proyecto, por lo que se centra en aspectos técnicos y de implementación más que en el uso de la aplicación.

2. Objetivo

Este manual tiene como objetivos:

Documentar la estructura técnica del proyecto.
Describir el funcionamiento de cada módulo.
Explicar el proceso de compilación.
Facilitar el mantenimiento del código.
Servir como guía para incorporar nuevas funcionalidades.
Documentar las dependencias necesarias para ejecutar el proyecto.

3. Requisitos del Sistema
Hardware

El proyecto no requiere hardware especializado. Puede ejecutarse en cualquier equipo capaz de soportar OpenGL y FreeGLUT.

Requisitos mínimos recomendados:

Procesador de 64 bits.
4 GB de memoria RAM.
Tarjeta gráfica compatible con OpenGL.
Resolución mínima recomendada de 1280×720 píxeles.
Software

Para compilar y ejecutar el proyecto se requiere:

Sistema operativo Windows 10 o superior.
Compilador compatible con C99 (MinGW GCC).
CMake 3.20 o superior.
Biblioteca OpenGL.
Biblioteca FreeGLUT.
Git (opcional para control de versiones).
Visual Studio Code u otro editor compatible.
Dependencias

Durante la compilación se enlazan las siguientes bibliotecas:

OpenGL (opengl32)
GLU (glu32)
FreeGLUT (freeglut)

Estas dependencias son configuradas automáticamente mediante el archivo CMakeLists.txt.

4. Tecnologías Utilizadas
Tecnología	Propósito
C99	Desarrollo del proyecto
OpenGL	Renderizado gráfico
FreeGLUT	Ventana y ciclo de eventos
CMake	Sistema de compilación
Git	Control de versiones
GitHub	Trabajo colaborativo
MSYS2 / MinGW	Compilador GCC
Estándares utilizados

Durante el desarrollo se siguieron los siguientes principios:

Programación modular.
Separación de responsabilidades.
Bajo acoplamiento.
Reutilización de código.
Organización mediante archivos de cabecera.
Desarrollo colaborativo utilizando Git Flow basado en ramas.
Organización general

El proyecto fue diseñado para que cada componente tenga una única responsabilidad, permitiendo que el código pueda mantenerse y extenderse sin modificar la estructura principal del sistema.

La arquitectura técnica descrita en este documento complementa las decisiones registradas en los ADR y la arquitectura general presentada en el documento anterior.

5. Estructura del Proyecto

El proyecto se encuentra organizado siguiendo una estructura modular que separa claramente el código fuente, los archivos de cabecera, la documentación y los archivos de configuración del sistema de compilación.

La siguiente estructura representa la organización general del repositorio.

RelojArtistico/
│
├── docs/
│   ├── 01-InvestigacionyPropuesta.md
│   ├── 02-ADR.md
│   ├── 03-Arquitectura.md
│   ├── 04-ManualTecnico.md
│   ├── 05-BitacoraDesarrollo.md
│   ├── 06-ComentariosCodigo.md
│   ├── 07-DecisionesTecnicas.md
│   ├── 08-PruebasYValidacion.md
│   └── README.md
│
├── include/
│   ├── clock.h
│   ├── display.h
│   ├── segments.h
│   ├── utils.h
│   ├── segment_hokusai.h
│   ├── segment_vangogh.h
│   ├── segment_kandinsky.h
│   ├── segment_mondrian.h
│   ├── segment_monet.h
│   └── segment_klimt.h
│
├── src/
│   ├── main.c
│   ├── display.c
│   ├── clock.c
│   ├── segments.c
│   ├── utils.c
│   ├── segment_hokusai.c
│   ├── segment_vangogh.c
│   ├── segment_kandinsky.c
│   ├── segment_mondrian.c
│   ├── segment_monet.c
│   └── segment_klimt.c
│
├── CMakeLists.txt
├── .gitignore
└── README.md

Esta organización permite localizar rápidamente cada componente del sistema y facilita el mantenimiento del código conforme el proyecto evoluciona.

5.1 Carpeta docs

La carpeta docs contiene toda la documentación técnica y académica desarrollada durante el proyecto.

Entre los documentos incluidos se encuentran:

Investigación y propuesta.
Registros de decisiones arquitectónicas (ADR).
Documento de arquitectura.
Manual técnico.
Bitácora del desarrollo.
Comentarios del código.
Decisiones técnicas.
Pruebas y validación.

Esta documentación complementa el código fuente y facilita tanto la evaluación académica como el mantenimiento futuro del proyecto.

5.2 Carpeta include

La carpeta include reúne todos los archivos de cabecera (.h) del proyecto.

Su función principal consiste en declarar las estructuras de datos, tipos, constantes y prototipos de funciones que serán utilizados por los distintos módulos.

Cada archivo fuente posee su correspondiente archivo de cabecera, permitiendo mantener separadas las declaraciones públicas de las implementaciones.

Esta organización favorece el encapsulamiento y reduce el acoplamiento entre módulos.

5.3 Carpeta src

La carpeta src contiene toda la implementación del proyecto.

Cada archivo fuente desarrolla una responsabilidad específica dentro del sistema.

Los módulos principales son:

Archivo	Responsabilidad
main.c	Punto de entrada del programa e inicialización del entorno gráfico.
display.c	Coordinación del renderizado de toda la escena.
clock.c	Implementación del reloj analógico y consulta de la hora del sistema.
segments.c	Administración de los segmentos, transformaciones y coordinación de las obras.
utils.c	Biblioteca de primitivas gráficas reutilizables.

Además, se implementan seis módulos artísticos independientes:

segment_hokusai.c
segment_vangogh.c
segment_kandinsky.c
segment_mondrian.c
segment_monet.c
segment_klimt.c

Cada uno de ellos representa una única obra artística.

5.4 Archivo CMakeLists.txt

El archivo CMakeLists.txt define la configuración de compilación del proyecto.

Entre sus responsabilidades se encuentran:

definir el estándar C99;
registrar todos los archivos fuente;
configurar los directorios de inclusión;
generar el ejecutable principal;
enlazar automáticamente las bibliotecas OpenGL, GLU y FreeGLUT.

Gracias a CMake, el proyecto puede compilarse de forma consistente utilizando diferentes compiladores compatibles.

5.5 Archivo .gitignore

El archivo .gitignore evita que Git registre archivos generados automáticamente durante la compilación.

Entre ellos se encuentran:

archivos temporales;
ejecutables;
objetos compilados;
directorios de construcción (build/);
configuraciones específicas del entorno de desarrollo.

Esto mantiene el repositorio limpio y evita conflictos innecesarios entre los integrantes del equipo.

5.6 Organización modular

La estructura del proyecto fue diseñada para cumplir tres principios fundamentales:

Una responsabilidad por archivo.
Interfaces públicas mediante archivos .h.
Implementaciones independientes en archivos .c.

Esta organización facilita la reutilización del código, simplifica las pruebas individuales de cada módulo y permite incorporar nuevas funcionalidades sin modificar significativamente la estructura general del proyecto.

6. Descripción de los Módulos

Cada módulo del proyecto fue diseñado para cumplir una única responsabilidad dentro del sistema. Esta organización permite localizar fácilmente la funcionalidad deseada, reducir el acoplamiento entre componentes y facilitar el mantenimiento del código.

A continuación, se describe la función de cada uno de los módulos principales.

6.1 main.c
Descripción

Es el punto de entrada de la aplicación y el encargado de inicializar el entorno gráfico.

Este módulo configura FreeGLUT, crea la ventana principal, inicializa OpenGL, registra las funciones callback y entrega el control al ciclo principal de eventos.

Una vez iniciada la aplicación, main.c no participa nuevamente en el flujo del programa.

Funciones principales
Función	Descripción
main()	Punto de entrada de la aplicación.
reshape()	Ajusta la proyección cuando cambia el tamaño de la ventana.
Dependencias
display.h
segments.h
Mantenimiento

Las modificaciones en este módulo normalmente se limitan a cambios relacionados con la creación de la ventana, configuración de OpenGL o registro de nuevos callbacks.

6.2 display.c
Descripción

Coordina el proceso de renderizado de toda la escena.

Durante cada cuadro limpia la pantalla, reinicia la matriz de modelado, dibuja el reloj, dibuja los segmentos y presenta el resultado mediante doble buffer.

Este módulo actúa como coordinador del renderizado sin implementar detalles específicos de los componentes gráficos.

Funciones principales
Función	Descripción
initDisplay()	Configura el estado inicial de OpenGL.
display()	Renderiza un fotograma completo.
timer()	Actualiza periódicamente la escena y solicita un nuevo renderizado.
Dependencias
clock.h
segments.h
utils.h
Mantenimiento

Las nuevas funcionalidades globales relacionadas con el orden de renderizado pueden incorporarse en este módulo.

No se recomienda incluir aquí lógica específica del reloj o de las obras artísticas.

6.3 clock.c
Descripción

Implementa toda la lógica relacionada con el reloj analógico.

Además de representar gráficamente la carátula y las manecillas, proporciona una interfaz para consultar la hora actual del sistema operativo.

Funciones principales
Función	Descripción
drawClock()	Coordina el dibujo completo del reloj.
getCurrentTime()	Obtiene la hora actual del sistema.

Internamente este módulo implementa funciones auxiliares para representar las distintas partes del reloj.

Dependencias
utils.h
Biblioteca estándar de tiempo (time.h)
Mantenimiento

Las modificaciones relacionadas con la apariencia del reloj o con el manejo de la hora deben realizarse exclusivamente en este módulo.

6.4 segments.c
Descripción

Constituye el núcleo del proyecto.

Administra el arreglo de segmentos, controla la rotación del anillo artístico, calcula la obra activa según la hora del sistema y coordina el dibujo de cada representación artística.

También implementa el mecanismo de resaltado visual mediante transformaciones geométricas y efectos gráficos.

Funciones principales
Función	Descripción
initSegments()	Inicializa los seis segmentos.
drawSegments()	Dibuja todos los segmentos del reloj.
drawSegment()	Renderiza un segmento individual.
updateSegments()	Actualiza el estado de la animación.
Funciones internas

Este módulo también contiene diversas funciones privadas (static) responsables de:

seleccionar la obra correspondiente;
calcular la obra activa;
dibujar los fondos;
representar la etiqueta central;
administrar los efectos visuales.
Dependencias
clock.h
utils.h
Todos los módulos segment_*.h
Mantenimiento

La incorporación de nuevas funcionalidades relacionadas con los segmentos debe realizarse aquí.

No se recomienda modificar los módulos artísticos cuando el cambio afecte al comportamiento general del reloj.

6.5 utils.c
Descripción

Implementa la biblioteca de primitivas gráficas reutilizables utilizada por el resto del proyecto.

Su objetivo es evitar la duplicación de código proporcionando funciones genéricas para representar figuras geométricas y realizar operaciones matemáticas frecuentes.

Funciones principales

Entre las funciones disponibles se encuentran:

drawLine()
drawCircle()
drawFilledCircle()
drawArc()
drawFilledArc()
degreesToRadians()
Dependencias

Únicamente OpenGL y la biblioteca matemática estándar.

Mantenimiento

Cuando sea necesario incorporar nuevas primitivas gráficas reutilizables, deben añadirse a este módulo para que puedan ser utilizadas por todo el proyecto.

6.6 Módulos artísticos (segment_*.c)
Descripción

Cada uno de estos módulos implementa una única obra artística inspirada en un pintor diferente.

Todos siguen la misma estructura, facilitando la lectura del código y el mantenimiento del proyecto.

Cada módulo contiene dos funciones públicas:

Función	Responsabilidad
draw<Artista>()	Dibuja la composición principal de la obra.
draw<Artista>Background()	Representa el fondo del segmento correspondiente.
Obras implementadas
Archivo	Obra representada
segment_hokusai.c	La Gran Ola de Kanagawa
segment_vangogh.c	La Noche Estrellada
segment_kandinsky.c	Composición abstracta inspirada en Kandinsky
segment_mondrian.c	Composición geométrica inspirada en Mondrian
segment_monet.c	Los Nenúfares
segment_klimt.c	El Beso
Mantenimiento

Para modificar una obra artística únicamente es necesario editar su módulo correspondiente.

Las modificaciones realizadas en un artista no afectan el funcionamiento de los demás módulos ni del motor de segmentos.

6. Descripción de los Módulos

Cada módulo del proyecto fue diseñado para cumplir una única responsabilidad dentro del sistema. Esta organización permite localizar fácilmente la funcionalidad deseada, reducir el acoplamiento entre componentes y facilitar el mantenimiento del código.

A continuación, se describe la función de cada uno de los módulos principales.

6.1 main.c
Descripción

Es el punto de entrada de la aplicación y el encargado de inicializar el entorno gráfico.

Este módulo configura FreeGLUT, crea la ventana principal, inicializa OpenGL, registra las funciones callback y entrega el control al ciclo principal de eventos.

Una vez iniciada la aplicación, main.c no participa nuevamente en el flujo del programa.

Funciones principales
Función	Descripción
main()	Punto de entrada de la aplicación.
reshape()	Ajusta la proyección cuando cambia el tamaño de la ventana.
Dependencias
display.h
segments.h
Mantenimiento

Las modificaciones en este módulo normalmente se limitan a cambios relacionados con la creación de la ventana, configuración de OpenGL o registro de nuevos callbacks.

6.2 display.c
Descripción

Coordina el proceso de renderizado de toda la escena.

Durante cada cuadro limpia la pantalla, reinicia la matriz de modelado, dibuja el reloj, dibuja los segmentos y presenta el resultado mediante doble buffer.

Este módulo actúa como coordinador del renderizado sin implementar detalles específicos de los componentes gráficos.

Funciones principales
Función	Descripción
initDisplay()	Configura el estado inicial de OpenGL.
display()	Renderiza un fotograma completo.
timer()	Actualiza periódicamente la escena y solicita un nuevo renderizado.
Dependencias
clock.h
segments.h
utils.h
Mantenimiento

Las nuevas funcionalidades globales relacionadas con el orden de renderizado pueden incorporarse en este módulo.

No se recomienda incluir aquí lógica específica del reloj o de las obras artísticas.

6.3 clock.c
Descripción

Implementa toda la lógica relacionada con el reloj analógico.

Además de representar gráficamente la carátula y las manecillas, proporciona una interfaz para consultar la hora actual del sistema operativo.

Funciones principales
Función	Descripción
drawClock()	Coordina el dibujo completo del reloj.
getCurrentTime()	Obtiene la hora actual del sistema.

Internamente este módulo implementa funciones auxiliares para representar las distintas partes del reloj.

Dependencias
utils.h
Biblioteca estándar de tiempo (time.h)
Mantenimiento

Las modificaciones relacionadas con la apariencia del reloj o con el manejo de la hora deben realizarse exclusivamente en este módulo.

6.4 segments.c
Descripción

Constituye el núcleo del proyecto.

Administra el arreglo de segmentos, controla la rotación del anillo artístico, calcula la obra activa según la hora del sistema y coordina el dibujo de cada representación artística.

También implementa el mecanismo de resaltado visual mediante transformaciones geométricas y efectos gráficos.

Funciones principales
Función	Descripción
initSegments()	Inicializa los seis segmentos.
drawSegments()	Dibuja todos los segmentos del reloj.
drawSegment()	Renderiza un segmento individual.
updateSegments()	Actualiza el estado de la animación.
Funciones internas

Este módulo también contiene diversas funciones privadas (static) responsables de:

seleccionar la obra correspondiente;
calcular la obra activa;
dibujar los fondos;
representar la etiqueta central;
administrar los efectos visuales.
Dependencias
clock.h
utils.h
Todos los módulos segment_*.h
Mantenimiento

La incorporación de nuevas funcionalidades relacionadas con los segmentos debe realizarse aquí.

No se recomienda modificar los módulos artísticos cuando el cambio afecte al comportamiento general del reloj.

6.5 utils.c
Descripción

Implementa la biblioteca de primitivas gráficas reutilizables utilizada por el resto del proyecto.

Su objetivo es evitar la duplicación de código proporcionando funciones genéricas para representar figuras geométricas y realizar operaciones matemáticas frecuentes.

Funciones principales

Entre las funciones disponibles se encuentran:

drawLine()
drawCircle()
drawFilledCircle()
drawArc()
drawFilledArc()
degreesToRadians()
Dependencias

Únicamente OpenGL y la biblioteca matemática estándar.

Mantenimiento

Cuando sea necesario incorporar nuevas primitivas gráficas reutilizables, deben añadirse a este módulo para que puedan ser utilizadas por todo el proyecto.

6.6 Módulos artísticos (segment_*.c)
Descripción

Cada uno de estos módulos implementa una única obra artística inspirada en un pintor diferente.

Todos siguen la misma estructura, facilitando la lectura del código y el mantenimiento del proyecto.

Cada módulo contiene dos funciones públicas:

Función	Responsabilidad
draw<Artista>()	Dibuja la composición principal de la obra.
draw<Artista>Background()	Representa el fondo del segmento correspondiente.
Obras implementadas
Archivo	Obra representada
segment_hokusai.c	La Gran Ola de Kanagawa
segment_vangogh.c	La Noche Estrellada
segment_kandinsky.c	Composición abstracta inspirada en Kandinsky
segment_mondrian.c	Composición geométrica inspirada en Mondrian
segment_monet.c	Los Nenúfares
segment_klimt.c	El Beso
Mantenimiento

Para modificar una obra artística únicamente es necesario editar su módulo correspondiente.

Las modificaciones realizadas en un artista no afectan el funcionamiento de los demás módulos ni del motor de segmentos.

7. Flujo de Compilación

El proyecto utiliza CMake como sistema de construcción, permitiendo automatizar el proceso de compilación y generación del ejecutable.

Durante la configuración, CMake analiza el archivo CMakeLists.txt, identifica los archivos fuente del proyecto, registra las dependencias necesarias y genera los archivos de construcción compatibles con el compilador seleccionado.

El proceso general de compilación se resume en la siguiente figura.

                Código Fuente
                     │
                     ▼
            CMakeLists.txt
                     │
                     ▼
          Configuración CMake
                     │
                     ▼
          Archivos de construcción
                     │
                     ▼
              Compilación GCC
                     │
                     ▼
          Enlace con OpenGL
             GLU y FreeGLUT
                     │
                     ▼
            RelojArtistico.exe

Compilación del proyecto

Desde la raíz del proyecto se ejecutan los siguientes comandos:

cmake -B build -S . -G "MinGW Makefiles"
cmake --build build

Una vez finalizada la compilación, el ejecutable se genera dentro de la carpeta build.

Archivos involucrados

Durante el proceso participan principalmente:

CMakeLists.txt
archivos .c
archivos .h
bibliotecas OpenGL
biblioteca FreeGLUT
biblioteca GLU
8. Mantenimiento y Extensión del Proyecto

La arquitectura modular facilita el mantenimiento del sistema y la incorporación de nuevas funcionalidades sin alterar significativamente el resto de la aplicación.

Incorporación de una nueva obra artística

Para agregar una nueva obra se recomienda seguir el siguiente procedimiento:

Crear un nuevo archivo fuente segment_nombre.c.
Crear su correspondiente archivo segment_nombre.h.
Implementar las funciones:
drawNombre()
drawNombreBackground()
Registrar el nuevo módulo en CMakeLists.txt.
Agregar el nuevo tipo al enum ArtworkType.
Actualizar el dispatcher del motor de segmentos.
Inicializar el nuevo segmento en initSegments().

Este procedimiento mantiene la coherencia con la arquitectura utilizada durante el desarrollo del proyecto.

Buenas prácticas de mantenimiento

Se recomienda:

mantener una única responsabilidad por módulo;
reutilizar las primitivas existentes en utils.c;
evitar duplicación de código;
documentar nuevas funcionalidades;
mantener sincronizados los archivos .c y .h;
realizar pruebas después de cada modificación importante.
9. Problemas Frecuentes y Soluciones

Durante el desarrollo del proyecto se identificaron algunos inconvenientes comunes relacionados con la compilación y la configuración del entorno.

Error: GL/freeglut.h: No such file or directory
Causa

La biblioteca FreeGLUT no se encuentra instalada o configurada correctamente.

Solución

Verificar la instalación de MSYS2/MinGW y confirmar que el compilador pueda localizar los archivos de cabecera y las bibliotecas correspondientes.

Error de enlace (undefined reference)
Causa

Algún archivo fuente no fue incluido dentro de CMakeLists.txt.

Solución

Confirmar que el archivo se encuentre registrado en la variable SOURCES y volver a generar el directorio build.

Error de CMake
Causa

La carpeta build contiene archivos generados con un compilador diferente.

Solución

Eliminar completamente la carpeta build y volver a configurar el proyecto.

Conflictos de Git
Causa

Dos ramas modificaron simultáneamente el mismo archivo.

Solución

Resolver los conflictos conservando las funcionalidades de ambas ramas, verificar la compilación y confirmar la correcta integración antes de realizar el merge.

10. Conclusiones

El Manual Técnico documenta los aspectos fundamentales necesarios para comprender, compilar, mantener y extender el proyecto Reloj Artístico.

La organización modular del código, el uso de CMake como sistema de compilación y la separación clara de responsabilidades permiten que nuevos desarrolladores puedan incorporarse al proyecto con relativa facilidad y localizar rápidamente los componentes relevantes para realizar modificaciones.

Asimismo, la documentación presentada complementa los registros de arquitectura (ADR) y el documento de arquitectura general, proporcionando una guía práctica para el mantenimiento del software y la incorporación de futuras funcionalidades.

Finalmente, la combinación de C99, OpenGL, FreeGLUT, Git y CMake permitió desarrollar una aplicación gráfica estructurada, reutilizable y alineada con los principios de ingeniería de software y computación gráfica estudiados durante la asignatura.



