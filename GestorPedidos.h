#pragma once
#include "Pedido.h"
#include "Pila.h"
#include "Cliente.h"
#include "Producto.h"
#include "Comprobante.h"
#include "Funcionalidades.h"

class GestorPedidos {
public:
    static bool guardarHistorial(const Pila<Pedido*>& historial, const string& nombreArchivo = "Data/historialPedidos.txt") {
        ofstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo " << nombreArchivo << " para escribir." << endl;
            return false;
        }
        archivo << "# Historial de Pedidos - Fridays Restaurant" << endl;
        archivo << "# Formato: PedidoID,ClienteNombre,ClienteDNI,ClienteTelefono,ClienteDireccion,Total,Fecha,Estado,TipoServicio,TipoComprobante,NumComprobante" << endl;

        for (int i = 0; i < historial.gettamano(); i++) {
            Pedido* p = historial.obtenerEnPosicion(i);
            Cliente* c = p->getCliente();
            Comprobante* comp = p->getComprobante();

            archivo << p->getNumeroPedido() << ","
                << (c ? c->getNombreCompleto() : "Sin cliente") << ","
                << (c ? c->getDni() : "") << ","
                << (c ? c->getTelefono() : "") << ","
                << (c ? c->getDireccion() : "") << ","
                << p->calcularTotal() << ","
                << (comp ? comp->getFecha() : "Sin fecha") << ","
                << p->getEstado() << ","
                << p->getTipoServicio() << ","
                << (comp ? comp->getTipoComprobante() : "Sin comprobante") << ","
                << (comp ? comp->getNumeroComprobante() : 0) << endl;
        }

        archivo.close();
        return true;
    }

    static void cargarHistorial(Pila<Pedido*>& historial, int& contadorPedidos, const string& nombreArchivo = "Data/historialPedidos.txt") {
        ifstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            cout << ">>> Archivo " << nombreArchivo << " no encontrado. Se creara uno nuevo." << endl;
            return;
        }

        string linea;
        int pedidosLeidos = 0;
        int maxNumeroPedido = 0;

        while (getline(archivo, linea)) {

            if (linea.empty() || linea[0] == '#') {
                continue;
            }


            stringstream ss(linea);
            string numPedidoStr, nombreCliente, dni, telefono, direccion;
            string totalStr, fecha, estado, tipoServicio, tipoComprobante, numComprobanteStr;

            if (getline(ss, numPedidoStr, ',') &&
                getline(ss, nombreCliente, ',') &&
                getline(ss, dni, ',') &&
                getline(ss, telefono, ',') &&
                getline(ss, direccion, ',') &&
                getline(ss, totalStr, ',') &&
                getline(ss, fecha, ',') &&
                getline(ss, estado, ',') &&
                getline(ss, tipoServicio, ',') &&
                getline(ss, tipoComprobante, ',') &&
                getline(ss, numComprobanteStr)) {

                try {
                    int numPedido = stoi(numPedidoStr);
                    double total = stod(totalStr);

                    // Actualizar contador de pedidos
                    if (numPedido > maxNumeroPedido) {
                        maxNumeroPedido = numPedido;
                    }

                    // Crear cliente temporal
                    string nombre = nombreCliente.substr(0, nombreCliente.find(' '));
                    string apellido = nombreCliente.substr(nombreCliente.find(' ') + 1);
                    Cliente* cliente = new Cliente(0, nombre, apellido, telefono, dni, direccion);

                    // Crear pedido básico
                    Pedido* pedido = new Pedido(numPedido, cliente, tipoServicio);
                    pedido->cambiarEstado(estado);


                    pedido->setTotalManual(total);

                    // Agregar a historial
                    historial.apilar(pedido);
                    pedidosLeidos++;
                }
                catch (const exception& e) {
                    cout << " Error al procesar linea: " << linea << endl;
                    cout << " Detalle: " << e.what() << endl;
                }
            }
        }

        archivo.close();


        if (maxNumeroPedido > 0) {
            contadorPedidos = maxNumeroPedido + 1;
        }

        if (pedidosLeidos > 0) {

        }
    }


    // Buscar pedidos por cliente (usando lambda)
    static Pila<Pedido*> buscarPorCliente(const Pila<Pedido*>& historial, const string& dni) {
        Pila<Pedido*> resultado;

        for (int i = 0; i < historial.gettamano(); i++) {
            Pedido* p = historial.obtenerEnPosicion(i);
            Cliente* c = p->getCliente();
            if (c && c->getDni() == dni) {
                resultado.apilar(p);
            }
        }

        return resultado;
    }

    // Filtrar pedidos usando lambda
    static Pila<Pedido*> filtrarPedidos(const Pila<Pedido*>& historial,
        function<bool(Pedido*)> condicion) {
        Pila<Pedido*> resultado;

        for (int i = 0; i < historial.gettamano(); i++) {
            Pedido* p = historial.obtenerEnPosicion(i);
            if (condicion(p)) {
                resultado.apilar(p);
            }
        }

        return resultado;
    }

    // Calcular total de ventas
    static double calcularTotalVentas(const Pila<Pedido*>& historial) {
        double total = 0.0;
        for (int i = 0; i < historial.gettamano(); i++) {
            total += historial.obtenerEnPosicion(i)->calcularTotal();
        }
        return total;
    }

    // ========================================
    // ORDENAMIENTO POR NOMBRE (Quick Sort) - USANDO GESTORORDENAMIENTO
    // ========================================
    static void ordenarPorNombre(Pila<Pedido*>& historial, bool ascendente = true) {
        if (historial.gettamano() <= 1) {
            cout << ">>> No hay suficientes pedidos para ordenar" << endl;
            return;
        }

        // Usar el GestorOrdenamiento con Quick Sort
        if (ascendente) {
            historial.ordenar([](Pedido* const& a, Pedido* const& b) {
                return compararNombres(a->getNombreCliente(), b->getNombreCliente()) < 0;
                }, GestorOrdenamiento<Pedido*>::QUICK_SORT);
        }
        else {
            historial.ordenar([](Pedido* const& a, Pedido* const& b) {
                return compararNombres(a->getNombreCliente(), b->getNombreCliente()) > 0;
                }, GestorOrdenamiento<Pedido*>::QUICK_SORT);
        }

        cout << VERDE << ">>> Pedidos ordenados por nombre del cliente "
            << (ascendente ? "(A-Z)" : "(Z-A)") << RESET << endl;
    }

    // ========================================
    // ORDENAMIENTO POR FECHA (Quick Sort) - USANDO GESTORORDENAMIENTO
    // ========================================
    static void ordenarPorFecha(Pila<Pedido*>& historial, bool ascendente = true) {
        if (historial.gettamano() <= 1) {
            cout << ">>> No hay suficientes pedidos para ordenar" << endl;
            return;
        }

        // Usar el GestorOrdenamiento con Quick Sort
        if (ascendente) {
            historial.ordenar([](Pedido* const& a, Pedido* const& b) {
                return compararFechas(a->getFechaPedido(), b->getFechaPedido()) < 0;
                }, GestorOrdenamiento<Pedido*>::QUICK_SORT);
        }
        else {
            historial.ordenar([](Pedido* const& a, Pedido* const& b) {
                return compararFechas(a->getFechaPedido(), b->getFechaPedido()) > 0;
                }, GestorOrdenamiento<Pedido*>::QUICK_SORT);
        }

        cout << VERDE << ">>> Pedidos ordenados por fecha "
            << (ascendente ? "(más antiguos primero)" : "(más recientes primero)")
            << RESET << endl;
    }

private:

    // ========================================
    // FUNCIONES AUXILIARES PARA COMPARACIÓN
    // (Usadas por los lambdas en ordenarPorNombre y ordenarPorFecha)
    // ========================================
    static int compararFechas(const string& fecha1, const string& fecha2) {
        // Formato esperado: DD/MM/YYYY
        if (fecha1.length() < 10 || fecha2.length() < 10) {
            return (fecha1.length() < fecha2.length()) ? -1 :
                (fecha1.length() > fecha2.length()) ? 1 : 0;
        }

        try {
            // Extraer día, mes, año de fecha1
            int dia1 = stoi(fecha1.substr(0, 2));
            int mes1 = stoi(fecha1.substr(3, 2));
            int anio1 = stoi(fecha1.substr(6, 4));

            // Extraer día, mes, año de fecha2
            int dia2 = stoi(fecha2.substr(0, 2));
            int mes2 = stoi(fecha2.substr(3, 2));
            int anio2 = stoi(fecha2.substr(6, 4));

            // Comparar año
            if (anio1 != anio2) return (anio1 < anio2) ? -1 : 1;

            // Comparar mes
            if (mes1 != mes2) return (mes1 < mes2) ? -1 : 1;

            // Comparar día
            if (dia1 != dia2) return (dia1 < dia2) ? -1 : 1;

            return 0; // Son iguales
        }
        catch (const exception& e) {
            return 0; // En caso de error, considerarlas iguales
        }
    }
    static int compararNombres(const string& nombre1, const string& nombre2) {
        // Convertir ambos nombres a minúsculas para comparación
        string n1 = "";
        string n2 = "";

        for (char c : nombre1) {
            if (c >= 'A' && c <= 'Z') {
                n1 += (c + 32);  // Convertir a minúscula
            }
            else {
                n1 += c;
            }
        }

        for (char c : nombre2) {
            if (c >= 'A' && c <= 'Z') {
                n2 += (c + 32);  // Convertir a minúscula
            }
            else {
                n2 += c;
            }
        }

        // Comparar usando el operador estándar
        if (n1 < n2) return -1;
        if (n1 > n2) return 1;
        return 0;
    }
};