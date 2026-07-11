# 🕒 Reloj Artístico

Proyecto final de la asignatura **Computación Gráfica I** desarrollado en lenguaje **C** utilizando **OpenGL**, **FreeGLUT** y **CMake**.

El proyecto representa un reloj analógico funcional cuya periferia está formada por seis segmentos circulares inspirados en reconocidas obras de la historia del arte. Cada obra es renderizada mediante primitivas gráficas de OpenGL y el sistema destaca automáticamente una de ellas según la hora actual del sistema.

---

# Características

- Reloj analógico sincronizado con la hora del sistema.
- Seis obras artísticas distribuidas alrededor del reloj.
- Arquitectura modular basada en responsabilidades.
- Motor de segmentos independiente.
- Selección automática de la obra destacada según la hora.
- Animaciones y efectos visuales.
- Sistema de compilación mediante CMake.
- Desarrollo colaborativo utilizando Git y GitHub.

---

# Obras representadas

- 🌊 La Gran Ola de Kanagawa — Katsushika Hokusai
- 🌌 La Noche Estrellada — Vincent van Gogh
- 🎨 Composición abstracta — Wassily Kandinsky
- 🟥 Composición geométrica — Piet Mondrian
- 🪷 Los Nenúfares — Claude Monet
- ✨ El Beso — Gustav Klimt

---

# Tecnologías utilizadas

- Lenguaje C (C99)
- OpenGL
- FreeGLUT
- CMake
- Git
- GitHub
- MSYS2 / MinGW GCC

---

# Estructura del proyecto

```
RelojArtistico/
│
├── docs/
├── include/
├── src/
├── CMakeLists.txt
└── README.md
```

---

# Compilación

Configurar el proyecto:

```bash
cmake -B build -S . -G "MinGW Makefiles"
```

Compilar:

```bash
cmake --build build
```

El ejecutable será generado dentro de la carpeta **build**.

---

# Arquitectura

La aplicación está organizada en módulos independientes:

| Módulo | Responsabilidad |
|---------|-----------------|
| `main.c` | Inicialización del programa |
| `display.c` | Coordinación del renderizado |
| `clock.c` | Implementación del reloj |
| `segments.c` | Administración de los segmentos |
| `utils.c` | Primitivas gráficas reutilizables |
| `segment_*.c` | Representación de cada obra |

---

# Documentación

Toda la documentación del proyecto se encuentra en la carpeta **docs**.

Incluye:

- Investigación y propuesta
- ADR (Architecture Decision Records)
- Arquitectura del sistema
- Manual técnico
- Bitácora de desarrollo
- Comentarios del código
- Decisiones técnicas
- Pruebas y validación

---

# Integrantes

- Rachel
- Joseidy

---

# Asignatura

**Computación Gráfica I**

Ingeniería en Software

---

# Licencia

Proyecto desarrollado con fines exclusivamente académicos.