#pragma once
#include "Nodo.h"
#include <iostream>
#include <functional>
using namespace std;

// Forward declaration para evitar dependencia circular
template<typename T> class GestorOrdenamiento;

template<typename T>
class Cola {
private:
    Nodo<T>* frente;
    Nodo<T>* final;
    int tamano;

public:

    Cola() : frente(nullptr), final(nullptr), tamano(0) {}


    ~Cola() {
        limpiar();
    }


    void encolar(T elemento) {
        Nodo<T>* nuevo = new Nodo<T>(elemento);

        if (estaVacia()) {
            frente = nuevo;
            final = nuevo;
        }
        else {
            final->setSiguiente(nuevo);
            final = nuevo;
        }
        tamano++;
    }


    T desencolar() {
        if (estaVacia()) {
            cout << "Cola vacia" << endl;
        }

        Nodo<T>* temp = frente;
        T elemento = temp->getData();
        frente = frente->getSiguiente();

        if (frente == nullptr) {
            final = nullptr;
        }

        delete temp;
        tamano--;
        return elemento;
    }

    bool eliminarEnPosicion(int posicion) {
        if (posicion < 0 || posicion >= tamano) {
            cout << "Posicion invalida" << endl;
            return false;
        }


        if (posicion == 0) {
            desencolar();
            return true;
        }


        Nodo<T>* actual = frente;
        for (int i = 0; i < posicion - 1; i++) {
            actual = actual->getSiguiente();
        }


        Nodo<T>* aEliminar = actual->getSiguiente();


        actual->setSiguiente(aEliminar->getSiguiente());


        if (aEliminar == final) {
            final = actual;
        }

        delete aEliminar;
        tamano--;
        return true;
    }


    T verFrente() const {
        if (estaVacia()) {
            cout << "Cola vacia" << endl;
        }
        return frente->getData();
    }


    bool estaVacia() const {
        return frente == nullptr;
    }


    int gettamano() const {
        return tamano;
    }


    void mostrar() const {
        if (estaVacia()) {
            cout << "Cola vacia" << endl;
            return;
        }

        cout << "Frente -> ";
        Nodo<T>* actual = frente;
        while (actual != nullptr) {
            cout << actual->getData() << " -> ";
            actual = actual->getSiguiente();
        }
        cout << "Final" << endl;
    }


    bool buscar(T elemento) const {
        Nodo<T>* actual = frente;
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
            cout << "Posicion invalida" << endl;
        }

        Nodo<T>* actual = frente;
        for (int i = 0; i < posicion; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getData();
    }


    int contarSi(function<bool(const T&)> condicion) const {
        int contador = 0;
        Nodo<T>* actual = frente;

        while (actual != nullptr) {
            if (condicion(actual->getData())) {
                contador++;
            }
            actual = actual->getSiguiente();
        }
        return contador;
    }

    // ========================================
    // MÉTODO DE ORDENAMIENTO
    // ========================================
    void ordenar(function<bool(const T&, const T&)> comparador,
        typename GestorOrdenamiento<T>::Algoritmo algoritmo = GestorOrdenamiento<T>::MERGE_SORT);

private:
    void limpiar() {
        while (!estaVacia()) {
            desencolar();
        }
    }
};

// ========================================
// IMPLEMENTACIÓN DEL MÉTODO ORDENAR
// (Fuera de la clase para evitar dependencia circular)
// ========================================
#include "GestorOrdenamiento.h"

template<typename T>
void Cola<T>::ordenar(function<bool(const T&, const T&)> comparador,
    typename GestorOrdenamiento<T>::Algoritmo algoritmo) {
    GestorOrdenamiento<T>::ordenarCola(*this, comparador, algoritmo);
}

