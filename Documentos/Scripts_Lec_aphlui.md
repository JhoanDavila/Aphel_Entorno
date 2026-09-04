

# Descripcion de los script del lector de aphlui
## Cleaner.h:

se encarga de definir la interfaz para limpiar archivos de texto, quitando líneas innecesarias como comentarios y espacios vacíos.

- no define atributos, funciona como una clase utilitaria.

metodos:

- removeCommentsAndEmptyLines(...): método estático que recibe las líneas originales de un archivo y devuelve solo aquellas que contienen código válido.
## Cleaner.cpp

contiene la lógica real para procesar y filtrar el contenido de los archivos.

- no define nuevos atributos, ejecuta la lógica del archivo Cleaner.h.

metodos:

- removeCommentsAndEmptyLines(...): recorre cada línea, recorta el texto eliminando los comentarios a partir del carácter # y guarda únicamente las líneas que conservan código o texto válido.
## ErrorManager.h

se encarga de registrar, consultar y gestionar los errores de sintaxis detectados durante el análisis.

- SyntaxError: una estructura que guarda el número de línea (line) y el mensaje descriptivo del error (message).

- errors: una lista estática que almacena todos los errores detectados de forma global.

metodos:

- logError(...): guarda un nuevo error indicando la línea y el mensaje.

- hasErrors(): devuelve true si hay al menos un error registrado en la lista.

- printErrors(): muestra o imprime en pantalla todos los errores acumulados.

- clear(): vacía la lista para limpiar todos los errores registrados.
## ErrorManager.cpp

contiene la lógica real para almacenar, revisar e imprimir los errores de sintaxis en la consola.

- no define nuevos atributos, ejecuta la lógica del archivo ErrorManager.h.

metodos:

- logError(...): crea un registro de error con la línea y el mensaje recibidos y lo guarda en la lista global errors.

- hasErrors(): revisa si la lista errors contiene elementos registrados.

- printErrors(): recorre la lista de errores e imprime cada uno en la salida de errores (std::cerr) con formato de línea y mensaje.

- clear(): elimina todos los elementos contenidos en la lista errors.
## Token.h

se encarga de definir la estructura básica para clasificar y almacenar fragmentos de texto (tokens) leídos del código.

- TokenType: una enumeración que define las categorías de tokens posibles (NODE_TYPE, NODE_NAME, PROPERTY, VALUE, UNKNOWN).

- type: indica la categoría asignada al token.

- value: el texto o contenido literal del token.

- line: el número de línea donde se encontró el token.

- indentLevel: la cantidad de espacios de sangría que determina la jerarquía visual del nodo.

metodos:

- no contiene métodos, es solo una estructura de datos.
## Tokenizer.h

se encarga de definir la interfaz para dividir el texto limpio en fichas individuales o componentes manejables (tokens).

- no define atributos, funciona como una clase utilitaria.

metodos:

- tokenize(...): método estático que recibe las líneas sin comentarios y devuelve una lista de objetos Token clasificados con su tipo, contenido, línea e indentación.
## Tokenizer.cpp

contiene la lógica para analizar línea por línea el código limpio, extraer su sangría y convertir las instrucciones en una lista estructurada de tokens, reportando errores de sintaxis si los encuentra.

- validNodeTypes: un conjunto estático de tipos de nodos reconocidos ("Node", "VisualNode", "Space2D", "Window").

metodos:

- tokenize(...): recorre cada línea calculando su nivel de sangría, identifica si se trata de una propiedad (.propiedad: valor) o de la declaración de un nodo (Tipo Nombre:), valida su sintaxis con ErrorManager y retorna la lista completa de tokens generados.
## Parser.h

se encarga de definir la interfaz para convertir una lista de tokens en una estructura de árbol de nodos reales, e incluye funciones auxiliares para validar y transformar los valores de sus propiedades.

- no define atributos, funciona como una clase utilitaria.

metodos:

- tryParseVector2D(...): intenta convertir un texto con formato de coordenadas en un objeto Vector2D (x, y).

- tryParseColor(...): intenta convertir un texto con formato de color en un objeto Color (RGBA).

- isValidStringFormat(...): verifica si un texto tiene un formato de cadena de caracteres válido (entre comillas).

- parse(...): método principal que procesa la lista de tokens y construye el árbol jerárquico de nodos, devolviendo la raíz del árbol.
## Parser.cpp

contiene la lógica para validar formatos de datos, asignar propiedades a los nodos y construir el árbol jerárquico según la sangría de los tokens.

- vecRegex / hexWrapperRegex / hexRegex / prefixRegex: expresiones regulares utilizadas para validar y procesar textos con formatos de vectores (x, y) y colores (HEX, RGB, RGBA).

metodos:

- isValidStringFormat(...): valida que un texto esté correctamente encerrado entre comillas dobles.

- tryParseVector2D(...): extrae los valores numéricos de un texto en formato de tupla (x, y) y los guarda en un objeto Vector2D.

- tryParseColor(...): convierte valores de color en formatos HEX, rgb(), rgba() o tuplas a componentes numéricos RGBA (0-255).

- parse(...): método principal que procesa la lista de tokens, valida que la raíz sea un nodo "Window", asigna sus propiedades mediante la sangría y retorna la raíz del árbol de nodos generado.
## AphluiReader.h
se encarga de definir la interfaz principal para cargar y leer archivos con extensión .aphlui.

- no define atributos, funciona como una clase utilitaria.

metodos:

- loadFromFile(...): método estático que recibe la ruta de un archivo, ejecuta todo el proceso de lectura (limpieza, tokenización y parseo) y devuelve el nodo raíz del árbol resultante.
## AphluiReader.cpp

contiene la implementación del proceso completo de carga y lectura de archivos .aphlui, orquestando todos los módulos del lector.

- no define nuevos atributos, ejecuta la lógica del archivo AphluiReader.h.

metodos:

- loadFromFile(...): borra errores previos, abre y lee el archivo de texto línea por línea, pasa el contenido por el limpiador (Cleaner), el tokenizador (Tokenizer) y el analizador (Parser), y retorna el nodo raíz del árbol si no se detectaron errores en el camino.