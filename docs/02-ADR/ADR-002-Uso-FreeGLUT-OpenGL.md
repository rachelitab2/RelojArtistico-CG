ADR-002 – Uso de OpenGL Clásico y FreeGLUT
Estado

Aceptado.

Contexto

El proyecto Reloj Artístico con Segmentos Rotativos debía implementar una aplicación gráfica bidimensional capaz de representar un reloj analógico funcional junto con seis obras artísticas estilizadas, utilizando transformaciones geométricas, primitivas de dibujo, colores, animaciones y sincronización con la hora del sistema.

Como parte de la asignatura Computación Gráfica I, era importante emplear una tecnología que permitiera trabajar directamente con los fundamentos del renderizado gráfico y comprender el funcionamiento del pipeline clásico de OpenGL, sin depender de motores gráficos o bibliotecas de alto nivel.

Problema

Seleccionar una biblioteca gráfica que permitiera implementar todas las funcionalidades del proyecto —renderizado de primitivas, transformaciones, animación, manejo de ventanas y procesamiento de eventos— manteniendo un enfoque didáctico alineado con los contenidos impartidos en la asignatura.

Alternativas consideradas
SDL

SDL proporciona herramientas para crear ventanas, procesar eventos y trabajar con gráficos, pero requiere una integración adicional con OpenGL para implementar el renderizado. Aunque es ampliamente utilizada en aplicaciones multimedia, incorpora una capa de abstracción que no era necesaria para este proyecto.

GLFW

GLFW constituye una alternativa moderna para la creación de ventanas y la gestión del contexto OpenGL. Sin embargo, está orientada principalmente al uso de OpenGL moderno basado en shaders y buffers, alejándose del enfoque de renderizado inmediato estudiado durante el curso.

Motores gráficos (Unity, Unreal Engine)

Los motores gráficos ofrecen numerosas herramientas de alto nivel para el desarrollo de aplicaciones visuales, pero abstraen gran parte del funcionamiento interno del pipeline gráfico, lo que dificulta el aprendizaje de los conceptos fundamentales que la asignatura busca desarrollar.

OpenGL Clásico + FreeGLUT

Esta combinación permite controlar directamente el proceso de renderizado utilizando primitivas geométricas, transformaciones mediante matrices y el modelo de estado de OpenGL. Además, FreeGLUT proporciona las funciones necesarias para crear la ventana, administrar el ciclo principal de la aplicación y gestionar los eventos de entrada.

Decisión

Se decidió utilizar OpenGL clásico (Fixed Function Pipeline) como API gráfica principal y FreeGLUT como biblioteca para la creación de ventanas, administración del contexto gráfico y manejo del ciclo de eventos.

La totalidad del renderizado del proyecto fue implementada utilizando primitivas de OpenGL clásico (GL_LINES, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, entre otras), junto con las funciones de transformación (glTranslatef, glRotatef, glScalef) y la pila de matrices (glPushMatrix y glPopMatrix). FreeGLUT se empleó para la inicialización del entorno gráfico, el temporizador de actualización, el renderizado continuo y la representación de texto mediante fuentes bitmap.

Justificación

La utilización de OpenGL clásico permitió implementar directamente todos los conceptos fundamentales estudiados en Computación Gráfica, incluyendo transformaciones geométricas, coordenadas cartesianas, composición mediante primitivas, manejo del estado gráfico y animaciones en tiempo real.

Por su parte, FreeGLUT proporcionó una interfaz sencilla y estable para crear la ventana de la aplicación, registrar los callbacks principales (display, reshape y timer), administrar el ciclo principal de ejecución y facilitar la representación de texto sobre la escena.

Esta combinación permitió desarrollar una solución completamente funcional sin incorporar dependencias adicionales, manteniendo un entorno de desarrollo ligero, portable y completamente alineado con los objetivos académicos del proyecto.

Consecuencias
Ventajas
Acceso directo al pipeline gráfico clásico.
Implementación explícita de transformaciones geométricas.
Comprensión del funcionamiento interno del proceso de renderizado.
Integración sencilla con CMake y GCC (MinGW).
Compatibilidad con los contenidos impartidos en la asignatura.
Baja cantidad de dependencias externas.
Desventajas
El pipeline fijo utilizado corresponde a una versión clásica de OpenGL y no incorpora técnicas modernas basadas en shaders.
La representación gráfica requiere un mayor número de llamadas manuales a la API.
El desarrollo demanda una organización cuidadosa del código para mantener la escalabilidad del proyecto.
Evidencia en el proyecto

La decisión puede observarse en diversos componentes del sistema:

Inclusión de la biblioteca GL/freeglut.h en todos los módulos gráficos.
Uso de funciones de OpenGL clásico como glBegin, glEnd, glVertex2f, glColor3f, glTranslatef, glRotatef, glScalef, glPushMatrix y glPopMatrix.
Inicialización de FreeGLUT mediante glutInit, glutCreateWindow, glutDisplayFunc, glutReshapeFunc, glutTimerFunc y glutMainLoop.
Configuración de las bibliotecas opengl32, glu32 y freeglut en el archivo CMakeLists.txt.
Relación con otros ADR

Esta decisión complementa al ADR-001, ya que la elección del lenguaje C99 permitió integrar de forma directa OpenGL y FreeGLUT. Asimismo, constituye la base tecnológica sobre la cual se sustentan las decisiones de arquitectura modular, el motor de segmentos y el sistema de renderizado documentados en los ADR posteriores.