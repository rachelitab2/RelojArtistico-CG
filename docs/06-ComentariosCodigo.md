06 – Comentarios del Código
1. Introducción

La documentación interna del código constituye una parte fundamental del desarrollo de software, ya que facilita la comprensión del funcionamiento del sistema, mejora el mantenimiento y reduce el tiempo necesario para incorporar nuevos desarrolladores al proyecto.

Durante el desarrollo del Reloj Artístico se estableció una estrategia de documentación basada en comentarios descriptivos, procurando explicar la finalidad de cada módulo, las responsabilidades de las funciones principales y aquellos fragmentos cuya lógica pudiera resultar menos evidente.

El objetivo de esta estrategia no fue comentar cada línea de código, sino documentar las decisiones importantes y proporcionar el contexto suficiente para comprender la implementación.

2. Objetivo

El propósito de la documentación interna es:

Facilitar la lectura del código.
Explicar la finalidad de cada módulo.
Describir el comportamiento de las funciones principales.
Reducir la dependencia del conocimiento del desarrollador original.
Favorecer el mantenimiento futuro.
Servir como apoyo durante la revisión y evaluación académica del proyecto.
3. Estrategia de Comentarios

Se adoptó una estrategia basada en comentarios de alto nivel, evitando describir instrucciones evidentes del lenguaje C.

Los comentarios fueron utilizados principalmente para explicar:

propósito de un módulo;
responsabilidad de una función;
lógica de algoritmos específicos;
cálculos geométricos;
transformaciones gráficas;
decisiones importantes de implementación.

Esta metodología busca mantener el código limpio, evitando comentarios redundantes que podrían dificultar su lectura.

4. Convenciones Utilizadas

Durante el proyecto se siguieron las siguientes convenciones.

4.1 Comentarios de módulo

Cada archivo fuente inicia con comentarios que describen la responsabilidad general del módulo y su función dentro de la arquitectura del proyecto.

Ejemplo:

/*
 * Módulo encargado de administrar los segmentos del reloj artístico.
 * Controla la rotación, la selección de la obra activa
 * y coordina el renderizado de los seis segmentos.
 */
4.2 Comentarios de funciones

Las funciones públicas incluyen una breve descripción de su propósito.

Ejemplo:

/*
 * Dibuja todos los segmentos del reloj.
 */
void drawSegments(void)

No se describen instrucciones individuales cuando el nombre de la función resulta suficientemente claro.

4.3 Comentarios sobre algoritmos

Cuando una sección del código implementa un procedimiento matemático o geométrico específico, se añade una explicación previa indicando el objetivo del algoritmo.

Ejemplo:

/*
 * Calcula el radio disponible para escalar la obra
 * respetando el margen interno del segmento.
 */

Este tipo de comentario facilita comprender cálculos relacionados con transformaciones y geometría.

4.4 Comentarios sobre transformaciones

Las transformaciones realizadas mediante OpenGL (glTranslatef, glRotatef, glScalef) incluyen comentarios cuando la secuencia de operaciones resulta importante para comprender el resultado visual.

Ejemplo:

/*
 * Traslada el origen al centro del lienzo local
 * antes de aplicar la escala correspondiente.
 */
4.5 Comentarios sobre decisiones importantes

Las decisiones relevantes que afectan la arquitectura o el comportamiento del sistema son documentadas antes del bloque correspondiente.

Ejemplo:

/*
 * El motor determina la obra activa utilizando
 * la hora del sistema. Ninguna obra conoce
 * directamente el estado del reloj.
 */
5. Estilo Adoptado

Durante el proyecto se procuró mantener un estilo uniforme en toda la documentación interna.

Las principales características fueron:

comentarios breves;
lenguaje técnico;
frases descriptivas;
explicación del propósito y no de la sintaxis;
evitar comentarios redundantes.

No se utilizaron comentarios para describir instrucciones cuya intención resulta evidente a partir del propio código.

6. Organización de las Funciones

Las funciones fueron organizadas siguiendo un criterio de responsabilidad.

Generalmente cada módulo presenta el siguiente orden:

Inclusión de bibliotecas.
Declaración de constantes.
Variables estáticas.
Funciones auxiliares privadas (static).
Funciones públicas.
Finalización del módulo.

Esta organización facilita localizar rápidamente la información relevante.

7. Documentación de Funciones Públicas

Las funciones accesibles desde otros módulos son las principales candidatas a recibir documentación.

Entre ellas se encuentran:

Función	Propósito
initDisplay()	Inicializa OpenGL.
display()	Renderiza un fotograma completo.
drawClock()	Dibuja el reloj analógico.
getCurrentTime()	Obtiene la hora del sistema.
initSegments()	Inicializa los segmentos.
drawSegments()	Renderiza los seis segmentos.
updateSegments()	Actualiza la animación del sistema.

Estas funciones constituyen la interfaz pública del proyecto y representan los principales puntos de interacción entre módulos.

8. Documentación de Funciones Privadas

Las funciones declaradas como static representan detalles internos de implementación.

No todas requieren documentación extensa.

Se recomienda documentar únicamente aquellas cuya lógica resulte compleja, como por ejemplo:

cálculos geométricos;
algoritmos matemáticos;
transformaciones;
selección de la obra activa;
generación de efectos visuales.

Las funciones privadas simples pueden mantenerse sin comentarios adicionales cuando su nombre describe claramente su comportamiento.

9. Buenas Prácticas Aplicadas

Durante el desarrollo se siguieron las siguientes recomendaciones:

Utilizar nombres descriptivos para funciones y variables.
Mantener funciones con una única responsabilidad.
Evitar bloques excesivamente largos.
Agrupar funciones relacionadas.
Explicar únicamente aquello que no resulta evidente.
Actualizar los comentarios cuando cambia la implementación.
Mantener coherencia en el estilo de documentación.

Estas prácticas permiten que el código sea más fácil de comprender incluso antes de leer los comentarios.

10. Recomendaciones para Futuras Modificaciones

Cuando se incorporen nuevas funcionalidades al proyecto se recomienda:

Documentar las funciones públicas.
Explicar algoritmos complejos.
Mantener la organización modular existente.
Eliminar comentarios obsoletos.
Evitar comentarios redundantes.
Conservar la misma estructura utilizada en el resto del proyecto.

De esta forma la documentación evolucionará junto con el software y conservará su utilidad.

11. Estado de la Documentación del Código

Al finalizar el desarrollo del proyecto se estableció el siguiente criterio:

Todos los módulos principales deben contener comentarios descriptivos.
Las funciones públicas deben estar documentadas.
Los algoritmos importantes deben incluir explicaciones previas.
Las transformaciones geométricas relevantes deben indicar su propósito.
Las decisiones arquitectónicas complejas deben acompañarse de comentarios aclaratorios.

Este criterio busca equilibrar la cantidad de comentarios con la legibilidad del código, evitando una documentación excesiva o insuficiente.

12. Conclusiones

La documentación interna del código constituye un complemento esencial de la documentación técnica desarrollada para el proyecto.

La estrategia adoptada prioriza la claridad, la mantenibilidad y la comprensión del funcionamiento del sistema, proporcionando información útil tanto para los integrantes del equipo como para futuros desarrolladores.

El uso de comentarios orientados a responsabilidades, algoritmos y decisiones de diseño permitió mantener un código organizado, fácil de seguir y coherente con la arquitectura modular implementada durante el desarrollo del Reloj Artístico.

Asimismo, esta documentación servirá como apoyo para futuras ampliaciones del proyecto y facilitará las actividades de mantenimiento y revisión del software.