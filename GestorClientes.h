#pragma once
#include "Cliente.h"
#include "Funcionalidades.h"
#include "GestorOrdenamiento.h"
#include <vector>
using namespace ConsolaColor;

class GestorClientes {
public:


    static void guardarCliente(Cliente* cliente, const string& archivo = "Data/clientes.txt") {

        ifstream check(archivo);
        bool vacio = check.peek() == ifstream::traits_type::eof();
        check.close();

        ofstream file(archivo, ios::app);
        if (!file.is_open()) {
            cout << "Error al guardar cliente" << endl;
            return;
        }

        if (vacio) {
            file << "# Base de Datos de Clientes - Fridays\n";
            file << "# ID,Nombre,Apellido,Telefono,DNI,Direccion,Categoria,PuntosLealtad\n";
        }

        file << cliente->getId() << ","
            << cliente->getNombre() << ","
            << cliente->getApellido() << ","
            << cliente->getTelefono() << ","
            << cliente->getDni() << ","
            << cliente->getDireccion() << ","
            << cliente->getCategoria() << ","
            << cliente->getPuntosLealtad() << "\n";

        file.close();
    }


    static Cliente* buscarClientePorDNI(const string& dni, const string& archivo = "Data/clientes.txt") {
        ifstream file(archivo);

        if (!file.is_open()) {
            return nullptr;
        }

        string linea;
        while (getline(file, linea)) {
            if (linea.empty() || linea[0] == '#') continue;

            stringstream ss(linea);
            string idStr, nombre, apellido, telefono, dniLeido, direccion, categoria, puntosStr;

            getline(ss, idStr, ',');
            getline(ss, nombre, ',');
            getline(ss, apellido, ',');
            getline(ss, telefono, ',');
            getline(ss, dniLeido, ',');
            getline(ss, direccion, ',');
            getline(ss, categoria, ',');
            getline(ss, puntosStr);

            if (dniLeido == dni) {
                int id = stoi(idStr);
                int puntos = stoi(puntosStr);

                Cliente* cliente = new Cliente(id, nombre, apellido, telefono,
                    dni, direccion, categoria, puntos);

                file.close();
                return cliente;
            }
        }

        file.close();
        return nullptr;
    }

    static void actualizarCliente(Cliente* cliente, const string& archivo = "Data/clientes.txt") {
        vector<string> lineas;
        ifstream archivoIn(archivo);
        string linea;

        while (getline(archivoIn, linea)) {
            if (linea.empty() || linea[0] == '#') {
                lineas.push_back(linea);
                continue;
            }

            stringstream ss(linea);
            string dniLeido, campo;


            for (int i = 0; i < 4; i++) {
                getline(ss, campo, ',');
            }
            getline(ss, dniLeido, ',');

            if (dniLeido == cliente->getDni()) {

                string nuevaLinea = to_string(cliente->getId()) + ","
                    + cliente->getNombre() + ","
                    + cliente->getApellido() + ","
                    + cliente->getTelefono() + ","
                    + cliente->getDni() + ","
                    + cliente->getDireccion() + ","
                    + cliente->getCategoria() + ","
                    + to_string(cliente->getPuntosLealtad());
                lineas.push_back(nuevaLinea);
            }
            else {
                lineas.push_back(linea);
            }
        }
        archivoIn.close();


        ofstream archivoOut(archivo);
        for (const string& l : lineas) {
            archivoOut << l << "\n";
        }
        archivoOut.close();
    }


    static int cargarDatasetClientes(const string& archivoCSV = "Data/clientes_dataset.csv",
        const string& archivoDestino = "Data/clientes.txt") {
        ifstream file(archivoCSV);

        if (!file.is_open()) {
            cout << "\n ERROR: No se encontro " << archivoCSV << endl;
            return 0;
        }

        string linea;
        int contador = 0;


        getline(file, linea);

        if (linea.empty()) {
            cout << " ERROR: El archivo CSV esta vacio" << endl;
            file.close();
            return 0;
        }


        ofstream destino(archivoDestino, ios::app);

        if (!destino.is_open()) {
            cout << " ERROR: No se pudo abrir " << archivoDestino << endl;
            file.close();
            return 0;
        }


        while (getline(file, linea)) {
            if (linea.empty()) continue;


            int comas = 0;
            for (char c : linea) {
                if (c == ',') comas++;
            }

            if (comas != 7) {
                cout << "    ! Linea omitida (formato incorrecto)" << endl;
                continue;
            }

            destino << linea << "\n";
            contador++;
        }

        file.close();
        destino.close();

        return contador;
    }


    static void listarTodosClientes(const string& archivo = "Data/clientes.txt") {
        ifstream file(archivo);

        if (!file.is_open()) {
            cout << "No hay clientes registrados" << endl;
            return;
        }

        cout << ROJO << "\n                   LISTA DE CLIENTES    " << RESET << endl;
        imprimirSeparadorRojoBlanco(30);

        cout << left << setw(8) << "ID"
            << setw(22) << "NOMBRE"
            << setw(10) << "DNI"
            << setw(12) << "CATEGORIA"
            << setw(8) << "PUNTOS" << endl;
        imprimirSeparadorRojoBlanco(30);

        string linea;
        int total = 0;

        while (getline(file, linea)) {
            if (linea.empty() || linea[0] == '#') continue;

            stringstream ss(linea);
            string idStr, nombre, apellido, telefono, dni, direccion, categoria, puntosStr;

            getline(ss, idStr, ',');
            getline(ss, nombre, ',');
            getline(ss, apellido, ',');
            getline(ss, telefono, ',');
            getline(ss, dni, ',');
            getline(ss, direccion, ',');
            getline(ss, categoria, ',');
            getline(ss, puntosStr);

            string nombreCompleto = nombre + " " + apellido;
            string colorCategoria = (categoria == "Premium") ? AMARILLO :
                (categoria == "VIP") ? VERDE : BLANCO;

            cout << left << setw(8) << idStr
                << setw(22) << nombreCompleto
                << setw(10) << dni
                << colorCategoria << setw(12) << categoria << RESET
                << setw(8) << puntosStr << endl;

            total++;
        }

        file.close();
        imprimirSeparadorRojoBlanco(30);
        cout << "Total clientes: " << total << endl;
        imprimirSeparadorRojoBlanco(30);
    }

    // ========================================
    // ORDENAMIENTO DE CLIENTES POR ID
    // Utiliza el algoritmo Insertion Sort
    // ========================================

    static void ordenarClientesPorID(bool ascendente = true, const string& archivo = "Data/clientes.txt") {
        ifstream file(archivo);
        if (!file.is_open()) {
            cout << "No hay clientes registrados" << endl;
            return;

        }

        // Vector para almacenar los clientes
        vector<Cliente> clientes;
        string linea;

        // Leer todos los clientes del archivo
        while (getline(file, linea)) {

            if (linea.empty() || linea[0] == '#') continue;
            stringstream ss(linea);
            string idStr, nombre, apellido, telefono, dni, direccion, categoria, puntosStr;
            getline(ss, idStr, ',');
            getline(ss, nombre, ',');
            getline(ss, apellido, ',');
            getline(ss, telefono, ',');
            getline(ss, dni, ',');
            getline(ss, direccion, ',');
            getline(ss, categoria, ',');
            getline(ss, puntosStr);

            int id = stoi(idStr);
            int puntos = stoi(puntosStr);

            // Crear cliente y agregarlo al vector
            Cliente cliente(id, nombre, apellido, telefono, dni, direccion, categoria, puntos);
            clientes.push_back(cliente);

        }



        file.close();

        // Verificar si hay clientes

        if (clientes.empty()) {
            cout << "No hay clientes para ordenar" << endl;
            return;
        }



        // Definir el comparador según el orden deseado
        function<bool(const Cliente&, const Cliente&)> comparador;

        if (ascendente) {
            // Orden ascendente: de menor a mayor ID
            comparador = [](const Cliente& a, const Cliente& b) {

                return a.getId() < b.getId();

                };

        }
        else {

            // Orden descendente: de mayor a menor ID
            comparador = [](const Cliente& a, const Cliente& b) {
                return a.getId() > b.getId();
                };

        }


        // Ordenar usando Insertion Sort
        GestorOrdenamiento<Cliente>::ordenarVector(clientes, comparador, GestorOrdenamiento<Cliente>::INSERTION_SORT);

        // Mostrar los clientes ordenados

        cout << ROJO << "\n          CLIENTES ORDENADOS POR ID (" << (ascendente ? "ASCENDENTE" : "DESCENDENTE") << ")    " << RESET << endl;

        imprimirSeparadorRojoBlanco(30);

        cout << left << setw(8) << "ID" << setw(22) << "NOMBRE" << setw(10) << "DNI" << setw(12) << "CATEGORIA" << setw(8) << "PUNTOS" << endl;

        imprimirSeparadorRojoBlanco(30);



        // Mostrar cada cliente ordenado

        for (const Cliente& cliente : clientes) {

            string nombreCompleto = cliente.getNombre() + " " + cliente.getApellido();

            string colorCategoria = (cliente.getCategoria() == "Premium") ? AMARILLO :

                (cliente.getCategoria() == "VIP") ? VERDE : BLANCO;



            cout << left << setw(8) << cliente.getId()

                << setw(22) << nombreCompleto

                << setw(10) << cliente.getDni()

                << colorCategoria << setw(12) << cliente.getCategoria() << RESET

                << setw(8) << cliente.getPuntosLealtad() << endl;

        }



        imprimirSeparadorRojoBlanco(30);

        cout << "Total clientes ordenados: " << clientes.size() << endl;

        imprimirSeparadorRojoBlanco(30);

        cout << "\n Algoritmo utilizado: INSERTION SORT" << endl;

        cout << " Complejidad: O(n^2) en peor caso, O(n) en mejor caso" << endl;

        imprimirSeparadorRojoBlanco(30);

    }

};
