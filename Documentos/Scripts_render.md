# documentacion de los scripts de proceso de renderizado.

## StructureRenderer.h

se encarga de definir la interfaz para transformar el árbol de nodos de la interfaz en geometrías (vértices e índices) listas para ser enviadas a la GPU.

- Vertex: una estructura que almacena las coordenadas bidimensionales de un punto (position) y sus componentes de color RGBA normalizadas de 0.0 a 1.0 (color).

- RenderBatch: una estructura que empaqueta los arreglos de vértices (vertices) e índices (indices) requeridos para el dibujado.

- batch: guarda la geometría acumulada durante el procesamiento de la jerarquía de nodos.

metodos:

- processNode(...): recorre un nodo individual, calcula su posición absoluta acumulando el desplazamiento de sus padres y procesa a sus hijos de forma recursiva.

- generateQuad(...): genera los 4 vértices y los 6 índices correspondientes a un rectángulo en pantalla.

- buildGeometry(...): método principal que recibe el nodo raíz del árbol, inicia el recorrido recursivo y devuelve el lote geométrico completo (RenderBatch).
## StructureRenderer.cpp

contiene la implementación para recorrer la jerarquía de nodos de forma recursiva, calcular sus posiciones globales y construir la malla de renderizado.

- no define nuevos atributos, ejecuta la lógica del archivo StructureRender.h.

metodos:

- buildGeometry(...): reinicia los arreglos de vértices e índices, inicia el recorrido recursivo desde la raíz enviando un origen (0.0, 0.0) y retorna la estructura RenderBatch resultante.

- processNode(...): evalúa si el nodo es de tipo VisualNode para acumular sus coordenadas, valida si implementa Space2D para solicitar la creación de su rectángulo y procesa recursivamente a todos sus nodos hijos.

- generateQuad(...): calcula y agrega los 4 vértices del rectángulo utilizando las dimensiones recibidas y registra los 6 índices divididos en dos triángulos para formar el plano.
## PaintRender.h
se encarga de definir la interfaz para gestionar los recursos de OpenGL (buffers y shaders) y enviar la geometría a la tarjeta gráfica para su dibujo en pantalla.

- vao / vbo / ebo: identificadores de los objetos de OpenGL para almacenar la configuración de atributos, los vértices y los índices en la memoria de la GPU.

- shaderProgram: identificador del programa compilado de shaders (Vertex y Fragment shader).

- projMatrixLocation: ubicación de la variable uniforme para la matriz de proyección ortográfica.

metodos:

- compileShaders(): compila y enlaza los shaders básicos necesarios para procesar la posición y color de los vértices.

- checkShaderErrors(...) / checkGLError(...): funciones internas para detectar y reportar fallos en la compilación de shaders o llamadas a OpenGL.

- init(): inicializa los buffers de OpenGL y compila los programas de renderizado.

- setOrthographicProjection(...): ajusta la matriz de proyección según las dimensiones de la pantalla.

- render(...): recibe la estructura RenderBatch con la geometría y ejecuta el dibujado mediante llamadas a la GPU.

- cleanup(): libera de la memoria de la GPU los objetos vao, vbo, ebo y el programa de shaders.

- debugPrintGPUState(...): imprime en consola la información detallada del lote geométrico enviada a la GPU.
## PaintRender.cpp

contiene la implementación para configurar el pipeline gráfico de OpenGL, compilar los shaders embebidos y transferir los datos geométricos a la GPU para su renderizado final.

- vertexShaderSource / fragmentShaderSource: cadenas de texto globales que definen el código GLSL de los shaders de vértices y fragmentos para proyectar posiciones 2D y pasar los colores de los vértices.

metodos:

- ~PaintRender(): destructor de la clase que invoca la limpieza automática de los recursos gráficos.

- init(): compila los shaders y configura el VAO asignando la estructura de atributos de los vértices (posiciones de 2 elementos y colores RGBA de 4 elementos).

- setOrthographicProjection(...): genera una matriz ortográfica de 2D en pantalla basada en el ancho y alto recibidos y la pasa al shader como variable uniforme.

- render(...): recibe el lote RenderBatch, actualiza dinámicamente el VBO y EBO con los datos geométricos y ejecuta la instrucción de dibujo glDrawElements.

- compileShaders(): crea, compila y enlaza el programa de shaders reduciendo la memoria al eliminar los objetos individuales tras el enlace.

- checkShaderErrors(...): función auxiliar para consultar el estado de compilación o enlace del pipeline de shaders.

- cleanup(): elimina los objetos VAO, VBO, EBO y el programa de shaders de la VRAM de la GPU.