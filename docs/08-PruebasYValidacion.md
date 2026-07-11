08 – Pruebas y Validación

1. Introducción

Una vez implementadas las funcionalidades del proyecto, se realizaron diversas pruebas con el objetivo de verificar el correcto funcionamiento del sistema, validar la integración entre módulos y confirmar que el comportamiento observado coincidiera con los requisitos definidos durante la fase de diseño.

Las pruebas abarcaron aspectos funcionales, estructurales y de integración.

2. Objetivos de las pruebas

Las pruebas realizadas tuvieron como propósito:

verificar la compilación del proyecto;
validar la correcta ejecución de la aplicación;
comprobar la integración entre módulos;
confirmar el funcionamiento del reloj;
verificar la representación de las seis obras;
comprobar el comportamiento del sistema de resaltado;
detectar posibles errores durante la integración.

3. Entorno de pruebas

Las pruebas fueron ejecutadas utilizando el siguiente entorno:

Elemento	Configuración
Sistema Operativo	Windows 11
Compilador	GCC (MSYS2 MinGW)
Sistema de construcción	CMake
API gráfica	OpenGL
Biblioteca de ventana	FreeGLUT
Control de versiones	Git

4. Pruebas de compilación

Se realizaron compilaciones completas del proyecto utilizando CMake.

Comandos utilizados:

cmake -B build -S . -G "MinGW Makefiles"
cmake --build build
Resultado

Compilación completada exitosamente.

No se detectaron errores de compilación en la versión final del proyecto.

5. Pruebas funcionales
Inicialización del programa

Objetivo

Verificar la creación correcta de la ventana y del contexto OpenGL.

Resultado

Correcto.

Representación del reloj

Objetivo

Comprobar el dibujo de la carátula y las manecillas.

Resultado

Correcto.

Representación de las obras

Objetivo

Verificar que las seis obras sean renderizadas correctamente.

Resultado

Correcto.

Rotación del anillo artístico

Objetivo

Confirmar la animación continua de los segmentos.

Resultado

Correcto.

Selección de la obra destacada

Objetivo

Comprobar que la obra activa cambie de acuerdo con la hora del sistema.

Resultado

Correcto.

Etiqueta informativa

Objetivo

Verificar la visualización de la hora y del artista activo.

Resultado

Correcto.

6. Pruebas de integración

Durante el desarrollo se realizaron múltiples procesos de integración mediante Git.

Se verificó:

integración de ramas individuales;
resolución de conflictos;
funcionamiento del proyecto después de cada merge;
compilación posterior a cada integración.

La versión final corresponde a una integración completa de todos los módulos desarrollados.

7. Pruebas de estabilidad

Se ejecutó la aplicación durante varios minutos verificando:

ausencia de cierres inesperados;
actualización continua del reloj;
rotación estable del anillo;
funcionamiento correcto del sistema de resaltado.

No se observaron fallos durante las pruebas realizadas.

8. Incidencias detectadas durante el desarrollo

Durante la implementación se identificaron diversos inconvenientes técnicos que fueron corregidos antes de la entrega final.

Entre ellos destacan:

conflictos de integración en Git;
errores de enlace por archivos no registrados en CMake;
diferencias entre generadores de Visual Studio y MinGW;
configuración incorrecta del entorno FreeGLUT;
conflictos durante procesos de rebase;
ajustes visuales en algunas obras artísticas;
corrección del sistema de resaltado y superposición gráfica.

Todas las incidencias fueron resueltas satisfactoriamente.

9. Validación de requisitos
Requisito	Estado
Reloj analógico funcional	✔ Cumplido
Uso de OpenGL	✔ Cumplido
Seis obras artísticas	✔ Cumplido
Arquitectura modular	✔ Cumplido
Animación	✔ Cumplido
Selección automática por hora	✔ Cumplido
Documentación técnica	✔ Cumplido
Control de versiones	✔ Cumplido

10. Conclusiones

Las pruebas realizadas permitieron validar el correcto funcionamiento del proyecto desde el punto de vista funcional, técnico y de integración.

Los resultados obtenidos demuestran que el sistema cumple los objetivos planteados durante la etapa de diseño, mantiene una arquitectura estable y presenta un comportamiento consistente durante su ejecución.

La combinación de pruebas de compilación, integración y funcionamiento permitió entregar una versión final estable del Reloj Artístico, preparada para su demostración y evaluación académica.

<!-- actualización -->