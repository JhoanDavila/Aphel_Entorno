# Documento con la descripcion del lector de apheLanguage

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