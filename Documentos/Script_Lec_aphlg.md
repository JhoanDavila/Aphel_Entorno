# Documento con la descripción del lector de AphelLanguage
## Cleaner.h

declara la interfaz encargada de limpiar el código fuente de los archivos de script .aphlg eliminando comentarios de una y varias líneas.

- no define atributos, solo ofrece un método estático de utilidad.

metodos:

- clean(const std::string& source): recibe la cadena completa del archivo .aphlg y devuelve el texto filtrado preservando los saltos de línea para el rastreo de errores.

## Cleaner.cpp

contiene la implementación basada en una máquina de estados para filtrar los comentarios sin alterar las líneas de código nativo.

- no define atributos, procesa el texto carácter por carácter evaluando las secuencias de escape.

metodos:

- clean(const std::string& source): recorre el texto y detecta el inicio de comentarios multilínea (///) y su cierre (\\\), así como los comentarios de una sola línea (//), removiendo todo el texto descartable hasta hallar el salto de línea (\n) correspondiente.

## ErrorManager.h

se encarga de registrar, consultar y gestionar los errores de sintaxis o ejecución detectados durante el análisis e interpretación de scripts .aphlg.

- SyntaxError: estructura que guarda el número de línea (line) y el mensaje descriptivo del error (message).

- errors: lista estática (std::vector<SyntaxError>) que almacena de forma global todos los errores detectados en AphelG.

metodos:

- logError(int line, const std::string& message): registra un nuevo error en la lista especificando la línea y la descripción del problema.

- hasErrors(): devuelve un valor booleano que indica si se ha registrado al menos un error.

- printErrors(): muestra o imprime en pantalla (std::cerr) todos los errores acumulados con su formato correspondiente.

- clear(): vacía la lista estática para limpiar todos los errores registrados.

## ErrorManager.cpp

contiene la lógica real para almacenar, revisar e imprimir los errores de sintaxis en la consola.

- no define nuevos atributos, ejecuta la lógica del archivo ErrorManager.h.

metodos:

- logError(int line, const std::string& message): crea un registro de tipo SyntaxError con la línea y el mensaje recibidos y lo guarda en la lista estática errors.

- hasErrors(): revisa si la lista errors contiene elementos registrados.

- printErrors(): recorre la lista de errores e imprime cada uno en la salida de errores (std::cerr) con formato de línea y mensaje.

- clear(): elimina todos los elementos contenidos en la lista errors
## Token.h

declara las estructuras y tipos de datos fundamentales para la representación de los elementos léxicos del lenguaje `AphelG`.

- TokenType: enumeración fuertemente tipada (`enum class`) que categoriza los tipos de token reconocidos por el analizador, incluyendo palabras reservadas (`File`, `Render`), símbolos (`=`, `(`, `)`), identificadores, cadenas de texto delimitadas por comillas (`STRING_LITERAL`), fin de archivo (`END_OF_FILE`) y elementos no reconocidos (`UNKNOWN`).

- Token: estructura contenedora que representa un token extraído. Guarda el tipo (`type`), el valor de texto (`value`), la línea (`line`) y la columna (`column`) para el diagnóstico de errores.

metodos:

- Token(TokenType t, const std::string& v, int l, int c): constructor parametrizado que inicializa todos los miembros de la estructura.

## Tokenizer.h

declara la clase responsable de realizar el análisis léxico sobre el código fuente filtrado de `.aphlg`, transformando la cadena de texto en un flujo ordenado de tokens.

- source: cadena de texto que contiene el código fuente procesado.
- index: posición actual del cursor dentro de la cadena `source`.
- line: contador de la línea actual en el análisis.
- column: contador de la columna actual en el análisis.

metodos:

- Tokenizer(const std::string& sourceCode): constructor que inicializa el analizador con el código fuente y posiciona el cursor en la posición inicial.
- tokenize(): método principal que recorre el texto y devuelve la colección de tokens procesados (`std::vector<Token>`).
- peek(): examina el carácter actual en la posición del cursor sin consumirlo.
- advance(): consume el carácter actual, avanza el cursor y actualiza los contadores de línea y columna.
- isAtEnd(): evalúa si el cursor ha alcanzado el final del texto.
- skipWhitespace(): descarta espacios, tabulaciones, retornos de carro y saltos de línea.
- makeString(): procesa y extrae secuencias delimitadas por comillas dobles (`"..."`), retornando un token de tipo `STRING_LITERAL` o `UNKNOWN` si la cadena no se cierra.
- makeIdentifierOrKeyword(): procesa subsecuencias alfanuméricas para retornar palabras clave (`KEYWORD_FILE`, `KEYWORD_RENDER`) o identificadores generales (`IDENTIFIER`).

## Tokenizer.cpp

contiene la implementación del análisis léxico optimizado para el lenguaje `AphelG`.

- no define atributos adicionales, implementa el recorrido de la máquina de estados sobre el texto fuente.

metodos:

- Tokenizer(const std::string& sourceCode): inicializa el estado del iterador de lectura.
- peek(): retorna `\0` si se alcanza el final del archivo o el carácter correspondiente a la posición actual.
- advance(): incrementa `index`, evalúa saltos de línea para reiniciar la columna e incrementar la línea, y retorna el carácter consumido.
- isAtEnd(): compara la posición actual con la longitud total del buffer.
- skipWhitespace(): consume caracteres continuos de espacio en blanco actualizando las coordenadas de lectura.
- makeString(): acumula los caracteres delimitados por comillas evitando copias intermedias de memoria y devuelve un `Token` utilizando semántica de movimiento (`std::move`). Si la comilla no se cierra antes de `EOF`, etiqueta el token como `UNKNOWN`.
- makeIdentifierOrKeyword(): extrae la subcadena alfanumérica y valida si coincide con alguna palabra reservada (`File` o `Render`). Si no coincide, la retorna etiquetada como `IDENTIFIER`.
- tokenize(): ejecuta el bucle de escaneo reservando espacio en memoria previa en el vector de salida. Identifica comillas, asignaciones, paréntesis, palabras clave, e identificadores. Cualquier carácter fuera del alfabeto permitido se empaqueta como un token `UNKNOWN` delegando el reporte al `Parser`. Finaliza agregando el token `END_OF_FILE`.