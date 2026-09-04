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