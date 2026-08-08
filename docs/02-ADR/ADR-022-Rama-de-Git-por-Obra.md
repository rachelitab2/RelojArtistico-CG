ADR-015 – Una rama de Git por obra artística
Estado
Aceptado.

Contexto
El ADR-005 ya establecía que cada obra vive en su propio módulo de código (segment_*.c/.h) para reducir conflictos de integración en un desarrollo colaborativo vía Git. Al ampliar el catálogo a 18 obras (ADR-011), con el trabajo desarrollado obra por obra a lo largo de varias sesiones, se hizo explícito un criterio adicional: no solo el código de cada obra debe estar aislado en su propio módulo, sino que el historial de Git debe reflejar esa misma separación, una rama por obra.

Problema
Decidir si el trabajo de implementar múltiples obras en una misma sesión se agrupa en una sola rama/commit grande, o si cada obra mantiene su propia rama, y qué hacer cuando un cambio no pertenece claramente a una sola obra (cambios al motor de segmentos, al catálogo compartido, o fixes transversales).

Alternativas consideradas

Una rama por sesión de trabajo — Agrupar todas las obras implementadas en una misma sesión bajo una sola rama. Se descartó como práctica general: mezcla en un mismo diff el trabajo de obras no relacionadas entre sí. De hecho, esto ocurrió una vez (Seurat y Escher quedaron combinadas en feature/sistema-salas) y debió deshacerse manualmente separando cada obra a su propia rama.

Commits directos a develop sin ramas intermedias — Se descartó: elimina la posibilidad de probar/revertir una obra puntual antes de integrarla, y no deja rastro de qué obra generó qué cambio.

Una rama feature/segment-<obra> por cada obra, mergeada a develop tras confirmación visual — La alternativa seleccionada: cada obra se implementa en su propia rama, creada desde develop, y solo se mergea después de que la obra fue confirmada visualmente. Cambios que tocan una obra existente pero no crean una nueva van en la rama del cambio más afín, o una rama de fix/ dedicada si es transversal (ver fix/gpu-scheduler-crash, fix/assets-working-directory).

Decisión
Cada obra nueva sigue el flujo: crear feature/segment-<nombre> desde develop, prototipar y ajustar hasta confirmación visual del usuario, commit acotado a los archivos de esa obra, push, y merge --no-ff a develop.

Cuando un mismo archivo compartido (segments.c, artwork_catalog.c, CMakeLists.txt) recibe cambios de más de una obra en paralelo, se usa git add -p (staging por hunk) para separar cada cambio a su commit/rama correspondiente en vez de forzar una única rama a cargar con todo.

Cambios que no son una obra nueva (mitigaciones de rendimiento, fixes de arranque, alias de teclado) van en su propia rama fix/<descripcion>, siguiendo el mismo criterio de "una unidad de cambio, una rama".

Justificación
Mantener el historial de Git alineado con la separación modular del código (ADR-005) hace que cada rama sea una unidad de revisión y reversión completa: si una obra necesita rehacerse (como ocurrió con Rothko, ver ADR-012) o descartarse, el cambio queda contenido a su propia rama sin arrastrar el trabajo de otras obras.

git add -p para separar hunks de un mismo archivo compartido evita el trade-off entre "una rama por obra" y "los archivos compartidos se tocan en varias obras a la vez".

Consecuencias

Ventajas: Cada obra es revisable, revertible y referenciable de forma aislada. Los conflictos de merge en archivos compartidos son pequeños y localizados, no masivos. El criterio es consistente entre "obra nueva" y "cambio transversal".

Desventajas: Multiplica la cantidad de ramas del repositorio, requiriendo limpieza periódica una vez que todo el trabajo relevante está mergeado a develop. Separar hunks de un archivo compartido entre varias ramas es una operación manual más propensa a error que un commit único.

Evidencia en el proyecto
El historial de ramas: feature/segment-escher, feature/segment-paul-klee, feature/segment-hilma-af-klint, feature/segment-joan-miro, feature/segment-kupka, feature/segment-rothko-v2, feature/segment-malevich-suprematist, fix/gpu-scheduler-crash, fix/assets-working-directory, entre otras, todas mergeadas a develop vía merge --no-ff.

Relación con otros ADR
Extiende a nivel de control de versiones el criterio de aislamiento por obra ya establecido en ADR-005. Es la práctica de proceso que acompañó la implementación de ADR-011 al escalar el catálogo de 6 a 18 obras.
