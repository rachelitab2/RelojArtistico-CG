ADR-022 – Alias de teclado 4/5/6 y resolución de assets relativa al ejecutable
Estado

Aceptado.

Contexto

Dos problemas de robustez, independientes entre sí pero ambos tocando main.c, surgieron durante el trabajo de la sala de 1 hora:

(a) Como parte de la investigación del crash del driver gráfico (ver ADR-021), era necesario descartar si el cierre abrupto estaba atado específicamente a las teclas 1/2/3 o al cambio de sala en sí, independientemente de qué tecla lo dispara.

(b) Todas las rutas de assets del catálogo (artwork_catalog.c, app_config.c) son relativas ("assets/images/...", "assets/audio/..."), lo que las hace depender del directorio de trabajo (CWD) en el momento de lanzar el proceso, no de dónde esté físicamente el ejecutable. Si algo lanza el .exe con el CWD en otro lado (un acceso directo, "Run" del IDE, abrir el .exe desde build/ en vez de la raíz del proyecto), las imágenes y el audio quedan en blanco sin ningún aviso al usuario.

Problema

Resolver ambos problemas sin introducir dependencias nuevas ni cambiar el formato de las rutas ya usadas en todo el catálogo.

Alternativas consideradas
(a) Reemplazar las teclas 1/2/3 en vez de agregar alias

Cambiar directamente el mapeo de teclas para las pruebas.

Se descartó: hubiera roto la interacción ya conocida por el usuario sin necesidad, cuando el objetivo era solo diagnosticar, no rediseñar el control.

(b) Convertir todas las rutas del catálogo a absolutas en tiempo de compilación

Hardcodear o generar rutas absolutas según la máquina de build.

Se descartó: rompe la portabilidad del proyecto entre máquinas y con el control de versiones (las rutas quedarían atadas a una carpeta específica).

(b) Buscar assets/ en varias ubicaciones candidatas en cada carga de recurso

Que loadTexture()/PlaySoundA() prueben varias rutas relativas por archivo.

Se descartó: duplica la lógica de búsqueda en cada punto de carga de recursos en vez de resolverlo una sola vez al arrancar.

(a) Agregar 4/5/6 como alias de 1/2/3; (b) fijar el CWD una vez al arrancar

Las alternativas seleccionadas: (a) case '4'/'5'/'6' se agregan a los mismos case de setActiveRoom() ya existentes para 1/2/3, sin quitar las teclas originales; (b) resolveAssetsWorkingDirectory() (main.c) ubica la carpeta del ejecutable vía GetModuleFileNameA y sube hasta 2 niveles buscando una carpeta assets/, fijando el CWD del proceso ahí antes de glutInit().

Decisión

(a) keyboard() en main.c: cada case de setActiveRoom(1/2/3) ahora acepta tanto la tecla original (1/2/3) como su alias (4/5/6) mediante fall-through de switch. Confirmado que el crash del driver ocurre igual con las teclas alias, descartando que estuviera atado a una tecla específica (ver ADR-021).

(b) resolveAssetsWorkingDirectory() se llama al inicio de main(), antes de glutInit(). Si no encuentra assets/ tras 2 niveles hacia arriba desde la carpeta del ejecutable, deja el directorio de trabajo como estaba — mismo criterio de degradación seguro que loadTexture() y PlaySoundA(): nunca falla ni interrumpe el arranque, en el peor caso el catálogo queda como antes de este fix.

Justificación

(a) Agregar alias en vez de reemplazar preserva la interacción ya validada por el usuario mientras habilita el diagnóstico sin fricción.

(b) Resolver el CWD una única vez al arrancar, en el punto de entrada, es más simple y centralizado que parchear cada llamada a loadTexture()/PlaySoundA() por separado, y sigue el mismo principio de "nunca romper el arranque" que ya regía esas funciones.

Consecuencias
Ventajas
El usuario puede probar el cambio de sala con dos juegos de teclas distintos sin perder la interacción original.
El ejecutable funciona igual sin importar desde dónde se lo lance (build/, un acceso directo, el IDE), siempre que assets/ exista a hasta 2 niveles de la carpeta del .exe.
Ambos cambios son de bajo riesgo: no tocan la lógica de renderizado ni el formato de las rutas existentes en el catálogo.
Desventajas
main.c ahora tiene 6 case de tecla para 3 acciones, ligeramente más ruido en el switch.
Si alguna vez el ejecutable se distribuye en una estructura de carpetas con assets/ a más de 2 niveles de distancia, el fix no lo encuentra y se vuelve al comportamiento anterior (silencioso, sin aviso).
Evidencia en el proyecto

case '4'/'5'/'6' en keyboard() (main.c); resolveAssetsWorkingDirectory() y su llamada al inicio de main() (main.c).

Relación con otros ADR

(a) es una herramienta de diagnóstico para ADR-021 (mitigación del crash del driver). (b) es independiente, motivado por robustez de arranque en distintos contextos de lanzamiento, sin relación directa con el sistema de salas (ADR-019) más allá de compartir el mismo archivo main.c.
