07 – Decisiones Técnicas
1. Introducción

Durante el desarrollo del proyecto Reloj Artístico fue necesario tomar diversas decisiones técnicas relacionadas con la implementación, la organización del código, las herramientas utilizadas y la forma de representar gráficamente las obras artísticas.

A diferencia de los Registros de Decisiones Arquitectónicas (ADR), este documento recopila decisiones de carácter técnico adoptadas durante la construcción del software y que influyeron directamente en la implementación final.

Su propósito es facilitar el mantenimiento del proyecto y documentar las razones que motivaron cada elección.

2. Uso del lenguaje C99
Decisión

Implementar el proyecto utilizando el estándar C99.

Justificación

La asignatura utiliza el lenguaje C como base para el aprendizaje de OpenGL y FreeGLUT. Además, C99 proporciona compatibilidad con estructuras, modularidad y funciones suficientes para el alcance del proyecto.

Beneficios
Compatibilidad con OpenGL clásico.
Alto rendimiento.
Control directo sobre la memoria.
Organización modular del código.
3. Uso de OpenGL en modo inmediato
Decisión

Utilizar OpenGL clásico mediante glBegin() y glEnd().

Justificación

El objetivo académico del proyecto consiste en aplicar los fundamentos de la computación gráfica, comprendiendo directamente la construcción de primitivas, transformaciones y composición de escenas.

Beneficios
Mayor facilidad para comprender el pipeline gráfico.
Código sencillo para fines educativos.
Compatibilidad con los contenidos vistos en clase.
4. Uso de FreeGLUT
Decisión

Emplear FreeGLUT para la gestión de la ventana y del ciclo de eventos.

Justificación

FreeGLUT proporciona una interfaz sencilla para crear aplicaciones OpenGL y administrar temporizadores, teclado, redimensionamiento de ventana y renderizado.

Beneficios
Configuración simple.
Integración directa con OpenGL.
Amplio soporte académico.
5. Organización Modular
Decisión

Separar el proyecto en módulos especializados.

Justificación

Reducir el acoplamiento y facilitar el trabajo colaborativo.

Beneficios
Mantenimiento sencillo.
Integración independiente.
Código reutilizable.
Menor cantidad de conflictos en Git.
6. Un archivo por obra artística
Decisión

Cada artista fue implementado en un módulo independiente.

Justificación

Cada obra posee características visuales diferentes y evoluciona de manera independiente.

Beneficios
Mejor organización.
Trabajo paralelo entre integrantes.
Mayor encapsulamiento.
Facilidad para incorporar nuevas obras.
7. Motor centralizado de segmentos
Decisión

Centralizar la administración de los segmentos en segments.c.

Justificación

Las obras únicamente representan contenido gráfico.

Toda la lógica relacionada con rotaciones, selección de la obra activa, transformaciones y efectos visuales permanece concentrada en un único módulo.

Beneficios
Las obras no conocen el estado del reloj.
Se evita duplicación de lógica.
Mantenimiento más sencillo.
8. Biblioteca de primitivas reutilizables
Decisión

Centralizar las primitivas gráficas en utils.c.

Justificación

Diversos módulos requieren representar líneas, arcos y circunferencias.

Beneficios
Reutilización.
Uniformidad visual.
Menor duplicación.
Mejor mantenimiento.
9. Sistema de compilación mediante CMake
Decisión

Utilizar CMake como sistema de construcción.

Justificación

Facilitar la compilación y administración de dependencias.

Beneficios
Compilación automatizada.
Portabilidad.
Configuración sencilla.
Escalabilidad.
10. Control de versiones con Git
Decisión

Utilizar Git y GitHub siguiendo un flujo basado en ramas.

Justificación

El proyecto fue desarrollado por dos integrantes trabajando simultáneamente.

Beneficios
Desarrollo paralelo.
Historial completo.
Integración controlada.
Recuperación sencilla ante errores.
11. Selección automática de la obra destacada
Decisión

Relacionar la obra destacada con la hora del sistema.

Justificación

Agregar dinamismo al reloj sin requerir interacción del usuario.

Beneficios
Comportamiento automático.
Mayor atractivo visual.
Integración entre reloj y obras.
12. Representación mediante coordenadas locales
Decisión

Cada obra se dibuja utilizando un sistema de coordenadas propio.

Justificación

Evitar que el artista deba conocer la posición real del segmento.

Beneficios
Reutilización.
Independencia.
Simplificación del código.
13. Uso de transformaciones jerárquicas
Decisión

Utilizar glPushMatrix() y glPopMatrix() para aislar las transformaciones.

Justificación

Evitar que las modificaciones realizadas por un componente afecten al resto de la escena.

Beneficios
Mayor estabilidad.
Código organizado.
Escena correctamente compuesta.
14. Conclusiones

Las decisiones técnicas documentadas en este apartado permitieron mantener un proyecto consistente, modular y alineado con los objetivos académicos de la asignatura.

Su registro facilita el mantenimiento futuro del software y proporciona contexto sobre las soluciones adoptadas durante la implementación.

<!-- actualización -->