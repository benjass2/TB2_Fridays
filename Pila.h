#pragma once
#include "Nodo.h"
#include <iostream>
#include <functional>
using namespace std;

// Forward declaration para evitar dependencia circular
template<typename T> class GestorOrdenamiento;

template<typename T>
class Pila {
private:
    Nodo<T>* tope;
    int tamano;

public:

    Pila() : tope(nullptr), tamano(0) {}


    ~Pila() {
        limpiar();
    }


    void apilar(T elemento) {
        Nodo<T>* nuevo = new Nodo<T>(elemento);
        nuevo->setSiguiente(tope);
        tope = nuevo;
        tamano++;
    }


    T desapilar() {
        if (estaVacia()) {
            throw runtime_error("Pila vacia");
        }

        Nodo<T>* temp = tope;
        T elemento = temp->getData();
        tope = tope->getSiguiente();

        delete temp;
        tamano--;
        return elemento;
    }


    T verTope() const {
        if (estaVacia()) {
            throw runtime_error("Pila vacia");
        }
        return tope->getData();
    }


    bool estaVacia() const {
        return tope == nullptr;
    }


    int gettamano() const {
        return tamano;
    }


    void mostrar() const {
        if (estaVacia()) {
            cout << "Pila vacia" << endl;
            return;
        }

        cout << "Tope:" << endl;
        Nodo<T>* actual = tope;
        while (actual != nullptr) {
            cout << "  | " << actual->getData() << " |" << endl;
            actual = actual->getSiguiente();
        }
        cout << "  +-----+" << endl;
    }


    bool buscar(T elemento) const {
        Nodo<T>* actual = tope;
        while (actual != nullptr) {
            if (actual->getData() == elemento) {
                return true;
            }
            actual = actual->getSiguiente();
        }
        return false;
    }


    T obtenerEnPosicion(int posicion) const {
        if (posicion < 0 || posicion >= tamano) {
            throw out_of_range("Posicion fuera de rango");
        }

        Nodo<T>* actual = tope;
        for (int i = 0; i < posicion; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getData();
    }


    Pila<T> filtrar(function<bool(const T&)> condicion) const {
        Pila<T> resultado;
        Pila<T> temporal;

        Nodo<T>* actual = tope;
        while (actual != nullptr) {
            if (condicion(actual->getData())) {
                temporal.apilar(actual->getData());
            }
            actual = actual->getSiguiente();
        }


        while (!temporal.estaVacia()) {
            resultado.apilar(temporal.desapilar());
        }

        return resultado;
    }

    // ========================================
    // MÉTODO DE ORDENAMIENTO
    // ========================================
    void ordenar(function<bool(const T&, const T&)> comparador,
        typename GestorOrdenamiento<T>::Algoritmo algoritmo = GestorOrdenamiento<T>::MERGE_SORT);

private:
    void limpiar() {
        while (!estaVacia()) {
            desapilar();
        }
    }
};

// ========================================
// IMPLEMENTACIÓN DEL MÉTODO ORDENAR
// (Fuera de la clase para evitar dependencia circular)
// ========================================
#include "GestorOrdenamiento.h"

template<typename T>
void Pila<T>::ordenar(function<bool(const T&, const T&)> comparador,
    typename GestorOrdenamiento<T>::Algoritmo algoritmo) {
    GestorOrdenamiento<T>::ordenarPila(*this, comparador, algoritmo);
}
