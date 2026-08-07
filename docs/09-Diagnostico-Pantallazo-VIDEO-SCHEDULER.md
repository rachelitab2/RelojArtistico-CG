# Diagnostico: VIDEO_SCHEDULER_INTERNAL_ERROR (0x119)

Fecha del levantamiento: 2026-07-29

## Resumen

El pantallazo observado corresponde a `VIDEO_SCHEDULER_INTERNAL_ERROR`
(`0x119`). Windows registro varios bugchecks con el mismo codigo, por lo que
no parece un cierre normal de la aplicacion ni un fallo aislado.

El proyecto `RelojArtistico` usa OpenGL/FreeGLUT con dibujo 2D inmediato. No se
observa carga de texturas, asignaciones dinamicas grandes ni escrituras
riesgosas en disco durante el renderizado. El patron apunta mas a una falla del
stack grafico de Windows/driver/GPU que a consumo excesivo del programa.

## Equipo detectado

- Modelo: Acer Predator PHN14-51
- CPU: Intel Core Ultra 7 155H, 16 nucleos, 22 hilos
- RAM: 16 GB
- GPU integrada: Intel Arc Graphics
  - Driver: `31.0.101.5126`
  - Fecha: 2023-12-18
- GPU dedicada: NVIDIA GeForce RTX 4070 Laptop GPU
  - Driver: `32.0.16.1062`
  - Fecha: 2026-06-10
- Esquema de energia activo: Acer

## Evidencia de Windows

Eventos `Microsoft-Windows-WER-SystemErrorReporting`, id `1001`:

- 2026-07-29 23:33:33: bugcheck `0x00000119`
- 2026-07-29 18:40:58: bugcheck `0x00000119`
- 2026-07-29 18:28:49: bugcheck `0x00000119`
- 2026-07-18 17:58:13: bugcheck `0x00000119`
- 2026-07-10 00:30:24: bugcheck `0x00000119`

Windows guardo minidumps en `C:\WINDOWS\Minidump\`.

Tambien aparecen eventos `Kernel-Power` id `41` inmediatamente despues de los
reinicios. Eso es consistente con pantallazos/reinicios inesperados.

## Observaciones del proyecto

Hallazgos relevantes:

- El programa renderizaba a ~60 FPS con `glutTimerFunc(16, ...)`.
- Habia dos llamadas a `glutPostRedisplay()` por ciclo: una en `timer()` y otra
  dentro de `updateSegments()`.
- No hay uso evidente de `malloc/free`, carga continua de imagenes, buffers
  OpenGL modernos, shaders, texturas grandes o escritura intensiva.
- El dibujo inmediato con `glBegin/glEnd` es antiguo, pero la carga geometrica
  del reloj es baja.

Cambios preventivos aplicados:

- Se redujo el timer a ~30 FPS (`33 ms`).
- Se dejo una sola solicitud de repintado por tick.
- Se agrego impresion de diagnostico de OpenGL:
  - `GL_VENDOR`
  - `GL_RENDERER`
  - `GL_VERSION`
- Se agrego una senal de alto rendimiento para NVIDIA Optimus en Windows
  (`NvOptimusEnablement`) para pedir que el ejecutable use la GPU dedicada
  cuando el driver lo permita.

Resultado observado tras imprimir OpenGL:

- `OpenGL vendor`: Intel
- `OpenGL renderer`: Intel(R) Arc(TM) Graphics
- `OpenGL version`: 4.6.0 - Build 31.0.101.5126

Esto confirma que el contexto OpenGL estaba corriendo sobre la iGPU Intel Arc,
no sobre la RTX 4070.

## Hipotesis principales

1. Driver grafico o conmutacion Intel/NVIDIA inestable.
   El equipo tiene graficos hibridos. Un programa OpenGL puede ejecutarse sobre
   Intel Arc o NVIDIA, y el cambio/negociacion entre GPUs puede disparar errores
   de scheduler grafico. En la primera prueba confirmada, OpenGL eligio Intel
   Arc.

2. Driver Intel Arc desactualizado frente al resto del sistema.
   El driver Intel detectado es de 2023. En equipos hibridos, aunque se piense
   que se usa la RTX, la iGPU participa en presentacion de pantalla y rutas de
   energia.

3. Software Acer/overlay/telemetria interactuando con graficos.
   Hay fallos repetidos de `InstalledPackagesAgent.exe` de Acer User Experience
   Improvement Program Service. No prueba causalidad, pero es una conducta
   anomala de software residente.

4. Estado de energia/suspension.
   Aparecen eventos NDIS de transicion de energia. En laptops gamer, cambios de
   energia, GPU mux/hybrid mode, overlays y drivers pueden combinarse mal.

5. Bug del proyecto como disparador, no como causa raiz.
   La doble solicitud de repintado y 60 FPS podian aumentar la frecuencia de uso
   del pipeline grafico. Eso puede detonar un fallo de driver, pero no deberia
   causar por si solo un bugcheck en un sistema sano.

## Protocolo de prueba recomendado

1. Ejecutar la version parchada desde consola para ver el renderer:

   ```powershell
   .\build\RelojArtistico.exe
   ```

   Guardar lo que imprima:

   - `OpenGL vendor`
   - `OpenGL renderer`
   - `OpenGL version`

2. Probar 10 minutos con la laptop conectada a corriente y modo de energia
   estable. No alternar GPU, no cambiar plan de energia durante la prueba.

3. Si sigue mostrando `Intel(R) Arc(TM) Graphics`, forzar el ejecutable a alto
   rendimiento desde Windows:

   `Configuracion > Sistema > Pantalla > Graficos > Agregar aplicacion de
   escritorio > build\RelojArtistico.exe > Opciones > Alto rendimiento`.

   Luego ejecutar de nuevo y verificar si `OpenGL renderer` cambia a NVIDIA.

4. Si no falla, probar 30 minutos. Observar temperatura/GPU desde PredatorSense,
   NVIDIA App o Administrador de tareas.

5. Si vuelve a fallar:

   - Anotar hora exacta.
   - Revisar si el renderer era Intel o NVIDIA.
   - Comparar con el evento nuevo en el Visor de eventos.

6. Actualizar primero desde canales del fabricante:

   - Acer: BIOS/firmware, chipset, Intel graphics, NVIDIA graphics.
   - NVIDIA: driver Game Ready/Studio compatible con laptop.
   - Intel: driver Arc, preferentemente validado por Acer si existe.

7. Probar una configuracion de GPU fija:

   - O bien forzar `RelojArtistico.exe` a NVIDIA en Configuracion de Windows.
   - O bien probar modo hibrido/deshabilitado segun PredatorSense/BIOS si el
     equipo lo permite.

## Senales de alarma

Conviene pausar pruebas y revisar drivers/soporte si ocurre cualquiera:

- Otro `0x119` despues de reducir FPS.
- Pantallazo con temperaturas normales y sin carga alta.
- Fallos tambien en otras apps 3D, navegadores con aceleracion, juegos o video.
- Artefactos visuales, pantalla negra, parpadeo persistente o reinicios al
  cambiar de bateria/corriente.

## Conclusion

El programa no muestra conductas tipicas de dano fisico o consumo excesivo de
recursos. La recurrencia del bugcheck `0x119` indica un problema real del stack
grafico. El proyecto probablemente actua como disparador porque abre un contexto
OpenGL y redibuja continuamente. La mitigacion aplicada reduce carga y agrega
visibilidad para identificar que GPU/renderizador participa en el fallo.

## Seguimiento (2026-08-07): crash reproducible al cambiar de sala

Con el sistema de salas de tiempo (3 salas, tecla 1/2/3), aparecio un cierre
reproducible del programa al presionar `2` (a veces tambien con `1`/`3`). A
diferencia del bugcheck `0x119` original (pantallazo azul), este caso es un
cierre del proceso sin reiniciar Windows.

### Evidencia recolectada

- `Get-WinEvent` (`Application Error`) confirmo 3 fallos con `Exception code:
  0xc0000005` (violacion de acceso) en `RelojArtistico.exe`, GPU `NVIDIA
  GeForce RTX 4070 Laptop GPU`, driver `610.62`.
- Se analizaron los `.dmp` de `%LOCALAPPDATA%\CrashDumps\` y se reprodujo el
  crash en vivo adjuntando `gdb -p <pid>` al proceso (WER dejo de generar
  volcados nuevos tras varias ocurrencias, asi que el attach en vivo fue la
  unica forma de seguir viendo el stack en intentos posteriores).
- El stack de todos los crashes es identico: 100% dentro de `nvoglv64.dll`,
  arrancando en `DrvPresentBuffers` (llamandose a si misma varias veces) y
  bajando a funciones internas sin simbolos, sin ningun frame de codigo propio
  del proyecto. En el ultimo intento, el crash ocurrio en un **thread distinto**
  al principal (uno de los threads internos del driver, no el que corre
  `glutMainLoop`).

### Mitigaciones probadas (en orden)

1. **Reducir carga de dibujo inmediato de las obras nuevas de la sala de 30
   min.** `segment_seurat.c` generaba del orden de 2000 puntos por fotograma
   en su sector, cada uno con `drawFilledCircle()` (64 segmentos, pensado
   para circulos grandes) para un punto de radio ~0.01 -- mas de 100 mil
   vertices por fotograma solo en esa obra. Se agrego `drawDot()` local de 8
   segmentos y se bajo la densidad de la trama. Tambien se bajaron los
   segmentos de las elipses de `segment_escher.c` (16 -> 10).
2. **Sacar un `glutPostRedisplay()` duplicado.** El manejador de teclado
   (`keyboard()` en `main.c`) pedia un repintado manual ademas del que ya
   pide el timer de 33ms (~30 FPS) cada ciclo -- el mismo patron de "doble
   solicitud de repintado" que motivo la mitigacion original de este
   documento, reintroducido por la funcionalidad de salas (no existia cuando
   se escribio la seccion de arriba).
3. **Forzar VSync explicito via `wglSwapIntervalEXT(1)`** despues de
   `glutCreateWindow()`, apuntando directamente a `DrvPresentBuffers` (la
   funcion exacta donde crashea) por si el driver estaba recibiendo pedidos
   de presentacion mas rapido de lo que podia procesar.

Las 3 mitigaciones son mejoras validas de rendimiento/higiene y se mantienen
en el codigo, pero **ninguna cambio el crash**: el stack de la excepcion es
byte-identico (mismas direcciones relativas dentro de `nvoglv64.dll`) antes y
despues de cada una. Eso, sumado a que el ultimo crash ocurrio en un thread
que no es el nuestro, es evidencia fuerte de que la causa raiz esta en el
driver grafico de esta maquina especifica, no en un patron de llamadas GL
del proyecto que se pueda corregir desde C.

### Decision

Se dejan las 3 mitigaciones aplicadas (no se revierten, son buenas practicas
independientes del bug) y se continua el desarrollo. Pendiente para quien
retome este diagnostico: probar con el driver NVIDIA actualizado (protocolo
de la seccion "Protocolo de prueba recomendado" arriba, pasos 6-7) para
confirmar si una version mas nueva resuelve el crash.
