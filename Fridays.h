#pragma once
#include "Funcionalidades.h"
#include "Carta.h"
#include "CartaLocal.h"
#include "CartaDelivery.h"
#include "Cliente.h"
#include "Pedido.h"
#include "Pila.h"
#include "Cola.h"
#include "GestorPedidos.h"
#include "GestorArchivos.h"
#include "PagoEfectivo.h"
#include "PagoTarjeta.h"
#include "Boleta.h"
#include "Factura.h"
#include "GestorReserva.h"
#include "GeneradorDataSet.h"
#include "GestorBackup.h"
#include "GestorClientes.h"
#include "ControladoraCliente.h"
#include "ControladoraAdmin.h"

using namespace UtilidadesConsola;
using namespace std;

class Fridays {
private:
    Carta* menuPrincipal;
    string nombreRestaurante;
    string direccion;
    string telefono;
    Cola<Pedido*> colaPedidos;
    Pila<Pedido*> historialPedidos;
    int contadorPedidos = 1;
    int contadorClientes = 1;
    GestorReserva* gestorReservas;

    ControladoraCliente* controladoraCliente;
    ControladoraAdmin* controladoraAdmin;

public:
    Fridays(string nombre = "Fridays Restaurant",
        string dir = "Av. Principal 123",
        string tel = "555-1234")
        : nombreRestaurante(nombre), direccion(dir), telefono(tel) {

        menuPrincipal = new Carta();
        gestorReservas = new GestorReserva();

        // Inicializar controladoras
        controladoraCliente = new ControladoraCliente(
            menuPrincipal, gestorReservas, colaPedidos,
            contadorPedidos, contadorClientes
        );

        controladoraAdmin = new ControladoraAdmin(
            menuPrincipal, gestorReservas, colaPedidos,
            historialPedidos, contadorPedidos
        );

        GestorBackup::crearBackupsTotales();
        cout << ">>> Sistema Fridays inicializado" << endl;
    }

    ~Fridays() {
        delete menuPrincipal;
        delete gestorReservas;
        delete controladoraCliente;
        delete controladoraAdmin;
    }

    Carta* getMenu() {
        return menuPrincipal;
    }

    const string espacio = "                                          ";

    int menuPrincipalUI() {
        limpiarPantalla();
        mostrarLogoPrincipal();
        cout << "\n\n";
        cout << espacio << ROJO << "!BIENVENIDOS! \n" << endl;
        cout << espacio << AMARILLO << "Seleccione modo de acceso:\n";
        cout << espacio;
        imprimirSeparadorRojoBlanco(20);
        cout << ConsolaColor::BLANCO;
        cout << espacio << "1. CLIENTE \n";
        cout << espacio << "2. ADMINISTRADOR \n";
        cout << espacio << "3. Salir\n";
        cout << espacio;
        imprimirSeparadorRojoBlanco(20);
        cout << espacio << "Opcion: ";

        return leerOpcion();
    }

    void ejecutar() {
        GestorPedidos::cargarHistorial(historialPedidos, contadorPedidos);
        int opcion;
        do {
            opcion = menuPrincipalUI();
            switch (opcion) {
            case 1:
                controladoraCliente->modoCliente();
                break;
            case 2:
                controladoraAdmin->modoAdministrador();
                break;
            case 3:

                limpiarPantalla();
                // Guardar datos antes de salir
                GestorPedidos::guardarHistorial(historialPedidos);
                GestorBackup::crearBackupsTotales();
                break;
            default:
                cout << espacio << ROJO_BRILLANTE << "Opcion invalida. Intente de nuevo." << endl;
                pausar();
                break;
            }
        } while (opcion != 3);
    }

    void limpiarPantalla() {
        system("cls");
    }

    void pausar() {
        cout << ROJO << "Presione Enter para continuar...";
        cin.ignore(10000, '\n');
    }

    int leerOpcion() {
        int opcion;
        while (true) {
            string entrada;
            getline(cin, entrada);
            try {
                opcion = stoi(entrada);
                break;
            }
            catch (const exception& e) {
                cout << "Entrada invalida. Ingrese un numero: ";
            }
        }
        return opcion;
    }

    void guardarDatos() {
        menuPrincipal->guardarEnArchivo();
        cout << VERDE << "Datos guardados exitosamente" << endl;
    }

    void cargarDatos() {
        menuPrincipal->cargarDesdeArchivo();
    }

    // Getters
    string getNombreRestaurante() const { return nombreRestaurante; }
    string getDireccion() const { return direccion; }
    string getTelefono() const { return telefono; }

    // Setters
    void setNombreRestaurante(const string& nombre) { nombreRestaurante = nombre; }
    void setDireccion(const string& dir) { direccion = dir; }
    void setTelefono(const string& tel) { telefono = tel; }
};