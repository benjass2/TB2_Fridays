#pragma once
#include "Funcionalidades.h"
#include "Mesa.h"
#include "Lista.h"
#include "Cola.h"
#include <vector>

using namespace std;
using namespace ConsolaColor;

const double INFINITO = 999999.0;

inline double raizCuadrada(double n) {
    if (n < 0) return 0;
    if (n == 0) return 0;

    double x = n;
    double y = 1;
    double precision = 0.000001;

    while (x - y > precision) {
        x = (x + y) / 2;
        y = n / x;
    }
    return x;
}

inline double potencia(double base, int exponente) {
    double resultado = 1;
    for (int i = 0; i < exponente; i++) {
        resultado *= base;
    }
    return resultado;
}

inline double valorAbsoluto(double x) {
    return (x < 0) ? -x : x;
}

struct Conexion {
    int mesaDestino;
    double distancia;

    Conexion() : mesaDestino(0), distancia(0) {}
    Conexion(int dest, double dist) : mesaDestino(dest), distancia(dist) {}

    bool operator==(const Conexion& otra) const {
        return mesaDestino == otra.mesaDestino;
    }
    bool operator!=(const Conexion& otra) const {
        return !(*this == otra);
    }
};

struct Coordenada {
    double x, y;

    Coordenada(double px = 0, double py = 0) : x(px), y(py) {}

    double calcularDistancia(const Coordenada& otra) const {
        double dx = x - otra.x;
        double dy = y - otra.y;
        return raizCuadrada(potencia(dx, 2) + potencia(dy, 2));
    }
};

struct InfoMesa {
    Mesa mesa;
    Coordenada posicion;
    Lista<Conexion> adyacencias;

    InfoMesa() : mesa(), posicion(), adyacencias() {}
    InfoMesa(const Mesa& m, const Coordenada& pos)
        : mesa(m), posicion(pos), adyacencias() {
    }
};

class GrafoMesas {
private:
    vector<InfoMesa> grafo;  // Vector de 26 posiciones (índice 0 no se usa, 1-25 para mesas)

    // Verificar si una mesa ya fue visitada
    bool estaVisitado(const Lista<int>& visitados, int mesa) const {
        for (int i = 0; i < visitados.gettamano(); i++) {
            if (visitados.obtenerEnPosicion(i) == mesa) {
                return true;
            }
        }
        return false;
    }

    // Agregar a visitados si no existe
    void marcarVisitado(Lista<int>& visitados, int mesa) const {
        if (!estaVisitado(visitados, mesa)) {
            visitados.insertarFinal(mesa);
        }
    }

public:
    GrafoMesas() {
        // Inicializar vector con 26 posiciones (0-25, donde 0 no se usa)
        grafo.resize(26);
    }

    // Agregar mesa al grafo con su posición
    void agregarMesa(const Mesa& mesa, double x, double y) {
        int numMesa = mesa.getNumeroMesa();
        if (numMesa >= 1 && numMesa <= 25) {
            grafo[numMesa] = InfoMesa(mesa, Coordenada(x, y));
        }
    }

    // Conectar dos mesas con su distancia
    void conectarMesas(int mesa1, int mesa2, double distancia = -1) {
        if (mesa1 < 1 || mesa1 > 25 || mesa2 < 1 || mesa2 > 25) return;

        // Si no se proporciona distancia, calcularla
        if (distancia < 0) {
            distancia = grafo[mesa1].posicion.calcularDistancia(grafo[mesa2].posicion);
        }

        // Agregar conexión bidireccional
        grafo[mesa1].adyacencias.insertarFinal(Conexion(mesa2, distancia));
        grafo[mesa2].adyacencias.insertarFinal(Conexion(mesa1, distancia));
    }

    // Inicializar el grafo con todas las mesas y sus posiciones
    void inicializarGrafoRestaurante(const Lista<Mesa>& listaMesas) {
        // Layout del restaurante (coordenadas aproximadas)
        // Interior - Mesas regulares (10 mesas) - 2 filas de 5
        for (int i = 1; i <= 5; i++) {
            Mesa mesa = listaMesas.obtenerEnPosicion(i - 1);
            agregarMesa(mesa, i * 2.0, 2.0);  // Fila superior
        }
        for (int i = 6; i <= 10; i++) {
            Mesa mesa = listaMesas.obtenerEnPosicion(i - 1);
            agregarMesa(mesa, (i - 5) * 2.0, 5.0);  // Fila inferior
        }

        // Interior - Mesas familiares (5 mesas) - Una fila central
        for (int i = 11; i <= 15; i++) {
            Mesa mesa = listaMesas.obtenerEnPosicion(i - 1);
            agregarMesa(mesa, (i - 10) * 2.5, 8.0);
        }

        // Terraza (5 mesas) - Área separada
        for (int i = 16; i <= 20; i++) {
            Mesa mesa = listaMesas.obtenerEnPosicion(i - 1);
            agregarMesa(mesa, (i - 15) * 2.0, 12.0);
        }

        // VIP (5 mesas) - Área exclusiva
        for (int i = 21; i <= 25; i++) {
            Mesa mesa = listaMesas.obtenerEnPosicion(i - 1);
            agregarMesa(mesa, (i - 20) * 2.5, 15.0);
        }

        // Conectar mesas cercanas automáticamente
        conectarMesasCercanas(3.0);  // Radio de 3 unidades
    }

    // Conectar automáticamente mesas que estén dentro de un radio
    void conectarMesasCercanas(double radioMaximo) {
        for (int i = 1; i <= 25; i++) {
            for (int j = i + 1; j <= 25; j++) {
                double dist = grafo[i].posicion.calcularDistancia(grafo[j].posicion);
                if (dist <= radioMaximo) {
                    conectarMesas(i, j, dist);
                }
            }
        }
    }

    // BFS: Buscar mesas por zona/ubicación
    Lista<int> buscarMesasPorZona(int mesaInicio, const string& zonaDeseada) {
        Lista<int> resultado;
        Lista<int> visitados;
        Cola<int> cola;

        cola.encolar(mesaInicio);
        marcarVisitado(visitados, mesaInicio);

        while (!cola.estaVacia()) {
            int actual = cola.desencolar();

            if (grafo[actual].mesa.getUbicacion() == zonaDeseada) {
                resultado.insertarFinal(actual);
            }

            // Explorar vecinos
            Lista<Conexion>& vecinos = grafo[actual].adyacencias;
            for (int i = 0; i < vecinos.gettamano(); i++) {
                Conexion con = vecinos.obtenerEnPosicion(i);
                if (!estaVisitado(visitados, con.mesaDestino)) {
                    cola.encolar(con.mesaDestino);
                    marcarVisitado(visitados, con.mesaDestino);
                }
            }
        }

        return resultado;
    }

    // DFS: Encontrar mesas que se pueden unir para grupos grandes
    Lista<int> encontrarMesasUnibles(int mesaInicio, int capacidadRequerida) {
        Lista<int> mesasSeleccionadas;
        Lista<int> visitados;
        int capacidadTotal = 0;

        dfsUnirMesas(mesaInicio, capacidadRequerida, capacidadTotal,
            visitados, mesasSeleccionadas);

        return mesasSeleccionadas;
    }

    // DFS auxiliar para unir mesas
    bool dfsUnirMesas(int mesaActual, int capacidadRequerida, int& capacidadTotal,
        Lista<int>& visitados, Lista<int>& resultado) {

        marcarVisitado(visitados, mesaActual);
        resultado.insertarFinal(mesaActual);
        capacidadTotal += grafo[mesaActual].mesa.getCapacidad();

        if (capacidadTotal >= capacidadRequerida) {
            return true;
        }

        // Explorar mesas vecinas
        Lista<Conexion>& vecinos = grafo[mesaActual].adyacencias;
        for (int i = 0; i < vecinos.gettamano(); i++) {
            Conexion con = vecinos.obtenerEnPosicion(i);

            if (!estaVisitado(visitados, con.mesaDestino)) {
                // Solo considerar mesas de la misma ubicación
                if (grafo[mesaActual].mesa.getUbicacion() ==
                    grafo[con.mesaDestino].mesa.getUbicacion()) {

                    if (dfsUnirMesas(con.mesaDestino, capacidadRequerida,
                        capacidadTotal, visitados, resultado)) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    // Dijkstra: Encontrar ruta más corta entre dos mesas
    Lista<int> rutaMasCorta(int mesaOrigen, int mesaDestino) {
        // Arrays para Dijkstra
        double distancias[26];
        int predecesores[26];
        bool visitados[26];

        // Inicializar
        for (int i = 0; i < 26; i++) {
            distancias[i] = INFINITO;
            predecesores[i] = -1;
            visitados[i] = false;
        }
        distancias[mesaOrigen] = 0;

        // Algoritmo de Dijkstra sin priority_queue
        for (int count = 0; count < 25; count++) {
            // Encontrar vértice no visitado con distancia mínima
            double minDist = INFINITO;
            int u = -1;

            for (int v = 1; v <= 25; v++) {
                if (!visitados[v] && distancias[v] < minDist) {
                    minDist = distancias[v];
                    u = v;
                }
            }

            if (u == -1 || u == mesaDestino) break;

            visitados[u] = true;

            // Actualizar distancias de vecinos
            Lista<Conexion>& vecinos = grafo[u].adyacencias;
            for (int i = 0; i < vecinos.gettamano(); i++) {
                Conexion con = vecinos.obtenerEnPosicion(i);
                int v = con.mesaDestino;
                double nuevaDist = distancias[u] + con.distancia;

                if (!visitados[v] && nuevaDist < distancias[v]) {
                    distancias[v] = nuevaDist;
                    predecesores[v] = u;
                }
            }
        }

        // Reconstruir ruta
        Lista<int> ruta;
        if (distancias[mesaDestino] != INFINITO) {
            int actual = mesaDestino;
            while (actual != mesaOrigen) {
                ruta.insertarInicio(actual);
                actual = predecesores[actual];
            }
            ruta.insertarInicio(mesaOrigen);
        }

        return ruta;
    }

    // Calcular distancia entre dos mesas
    double calcularDistancia(int mesa1, int mesa2) {
        if (mesa1 >= 1 && mesa1 <= 25 && mesa2 >= 1 && mesa2 <= 25) {
            return grafo[mesa1].posicion.calcularDistancia(grafo[mesa2].posicion);
        }
        return -1;
    }

    // Obtener mesas vecinas a una mesa específica
    Lista<int> obtenerVecinos(int numeroMesa) {
        Lista<int> vecinos;

        if (numeroMesa >= 1 && numeroMesa <= 25) {
            Lista<Conexion>& conexiones = grafo[numeroMesa].adyacencias;
            for (int i = 0; i < conexiones.gettamano(); i++) {
                Conexion con = conexiones.obtenerEnPosicion(i);
                vecinos.insertarFinal(con.mesaDestino);
            }
        }

        return vecinos;
    }

    // Mostrar mapa visual del restaurante
    void mostrarMapaRestaurante() {
        cout << ROJO << "        MAPA DEL RESTAURANTE FRIDAY'S" << RESET << endl;
        imprimirSeparadorRojoBlanco(50);
        cout << endl;

        // Mostrar por zonas
        string zonas[] = { "Interior", "Terraza", "VIP" };

        for (int z = 0; z < 3; z++) {
            cout << AMARILLO << "=== " << zonas[z] << " ===" << RESET << endl;

            for (int i = 1; i <= 25; i++) {
                if (grafo[i].mesa.getUbicacion() == zonas[z]) {
                    Mesa& m = grafo[i].mesa;
                    Coordenada& pos = grafo[i].posicion;

                    cout << "  Mesa " << setw(2) << i << " ";
                    cout << "[" << m.getTipoMesa() << "] ";
                    cout << "Cap:" << m.getCapacidad() << " ";
                    cout << "Pos:(" << fixed << setprecision(1)
                        << pos.x << "," << pos.y << ")";

                    // Mostrar vecinos
                    cout << " -> Cercanas: ";
                    Lista<int> vecinos = obtenerVecinos(i);
                    for (int j = 0; j < vecinos.gettamano() && j < 3; j++) {
                        cout << vecinos.obtenerEnPosicion(j);
                        if (j < vecinos.gettamano() - 1 && j < 2) cout << ",";
                    }
                    cout << endl;
                }
            }
            cout << endl;
        }

        imprimirSeparadorRojoBlanco(50);
        cout << "Total de mesas: 25" << endl;
        cout << "Total de conexiones: " << contarConexiones() << endl;
    }

    // Mostrar estadísticas del grafo
    void mostrarEstadisticas() {
        cout << ROJO << "        ESTADISTICAS DEL GRAFO DE MESAS" << RESET << endl;
        imprimirSeparadorRojoBlanco(40);

        cout << "Total de nodos (mesas): 25" << endl;
        cout << "Total de aristas (conexiones): " << contarConexiones() << endl;
        cout << endl;

        // Mesa con más conexiones
        int mesaMasConectada = -1;
        int maxConexiones = 0;

        for (int i = 1; i <= 25; i++) {
            int conexiones = grafo[i].adyacencias.gettamano();
            if (conexiones > maxConexiones) {
                maxConexiones = conexiones;
                mesaMasConectada = i;
            }
        }

        if (mesaMasConectada != -1) {
            cout << "Mesa mas conectada: Mesa " << mesaMasConectada
                << " con " << maxConexiones << " conexiones" << endl;
        }

        // Distribución por zona
        int contadorInterior = 0;
        int contadorTerraza = 0;
        int contadorVIP = 0;

        for (int i = 1; i <= 25; i++) {
            string ubicacion = grafo[i].mesa.getUbicacion();
            if (ubicacion == "Interior") contadorInterior++;
            else if (ubicacion == "Terraza") contadorTerraza++;
            else if (ubicacion == "VIP") contadorVIP++;
        }

        cout << endl;
        cout << "Distribucion por zona:" << endl;
        cout << "  Interior: " << contadorInterior << " mesas" << endl;
        cout << "  Terraza: " << contadorTerraza << " mesas" << endl;
        cout << "  VIP: " << contadorVIP << " mesas" << endl;

        imprimirSeparadorRojoBlanco(40);
    }

    // Encontrar mesa más cercana a una posición
    int mesaMasCercana(int mesaReferencia, const string& ubicacion = "") {
        double minDist = INFINITO;
        int mesaCercana = -1;

        for (int i = 1; i <= 25; i++) {
            if (i == mesaReferencia) continue;

            // Filtrar por ubicación si se especifica
            if (!ubicacion.empty() && grafo[i].mesa.getUbicacion() != ubicacion) {
                continue;
            }

            double dist = grafo[mesaReferencia].posicion.calcularDistancia(grafo[i].posicion);
            if (dist < minDist) {
                minDist = dist;
                mesaCercana = i;
            }
        }

        return mesaCercana;
    }

private:
    int contarConexiones() const {
        int total = 0;
        for (int i = 1; i <= 25; i++) {
            total += grafo[i].adyacencias.gettamano();
        }
        return total / 2;  // Dividir por 2 porque las conexiones son bidireccionales
    }
};