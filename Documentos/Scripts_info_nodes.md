# Descripcion de uso de scripts

## Node.h

se encarga de crear los contenedores de los atributos y los metodos de el nodo Node.

- type: un string para su tipo
- name: un string para su nombre 
- parent: un enlace para saber quien es su padre
- children: una lista de sus nodos hijos

metodos:

- Node(...): se crea el nodo ingresando, nombre, tipo, padre
- ~Node: se encarga de limpiar el nodo de la memoria si no se usa.
- getType(): Te dice qué tipo de nodo es.
- addChild(...): Le engancha un nuevo nodo hijo a su lista.
## Node.cpp

contiene la lógica y el funcionamiento real de los métodos definidos en la cabecera.

- no define nuevos atributos, ejecuta la lógica del archivo Node.h.

metodos:

- Node(...): asigna directamente el tipo, nombre y padre recibidos al momento de crear el nodo.

- addChild(...): verifica que el hijo exista, le asigna este nodo como su nuevo padre y lo agrega a la lista de hijos.
## VisualNode.h

se encarga de extender el nodo básico para agregarle coordenadas visuales (posición en pantalla).

- Vector2D: una estructura con coordenadas (x, y) que inicia en 0,0.

- position: guarda la posición x e y del nodo dentro del plano.

metodos:

- VisualNode(...): crea el nodo asignándole por defecto el tipo "VisualNode", junto con su nombre, padre y posición.
## VisualNode.cpp

contiene la implementación del constructor del nodo visual.

- no define nuevos atributos, ejecuta la lógica del archivo VisualNode.h.

metodos:

- VisualNode(...): crea el nodo pasando el tipo, nombre y padre a la clase base (Node) e inicializa su posición en las coordenadas (0, 0).
## 2DSpace.h

se encarga de agregar dimensiones (ancho y alto) al nodo visual para representar un espacio o área bidimensional.

- Size2D: una estructura con dimensiones (width, height) que inicia en 0,0.

- size: guarda el ancho y el alto del espacio 2D.

metodos:

- Space2D(...): crea el nodo asignándole por defecto el tipo "Space2D", junto con su nombre y padre opcional.
## 2DSpace.cpp

contiene la implementación del constructor para la clase Space2D.

- no define nuevos atributos, ejecuta la lógica del archivo Space2D.h.

metodos:

- Space2D(...): crea el nodo pasando el tipo, nombre y padre a la clase base (VisualNode) e inicializa su tamaño en (0, 0).
## Window.h

se encarga de agregar características de una ventana gráfica (título y color de fondo) al espacio 2D.

- Color: una estructura con canales RGBA (rojo, verde, azul, transparencia) que inicia en blanco opaco.

- title: un string con el título de la ventana.

- backgroundColor: guarda el color de fondo de la ventana.

metodos:

- rFloat() / gFloat() / bFloat() / aFloat(): convierten los valores de color de rango 0-255 a flotantes de 0.0 a 1.0 para gráficos (como OpenGL).

- WindowNode(...): crea el nodo asignándole por defecto el tipo "Window", junto con su nombre y padre opcional.
## Window.cpp

contiene la implementación del constructor para la clase WindowNode.

- no define nuevos atributos, ejecuta la lógica del archivo WindowNode.h.

metodos:

- WindowNode(...): crea el nodo pasando el tipo, nombre y padre a la clase base (Space2D), le asigna el título por defecto "Untitled Window" e inicializa el color de fondo con sus valores base (blanco opaco).