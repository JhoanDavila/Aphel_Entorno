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
## WindowSO.h

se encarga de definir la interfaz para inicializar el contexto del sistema operativo, crear la ventana nativa a partir de las propiedades de un nodo WindowNode y gestionar el bucle de eventos.

window: un puntero a la ventana nativa de la biblioteca GLFW (GLFWwindow*).

- painter: una instancia de la clase PaintRender para orquestar los shaders, buffers y dibujado en la GPU.

- structure: una instancia de la clase StructureRender para procesar el árbol de nodos y generar la geometría (RenderBatch).

metodos:

- WindowSO(): constructor por defecto que asigna nullptr al puntero de la ventana nativa.

- ~WindowSO(): destructor que asegura la liberación de los recursos llamando a close().

- init(...): recibe un WindowNode, extrae su tamaño y título para crear la ventana física con GLFW e inicializa GLEW junto con PaintRender.

- startLoop(...): ejecuta el bucle de renderizado mientras la ventana siga abierta, limpiando el fondo con backgroundColor, convirtiendo la jerarquía de nodos en vértices y dibujándolos.

- close(): destruye la ventana nativa de GLFW, libera la VRAM desde PaintRender y finaliza la biblioteca gráfica.

## WindowSO.cpp

contiene la implementación real de la creación de la ventana nativa y la orquestación del bucle principal de la aplicación.

no define nuevos atributos, ejecuta la lógica del archivo WindowSO.h.

metodos:

- WindowSO(): constructor que asigna nullptr al puntero de la ventana nativa.

- ~WindowSO(): destructor que invoca close() para liberar los recursos.

- init(...): valida que el nodo recibido sea válido, inicializa GLFW, extrae size y title del objeto WindowNode, crea la ventana física, activa el contexto gráfico, arranca GLEW y establece la proyección ortográfica en PaintRender.

- startLoop(...): ejecuta el bucle de refresco continuo en pantalla; aplica glClearColor mapeando las funciones rFloat(), gFloat(), bFloat(), aFloat() del WindowNode, solicita la geometría a StructureRender y la dibuja con PaintRender.

- close(): libera los recursos asignados en la VRAM a través de PaintRender, destruye la ventana de GLFW y finaliza el entorno gráfico.
## RenderEngine.h

se encarga de orquestar el ciclo de vida del subsistema gráfico (inicialización de ventana, bucle de pintado y liberación) a partir de un árbol de nodos previamente cargado.

- appWindow: instancia de WindowSO que abstrae el manejo de la ventana nativa de GLFW y el contexto de OpenGL.

metodos:

- init(...): recibe el rootNode, valida que sea de tipo WindowNode e inicializa la ventana con appWindow.init().

- run(...): inicia el bucle de eventos e interacción visual invocando appWindow.startLoop().

- stop(): cierra la ventana y destruye los recursos invocando appWindow.close().

## RenderEngine.cpp

contiene la implementación para coordinar los componentes de renderizado aislándolos del punto de entrada principal.

- no define nuevos atributos, ejecuta la lógica definida en RenderEngine.h.

metodos:

- init(...): realiza la conversión del nodo raíz a WindowNode y arranca WindowSO.

- run(...): pasa la ejecución al bucle principal de renderizado.

- stop(): invoca la limpieza de la ventana gráfica.