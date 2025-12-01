#pragma once
#include "Funcionalidades.h"

class GeneradorDataset {
public:
    static int cargarDatasetPedidos(const string& archivoCSV = "Data/pedidos_dataset.csv") {
        ifstream file(archivoCSV);

        if (!file.is_open()) {
            cout << "\n ERROR: No se encontro " << archivoCSV << endl;
            return 0;
        }

        string linea;
        int contador = 0;
        int clientesNuevos = 0;

        getline(file, linea);

        if (linea.empty()) {
            cout << " ERROR: El archivo CSV esta vacio" << endl;
            file.close();
            return 0;
        }

        if (linea.find(',') == string::npos) {
            cout << ">>> ERROR: El archivo no usa coma (,) como separador" << endl;
            file.close();
            return 0;
        }


        ofstream historial("Data/historialPedidos.txt", ios::app);

        if (!historial.is_open()) {
            cout << " ERROR: No se pudo abrir historialPedidos.txt" << endl;
            file.close();
            return 0;
        }

        //Rastrear DNIs ya procesados
        Lista<string> dnisYaProcesados;

        // Copiar líneas al historial
        while (getline(file, linea)) {
            if (linea.empty()) continue;


            int comas = 0;
            for (char c : linea) {
                if (c == ',') comas++;
            }

            if (comas != 10) {  // 11 campos = 10 comas
                cout << "    ! Linea " << (contador + 1) << " omitida (formato incorrecto)" << endl;
                continue;
            }


            stringstream ss(linea);
            string pedidoID, nombreCompleto, dni, telefono, direccion;
            string resto;

            getline(ss, pedidoID, ',');
            getline(ss, nombreCompleto, ',');
            getline(ss, dni, ',');
            getline(ss, telefono, ',');
            getline(ss, direccion, ',');
            getline(ss, resto);

            //Guardar cliente si no existe (verificar si DNI ya fue procesado)
            bool yaExiste = false;
            for (int i = 0; i < dnisYaProcesados.gettamano(); i++) {
                if (dnisYaProcesados.obtenerEnPosicion(i) == dni) {
                    yaExiste = true;
                    break;
                }
            }

            if (!yaExiste) {
                if (guardarClienteDesdeDataset(nombreCompleto, dni, telefono, direccion)) {
                    clientesNuevos++;
                    dnisYaProcesados.insertarFinal(dni);
                }
            }

            // Guardar pedido
            historial << linea << "\n";
            contador++;
        }

        file.close();
        historial.close();


        if (clientesNuevos > 0) {
            cout << VERDE << "\n>>> Clientes extraidos del dataset: " << clientesNuevos << RESET << endl;
        }

        return contador;
    }

    //Cargar dataset de reservas
    static int cargarDatasetReservas(const string& archivoCSV = "Data/reservas_dataset.csv") {
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


        if (linea.find(',') == string::npos) {
            cout << ">>> ERROR: El archivo no usa coma (,) como separador" << endl;
            file.close();
            return 0;
        }


        ofstream reservas("Data/reservas.txt", ios::app);

        if (!reservas.is_open()) {
            cout << " ERROR: No se pudo abrir reservas.txt" << endl;
            file.close();
            return 0;
        }

        // Copiar líneas al archivo de reservas
        while (getline(file, linea)) {
            if (linea.empty()) continue;

            int comas = 0;
            for (char c : linea) {
                if (c == ',') comas++;
            }

            if (comas != 14) {  // 15 campos = 14 comas
                cout << "    ! Linea " << (contador + 1) << " omitida (formato incorrecto)" << endl;
                continue;
            }

            reservas << linea << "\n";
            contador++;
        }

        file.close();
        reservas.close();

        return contador;
    }




    //Dataset de clientes
    static int cargarDatasetClientes(const string& archivoCSV = "Data/clientes_dataset.csv") {
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

        ofstream clientes("Data/clientes.txt", ios::app);

        if (!clientes.is_open()) {
            cout << " ERROR: No se pudo abrir clientes.txt" << endl;
            file.close();
            return 0;
        }

        // Copiar líneas
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

            clientes << linea << "\n";
            contador++;
        }

        file.close();
        clientes.close();

        return contador;
    }

    static int cargarDatasetListaEspera(const string& archivoCSV = "Data/lista_espera_dataset.csv") {
        ifstream file(archivoCSV);

        if (!file.is_open()) {
            cout << "\n ERROR: No se encontro " << archivoCSV << endl;
            return 0;
        }

        string linea;
        int contador = 0;

        // Saltar encabezado
        getline(file, linea);

        if (linea.empty()) {
            cout << " ERROR: El archivo CSV esta vacio" << endl;
            file.close();
            return 0;
        }

        // Abrir archivo de lista de espera
        ofstream listaEspera("Data/lista_espera.txt", ios::app);

        if (!listaEspera.is_open()) {
            cout << " ERROR: No se pudo abrir lista_espera.txt" << endl;
            file.close();
            return 0;
        }

        // Copiar líneas
        while (getline(file, linea)) {
            if (linea.empty()) continue;

            int comas = 0;
            for (char c : linea) {
                if (c == ',') comas++;
            }

            // Formato: DNI,Nombre,Apellido,Telefono,Fecha,Hora,NumPersonas,NumMesa,Categoria,PuntosLealtad
            if (comas != 9) {
                cout << "    ! Linea omitida (formato incorrecto)" << endl;
                continue;
            }

            listaEspera << linea << "\n";
            contador++;
        }

        file.close();
        listaEspera.close();

        return contador;
    }












    static void cargarDataset() {
        int pedidos = cargarDatasetPedidos();
        int reservas = cargarDatasetReservas();
    }


private:
    // Función auxiliar para guardar cliente desde dataset
    static bool guardarClienteDesdeDataset(const string& nombreCompleto,
        const string& dni,
        const string& telefono,
        const string& direccion) {
        // Verificar si el cliente ya existe
        ifstream checkFile("Data/clientes.txt");
        if (checkFile.is_open()) {
            string linea;
            while (getline(checkFile, linea)) {
                if (linea.find(dni) != string::npos) {
                    checkFile.close();
                    return false; // Cliente ya existe
                }
            }
            checkFile.close();
        }


        string nombre, apellido;
        size_t pos = nombreCompleto.find(' ');

        if (pos != string::npos) {
            nombre = nombreCompleto.substr(0, pos);
            apellido = nombreCompleto.substr(pos + 1);
        }
        else {
            nombre = nombreCompleto;
            apellido = "";
        }

        // Generar ID único 
        int id = stoi(dni.substr(0, min(dni.length(), size_t(8)))) % 100000;

        // Abrir archivo de clientes
        ofstream clientesFile("Data/clientes.txt", ios::app);
        if (!clientesFile.is_open()) {
            return false;
        }


        clientesFile << id << ","
            << nombre << ","
            << apellido << ","
            << telefono << ","
            << dni << ","
            << direccion << ","
            << "Regular" << ","
            << "0" << "\n";

        clientesFile.close();
        return true;
    }
};

