# Reloj Artístico

## Descripción

Proyecto final de la asignatura **Computación Gráfica I**, desarrollado en lenguaje **C** con **OpenGL**, **FreeGLUT** y **CMake**. Es un reloj analógico funcional cuya periferia está formada por seis segmentos circulares inspirados en reconocidas obras de la historia del arte, organizados en tres salas temáticas (18 obras en total). Cada obra se renderiza con primitivas gráficas de OpenGL y el sistema destaca automáticamente una de ellas según la hora actual del sistema.

## Objetivos

- Representar un reloj analógico funcional sincronizado con la hora del sistema.
- Recrear 18 obras de la historia del arte usando exclusivamente primitivas gráficas de OpenGL.
- Organizar las obras en tres salas temáticas, cada una con su propio ritmo de cambio (15/30/60 min).
- Aplicar una arquitectura modular que facilite el desarrollo colaborativo y el mantenimiento futuro.

## Tecnologías

- Lenguaje C (C99)
- OpenGL
- FreeGLUT
- CMake
- Git / GitHub
- MSYS2 / MinGW GCC

## Arquitectura

La aplicación está organizada en módulos independientes, cada uno con una única responsabilidad:

| Módulo | Responsabilidad |
| --- | --- |
| `main.c` | Punto de entrada, ventana y callbacks |
| `display.c` | Coordinación del renderizado y pantallas (carga, intro, reloj) |
| `clock.c` | Carátula del reloj y sincronización con la hora del sistema |
| `segments.c` | Motor de segmentos: salas, layout, resaltado y despacho por obra |
| `ui.c` | Paneles informativos, selector de sala, navegación manual |
| `audio.c` | Música de fondo por sala |
| `utils.c` / `texture.c` | Primitivas gráficas reutilizables y carga de imágenes |
| `segment_*.c` | Implementación de cada una de las 18 obras |

Las decisiones de diseño más importantes están documentadas como ADR (Architecture Decision Records) en `docs/02-ADR/`.

### Obras representadas

**Sala 15 min — Color y geometría**: Cuadrado negro y cuadrado rojo (Malevich) · Ritmos (Delaunay) · Vi la cifra 5 en oro (Demuth) · Composición VIII (Kandinsky) · Composición con rojo, azul y amarillo (Mondrian) · Ritmo de color (Riley)

**Sala 30 min — Luz, ritmo y color**: Nenúfares (Monet) · La noche estrellada (Van Gogh) · Symmetry Drawing No. 69 (Escher) · Un domingo en la Grande Jatte (Seurat) · Fiesta de primavera (Alma Thomas) · Castillo y sol (Paul Klee)

**Sala 60 min — Símbolo, ornamento y abstracción**: El beso (Klimt) · Color fields for kids (Rothko) · El Cisne, No. 17 (Hilma af Klint) · El Jardín (Joan Miró) · Composición Suprematista (Malevich) · Amorfa, fuga en dos colores (Kupka)

## Cómo compilar

```bash
cmake -B build -S . -G "MinGW Makefiles"
cmake --build build
```

El ejecutable se genera en `build/RelojArtistico.exe`.

## Cómo ejecutar

```bash
./build/RelojArtistico.exe
```

Controles dentro de la aplicación:

- **1 / 2 / 3** (o **4 / 5 / 6**): cambiar de sala (15 min / 30 min / 1 hora).
- **‹ Anterior / Siguiente ›**: navegar manualmente entre las obras de la sala activa (el cambio automático por hora sigue funcionando en paralelo).
- **M**, o el botón "sonido" en pantalla: silenciar/activar la música de fondo.
- Cualquier tecla avanza la pantalla de introducción.

## Integrantes

- Rachel
- Joseidy

## Licencia

Proyecto desarrollado con fines exclusivamente académicos (asignatura Computación Gráfica I, Ingeniería en Software).
