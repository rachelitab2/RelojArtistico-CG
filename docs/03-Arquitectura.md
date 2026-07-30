1. Introducción
Objetivo

La arquitectura del proyecto fue diseñada para desarrollar una aplicación gráfica modular capaz de representar un reloj analógico artístico compuesto por seis obras inspiradas en distintos pintores, utilizando el lenguaje C, OpenGL clásico y FreeGLUT.

Desde el inicio del desarrollo se buscó construir una solución organizada que facilitara el trabajo colaborativo entre los integrantes del equipo, permitiera la integración progresiva de nuevas funcionalidades y simplificara el mantenimiento del código durante todas las etapas del proyecto.

La arquitectura implementada sigue el principio de separación de responsabilidades, donde cada módulo posee una función específica dentro del sistema y se comunica con los demás únicamente mediante interfaces claramente definidas.

Gracias a esta organización fue posible desarrollar de manera independiente el reloj, el motor de segmentos, las primitivas gráficas reutilizables y cada una de las seis obras artísticas, reduciendo el acoplamiento entre componentes y facilitando el proceso de integración mediante Git y GitHub.

Asimismo, esta arquitectura permitió incorporar posteriormente funcionalidades adicionales, como el resaltado dinámico de la obra correspondiente a la hora actual y la visualización de información en el centro del reloj, sin modificar la implementación individual de cada obra artística.

2. Objetivos de la Arquitectura

La arquitectura del proyecto fue definida con los siguientes objetivos principales:

Separar claramente las responsabilidades de cada módulo del sistema.
Facilitar el desarrollo colaborativo mediante ramas independientes de Git.
Reducir el acoplamiento entre los distintos componentes de la aplicación.
Favorecer la reutilización de funciones gráficas comunes.
Simplificar el mantenimiento y la evolución del proyecto.
Permitir la incorporación de nuevas obras artísticas sin modificar la estructura principal del sistema.
Mantener una organización coherente entre la lógica del reloj, el motor de renderizado y los módulos encargados del dibujo artístico.
Aplicar principios de modularidad y encapsulamiento estudiados durante la asignatura de Computación Gráfica.

. Vista General del Sistema

La aplicación sigue una arquitectura modular organizada alrededor del ciclo de renderizado de OpenGL. El programa inicia configurando la biblioteca FreeGLUT y registrando las funciones encargadas del dibujo y de la actualización periódica de la escena.

Durante la ejecución, el sistema mantiene un ciclo continuo donde se realizan dos procesos principales:

Actualización del estado del reloj y de los segmentos.
Renderizado completo de la escena gráfica.

El flujo general de funcionamiento puede resumirse de la siguiente manera:

Inicio del programa
                            │
                            ▼
                     main.c inicializa
                     OpenGL y FreeGLUT
                            │
                            ▼
                 Registro de callbacks
                            │
                            ▼
                     glutMainLoop()
                            │
          ┌─────────────────┴─────────────────┐
          ▼                                   ▼
      display()                           timer()
          │                                   │
          ▼                                   ▼
   drawClock()                       updateSegments()
   drawSegments()                          │
          │                                │
          └───────────────┬────────────────┘
                          ▼
                 Actualización de la escena
                          │
                          ▼
                    Renderizado final

Esta organización mantiene completamente separadas las tareas de actualización del estado interno del sistema y las tareas relacionadas con el dibujo de los elementos gráficos.

4. Arquitectura por Módulos

La aplicación fue organizada siguiendo una arquitectura modular basada en responsabilidades. Cada archivo fuente implementa una función específica dentro del sistema, evitando mezclar lógica de inicialización, renderizado, administración del tiempo y representación artística.

Esta organización permitió distribuir el desarrollo entre los integrantes del equipo, reducir el acoplamiento entre componentes y facilitar el mantenimiento del código conforme el proyecto fue evolucionando.

La siguiente figura resume la estructura general del sistema.

                            main.c
                              │
                              ▼
                        display.c
                              │
      ┌───────────────┬───────────────┬───────────────┐
      ▼               ▼               ▼               ▼
   clock.c       segments.c        utils.c      OpenGL/GLUT
                        │
                        │
        ┌───────────────┼─────────────────────────────┐
        ▼               ▼               ▼             ▼
 segment_hokusai.c segment_vangogh.c segment_kandinsky.c
        │               │               │
        ├───────────────┼───────────────┤
        ▼               ▼
segment_mondrian.c segment_monet.c
        │
        ▼
segment_klimt.c

Cada uno de estos módulos posee una responsabilidad claramente definida.

4.1 Módulo main.c

El archivo main.c constituye el punto de entrada de la aplicación.

Su responsabilidad consiste únicamente en preparar el entorno de ejecución. Para ello inicializa la biblioteca FreeGLUT, crea la ventana principal, configura OpenGL, registra las funciones callback que controlan el renderizado y la actualización periódica de la escena, e inicia el ciclo principal de eventos mediante glutMainLoop().

Una vez iniciado dicho ciclo, main.c deja de intervenir directamente en el funcionamiento del sistema, delegando completamente el control a los módulos especializados.

Sus responsabilidades son:

Inicializar FreeGLUT.
Crear la ventana principal.
Configurar el contexto OpenGL.
Registrar las funciones callback.
Iniciar el ciclo principal de la aplicación.

4.2 Módulo display.c

El módulo display.c es responsable del renderizado completo de la escena.

Cada vez que OpenGL solicita un nuevo dibujo, este módulo limpia el framebuffer, restablece la matriz de modelado y coordina el renderizado de los dos componentes principales del proyecto:

el reloj analógico;
los segmentos que contienen las obras artísticas.

Finalmente realiza el intercambio de buffers para presentar la imagen terminada en pantalla.

Este módulo no implementa detalles gráficos específicos; únicamente coordina el orden correcto de dibujo.

Sus responsabilidades incluyen:

Limpiar la pantalla.
Restablecer la matriz de transformación.
Dibujar el reloj.
Dibujar los segmentos.
Actualizar la imagen visible mediante doble buffer.

4.3 Módulo clock.c

El módulo clock.c implementa toda la lógica relacionada con el reloj analógico.

Entre sus funciones se encuentran la obtención de la hora actual del sistema operativo, el cálculo de los ángulos correspondientes a las manecillas y el dibujo de los distintos elementos que conforman la carátula del reloj.

La separación de este módulo evita que la lógica temporal se mezcle con el renderizado de las obras artísticas.

Dentro de este componente se representan elementos como:

carátula principal;
marcas horarias;
manecilla de horas;
manecilla de minutos;
manecilla de segundos;
pivote central.

Además, proporciona al resto del sistema una única fuente de información temporal mediante una función encargada de consultar la hora del sistema.

4.4 Módulo segments.c

El módulo segments.c constituye el núcleo del proyecto.

Su responsabilidad principal consiste en administrar los seis segmentos que forman el anillo artístico del reloj.

Este módulo mantiene la información asociada a cada segmento, controla la rotación de la rueda, calcula cuál obra corresponde destacar según la hora actual y coordina el dibujo del fondo y del contenido artístico de cada obra.

Para ello administra un arreglo de estructuras Segment, recorriéndolo durante cada ciclo de renderizado para aplicar las transformaciones necesarias antes de delegar el dibujo al módulo artístico correspondiente.

Asimismo, este componente implementa el sistema de resaltado dinámico mediante efectos de escala, borde, iluminación y etiqueta informativa.

4.5 Módulo utils.c

El módulo utils.c reúne todas las primitivas gráficas reutilizables empleadas por el resto del proyecto.

En lugar de implementar repetidamente líneas, circunferencias, arcos o conversiones matemáticas, estas operaciones fueron centralizadas en un único módulo para facilitar su reutilización.

Entre las funciones implementadas se encuentran:

dibujo de líneas;
circunferencias;
circunferencias rellenas;
arcos;
sectores circulares;
conversiones entre grados y radianes.

Gracias a esta organización, cualquier mejora realizada sobre una primitiva beneficia automáticamente a todos los módulos que la utilizan.

4.6 Módulos artísticos (segment_*.c)

Cada obra artística fue implementada en un módulo independiente.

Esta decisión permitió desarrollar cada representación de forma aislada y reducir los conflictos durante el trabajo colaborativo.

Cada módulo implementa exclusivamente dos responsabilidades:

representar el fondo del segmento;
representar el contenido principal de la obra.

El motor de segmentos es el único encargado de decidir cuándo debe ejecutarse cada uno de estos módulos, manteniendo completamente desacopladas las representaciones artísticas del funcionamiento interno del reloj.

Los módulos implementados corresponden a:

Hokusai.
Vincent van Gogh.
Wassily Kandinsky.
Piet Mondrian.
Claude Monet.
Gustav Klimt.

4.7 Modulo artwork_catalog.c

El modulo artwork_catalog.c centraliza la informacion textual y visual de las obras artisticas.

Su responsabilidad consiste en almacenar los metadatos de cada obra, incluyendo titulo, autor, anio, movimiento artistico, descripcion, color de fondo, color de acento y paleta cromatica.

Esta separacion evita que la interfaz o el motor de segmentos dupliquen informacion artistica. En lugar de almacenar nombres o colores en distintos archivos, los modulos consultan el catalogo mediante getArtworkInfo().

Gracias a esta decision, futuras funcionalidades como imagenes de referencia, musica por obra o fichas extendidas podran integrarse sin modificar directamente las funciones de dibujo de cada obra.

4.8 Modulo ui.c

El modulo ui.c implementa la capa de interfaz grafica del proyecto.

Esta capa no dibuja el reloj ni las obras, sino elementos informativos y de control, como el panel descriptivo de la obra activa, la paleta cromatica y el selector de intervalo de cambio.

La interfaz consulta el catalogo de obras para mostrar informacion artistica y utiliza los colores de acento para mantener coherencia visual con la obra activa.

Separar la UI del renderizado principal permite evolucionar la experiencia de usuario sin sobrecargar display.c ni segments.c.

4.9 Modulo app_config.c

El modulo app_config.c encapsula la configuracion activa de la aplicacion.

Actualmente administra el intervalo de cambio de obra, permitiendo seleccionar entre 15 minutos, 30 minutos y 1 hora.

Esta configuracion es utilizada por el motor de segmentos para determinar que obra debe estar activa y por la interfaz para mostrar la opcion seleccionada.

Su separacion permite que nuevas preferencias puedan agregarse posteriormente sin mezclar estado de configuracion con logica grafica.

5. Flujo Completo de Ejecución

La aplicación sigue el modelo de ejecución basado en eventos implementado por FreeGLUT. Después de inicializar el entorno gráfico, el programa permanece dentro del ciclo principal (glutMainLoop()), donde la biblioteca se encarga de invocar automáticamente las funciones registradas para el dibujo y la actualización de la escena.

Cada cuadro mostrado en pantalla es el resultado de un proceso compuesto por dos etapas principales:

Actualización del estado interno del sistema.
Renderizado completo de la escena.

Este proceso se repite continuamente hasta que la aplicación finaliza.

5.1 Inicialización del programa

Cuando el usuario ejecuta la aplicación, el control comienza en main.c.

Durante esta etapa se realizan las siguientes operaciones:

Inicialización de FreeGLUT.
Configuración del modo de visualización.
Creación de la ventana principal.
Inicialización de OpenGL.
Inicialización de los segmentos del reloj.
Registro de las funciones callback.
Inicio del ciclo principal mediante glutMainLoop().

A partir de este momento, el flujo de ejecución deja de depender directamente de main.c y pasa a ser controlado por FreeGLUT.

5.2 Actualización del estado de la aplicación

Periódicamente, FreeGLUT ejecuta la función timer().

Su objetivo consiste en mantener actualizado el estado interno del proyecto.

Durante esta etapa se realizan operaciones como:

actualización de la rotación de los segmentos;
obtención de la hora actual del sistema;
cálculo de la obra artística activa;
actualización de la animación del resaltado;
solicitud de un nuevo renderizado mediante glutPostRedisplay().

Gracias a esta separación, la lógica de actualización permanece independiente del proceso de dibujo.

5.3 Renderizado de la escena

Cuando OpenGL solicita un nuevo dibujo, se ejecuta la función display().

El proceso ocurre en el siguiente orden:

Limpieza del framebuffer.
Restablecimiento de la matriz de modelado.
Dibujo del reloj analógico.
Dibujo de los seis segmentos artísticos.
Intercambio de buffers.

Este orden garantiza que todos los elementos sean renderizados correctamente sin producir parpadeos gracias al uso de doble buffer.

5.4 Renderizado del reloj

La función drawClock() coordina el dibujo de todos los componentes del reloj.

Entre ellos se encuentran:

carátula;
marcas horarias;
manecilla de horas;
manecilla de minutos;
manecilla de segundos;
pivote central.

Los ángulos de cada manecilla son calculados utilizando la hora obtenida desde el sistema operativo.

5.5 Renderizado de los segmentos

Después de dibujar el reloj, display() invoca drawSegments().

Esta función recorre el arreglo que almacena los seis segmentos mediante un ciclo for.

Para cada segmento se realizan las siguientes operaciones:

Aplicación de la rotación correspondiente.
Dibujo del fondo artístico.
Dibujo del borde del segmento.
Cálculo del lienzo local.
Aplicación de las transformaciones necesarias.
Dibujo de la obra artística.
Aplicación de efectos visuales cuando el segmento corresponde a la hora actual.

Este procedimiento se repite exactamente seis veces durante cada cuadro.

5.6 Selección de la obra activa

Durante la actualización del sistema se consulta la hora actual mediante el módulo del reloj.

A partir de dicha información, el motor de segmentos determina cuál de las seis obras corresponde destacar.

La selección se realiza automáticamente y no requiere interacción del usuario.

Una vez identificado el segmento activo, el sistema aplica efectos visuales adicionales, como:

incremento de escala;
borde dorado;
superposición luminosa;
etiqueta central con la hora y el nombre del artista.

Los demás segmentos continúan renderizándose normalmente.

5.7 Ciclo continuo de ejecución

Todo el proceso descrito anteriormente se repite aproximadamente sesenta veces por segundo.

Este comportamiento produce una animación fluida donde:

el reloj permanece sincronizado con la hora del sistema;
la rueda artística continúa girando;
el segmento destacado cambia automáticamente cuando corresponde una nueva hora;
la escena se mantiene completamente actualizada durante toda la ejecución.

Diagrama del flujo de ejecución

Inicio
                       │
                       ▼
                  main()
                       │
     ┌─────────────────────────────────┐
     │ Inicialización de FreeGLUT       │
     │ Creación de ventana              │
     │ Inicialización OpenGL            │
     │ initSegments()                   │
     │ Registro de callbacks            │
     └─────────────────────────────────┘
                       │
                       ▼
               glutMainLoop()
                       │
          ┌────────────┴────────────┐
          ▼                         ▼
      timer()                  display()
          │                         │
          ▼                         ▼
 updateSegments()             drawClock()
          │                   drawSegments()
          │                         │
          └────────────┬────────────┘
                       ▼
               glutPostRedisplay()
                       │
                       ▼
                Nuevo fotograma
                       │
                       └───────────────► Se repite

6. Relaciones entre Módulos

La arquitectura del proyecto fue diseñada para mantener un bajo nivel de acoplamiento entre sus componentes. Cada módulo conoce únicamente las interfaces públicas de los módulos con los que necesita interactuar, evitando dependencias innecesarias y facilitando el mantenimiento del sistema.

La comunicación entre módulos se realiza mediante archivos de cabecera (.h), donde se declaran únicamente las funciones necesarias para la interacción entre componentes.

El siguiente diagrama resume las relaciones principales del proyecto.

                            main.c
                             │
            ┌────────────────┴────────────────┐
            ▼                                 ▼
      display.c                         initDisplay()
            │
      ┌─────┴───────────────┐
      ▼                     ▼
 clock.c              segments.c
      │                     │
      │                     ├─────────────────────────────┐
      │                     ▼                             ▼
      │                utils.c                    segment_*.c
      │
      ▼
 Hora del sistema

Relación entre main.c y display.c

El módulo main.c únicamente registra la función display() como callback de renderizado. A partir de ese momento, toda la representación gráfica es responsabilidad del módulo display.c.

Esta separación evita que el punto de entrada de la aplicación contenga lógica relacionada con el dibujo de la escena.

Relación entre display.c y clock.c

Durante cada ciclo de renderizado, display.c invoca la función drawClock(), delegando completamente la representación del reloj al módulo especializado.

De esta forma, display.c no necesita conocer cómo se calculan las manecillas ni cómo se construye la carátula del reloj.

Relación entre display.c y segments.c

El módulo display.c también coordina el dibujo de los segmentos mediante la función drawSegments().

Toda la lógica relacionada con la rueda artística permanece encapsulada dentro de segments.c, incluyendo las transformaciones, el resaltado dinámico y la coordinación de las obras.

Relación entre segments.c y los módulos artísticos

El motor de segmentos mantiene la información de cada sector mediante una estructura Segment, la cual almacena, entre otros datos, la obra artística asociada.

Durante el renderizado, el motor identifica el tipo de obra correspondiente y delega el dibujo al módulo especializado utilizando funciones específicas para el fondo y para el contenido principal.

Gracias a esta organización, ninguno de los módulos artísticos necesita conocer la existencia de los demás ni interactuar directamente con ellos.

Relación con utils.c

El módulo utils.c constituye una biblioteca de primitivas reutilizables utilizada por prácticamente todo el proyecto.

Componentes como el reloj, el motor de segmentos y las distintas obras reutilizan funciones comunes para representar líneas, arcos, circunferencias y otras figuras geométricas.

Esta decisión evita duplicar código y garantiza una representación uniforme de las primitivas utilizadas durante el renderizado.

Dependencias externas

Además de los módulos desarrollados por el equipo, la aplicación utiliza tres bibliotecas externas principales:

OpenGL, responsable del renderizado gráfico.
FreeGLUT, encargada de la creación de la ventana y el manejo del ciclo de eventos.
Biblioteca estándar de C, utilizada para operaciones matemáticas, manejo del tiempo y funcionalidades básicas del lenguaje.

Estas dependencias permanecen completamente encapsuladas dentro de los módulos que las requieren, evitando exponer detalles de implementación al resto del sistema.

7. Conceptos de Computación Gráfica implementados

Uno de los objetivos principales del proyecto fue aplicar de forma práctica los conceptos estudiados durante la asignatura de Computación Gráfica I. La arquitectura desarrollada permitió integrar dichos conceptos en un proyecto funcional, combinando geometría, transformaciones, animación y renderizado en tiempo real.

Entre los conceptos implementados se encuentran:

Sistema de coordenadas cartesianas

Toda la escena gráfica se construye utilizando el sistema de coordenadas bidimensional proporcionado por OpenGL.

Cada elemento del reloj y de las obras artísticas se posiciona mediante coordenadas cartesianas relativas al origen de la escena.

Transformaciones geométricas

El proyecto utiliza diversas transformaciones de la matriz de modelado para construir la escena.

Entre ellas destacan:

traslaciones (glTranslatef);
rotaciones (glRotatef);
escalados (glScalef).

Estas operaciones permiten reutilizar el mismo dibujo en diferentes posiciones y orientaciones sin modificar la geometría original.

Composición jerárquica

El reloj se construye mediante una composición jerárquica donde cada elemento se dibuja utilizando la pila de matrices (glPushMatrix y glPopMatrix).

Este mecanismo evita que las transformaciones aplicadas a un componente afecten al resto de la escena.

Primitivas gráficas

Las distintas obras y el reloj fueron construidos utilizando primitivas básicas de OpenGL, entre ellas:

líneas;
triángulos;
polígonos;
tiras de triángulos;
sectores circulares.

A partir de estas primitivas fue posible representar composiciones gráficas de mayor complejidad.

Animación

La animación del proyecto se logra mediante la actualización periódica del estado interno utilizando callbacks temporizados.

Este mecanismo permite mantener la rotación continua de los segmentos y los efectos visuales asociados a la obra destacada.

Renderizado mediante doble buffer

La aplicación utiliza doble buffer para evitar parpadeos durante el renderizado.

Mientras un buffer es presentado al usuario, el siguiente cuadro se construye en memoria y posteriormente ambos buffers intercambian su contenido mediante glutSwapBuffers().

Sincronización con el tiempo del sistema

El reloj obtiene la hora directamente del sistema operativo, permitiendo representar una hora real y utilizar dicha información para seleccionar automáticamente la obra artística destacada.

8. Tecnologías Utilizadas

El desarrollo del proyecto se realizó utilizando herramientas y tecnologías ampliamente empleadas en el ámbito de la programación gráfica, seleccionadas por su compatibilidad con los contenidos de la asignatura y por facilitar la construcción de aplicaciones interactivas en lenguaje C.

Lenguaje de programación

C99

El proyecto fue implementado utilizando el estándar C99, el cual proporciona un control directo sobre la memoria y una sintaxis adecuada para trabajar con bibliotecas gráficas como OpenGL.

Su utilización permitió comprender de forma práctica conceptos relacionados con estructuras de datos, modularidad y organización del código fuente.

Biblioteca gráfica

OpenGL (modo inmediato)

OpenGL fue utilizado como API principal para el renderizado de todos los elementos gráficos del proyecto.

Mediante esta biblioteca se implementó el dibujo del reloj, los segmentos, las obras artísticas y las distintas transformaciones geométricas necesarias para construir la escena.

Entre las funciones más utilizadas se encuentran:

glBegin()
glEnd()
glVertex2f()
glColor3f()
glTranslatef()
glRotatef()
glScalef()
glPushMatrix()
glPopMatrix()
Biblioteca de ventanas

FreeGLUT

FreeGLUT fue utilizada para administrar la creación de la ventana, el ciclo principal de ejecución, los temporizadores y el manejo del contexto OpenGL.

Además, permitió implementar el sistema de actualización periódica del proyecto mediante callbacks.

Sistema de construcción

CMake

La compilación del proyecto fue organizada mediante CMake.

Este sistema permitió administrar los archivos fuente, configurar automáticamente la generación del ejecutable y facilitar la compilación utilizando distintos compiladores compatibles con el estándar C.

Control de versiones

Git y GitHub

El desarrollo colaborativo fue gestionado mediante Git y GitHub.

Cada integrante trabajó sobre ramas independientes para implementar funcionalidades específicas, las cuales posteriormente fueron integradas en la rama develop utilizando procesos de revisión y resolución de conflictos.

Entorno de desarrollo

Durante el desarrollo del proyecto se utilizaron herramientas compatibles con C y OpenGL, incluyendo Visual Studio Code y el compilador MinGW (MSYS2), configurados mediante CMake para automatizar el proceso de construcción del ejecutable.

9. Extensibilidad del Sistema

Uno de los objetivos de la arquitectura fue permitir que el proyecto pudiera evolucionar sin requerir modificaciones importantes en su estructura principal.

Gracias a la separación de responsabilidades y a la organización modular del código, es posible incorporar nuevas funcionalidades manteniendo el mismo diseño arquitectónico.

Entre las posibles extensiones futuras se encuentran:

incorporación de nuevas obras artísticas;
aumento del número de segmentos del reloj;
implementación de nuevos efectos visuales;
incorporación de animaciones adicionales;
migración hacia versiones modernas de OpenGL;
personalización dinámica de colores o temas visuales;
interacción mediante teclado o ratón;
configuración del proyecto mediante archivos externos.

La arquitectura implementada permite realizar estas mejoras modificando únicamente los módulos involucrados, sin afectar el funcionamiento general del sistema.

10. Conclusiones

La arquitectura desarrollada permitió construir una aplicación gráfica organizada, modular y fácilmente mantenible, cumpliendo los objetivos establecidos al inicio del proyecto.

La división del sistema en módulos especializados facilitó el desarrollo colaborativo, redujo el acoplamiento entre componentes y simplificó la integración de nuevas funcionalidades durante las distintas fases del proyecto.

Asimismo, la utilización de un motor centralizado para la administración de los segmentos, junto con módulos independientes para cada obra artística y una biblioteca común de primitivas gráficas, permitió aplicar principios de reutilización, encapsulamiento y separación de responsabilidades.

Desde el punto de vista académico, el proyecto constituye una aplicación práctica de los conceptos fundamentales estudiados en la asignatura de Computación Gráfica I, integrando transformaciones geométricas, composición jerárquica, primitivas gráficas, animación, renderizado en tiempo real y sincronización con la hora del sistema.

Finalmente, la arquitectura propuesta proporciona una base sólida para futuras ampliaciones del proyecto, permitiendo incorporar nuevas funcionalidades sin alterar significativamente la organización general del software.

<!-- actualización -->

