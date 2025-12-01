# 🍔 Sistema de Gestión de Restaurante TGI Fridays

## 📋 Índice
- [Descripción General](#-descripción-general)
- [Características Principales](#-características-principales)
- [Arquitectura del Sistema](#-arquitectura-del-sistema)
- [Estructuras de Datos Implementadas](#-estructuras-de-datos-implementadas)
- [Módulos del Sistema](#-módulos-del-sistema)
- [Funcionalidades Detalladas](#-funcionalidades-detalladas)
- [Flujo de Trabajo](#-flujo-de-trabajo)
- [Tecnologías Utilizadas](#-tecnologías-utilizadas)
- [Instalación y Ejecución](#-instalación-y-ejecución)

---

## 🎯 Descripción General

**TB2_Fridays** es un sistema integral de gestión de restaurante desarrollado en C++ que implementa múltiples estructuras de datos avanzadas para optimizar las operaciones de un restaurante tipo TGI Fridays. El sistema permite gestionar pedidos, reservas, clientes, menú y pagos de manera eficiente mediante el uso de algoritmos y estructuras de datos complejas.

### Propósito del Sistema
El sistema está diseñado para:
- **Automatizar** la gestión completa de un restaurante
- **Optimizar** el manejo de pedidos y reservas mediante estructuras de datos eficientes
- **Mejorar** la experiencia del cliente con un sistema de lealtad
- **Facilitar** la administración del menú y recursos del restaurante
- **Demostrar** la aplicación práctica de estructuras de datos avanzadas en un sistema real

---

## ✨ Características Principales

### Para Clientes
- 🍽️ **Pedidos en Local y Delivery**: Sistema dual para atención presencial y a domicilio
- 📅 **Sistema de Reservas**: Gestión inteligente de reservas con selección de mesas
- 🎁 **Programa de Lealtad**: Acumulación de puntos y categorías (Regular, Premium, VIP)
- 💳 **Múltiples Métodos de Pago**: Efectivo, tarjeta de crédito y débito
- 🧾 **Comprobantes Electrónicos**: Generación de boletas y facturas

### Para Administradores
- 📊 **Gestión de Pedidos**: Cola de procesamiento y historial completo
- 🍕 **Administración de Menú**: CRUD completo de productos con búsqueda avanzada
- 🪑 **Gestión de Mesas**: Sistema de mapeo y optimización de ubicación
- 👥 **Gestión de Clientes**: Base de datos con búsqueda y ordenamiento
- 📈 **Estadísticas**: Análisis de rendimiento del sistema
- 🔄 **Algoritmos de Ordenamiento**: Múltiples criterios de ordenación

---

## 🏗️ Arquitectura del Sistema

### Diagrama de Componentes

```
┌─────────────────────────────────────────────────────────────┐
│                    FRIDAYS SYSTEM                           │
│                   (Clase Principal)                         │
└────────────────┬────────────────────────────┬───────────────┘
                 │                            │
    ┌────────────▼───────────┐   ┌───────────▼────────────┐
    │  ControladoraCliente   │   │  ControladoraAdmin     │
    │  - Modo Cliente        │   │  - Gestión Pedidos     │
    │  - Realizar Pedidos    │   │  - Gestión Menú        │
    │  - Hacer Reservas      │   │  - Gestión Reservas    │
    │  - Procesar Pagos      │   │  - Gestión Clientes    │
    └────────────┬───────────┘   └───────────┬────────────┘
                 │                            │
    ┌────────────┴────────────────────────────┴────────────┐
    │                                                       │
┌───▼─────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐
│GestorPedidos│  │GestorReserva│ │GestorClientes│ │GestorPagos│
└─────────────┘  └──────────┘  └──────────┘  └──────────┘
```

### Estructura de Carpetas

```
TB2_Fridays/
│
├── TB2_Fridays.cpp          # Punto de entrada principal
├── Fridays.h                # Clase principal del sistema
│
├── Controladoras/
│   ├── ControladoraAdmin.h     # Controlador del panel administrativo
│   └── ControladoraCliente.h   # Controlador del panel de clientes
│
├── Estructuras de Datos/
│   ├── ArbolAVL.h           # Árbol AVL auto-balanceado
│   ├── ArbolTreap.h         # Árbol Treap con prioridades
│   ├── HashTable.h          # Tabla hash con colisiones
│   ├── Grafo.h              # Grafo para mesas del restaurante
│   ├── Lista.h              # Lista enlazada genérica
│   ├── Cola.h               # Cola FIFO
│   ├── Pila.h               # Pila LIFO
│   └── Nodo.h               # Nodo genérico
│
├── Gestores/
│   ├── GestorPedidos.h      # Gestión de pedidos
│   ├── GestorReserva.h      # Gestión de reservas y mesas
│   ├── GestorClientes.h     # Gestión de clientes
│   ├── GestorPagos.h        # Gestión de pagos
│   ├── GestorArchivos.h     # Persistencia de datos
│   ├── GestorBackup.h       # Respaldos del sistema
│   ├── GestorOrdenamiento.h # Algoritmos de ordenamiento
│   └── GestorListaEspera.h  # Lista de espera de reservas
│
├── Modelos/
│   ├── Cliente.h            # Modelo de cliente
│   ├── Pedido.h             # Modelo de pedido
│   ├── Producto.h           # Modelo de producto
│   ├── Mesa.h               # Modelo de mesa
│   ├── Reserva.h            # Modelo de reserva
│   ├── Pago.h               # Clase base de pago
│   ├── PagoEfectivo.h       # Pago en efectivo
│   ├── PagoTarjeta.h        # Pago con tarjeta
│   ├── Comprobante.h        # Clase base de comprobante
│   ├── Boleta.h             # Boleta de venta
│   └── Factura.h            # Factura
│
├── Menu/
│   ├── Carta.h              # Carta principal del restaurante
│   ├── CartaLocal.h         # Carta para consumo local
│   ├── CartaDelivery.h      # Carta para delivery
│   ├── PlatoPrincipal.h     # Platos principales
│   ├── Bebida.h             # Bebidas
│   └── Postre.h             # Postres
│
├── Utilidades/
│   ├── UtilidadesConsola.h  # Funciones de consola
│   ├── ConsolaColor.h       # Colores para la consola
│   ├── Funcionalidades.h    # Funciones auxiliares
│   └── GeneradorDataSet.h   # Generación de datos de prueba
│
└── Data/                    # Archivos de persistencia
    ├── clientes.txt
    ├── menu.txt
    ├── reservas.txt
    ├── mesas.txt
    ├── pedidos.txt
    ├── historialPedidos.txt
    ├── lista_espera.txt
    ├── clientes_dataset.csv
    ├── pedidos_dataset.csv
    ├── reservas_dataset.csv
    └── lista_espera_dataset.csv
```

---

## 🗂️ Estructuras de Datos Implementadas

### 1. **Árbol AVL (ArbolAVL.h)**

#### Propósito
Almacenamiento y búsqueda balanceada de datos del sistema garantizando operaciones en tiempo O(log n).

#### Características Técnicas
- **Auto-balanceo**: Mantiene factor de balance entre -1 y 1
- **Rotaciones**: Implementa rotaciones simples y dobles (derecha, izquierda, izquierda-derecha, derecha-izquierda)
- **Operaciones**: Inserción, eliminación, búsqueda en O(log n)
- **Recorridos**: InOrder, PreOrder, PostOrder

#### Uso en el Sistema
- Gestión de reservas ordenadas por código
- Organización de clientes por ID
- Índice de productos por categoría

```cpp
// Ejemplo de uso
ArbolAVL<Reserva> arbolReservas;
arbolReservas.insertar(reserva);
bool existe = arbolReservas.buscar(reserva);
```

#### Complejidad
- Inserción: O(log n)
- Búsqueda: O(log n)
- Eliminación: O(log n)
- Espacio: O(n)

---

### 2. **Árbol Treap (ArbolTreap.h)**

#### Propósito
Estructura híbrida que combina árbol binario de búsqueda con heap, utilizada para manejar prioridades.

#### Características Técnicas
- **Ordenamiento Dual**: Por clave (BST) y por prioridad (heap)
- **Aleatorización**: Usa prioridades aleatorias para balanceo probabilístico
- **Rotaciones**: Mantiene propiedad de heap mediante rotaciones

#### Uso en el Sistema
- **Lista de Espera de Reservas**: Clientes ordenados por prioridad
  - Prioridad alta: Clientes VIP
  - Prioridad media: Clientes Premium
  - Prioridad baja: Clientes Regular

```cpp
// Ejemplo de uso
ArbolTreap<Cliente*> listaEspera;
listaEspera.insertar(cliente, prioridad);
Cliente* siguiente = listaEspera.obtenerMayorPrioridad();
```

#### Complejidad
- Inserción: O(log n) esperado
- Búsqueda: O(log n) esperado
- Eliminación: O(log n) esperado

---

### 3. **Tabla Hash (HashTable.h)**

#### Propósito
Búsqueda ultra-rápida de productos del menú por ID o nombre.

#### Características Técnicas
- **Función Hash**: Algoritmo djb2 para strings, módulo para enteros
- **Resolución de Colisiones**: Encadenamiento (chaining) con listas enlazadas
- **Redimensionamiento Dinámico**: Factor de carga 0.75
- **Genérica**: Soporta cualquier tipo de clave-valor

#### Uso en el Sistema
- **Búsqueda de Productos**: Por ID en tiempo constante
- **Búsqueda por Nombre**: Acceso rápido a productos del menú
- **Cache de Clientes**: Búsqueda rápida por DNI

```cpp
// Ejemplo de uso
HashTable<int, Producto> menuHash;
menuHash.insertar(producto.getId(), producto);
Producto* p = menuHash.buscar(101); // O(1)
```

#### Complejidad
- Inserción: O(1) promedio
- Búsqueda: O(1) promedio
- Eliminación: O(1) promedio
- Peor caso: O(n) con muchas colisiones

#### Estadísticas del Sistema
El sistema proporciona métricas de rendimiento:
- Capacidad de la tabla
- Número de elementos
- Factor de carga actual
- Número de colisiones
- Longitud máxima de cadena

---

### 4. **Grafo (Grafo.h - GrafoMesas)**

#### Propósito
Modelar la distribución física de mesas del restaurante y sus conexiones.

#### Características Técnicas
- **Tipo**: Grafo no dirigido ponderado
- **Representación**: Lista de adyacencias
- **Pesos**: Distancias euclidianas entre mesas
- **Coordenadas**: Posicionamiento 2D de cada mesa

#### Algoritmos Implementados

##### BFS (Breadth-First Search)
```cpp
Lista<int> buscarMesasPorZona(int mesaInicio, const string& zonaDeseada)
```
- **Propósito**: Encontrar todas las mesas de una zona específica
- **Uso**: Búsqueda de mesas disponibles en Interior, Terraza o VIP
- **Complejidad**: O(V + E)

##### DFS (Depth-First Search)
```cpp
Lista<int> encontrarMesasUnibles(int mesaInicio, int capacidadRequerida)
```
- **Propósito**: Encontrar mesas adyacentes que se pueden unir para grupos grandes
- **Uso**: Reservas para grupos grandes (más de 8 personas)
- **Complejidad**: O(V + E)

##### Dijkstra
```cpp
Lista<int> rutaMasCorta(int mesaOrigen, int mesaDestino)
```
- **Propósito**: Encontrar la ruta más corta entre dos mesas
- **Uso**: Optimización de recorrido de meseros
- **Complejidad**: O(V²) con implementación simple

#### Distribución de Mesas
```
Interior (Mesas 1-15):
  - Regulares: Mesas 1-10 (capacidad 2-4)
  - Familiares: Mesas 11-15 (capacidad 6-8)

Terraza (Mesas 16-20):
  - Capacidad: 4 personas

VIP (Mesas 21-25):
  - Capacidad: 2-6 personas
  - Zona exclusiva
```

---

### 5. **Cola (Cola.h)**

#### Propósito
Gestión FIFO (First In, First Out) de pedidos pendientes.

#### Características Técnicas
- **Tipo**: Cola genérica basada en lista enlazada
- **Operaciones**: Encolar, Desencolar, Peek

#### Uso en el Sistema
- **Cola de Pedidos**: Procesamiento secuencial de pedidos
  1. Cliente realiza pedido → Encolar
  2. Cocina procesa → Desencolar
  3. Garantiza orden justo (primer pedido, primero se procesa)

```cpp
Cola<Pedido*> colaPedidos;
colaPedidos.encolar(nuevoPedido);
Pedido* siguiente = colaPedidos.desencolar();
```

#### Complejidad
- Encolar: O(1)
- Desencolar: O(1)
- Peek: O(1)

---

### 6. **Pila (Pila.h)**

#### Propósito
Gestión LIFO (Last In, First Out) del historial de pedidos.

#### Características Técnicas
- **Tipo**: Pila genérica basada en lista enlazada
- **Operaciones**: Apilar, Desapilar, Peek

#### Uso en el Sistema
- **Historial de Pedidos**: Los pedidos completados se apilan
- **Acceso Rápido**: El último pedido completado está en el tope
- **Persistencia**: Se guarda en archivo al cerrar el sistema

```cpp
Pila<Pedido*> historialPedidos;
historialPedidos.apilar(pedidoCompletado);
```

#### Complejidad
- Apilar: O(1)
- Desapilar: O(1)
- Peek: O(1)

---

### 7. **Lista Enlazada (Lista.h)**

#### Propósito
Estructura de datos fundamental utilizada como base para otras estructuras.

#### Características Técnicas
- **Tipo**: Lista doblemente enlazada genérica
- **Operaciones**: Inserción (inicio/final/posición), eliminación, búsqueda, acceso por índice

#### Uso en el Sistema
- Base para Cola y Pila
- Almacenamiento de productos en pedidos
- Lista de clientes
- Adyacencias en el grafo

```cpp
Lista<Producto*> productos;
productos.insertarFinal(producto);
Producto* p = productos.obtenerEnPosicion(5);
```

#### Complejidad
- Insertar inicio/final: O(1)
- Insertar en posición: O(n)
- Buscar: O(n)
- Acceso por índice: O(n)

---

## 📦 Módulos del Sistema

### 1. **Módulo de Gestión de Pedidos (GestorPedidos.h)**

#### Funcionalidades
- **Creación de Pedidos**: Local y Delivery
- **Procesamiento**: Sistema de cola para procesamiento secuencial
- **Historial**: Almacenamiento en pila con persistencia
- **Ordenamiento**: Por nombre de cliente, fecha, total
  - QuickSort para ordenamiento rápido
  - MergeSort para ordenamiento estable
- **Estadísticas**: Cálculo de totales, promedios, pedidos más frecuentes

#### Flujo de un Pedido
```
1. Cliente selecciona productos
2. Pedido se crea con estado "pendiente"
3. Cliente procesa pago
4. Estado cambia a "pagado"
5. Pedido se encola en cola de cocina
6. Administrador procesa pedido
7. Estado cambia a "completado"
8. Pedido se mueve al historial (pila)
9. Se guarda en archivo de persistencia
```

---

### 2. **Módulo de Gestión de Reservas (GestorReserva.h)**

#### Funcionalidades Principales

##### Gestión de Reservas
- **Crear Reserva**: Asignación inteligente de mesas
- **Buscar Reserva**: Por código (AVL) en O(log n)
- **Cancelar Reserva**: Liberación automática de mesa
- **Listar por Fecha**: Filtrado por día específico

##### Sistema de Mesas (Grafo)
- **25 Mesas Totales** distribuidas en 3 zonas
- **Tipos de Mesa**: Regular, Familiar
- **Capacidades**: 2, 4, 6, 8 personas
- **Estados**: Disponible, Ocupada, Reservada

##### Lista de Espera (Treap)
- **Gestión por Prioridad**: Clientes ordenados por categoría
  - VIP: Prioridad 100
  - Premium: Prioridad 50
  - Regular: Prioridad 10
- **Procesamiento Automático**: Cuando se libera una mesa

##### Algoritmos de Optimización
```cpp
// Sugerir mesa óptima
void sugerirMesaOptima(int personas, const string& zona) {
    1. Buscar mesas disponibles en zona
    2. Filtrar por capacidad >= personas
    3. Seleccionar mesa con menor diferencia de capacidad
    4. Usar Dijkstra para encontrar mesa más accesible
}

// Calcular ruta entre mesas
void calcularRuta(int mesaOrigen, int mesaDestino) {
    1. Aplicar algoritmo de Dijkstra
    2. Calcular distancia total
    3. Mostrar ruta paso a paso
}
```

---

### 3. **Módulo de Gestión de Clientes (GestorClientes.h)**

#### Sistema de Lealtad

##### Categorías de Clientes
```
Regular (0-99 puntos)
  ├─ Sin beneficios especiales
  └─ Prioridad baja en lista de espera

Premium (100-499 puntos)
  ├─ Descuentos especiales
  ├─ Prioridad media en lista de espera
  └─ Acceso a promociones

VIP (500+ puntos)
  ├─ Máximo descuento
  ├─ Prioridad máxima en reservas
  ├─ Acceso zona VIP
  └─ Atención preferencial
```

##### Acumulación de Puntos
- Cada pedido: +20 puntos
- Pedidos mayores a S/100: +50 puntos adicionales
- Recomendación de amigos: +100 puntos

#### Funcionalidades
- **Registro de Clientes**: Creación con validación de DNI
- **Búsqueda**: Por DNI usando HashTable O(1)
- **Actualización Automática**: Categoría según puntos
- **Ordenamiento**: Por ID usando algoritmos avanzados
- **Estadísticas**: Clientes VIP, Premium, totales

---

### 4. **Módulo de Gestión de Menú (Carta.h)**

#### Estructura del Menú

```
Carta Principal
├── HashTable por ID (búsqueda O(1))
├── HashTable por Nombre (búsqueda O(1))
└── Lista de Productos

CartaLocal (hereda de Carta)
├── Productos disponibles para consumo local
└── Precios estándar

CartaDelivery (hereda de Carta)
├── Productos disponibles para delivery
├── Precios con recargo
└── Tarifa de envío: S/8.90
```

#### Categorías de Productos
- **Platos Principales**: Hamburguesas, carnes, pastas
- **Bebidas**: Gaseosas, jugos, cervezas, cócteles
- **Postres**: Helados, tortas, brownies

#### Funcionalidades
- **CRUD Completo**: Create, Read, Update, Delete
- **Búsqueda Múltiple**:
  - Por ID (O(1))
  - Por nombre exacto (O(1))
  - Por nombre parcial (O(n))
  - Por categoría (O(n))
- **Ordenamiento**: Por precio (ascendente/descendente)
- **Disponibilidad**: Control de stock en tiempo real
- **Persistencia**: Guardado automático en archivo

---

### 5. **Módulo de Gestión de Pagos (GestorPagos.h)**

#### Métodos de Pago Implementados

##### Efectivo (PagoEfectivo.h)
```cpp
class PagoEfectivo : public Pago {
    double montoPagado;
    double calcularVuelto() {
        return montoPagado - monto;
    }
    string procesarPago() {
        if (montoPagado >= monto)
            return "APROBADO";
        return "RECHAZADO - Monto insuficiente";
    }
}
```

##### Tarjeta (PagoTarjeta.h)
```cpp
class PagoTarjeta : public Pago {
    string numeroTarjeta;
    string tipoTarjeta; // Crédito o Débito
    string banco;

    string procesarPago() {
        // Simulación de validación
        if (numeroTarjeta.length() >= 16)
            return "APROBADO";
        return "RECHAZADO - Tarjeta inválida";
    }
}
```

#### Comprobantes

##### Boleta (Boleta.h)
- Para personas naturales
- Requiere: Nombre, DNI
- IGV incluido en precio

##### Factura (Factura.h)
- Para empresas
- Requiere: RUC (11 dígitos), Razón Social
- IGV discriminado

---

### 6. **Módulo de Ordenamiento (GestorOrdenamiento.h)**

#### Algoritmos Implementados

##### QuickSort
```cpp
void quickSort(Pila<Pedido*>& pila, bool ascendente)
```
- **Uso**: Ordenar pedidos por total
- **Complejidad**: O(n log n) promedio, O(n²) peor caso
- **Ventaja**: In-place, rápido en promedio

##### MergeSort
```cpp
void mergeSort(Pila<Pedido*>& pila, bool ascendente)
```
- **Uso**: Ordenar pedidos por fecha o nombre
- **Complejidad**: O(n log n) garantizado
- **Ventaja**: Estable, predecible

##### Comparadores Personalizados
```cpp
// Por nombre de cliente
bool compararPorNombre(Pedido* a, Pedido* b, bool ascendente)

// Por fecha (DD/MM/YYYY)
bool compararPorFecha(Pedido* a, Pedido* b, bool ascendente)

// Por total
bool compararPorTotal(Pedido* a, Pedido* b, bool ascendente)
```

---

### 7. **Módulo de Persistencia (GestorArchivos.h)**

#### Archivos del Sistema

##### Formato de Archivos

**clientes.txt**
```
# Formato: ID,Nombre,Apellido,Telefono,DNI,Direccion,Categoria,Puntos
1,Juan,Pérez,987654321,12345678,Av. Siempre Viva 123,Regular,50
2,María,García,998877665,87654321,Jr. Los Olivos 456,Premium,250
```

**menu.txt**
```
# Formato: ID,Nombre,Precio,Categoria,Disponible
101,Hamburguesa Clásica,25.90,plato,1
102,Coca Cola,5.00,bebida,1
```

**reservas.txt**
```
# Formato: Codigo,ClienteDNI,NumeroMesa,Fecha,Hora,NumPersonas,Estado,Observaciones
RES001,12345678,5,01/12/2024,19:00,4,confirmada,Cumpleaños
```

**historialPedidos.txt**
```
# Formato: NumeroPedido,Fecha,ClienteNombre,ClienteDNI,Total,Estado,TipoServicio
1,01/12/2024,Juan Pérez,12345678,75.50,completado,local
```

#### Sistema de Backup (GestorBackup.h)
```cpp
void crearBackupsTotales() {
    1. Copiar clientes.txt → clientes_backup.txt
    2. Copiar menu.txt → menu_backup.txt
    3. Copiar reservas.txt → reservas_backup.txt
    4. Copiar historialPedidos.txt → pedidos_backup.txt
    5. Timestamp: YYYYMMDD_HHMMSS
}
```

---

### 8. **Módulo de Generación de Datos (GeneradorDataSet.h)**

#### Generadores de Datos de Prueba

##### Dataset de Clientes
```cpp
int cargarDatasetClientes()
```
- Genera 100-500 clientes aleatorios
- Nombres y apellidos aleatorios
- DNIs únicos de 8 dígitos
- Distribución: 60% Regular, 30% Premium, 10% VIP

##### Dataset de Pedidos
```cpp
int cargarDatasetPedidos()
```
- Genera 50-200 pedidos
- Fechas de último mes
- Productos aleatorios del menú
- Estados: 80% completados, 20% pendientes

##### Dataset de Reservas
```cpp
int cargarDatasetReservas()
```
- Genera 30-100 reservas
- Fechas próximos 30 días
- Horarios: 12:00-15:00 (almuerzo), 19:00-23:00 (cena)
- Distribución de zonas: 50% Interior, 30% Terraza, 20% VIP

---

## 🔄 Funcionalidades Detalladas

### Panel de Cliente

#### 1. Realizar Pedido Local
```
Flujo:
1. Seleccionar "Pedido Local"
2. Registrar/Identificar cliente (DNI)
3. Navegar menú:
   - Ver Platos Principales
   - Ver Bebidas
   - Ver Postres
   - Ver Menú Completo
4. Agregar productos al carrito
5. Revisar carrito (ver/eliminar productos)
6. Confirmar pedido
7. Seleccionar método de pago
8. Procesar pago
9. Generar comprobante (Boleta/Factura)
10. Recibir puntos de lealtad (+20)
11. Pedido entra a cola de cocina
```

#### 2. Realizar Pedido Delivery
```
Flujo:
1. Seleccionar "Pedido Delivery"
2. Registrar cliente con dirección de entrega
3. Ver menú de delivery (productos disponibles)
4. Agregar productos al carrito
5. Ver subtotal + tarifa delivery (S/8.90)
6. Confirmar pedido
7. Procesar pago
8. Generar comprobante
9. Recibir puntos de lealtad
10. Tiempo estimado: 45 minutos
```

#### 3. Hacer Reserva
```
Flujo:
1. Seleccionar "Hacer Reserva"
2. Registrar/Identificar cliente
3. Ingresar fecha (DD/MM/YYYY)
4. Ingresar hora (HH:MM)
5. Ingresar número de personas
6. Sistema muestra mesas disponibles:
   - Filtra por capacidad >= personas
   - Muestra por zona (Interior/Terraza/VIP)
   - Indica disponibilidad
7. Seleccionar mesa
8. Agregar observaciones (opcional)
9. Confirmar reserva
10. Recibir código de reserva (RES001)
```

---

### Panel de Administrador

#### 1. Gestión de Pedidos

##### Ver Cola de Pedidos
- Lista todos los pedidos pendientes
- Información: #Pedido, Cliente, Total, Estado, Tipo
- Orden: FIFO (primero en entrar, primero se muestra)

##### Procesar Pedido
```
Acción:
1. Tomar primer pedido de la cola
2. Mostrar resumen del pedido
3. Simular preparación (3 segundos)
4. Cambiar estado a "completado"
5. Mover a historial (pila)
6. Guardar en archivo
7. Liberar de cola
```

##### Ver Historial de Pedidos
- Muestra todos los pedidos completados
- Opción para ver comprobante de cualquier pedido
- Información detallada de cada pedido

##### Ordenar Historial
```
Criterios disponibles:
1. Por Nombre del Cliente (A-Z o Z-A)
2. Por Fecha del Pedido (más antiguo/reciente)

Algoritmo usado: MergeSort (estable)

Post-ordenamiento:
- Ver lista completa ordenada
- Seleccionar pedido para ver detalles
- Ver comprobante
```

##### Cargar Dataset
- Carga archivo `pedidos_dataset.csv`
- Importa 50-200 pedidos de prueba
- Actualiza historial automáticamente

---

#### 2. Gestión de Menú

##### Ver Menú Completo
- Muestra todos los productos organizados por categoría
- Información: ID, Nombre, Precio, Disponibilidad

##### Ordenar por Precio
```
Opciones:
1. Menor a Mayor (ascendente)
2. Mayor a Menor (descendente)

Algoritmo: QuickSort
Tiempo: O(n log n)
```

##### Agregar Producto
```
Datos requeridos:
- ID único (verificación automática)
- Nombre del producto
- Precio (S/)
- Categoría (plato/bebida/postre)
- Disponibilidad (Sí/No)

Validaciones:
- ID no debe existir
- Precio debe ser > 0
- Categoría debe ser válida

Persistencia: Guardado automático en menu.txt
```

##### Eliminar Producto
```
Proceso:
1. Mostrar menú completo
2. Solicitar ID del producto
3. Buscar producto (HashTable O(1))
4. Mostrar información del producto
5. Confirmar eliminación
6. Eliminar de HashTable y archivo
7. Actualizar menú
```

##### Buscar Producto
```
Métodos de búsqueda:

1. Por ID (O(1))
   - Búsqueda directa en HashTable
   - Resultado inmediato

2. Por Nombre Exacto (O(1))
   - Búsqueda en HashTable de nombres
   - Case-sensitive

3. Por Nombre Parcial (O(n))
   - Búsqueda lineal
   - Retorna todas las coincidencias
   - Case-insensitive

4. Por Categoría (O(n))
   - Filtra por tipo
   - Retorna lista completa de categoría

5. Ver Estadísticas HashTable
   - Capacidad actual
   - Elementos almacenados
   - Factor de carga
   - Número de colisiones
   - Longitud máxima de cadena
```

##### Modificar Producto
```
Campos modificables:
- Nombre
- Precio
- Categoría
- Disponibilidad

Proceso:
1. Buscar producto por ID
2. Mostrar valores actuales
3. Solicitar nuevos valores (Enter para mantener)
4. Confirmar cambios
5. Actualizar en HashTable
6. Guardar en archivo
```

---

#### 3. Gestión de Reservas

##### Ver Todas las Reservas
- Lista completa de reservas activas
- Recorrido InOrder del AVL
- Información: Código, Cliente, Mesa, Fecha, Hora, Personas

##### Buscar Reserva por Código
```
Entrada: RES001
Algoritmo: Búsqueda en AVL
Complejidad: O(log n)
Resultado:
  - Información completa de la reserva
  - Estado (confirmada/cancelada)
  - Datos del cliente
```

##### Reservas del Día
- Filtrar por fecha específica (DD/MM/YYYY)
- Ordenadas por hora
- Útil para planificación diaria

##### Cancelar Reserva
```
Proceso:
1. Ingresar código de reserva
2. Buscar en AVL
3. Mostrar información
4. Confirmar cancelación
5. Cambiar estado a "cancelada"
6. Liberar mesa (estado → disponible)
7. Actualizar archivo
```

##### Estadísticas de Reservas
```
Métricas mostradas:
- Total de reservas activas
- Reservas por zona:
  * Interior
  * Terraza
  * VIP
- Promedio de personas por reserva
- Horarios más solicitados
- Mesas más reservadas
```

##### Ver Todas las Mesas
```
Información por mesa:
- Número de mesa (1-25)
- Tipo (Regular/Familiar)
- Capacidad (2,4,6,8 personas)
- Ubicación (Interior/Terraza/VIP)
- Estado (Disponible/Ocupada/Reservada)
```

##### Cargar Dataset de Reservas
- Importa archivo `reservas_dataset.csv`
- Genera 30-100 reservas de prueba
- Recarga árbol AVL automáticamente

---

#### 4. Gestión de Lista de Espera (Treap)

##### Ver Lista de Espera
```
Visualización:
- Ordenados por prioridad (mayor a menor)
- Información por solicitud:
  * Nombre del cliente
  * Categoría (VIP/Premium/Regular)
  * Prioridad (100/50/10)
  * Número de personas
  * Fecha de solicitud
  * Tiempo de espera
```

##### Procesar Siguiente en Espera
```
Algoritmo:
1. Obtener cliente con mayor prioridad (raíz del Treap)
2. Verificar mesas disponibles para su capacidad
3. Si hay mesa disponible:
   a. Crear reserva automática
   b. Asignar mesa
   c. Eliminar de lista de espera
   d. Notificar al cliente
4. Si no hay mesa:
   a. Informar al administrador
   b. Cliente permanece en lista
```

##### Cargar Dataset de Lista de Espera
- Importa archivo `lista_espera_dataset.csv`
- Genera 20-50 solicitudes
- Asigna prioridades según categoría de cliente

---

#### 5. Gestión con Grafo de Mesas

##### Ver Mapa del Restaurante
```
Visualización:
=== Interior ===
  Mesa  1 [Regular] Cap:2 Pos:(2.0,2.0) -> Cercanas: 2,6
  Mesa  2 [Regular] Cap:4 Pos:(4.0,2.0) -> Cercanas: 1,3,7
  ...

=== Terraza ===
  Mesa 16 [Regular] Cap:4 Pos:(2.0,12.0) -> Cercanas: 17
  ...

=== VIP ===
  Mesa 21 [Familiar] Cap:6 Pos:(2.5,15.0) -> Cercanas: 22
  ...

Estadísticas:
- Total de mesas: 25
- Total de conexiones: 45
```

##### Calcular Ruta entre Mesas
```
Algoritmo: Dijkstra

Entrada:
- Mesa origen (1-25)
- Mesa destino (1-25)

Salida:
- Ruta óptima: [1 → 2 → 7 → 12]
- Distancia total: 8.5 unidades
- Número de pasos: 3

Uso práctico:
- Optimizar recorrido de meseros
- Planificar distribución de pedidos
- Calcular tiempos de servicio
```

##### Sugerir Mesa Óptima
```
Parámetros de entrada:
1. Número de personas
2. Zona preferida (Interior/Terraza/VIP)

Algoritmo de selección:
1. Filtrar mesas por zona
2. Filtrar mesas disponibles
3. Filtrar por capacidad >= personas
4. Seleccionar mesa con:
   - Menor exceso de capacidad
   - Más accesible (menos conexiones)
5. Si no hay disponible:
   - Buscar mesas adyacentes para unir (DFS)
   - Sugerir mesa en otra zona

Salida:
- Número de mesa sugerida
- Capacidad
- Ubicación exacta
- Mesas vecinas disponibles
```

##### Estadísticas del Grafo
```
Métricas del grafo:
- Total de nodos: 25
- Total de aristas: 45
- Mesa más conectada: Mesa X con Y conexiones
- Distribución por zona:
  * Interior: 15 mesas
  * Terraza: 5 mesas
  * VIP: 5 mesas
- Grado promedio: 3.6
- Distancia promedio entre mesas: 4.2 unidades
```

---

#### 6. Gestión de Clientes

##### Ver Todos los Clientes
```
Listado completo con:
- ID
- Nombre completo
- DNI
- Teléfono
- Categoría (con color)
  * VIP: Verde
  * Premium: Amarillo
  * Regular: Blanco
- Puntos de lealtad
- Pedidos realizados
```

##### Buscar Cliente por DNI
```
Entrada: 12345678
Algoritmo: Búsqueda en HashTable
Complejidad: O(1)

Salida:
┌─────────────────────────────────┐
│   INFORMACIÓN DEL CLIENTE       │
├─────────────────────────────────┤
│ ID: 001                         │
│ Nombre: Juan Pérez              │
│ DNI: 12345678                   │
│ Teléfono: 987654321             │
│ Categoría: VIP                  │
│ Puntos: 520                     │
│ Pedidos realizados: 26          │
│ Cliente desde: 01/01/2024       │
└─────────────────────────────────┘
```

##### Ver Clientes VIP/Premium
- Filtra solo clientes con categoría VIP o Premium
- Ordenados por puntos (mayor a menor)
- Muestra total de clientes especiales

##### Cargar Dataset de Clientes
- Importa archivo `clientes_dataset.csv`
- Genera 100-500 clientes de prueba
- Actualiza HashTable y archivos

##### Ordenar Clientes por ID
```
Opciones:
1. Ascendente (ID menor a mayor)
2. Descendente (ID mayor a menor)

Algoritmo: QuickSort
Complejidad: O(n log n)

Post-ordenamiento:
- Muestra lista ordenada completa
- Útil para auditorías
- Exportable a archivo
```

---

#### 7. Información del Restaurante

Muestra información básica:
```
┌─────────────────────────────────────┐
│  INFORMACIÓN DEL RESTAURANTE        │
├─────────────────────────────────────┤
│ Nombre: Fridays Restaurant          │
│ Dirección: Av. Principal 123        │
│ Teléfono: 555-1234                  │
│                                     │
│ Horario de Atención:                │
│   Lunes - Domingo                   │
│   12:00 - 23:00                     │
│                                     │
│ Capacidad Total: 25 mesas           │
│ Zonas: Interior, Terraza, VIP       │
│                                     │
│ Servicios:                          │
│   ✓ Consumo en local                │
│   ✓ Delivery                        │
│   ✓ Reservas                        │
│   ✓ Eventos                         │
└─────────────────────────────────────┘
```

---

## 🔄 Flujo de Trabajo del Sistema

### Inicialización del Sistema
```
1. main() ejecuta
2. Se crea instancia de Fridays
3. Se inicializan estructuras:
   ├─ Carta (HashTable para menú)
   ├─ GestorReservas (AVL + Grafo + Treap)
   ├─ ControladoraCliente
   └─ ControladoraAdmin
4. Se crean backups totales
5. Se carga historial de pedidos desde archivo
6. Sistema listo para operar
```

### Flujo de Datos - Pedido Completo
```
┌──────────────┐
│   CLIENTE    │
└──────┬───────┘
       │ 1. Selecciona productos
       ▼
┌──────────────┐
│   CARRITO    │ (Lista de productos)
│   (Pedido)   │
└──────┬───────┘
       │ 2. Confirma
       ▼
┌──────────────┐
│ PAGO SYSTEM  │
│ (Efectivo/   │
│  Tarjeta)    │
└──────┬───────┘
       │ 3. Procesa pago
       ▼
┌──────────────┐
│ COMPROBANTE  │ (Boleta/Factura)
└──────┬───────┘
       │ 4. Genera
       ▼
┌──────────────┐
│ COLA PEDIDOS │ (Cola FIFO)
│              │
└──────┬───────┘
       │ 5. Espera procesamiento
       ▼
┌──────────────┐
│ ADMIN        │
│ PROCESA      │
└──────┬───────┘
       │ 6. Completa pedido
       ▼
┌──────────────┐
│ HISTORIAL    │ (Pila LIFO)
│              │
└──────┬───────┘
       │ 7. Persiste
       ▼
┌──────────────┐
│ ARCHIVO      │ historialPedidos.txt
└──────────────┘
```

### Flujo de Datos - Reserva Completa
```
┌──────────────┐
│   CLIENTE    │
└──────┬───────┘
       │ 1. Solicita reserva
       ▼
┌──────────────┐
│ GRAFO MESAS  │
│              │ 2. Busca mesas disponibles
│ (Algoritmo:  │    (BFS por zona)
│  BFS)        │
└──────┬───────┘
       │ 3. Mesas encontradas
       ▼
┌──────────────┐
│ SELECCIÓN    │
│ DE MESA      │ 4. Cliente elige
└──────┬───────┘
       │
       ▼
┌──────────────┐
│  ÁRBOL AVL   │
│  (Reservas)  │ 5. Inserta reserva O(log n)
└──────┬───────┘
       │
       ▼
┌──────────────┐
│   ARCHIVO    │ reservas.txt
│              │ 6. Persiste
└──────────────┘

Si no hay mesa disponible:
       │
       ▼
┌──────────────┐
│ TREAP        │
│ (Lista de    │ 7. Inserta con prioridad
│  Espera)     │    basada en categoría
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ ESPERA       │ 8. Cliente notificado
│ NOTIFICACIÓN │    de posición en lista
└──────────────┘
```

---

## 🛠️ Tecnologías Utilizadas

### Lenguaje de Programación
- **C++** (Standard C++17)
  - Programación Orientada a Objetos
  - Templates (Programación genérica)
  - Manejo de memoria manual (new/delete)
  - STL limitado (solo `<vector>`, `<string>`)

### Paradigmas de Programación
1. **POO (Programación Orientada a Objetos)**
   - Herencia (Pago → PagoEfectivo, PagoTarjeta)
   - Polimorfismo (Comprobante → Boleta, Factura)
   - Encapsulamiento (getters/setters)

2. **Programación Genérica**
   - Templates para estructuras de datos
   - Código reutilizable

3. **Programación Funcional**
   - Uso de funciones lambda
   - Callbacks en recorridos de árboles

### Bibliotecas Utilizadas
```cpp
#include <iostream>      // Entrada/salida estándar
#include <fstream>       // Manejo de archivos
#include <string>        // Strings
#include <vector>        // Vectores (solo en Grafo)
#include <sstream>       // String streams
#include <iomanip>       // Formato de salida
#include <ctime>         // Manejo de fechas
#include <cstdlib>       // Funciones estándar
#include <windows.h>     // Colores de consola (Windows)
```

### Estructuras de Datos Implementadas Manualmente
- Árbol AVL
- Árbol Treap
- Tabla Hash con encadenamiento
- Grafo (lista de adyacencias)
- Lista enlazada
- Cola
- Pila

### Algoritmos Implementados
- **Ordenamiento**: QuickSort, MergeSort
- **Búsqueda**: Búsqueda binaria (implícita en AVL)
- **Grafos**: BFS, DFS, Dijkstra
- **Árboles**: InOrder, PreOrder, PostOrder
- **Hash**: djb2 para strings

---

## 💻 Instalación y Ejecución

### Requisitos Previos
```
- Sistema Operativo: Windows 10/11
- Compilador: Visual Studio 2019/2022 o MinGW
- IDE: Visual Studio (recomendado) o Code::Blocks
- RAM: Mínimo 4GB
- Espacio en disco: 100MB
```

### Compilación con Visual Studio

#### Opción 1: Abrir Solución
```
1. Navegar a la carpeta del proyecto
2. Doble clic en TB2_Fridays.sln
3. Visual Studio se abrirá automáticamente
4. Presionar F5 o clic en "Iniciar"
```

#### Opción 2: Línea de Comandos
```bash
# Navegar a la carpeta del proyecto
cd TB2_Fridays

# Compilar (x64 Debug)
msbuild TB2_Fridays.sln /p:Configuration=Debug /p:Platform=x64

# Ejecutar
cd x64\Debug
TB2_Fridays.exe
```

### Compilación con MinGW (g++)
```bash
# Compilar
g++ TB2_Fridays.cpp -o TB2_Fridays.exe -std=c++17 -I.

# Ejecutar
./TB2_Fridays.exe
```

### Primera Ejecución
```
Al ejecutar por primera vez, el sistema:
1. Crea carpeta "Data" si no existe
2. Genera archivos de texto vacíos
3. Crea backups iniciales
4. Carga menú predeterminado (si existe)
5. Muestra menú principal
```

### Estructura de Datos Inicial
```
Data/
├── clientes.txt (vacío)
├── menu.txt (menú predeterminado)
├── reservas.txt (vacío)
├── mesas.txt (25 mesas)
├── historialPedidos.txt (vacío)
├── lista_espera.txt (vacío)
└── Datasets/
    ├── clientes_dataset.csv
    ├── pedidos_dataset.csv
    ├── reservas_dataset.csv
    └── lista_espera_dataset.csv
```

---

## 📊 Análisis de Complejidad

### Tabla Resumen de Operaciones

| Operación | Estructura | Complejidad Temporal | Complejidad Espacial |
|-----------|------------|---------------------|---------------------|
| Insertar pedido | Cola | O(1) | O(1) |
| Procesar pedido | Cola | O(1) | O(1) |
| Guardar en historial | Pila | O(1) | O(1) |
| Buscar producto por ID | HashTable | O(1) promedio | O(n) |
| Buscar producto por nombre | HashTable | O(1) promedio | O(n) |
| Insertar reserva | AVL | O(log n) | O(n) |
| Buscar reserva | AVL | O(log n) | O(n) |
| Insertar en lista espera | Treap | O(log n) esperado | O(n) |
| Obtener mayor prioridad | Treap | O(1) | O(1) |
| Buscar mesa por zona | Grafo (BFS) | O(V + E) | O(V) |
| Ruta más corta | Grafo (Dijkstra) | O(V²) | O(V) |
| Ordenar pedidos | MergeSort | O(n log n) | O(n) |
| Ordenar clientes | QuickSort | O(n log n) promedio | O(log n) |

### Análisis de Espacio Total
```
Memoria aproximada (con 1000 registros):
- Clientes (AVL): ~40 KB
- Menú (HashTable): ~20 KB
- Reservas (AVL): ~30 KB
- Pedidos (Cola + Pila): ~100 KB
- Grafo de Mesas: ~5 KB
- Lista de Espera (Treap): ~10 KB
TOTAL: ~205 KB
```

---

## 🎓 Conceptos Aplicados

### Estructuras de Datos
1. **Árboles Balanceados (AVL)**
   - Auto-balanceo mediante rotaciones
   - Factor de balance
   - Aplicación: Sistemas de búsqueda ordenada

2. **Árboles Aleatorios (Treap)**
   - Aleatorización para balanceo
   - Prioridades basadas en heap
   - Aplicación: Colas de prioridad

3. **Tablas Hash**
   - Funciones hash
   - Resolución de colisiones por encadenamiento
   - Factor de carga y redimensionamiento
   - Aplicación: Búsquedas en tiempo constante

4. **Grafos**
   - Representación por lista de adyacencias
   - Grafos ponderados
   - Aplicación: Modelado de espacios físicos

### Algoritmos de Grafos
1. **BFS (Búsqueda en Anchura)**
   - Exploración por niveles
   - Aplicación: Búsqueda de mesas por zona

2. **DFS (Búsqueda en Profundidad)**
   - Exploración en profundidad
   - Aplicación: Encontrar mesas unibles

3. **Dijkstra**
   - Ruta más corta en grafos ponderados
   - Aplicación: Optimización de recorridos

### Algoritmos de Ordenamiento
1. **QuickSort**
   - Divide y conquista
   - Pivote y particionamiento
   - Aplicación: Ordenamiento rápido

2. **MergeSort**
   - Divide y conquista
   - Mezcla ordenada
   - Aplicación: Ordenamiento estable

### Patrones de Diseño
1. **Herencia y Polimorfismo**
   - Carta → CartaLocal, CartaDelivery
   - Pago → PagoEfectivo, PagoTarjeta
   - Comprobante → Boleta, Factura

2. **Composición**
   - Fridays contiene Carta, GestorReservas
   - Pedido contiene Lista de Productos

3. **Singleton** (implícito)
   - GestorArchivos (métodos estáticos)
   - GestorBackup (métodos estáticos)

---

## 📝 Casos de Uso del Sistema

### Caso 1: Cliente Nuevo Hace Pedido Local
```
Actor: Cliente
Precondición: Sistema operativo

1. Cliente selecciona "Modo Cliente"
2. Cliente selecciona "Pedido Local"
3. Sistema solicita DNI
4. Cliente ingresa DNI (nuevo)
5. Sistema solicita datos completos
6. Cliente ingresa: nombre, apellido, teléfono
7. Sistema registra cliente (categoría Regular, 0 puntos)
8. Sistema muestra menú
9. Cliente selecciona productos y los agrega al carrito
10. Cliente confirma pedido
11. Sistema calcula total
12. Cliente selecciona método de pago (efectivo)
13. Cliente ingresa monto
14. Sistema valida pago
15. Sistema genera comprobante (boleta)
16. Sistema agrega +20 puntos al cliente
17. Pedido se encola en cola de cocina
18. Sistema confirma pedido con número

Postcondición:
- Cliente registrado en sistema
- Pedido en cola
- Comprobante generado
- Puntos acumulados
```

### Caso 2: Admin Procesa Reserva con Lista de Espera
```
Actor: Administrador
Precondición: Cliente en lista de espera

1. Admin selecciona "Gestión de Reservas"
2. Admin selecciona "Ver lista de espera"
3. Sistema muestra clientes ordenados por prioridad (Treap)
4. Admin selecciona "Procesar siguiente"
5. Sistema obtiene cliente con mayor prioridad (VIP)
6. Sistema busca mesas disponibles (BFS en grafo)
7. Sistema encuentra mesa adecuada
8. Sistema crea reserva automáticamente (AVL)
9. Sistema elimina cliente de lista de espera
10. Sistema notifica disponibilidad
11. Reserva confirmada

Postcondición:
- Cliente removido de lista de espera
- Reserva creada
- Mesa asignada
```

### Caso 3: Búsqueda Optimizada de Mesa para Grupo
```
Actor: Cliente (grupo grande)
Precondición: Cliente solicita mesa para 12 personas

1. Cliente ingresa número de personas (12)
2. Sistema calcula que necesita 2 mesas
3. Sistema ejecuta DFS en zona Interior
4. Sistema encuentra mesas adyacentes:
   - Mesa 11 (familiar, cap 6)
   - Mesa 12 (familiar, cap 6)
5. Sistema valida que estén disponibles
6. Sistema calcula distancia entre mesas (Dijkstra)
7. Sistema confirma que son unibles
8. Sistema sugiere combinación
9. Cliente acepta
10. Sistema crea reserva para ambas mesas
11. Sistema marca mesas como "reservadas juntas"

Postcondición:
- Dos mesas reservadas y vinculadas
- Capacidad total: 12 personas
```

---

## 🔍 Características Avanzadas

### 1. Sistema de Colores en Consola
```cpp
namespace ConsolaColor {
    const string ROJO = "\033[31m";
    const string VERDE = "\033[32m";
    const string AMARILLO = "\033[33m";
    const string AZUL = "\033[34m";
    const string BLANCO = "\033[37m";
    const string RESET = "\033[0m";
}
```

### 2. Validación de Datos
- **DNI**: 8 dígitos numéricos
- **RUC**: 11 dígitos numéricos
- **Tarjeta**: Mínimo 16 dígitos
- **Fecha**: Formato DD/MM/YYYY
- **Hora**: Formato HH:MM

### 3. Persistencia Automática
- Guardado automático al procesar pedidos
- Guardado automático al modificar menú
- Backups totales al salir del sistema
- Carga automática al iniciar

### 4. Generación de Códigos Únicos
```cpp
// Código de reserva: RESXXX
string generarCodigoReserva() {
    return "RES" + to_string(++contadorReservas);
}

// Número de pedido: P-YYYYMMDD-XXX
string generarNumeroPedido() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    return "P-" + to_string(1900 + ltm->tm_year)
           + to_string(1 + ltm->tm_mon)
           + to_string(ltm->tm_mday)
           + "-" + to_string(++contadorPedidos);
}
```

---

## 🚀 Futuras Mejoras

### Fase 1: Mejoras de Interfaz
- [ ] Interfaz gráfica con Qt
- [ ] Aplicación web con backend C++
- [ ] App móvil para clientes

### Fase 2: Funcionalidades
- [ ] Sistema de cupones y descuentos
- [ ] Integración con pasarelas de pago reales
- [ ] Notificaciones por SMS/Email
- [ ] Sistema de calificaciones y reseñas

### Fase 3: Optimizaciones
- [ ] Base de datos SQL en lugar de archivos
- [ ] Cache de búsquedas frecuentes
- [ ] Procesamiento paralelo de pedidos
- [ ] Machine Learning para recomendaciones

### Fase 4: Expansión
- [ ] Multi-restaurante (cadena)
- [ ] Sistema de inventario
- [ ] Gestión de empleados
- [ ] Reportes y analytics avanzados

---

## 👥 Autores

**Equipo de Desarrollo TB2_Fridays**

---

## 📄 Licencia

Este proyecto es un trabajo académico desarrollado con fines educativos.

---

## 📞 Soporte

Para preguntas o problemas:
- Revisar documentación del código
- Consultar comentarios en archivos .h
- Verificar archivos de log en carpeta Data/

---

## 🎉 Agradecimientos

- Profesores del curso de Algoritmos y Estructuras de Datos
- Comunidad de C++ por recursos educativos
- TGI Fridays por la inspiración del proyecto

---

**Versión del Sistema:** 2.0
**Fecha de Última Actualización:** Diciembre 2024
**Estado:** En Producción (Académico)

---

