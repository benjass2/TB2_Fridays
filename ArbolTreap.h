#pragma once
#include "Funcionalidades.h"

template<typename T>
class NodoArbolTreap {
public:
    T dato;
    int prioridad;
    NodoArbolTreap<T>* izquierda;
    NodoArbolTreap<T>* derecha;

    NodoArbolTreap(T valor, int prio)
        : dato(valor), prioridad(prio),
        izquierda(nullptr), derecha(nullptr) {
    }
};

template<typename T>
class ArbolTreap {
private:
    NodoArbolTreap<T>* raiz;
    int tamano;


    NodoArbolTreap<T>* rotarDerecha(NodoArbolTreap<T>* y) {
        NodoArbolTreap<T>* x = y->izquierda;
        NodoArbolTreap<T>* T2 = x->derecha;

        x->derecha = y;
        y->izquierda = T2;

        return x;
    }


    NodoArbolTreap<T>* rotarIzquierda(NodoArbolTreap<T>* x) {
        NodoArbolTreap<T>* y = x->derecha;
        NodoArbolTreap<T>* T2 = y->izquierda;

        y->izquierda = x;
        x->derecha = T2;

        return y;
    }

    // Insertar con prioridad
    NodoArbolTreap<T>* insertarRec(NodoArbolTreap<T>* nodo, T valor, int prioridad) {

        if (nodo == nullptr) {
            tamano++;
            return new NodoArbolTreap<T>(valor, prioridad);
        }

        if (valor < nodo->dato) {
            nodo->izquierda = insertarRec(nodo->izquierda, valor, prioridad);

            if (nodo->izquierda->prioridad > nodo->prioridad) {
                nodo = rotarDerecha(nodo);
            }
        }
        else if (valor > nodo->dato) {
            nodo->derecha = insertarRec(nodo->derecha, valor, prioridad);

            if (nodo->derecha->prioridad > nodo->prioridad) {
                nodo = rotarIzquierda(nodo);
            }
        }
        return nodo;
    }


    NodoArbolTreap<T>* eliminarRec(NodoArbolTreap<T>* nodo, T valor) {
        if (nodo == nullptr) return nullptr;

        if (valor < nodo->dato) {
            nodo->izquierda = eliminarRec(nodo->izquierda, valor);
        }
        else if (valor > nodo->dato) {
            nodo->derecha = eliminarRec(nodo->derecha, valor);
        }
        else {

            if (nodo->izquierda == nullptr) {
                NodoArbolTreap<T>* temp = nodo->derecha;
                delete nodo;
                tamano--;
                return temp;
            }
            else if (nodo->derecha == nullptr) {
                NodoArbolTreap<T>* temp = nodo->izquierda;
                delete nodo;
                tamano--;
                return temp;
            }


            if (nodo->izquierda->prioridad > nodo->derecha->prioridad) {
                nodo = rotarDerecha(nodo);
                nodo->derecha = eliminarRec(nodo->derecha, valor);
            }
            else {
                nodo = rotarIzquierda(nodo);
                nodo->izquierda = eliminarRec(nodo->izquierda, valor);
            }
        }

        return nodo;
    }


    void inOrderRec(NodoArbolTreap<T>* nodo, function<void(T, int)> funcion) const {
        if (nodo != nullptr) {
            inOrderRec(nodo->izquierda, funcion);
            funcion(nodo->dato, nodo->prioridad);
            inOrderRec(nodo->derecha, funcion);
        }
    }


    void recorrerPorNivelesRec(NodoArbolTreap<T>* nodo, function<void(T, int)> funcion) const {
        if (nodo == nullptr) return;


        vector<NodoArbolTreap<T>*> cola;
        cola.push_back(nodo);

        while (!cola.empty()) {
            NodoArbolTreap<T>* actual = cola.front();
            cola.erase(cola.begin());

            funcion(actual->dato, actual->prioridad);

            if (actual->izquierda) cola.push_back(actual->izquierda);
            if (actual->derecha) cola.push_back(actual->derecha);
        }
    }


    void destruirArbol(NodoArbolTreap<T>* nodo) {
        if (nodo != nullptr) {
            destruirArbol(nodo->izquierda);
            destruirArbol(nodo->derecha);
            delete nodo;
        }
    }

public:
    ArbolTreap() : raiz(nullptr), tamano(0) {
        srand(static_cast<unsigned int>(time(0)));
    }

    ~ArbolTreap() {
        destruirArbol(raiz);
    }

    void insertar(T valor, int prioridad) {
        raiz = insertarRec(raiz, valor, prioridad);
    }

    void eliminar(T valor) {
        raiz = eliminarRec(raiz, valor);
    }


    void recorrerInOrder(function<void(T, int)> funcion) const {
        inOrderRec(raiz, funcion);
    }


    void recorrerPorPrioridad(function<void(T, int)> funcion) const {
        if (raiz == nullptr) return;

        // Recolectar todos los nodos
        vector<pair<T, int>> nodos;
        recolectarNodos(raiz, nodos);

        // Ordenar manualmente por prioridad (Bubble Sort descendente)
        int n = nodos.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (nodos[j].second < nodos[j + 1].second) {  // Orden descendente
                    // Intercambiar
                    pair<T, int> temp = nodos[j];
                    nodos[j] = nodos[j + 1];
                    nodos[j + 1] = temp;
                }
            }
        }

        // Ejecutar la función sobre cada nodo en orden
        for (int i = 0; i < n; i++) {
            funcion(nodos[i].first, nodos[i].second);
        }
    }


    T obtenerMayorPrioridad() const {
        if (raiz == nullptr) {
            throw runtime_error("ArbolTreap vacio");
        }
        return raiz->dato;
    }

    int getPrioridadMaxima() const {
        return raiz ? raiz->prioridad : 0;
    }

    bool estaVacio() const {
        return raiz == nullptr;
    }

    int getTamano() const {
        return tamano;
    }

    void limpiar() {
        destruirArbol(raiz);
        raiz = nullptr;
        tamano = 0;
    }

private:
    void recolectarNodos(NodoArbolTreap<T>* nodo, vector<pair<T, int>>& nodos) const {
        if (nodo != nullptr) {
            nodos.push_back(make_pair(nodo->dato, nodo->prioridad));
            recolectarNodos(nodo->izquierda, nodos);
            recolectarNodos(nodo->derecha, nodos);
        }
    }
};