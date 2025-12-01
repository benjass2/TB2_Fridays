#pragma once
#include "Funcionalidades.h"

template<typename T>
class NodoAVL {
public:
    T dato;
    NodoAVL<T>* izquierda;
    NodoAVL<T>* derecha;
    int altura;

    NodoAVL(T valor) : dato(valor), izquierda(nullptr), derecha(nullptr), altura(1) {}
};


template<typename T>
class ArbolAVL {
private:
    NodoAVL<T>* raiz;
    int tamano;

    int obtenerAltura(NodoAVL<T>* nodo) const {
        return (nodo == nullptr) ? 0 : nodo->altura;
    }


    void actualizarAltura(NodoAVL<T>* nodo) {
        if (nodo != nullptr) {
            nodo->altura = 1 + max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));
        }
    }


    int obtenerFactorBalanceo(NodoAVL<T>* nodo) const {
        return (nodo == nullptr) ? 0 : obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha);
    }


    NodoAVL<T>* rotarDerecha(NodoAVL<T>* y) {
        NodoAVL<T>* x = y->izquierda;
        NodoAVL<T>* T2 = x->derecha;


        x->derecha = y;
        y->izquierda = T2;

        actualizarAltura(y);
        actualizarAltura(x);

        return x;
    }

    NodoAVL<T>* rotarIzquierda(NodoAVL<T>* x) {
        NodoAVL<T>* y = x->derecha;
        NodoAVL<T>* T2 = y->izquierda;

        y->izquierda = x;
        x->derecha = T2;

        actualizarAltura(x);
        actualizarAltura(y);

        return y;
    }


    NodoAVL<T>* balancear(NodoAVL<T>* nodo) {

        actualizarAltura(nodo);


        int balance = obtenerFactorBalanceo(nodo);


        if (balance > 1 && obtenerFactorBalanceo(nodo->izquierda) >= 0) {
            return rotarDerecha(nodo);
        }


        if (balance < -1 && obtenerFactorBalanceo(nodo->derecha) <= 0) {
            return rotarIzquierda(nodo);
        }


        if (balance > 1 && obtenerFactorBalanceo(nodo->izquierda) < 0) {
            nodo->izquierda = rotarIzquierda(nodo->izquierda);
            return rotarDerecha(nodo);
        }


        if (balance < -1 && obtenerFactorBalanceo(nodo->derecha) > 0) {
            nodo->derecha = rotarDerecha(nodo->derecha);
            return rotarIzquierda(nodo);
        }

        return nodo;
    }


    NodoAVL<T>* insertarRec(NodoAVL<T>* nodo, T valor) {

        if (nodo == nullptr) {
            tamano++;
            return new NodoAVL<T>(valor);
        }

        if (valor < nodo->dato) {
            nodo->izquierda = insertarRec(nodo->izquierda, valor);
        }
        else if (valor > nodo->dato) {
            nodo->derecha = insertarRec(nodo->derecha, valor);
        }
        else {

            return nodo;
        }


        return balancear(nodo);
    }


    NodoAVL<T>* buscarRec(NodoAVL<T>* nodo, T valor) const {
        if (nodo == nullptr || nodo->dato == valor) {
            return nodo;
        }

        if (valor < nodo->dato) {
            return buscarRec(nodo->izquierda, valor);
        }
        return buscarRec(nodo->derecha, valor);
    }


    NodoAVL<T>* encontrarMinimo(NodoAVL<T>* nodo) const {
        while (nodo->izquierda != nullptr) {
            nodo = nodo->izquierda;
        }
        return nodo;
    }

    NodoAVL<T>* eliminarRec(NodoAVL<T>* nodo, T valor) {

        if (nodo == nullptr) return nullptr;

        if (valor < nodo->dato) {
            nodo->izquierda = eliminarRec(nodo->izquierda, valor);
        }
        else if (valor > nodo->dato) {
            nodo->derecha = eliminarRec(nodo->derecha, valor);
        }
        else {

            if (nodo->izquierda == nullptr) {
                NodoAVL<T>* temp = nodo->derecha;
                delete nodo;
                tamano--;
                return temp;
            }
            else if (nodo->derecha == nullptr) {
                NodoAVL<T>* temp = nodo->izquierda;
                delete nodo;
                tamano--;
                return temp;
            }


            NodoAVL<T>* temp = encontrarMinimo(nodo->derecha);
            nodo->dato = temp->dato;
            nodo->derecha = eliminarRec(nodo->derecha, temp->dato);
        }


        if (nodo == nullptr) return nodo;

        return balancear(nodo);
    }


    void inOrderRec(NodoAVL<T>* nodo, function<void(T)> funcion) const {
        if (nodo != nullptr) {
            inOrderRec(nodo->izquierda, funcion);
            funcion(nodo->dato);
            inOrderRec(nodo->derecha, funcion);
        }
    }

    void preOrderRec(NodoAVL<T>* nodo, function<void(T)> funcion) const {
        if (nodo != nullptr) {
            funcion(nodo->dato);
            preOrderRec(nodo->izquierda, funcion);
            preOrderRec(nodo->derecha, funcion);
        }
    }

    void postOrderRec(NodoAVL<T>* nodo, function<void(T)> funcion) const {
        if (nodo != nullptr) {
            postOrderRec(nodo->izquierda, funcion);
            postOrderRec(nodo->derecha, funcion);
            funcion(nodo->dato);
        }
    }


    void destruirArbol(NodoAVL<T>* nodo) {
        if (nodo != nullptr) {
            destruirArbol(nodo->izquierda);
            destruirArbol(nodo->derecha);
            delete nodo;
        }
    }


    int alturaRec(NodoAVL<T>* nodo) const {
        if (nodo == nullptr) return 0;

        int altIzq = alturaRec(nodo->izquierda);
        int altDer = alturaRec(nodo->derecha);

        return 1 + max(altIzq, altDer);
    }

public:

    ArbolAVL() : raiz(nullptr), tamano(0) {}


    ~ArbolAVL() {
        destruirArbol(raiz);
    }


    void insertar(T valor) {
        raiz = insertarRec(raiz, valor);
    }


    bool buscar(T valor) const {
        return buscarRec(raiz, valor) != nullptr;
    }


    void eliminar(T valor) {
        raiz = eliminarRec(raiz, valor);
    }


    void inOrder(function<void(T)> funcion) const {
        inOrderRec(raiz, funcion);
    }


    void preOrder(function<void(T)> funcion) const {
        preOrderRec(raiz, funcion);
    }


    void postOrder(function<void(T)> funcion) const {
        postOrderRec(raiz, funcion);
    }

    int gettamano() const {
        return tamano;
    }


    bool estaVacio() const {
        return raiz == nullptr;
    }


    int getAltura() const {
        return alturaRec(raiz);
    }


    void limpiar() {
        destruirArbol(raiz);
        raiz = nullptr;
        tamano = 0;
    }


    int getFactorBalanceoRaiz() const {
        return obtenerFactorBalanceo(raiz);
    }


    bool estaBalanceado() const {
        return verificarBalanceo(raiz);
    }

private:

    bool verificarBalanceo(NodoAVL<T>* nodo) const {
        if (nodo == nullptr) return true;

        int balance = obtenerFactorBalanceo(nodo);
        if (abs(balance) > 1) return false;

        return verificarBalanceo(nodo->izquierda) && verificarBalanceo(nodo->derecha);
    }
};


