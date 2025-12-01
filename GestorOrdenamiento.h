#pragma once
#include "Nodo.h"
#include "Funcionalidades.h"
#include <vector>
#include <functional>

using namespace std;

// Forward declarations para evitar dependencias circulares
template<typename T> class Pila;
template<typename T> class Cola;

template<typename T>
class GestorOrdenamiento {
public:
    enum Algoritmo {
        MERGE_SORT,
        QUICK_SORT,
        HEAP_SORT,
        INSERTION_SORT,
    };

    // ========================================
    // MÉTODO PARA LISTAS DOBLEMENTE ENLAZADAS
    // ========================================
    static void ordenar(Nodo<T>*& cabeza, Nodo<T>*& cola,
        function<bool(const T&, const T&)> comparador,
        Algoritmo algoritmo = MERGE_SORT) {

        if (!cabeza || !cabeza->getSiguiente()) return;

        switch (algoritmo) {
        case MERGE_SORT:
            cabeza = mergeSort(cabeza, comparador);
            break;
        case QUICK_SORT:
            quickSortWrapper(cabeza, cola, comparador);
            break;
        case HEAP_SORT:
            heapSort(cabeza, comparador);
            break;
        case INSERTION_SORT:
            insertionSort(cabeza, comparador);
            break;
        }

        actualizarCola(cabeza, cola);
    }

    // ========================================
    // MÉTODO PARA PILA
    // ========================================
    static void ordenarPila(Pila<T>& pila,
        function<bool(const T&, const T&)> comparador,
        Algoritmo algoritmo = MERGE_SORT) {

        if (pila.gettamano() <= 1) return;

        // Convertir pila a vector
        vector<T> elementos;
        for (int i = 0; i < pila.gettamano(); i++) {
            elementos.push_back(pila.obtenerEnPosicion(i));
        }

        // Ordenar usando el algoritmo especificado
        ordenarVector(elementos, comparador, algoritmo);

        // Reconstruir la pila (vaciarla y rellenarla en orden inverso)
        while (!pila.estaVacia()) {
            pila.desapilar();
        }

        // Apilar en orden inverso para mantener el orden correcto
        // (ya que apilar invierte el orden)
        for (int i = elementos.size() - 1; i >= 0; i--) {
            pila.apilar(elementos[i]);
        }
    }

    // ========================================
    // MÉTODO PARA COLA
    // ========================================
    static void ordenarCola(Cola<T>& cola,
        function<bool(const T&, const T&)> comparador,
        Algoritmo algoritmo = MERGE_SORT) {

        if (cola.gettamano() <= 1) return;

        // Convertir cola a vector
        vector<T> elementos;
        for (int i = 0; i < cola.gettamano(); i++) {
            elementos.push_back(cola.obtenerEnPosicion(i));
        }

        // Ordenar usando el algoritmo especificado
        ordenarVector(elementos, comparador, algoritmo);

        // Reconstruir la cola (vaciarla y rellenarla en orden)
        while (!cola.estaVacia()) {
            cola.desencolar();
        }

        // Encolar en el orden correcto
        for (const T& elemento : elementos) {
            cola.encolar(elemento);
        }
    }

    // ========================================
    // INSERTION SORT PARA VECTORES
    // ========================================

    static void insertionSortVector(vector<T>& arr, function<bool(const T&, const T&)> comparador) {
        int n = arr.size();

        // Recorrer desde el segundo elemento hasta el final
        for (int i = 1; i < n; i++) {
            T clave = arr[i];
            int j = i - 1;

            // Mover elementos mayores que la clave una posición adelante
            while (j >= 0 && !comparador(arr[j], clave) && arr[j] != clave) {
                arr[j + 1] = arr[j];
                j--;
            }

            // Insertar la clave en su posición correcta
            arr[j + 1] = clave;
        }
    }

    // ========================================
    // MÉTODO PARA VECTORES
    // ========================================

    static void ordenarVector(vector<T>& elementos, function<bool(const T&, const T&)> comparador, Algoritmo algoritmo = MERGE_SORT) {


        if (elementos.size() <= 1) return;

        switch (algoritmo) {

        case MERGE_SORT:
            mergeSortVector(elementos, 0, elementos.size() - 1, comparador);
            break;
        case QUICK_SORT:
            quickSortVector(elementos, 0, elementos.size() - 1, comparador);
            break;
        case HEAP_SORT:
            heapSortVector(elementos, comparador);
            break;
        case INSERTION_SORT:
            insertionSortVector(elementos, comparador);
            break;
        }
    }

private:



    // ========================================
    // MERGE SORT PARA VECTORES
    // ========================================
    static void mergeSortVector(vector<T>& arr, int izq, int der,
        function<bool(const T&, const T&)> comparador) {
        if (izq >= der) return;

        int medio = izq + (der - izq) / 2;
        mergeSortVector(arr, izq, medio, comparador);
        mergeSortVector(arr, medio + 1, der, comparador);
        mergeVector(arr, izq, medio, der, comparador);
    }

    static void mergeVector(vector<T>& arr, int izq, int medio, int der,
        function<bool(const T&, const T&)> comparador) {
        int n1 = medio - izq + 1;
        int n2 = der - medio;

        vector<T> L(n1), R(n2);

        for (int i = 0; i < n1; i++)
            L[i] = arr[izq + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[medio + 1 + j];

        int i = 0, j = 0, k = izq;

        while (i < n1 && j < n2) {
            if (comparador(L[i], R[j])) {
                arr[k] = L[i];
                i++;
            }
            else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    // ========================================
    // QUICK SORT PARA VECTORES
    // ========================================
    static void quickSortVector(vector<T>& arr, int bajo, int alto,
        function<bool(const T&, const T&)> comparador) {
        if (bajo >= alto) return;

        // Usar mediana de tres para mejor rendimiento
        int medio = bajo + (alto - bajo) / 2;
        int pivotIndex = medianaDeTresVector(arr, bajo, medio, alto, comparador);

        // Mover pivote al final temporalmente
        swap(arr[pivotIndex], arr[alto]);

        int indiceParticion = particionVector(arr, bajo, alto, comparador);

        quickSortVector(arr, bajo, indiceParticion - 1, comparador);
        quickSortVector(arr, indiceParticion + 1, alto, comparador);
    }

    static int particionVector(vector<T>& arr, int bajo, int alto,
        function<bool(const T&, const T&)> comparador) {
        T pivot = arr[alto];
        int i = bajo - 1;

        for (int j = bajo; j < alto; j++) {
            if (comparador(arr[j], pivot)) {
                i++;
                swap(arr[i], arr[j]);
            }
        }

        swap(arr[i + 1], arr[alto]);
        return i + 1;
    }

    static int medianaDeTresVector(vector<T>& arr, int bajo, int medio, int alto,
        function<bool(const T&, const T&)> comparador) {
        if (comparador(arr[bajo], arr[medio])) {
            if (comparador(arr[medio], arr[alto]))
                return medio;
            else if (comparador(arr[bajo], arr[alto]))
                return alto;
            else
                return bajo;
        }
        else {
            if (comparador(arr[bajo], arr[alto]))
                return bajo;
            else if (comparador(arr[medio], arr[alto]))
                return alto;
            else
                return medio;
        }
    }

    // ========================================
    // HEAP SORT PARA VECTORES
    // ========================================
    static void heapSortVector(vector<T>& arr,
        function<bool(const T&, const T&)> comparador) {
        int n = arr.size();

        // Construir heap
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapifyVector(arr, n, i, comparador);
        }

        // Extraer elementos del heap uno por uno
        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapifyVector(arr, i, 0, comparador);
        }
    }

    static void heapifyVector(vector<T>& arr, int n, int i,
        function<bool(const T&, const T&)> comparador) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && comparador(arr[largest], arr[left])) {
            largest = left;
        }

        if (right < n && comparador(arr[largest], arr[right])) {
            largest = right;
        }

        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapifyVector(arr, n, largest, comparador);
        }
    }



    // ========================================
    // MÉTODOS PARA LISTAS ENLAZADAS
    // ========================================
    static void actualizarCola(Nodo<T>* cabeza, Nodo<T>*& cola) {
        if (!cabeza) {
            cola = nullptr;
            return;
        }

        Nodo<T>* temp = cabeza;
        while (temp->getSiguiente() != nullptr) {
            temp = temp->getSiguiente();
        }
        cola = temp;
    }

    static Nodo<T>* mergeSort(Nodo<T>* head, function<bool(const T&, const T&)> comparador) {
        if (!head || !head->getSiguiente()) return head;

        Nodo<T>* middle = dividir(head);
        Nodo<T>* mitad_derecha = middle->getSiguiente();
        middle->setSiguiente(nullptr);
        if (mitad_derecha) mitad_derecha->setAnterior(nullptr);

        Nodo<T>* izquierda = mergeSort(head, comparador);
        Nodo<T>* derecha = mergeSort(mitad_derecha, comparador);

        return merge(izquierda, derecha, comparador);
    }

    static Nodo<T>* merge(Nodo<T>* izquierda, Nodo<T>* derecha,
        function<bool(const T&, const T&)> comparador) {
        if (!izquierda) return derecha;
        if (!derecha) return izquierda;

        Nodo<T>* resultado = nullptr;

        if (comparador(izquierda->getData(), derecha->getData())) {
            resultado = izquierda;
            resultado->setSiguiente(merge(izquierda->getSiguiente(), derecha, comparador));
            if (resultado->getSiguiente()) {
                resultado->getSiguiente()->setAnterior(resultado);
            }
            resultado->setAnterior(nullptr);
        }
        else {
            resultado = derecha;
            resultado->setSiguiente(merge(izquierda, derecha->getSiguiente(), comparador));
            if (resultado->getSiguiente()) {
                resultado->getSiguiente()->setAnterior(resultado);
            }
            resultado->setAnterior(nullptr);
        }
        return resultado;
    }

    static Nodo<T>* dividir(Nodo<T>* head) {
        Nodo<T>* lento = head;
        Nodo<T>* rapido = head->getSiguiente();

        while (rapido && rapido->getSiguiente()) {
            lento = lento->getSiguiente();
            rapido = rapido->getSiguiente()->getSiguiente();
        }
        return lento;
    }

    static void quickSortWrapper(Nodo<T>*& cabeza, Nodo<T>*& cola,
        function<bool(const T&, const T&)> comparador) {
        quickSort(cabeza, cola, comparador);
    }

    static void quickSort(Nodo<T>* cabeza, Nodo<T>* cola,
        function<bool(const T&, const T&)> comparador) {
        if (!cabeza || cabeza == cola || !cola) return;

        Nodo<T>* pivot = particion(cabeza, cola, comparador);

        if (pivot != nullptr && pivot->getAnterior() != nullptr) {
            quickSort(cabeza, pivot->getAnterior(), comparador);
        }

        if (pivot != nullptr && pivot->getSiguiente() != nullptr) {
            quickSort(pivot->getSiguiente(), cola, comparador);
        }
    }

    static Nodo<T>* particion(Nodo<T>* cabeza, Nodo<T>* cola,
        function<bool(const T&, const T&)> comparador) {
        T pivotData = cola->getData();
        Nodo<T>* i = nullptr;

        for (Nodo<T>* j = cabeza; j != cola; j = j->getSiguiente()) {
            if (comparador(j->getData(), pivotData) || j->getData() == pivotData) {
                if (i == nullptr) {
                    i = cabeza;
                }
                else {
                    i = i->getSiguiente();
                }

                T temp = i->getData();
                i->setData(j->getData());
                j->setData(temp);
            }
        }

        if (i == nullptr) {
            i = cabeza;
        }
        else {
            i = i->getSiguiente();
        }

        T temp = i->getData();
        i->setData(cola->getData());
        cola->setData(temp);

        return i;
    }

    static void heapify(vector<T>& datos, int n, int i, function<bool(const T&, const T&)> comparador) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && comparador(datos[largest], datos[left])) {
            largest = left;
        }

        if (right < n && comparador(datos[largest], datos[right])) {
            largest = right;
        }

        if (largest != i) {
            T temp = datos[i];
            datos[i] = datos[largest];
            datos[largest] = temp;

            heapify(datos, n, largest, comparador);
        }
    }

    static void construirHeapManual(vector<T>& datos, function<bool(const T&, const T&)> comparador) {
        int n = datos.size();
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(datos, n, i, comparador);
        }
    }

    static void heapSort(Nodo<T>* cabeza, function<bool(const T&, const T&)> comparador) {
        if (!cabeza) return;

        vector<T> datos;
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            datos.push_back(actual->getData());
            actual = actual->getSiguiente();
        }

        int n = datos.size();

        construirHeapManual(datos, comparador);

        for (int i = n - 1; i > 0; i--) {
            T temp = datos[0];
            datos[0] = datos[i];
            datos[i] = temp;

            heapify(datos, i, 0, comparador);
        }

        actual = cabeza;
        for (const T& datoOrdenado : datos) {
            actual->setData(datoOrdenado);
            actual = actual->getSiguiente();
        }
    }

    static void insertionSort(Nodo<T>* cabeza, function<bool(const T&, const T&)> comparador) {

        if (!cabeza) return;
        // Convertir lista enlazada a vector
        vector<T> datos;
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            datos.push_back(actual->getData());
            actual = actual->getSiguiente();
        }

        // Ordenar el vector usando Insertion Sort
        insertionSortVector(datos, comparador);
        // Actualizar los datos en la lista enlazada
        actual = cabeza;
        for (const T& datoOrdenado : datos) {
            actual->setData(datoOrdenado);
            actual = actual->getSiguiente();
        }
    }
};