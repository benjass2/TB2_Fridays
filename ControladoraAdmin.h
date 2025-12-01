#pragma once
#include "Funcionalidades.h"
#include "Carta.h"
#include "Pedido.h"
#include "Pila.h"
#include "Cola.h"
#include "GestorPedidos.h"
#include "GestorReserva.h"
#include "GestorClientes.h"
#include "GeneradorDataSet.h"

using namespace UtilidadesConsola;
using namespace std;

class ControladoraAdmin {
private:
    Carta* menuPrincipal;
    GestorReserva* gestorReservas;
    Cola<Pedido*>& colaPedidos;
    Pila<Pedido*>& historialPedidos;
    int& contadorPedidos;

public:
    ControladoraAdmin(Carta* menu, GestorReserva* gestor,
        Cola<Pedido*>& cola, Pila<Pedido*>& historial, int& contPed)
        : menuPrincipal(menu), gestorReservas(gestor),
        colaPedidos(cola), historialPedidos(historial),
        contadorPedidos(contPed) {
    }

    void modoAdministrador() {
        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        PANEL ADMINISTRADOR" << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << BLANCO;
            cout << endl;
            cout << "  1. Gestion de Pedidos" << endl;
            cout << "  2. Gestion de Menu" << endl;
            cout << "  3. Gestion de Reservas" << endl;
            cout << "  4. Gestion de clientes" << endl;
            cout << "  5. Info del restaurante" << endl;
            cout << "  6. Volver" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << AMARILLO << "Opcion: " << BLANCO;

            opcion = leerOpcion();

            switch (opcion) {
            case 1:
                menuGestionPedidos();
                break;
            case 2:
                menuGestionMenu();
                break;
            case 3:
                menuGestionReservas();
                break;
            case 4:
                menuGestionClientes();
                break;
            case 5:
                mostrarInfoRestaurante();
                break;
            case 6:
                break;
            default:
                cout << ROJO_BRILLANTE << "\nOpcion invalida. Intente de nuevo." << RESET << endl;
                pausar();
                break;
            }
        } while (opcion != 6);
    }

private:
    void menuGestionPedidos() {
        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        GESTION DE PEDIDOS" << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << BLANCO;
            cout << "\n  1. Ver Cola de Pedidos" << endl;
            cout << "  2. Procesar pedido" << endl;
            cout << "  3. Ver Historial" << endl;
            cout << "  4. Ordenar Historial de Pedidos" << endl;  // ? NUEVA OPCIÓN
            cout << "  5. Cargar dataset" << endl;
            cout << "  6. Volver al menu principal" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << AMARILLO << "Opcion: " << BLANCO;

            opcion = leerOpcion();

            switch (opcion) {
            case 1:
                verColaPedidos();
                break;
            case 2:
                procesarSiguientePedido();
                break;
            case 3:
                verHistorialPedidos();
                break;
            case 4:
                menuOrdenarPedidos();  // ? NUEVA FUNCIÓN
                break;
            case 5:
            {
                limpiarPantalla();
                cout << ROJO << "          CARGAR DATASET DE PEDIDOS" << endl;
                imprimirSeparadorRojoBlanco(20);

                cout << "\n¿Continuar con la carga? (s/n): ";
                char resp;
                cin >> resp;
                cin.ignore();

                if (resp == 's' || resp == 'S') {
                    int pedidosAntes = historialPedidos.gettamano();
                    int pedidosNuevos = GeneradorDataset::cargarDatasetPedidos();

                    if (pedidosNuevos > 0) {
                        while (!historialPedidos.estaVacia()) {
                            historialPedidos.desapilar();
                        }
                        GestorPedidos::cargarHistorial(historialPedidos, contadorPedidos);
                        int pedidosDespues = historialPedidos.gettamano();

                        cout << VERDE << "\n CARGA EXITOSA!" << RESET << endl;
                    }
                    else {
                        cout << ROJO << "\n No se cargaron pedidos." << RESET << endl;
                    }
                }

                pausar();
                break;
            }
            case 6:
                break;
            default:
                cout << ROJO_BRILLANTE << "\nOpcion invalida. Intente de nuevo." << RESET << endl;
                pausar();
                break;
            }
        } while (opcion != 6);
    }

    void menuOrdenarPedidos() {
        if (historialPedidos.estaVacia()) {
            limpiarPantalla();
            cout << ROJO << "No hay pedidos en el historial para ordenar." << RESET << endl;
            pausar();
            return;
        }

        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        ORDENAR HISTORIAL DE PEDIDOS" << endl;
            imprimirSeparadorRojoBlanco(30);
            cout << BLANCO;
            cout << "\n  Total de pedidos: " << historialPedidos.gettamano() << "\n" << endl;

            cout << "  CRITERIOS DE ORDENAMIENTO:" << endl;
            cout << "  1. Por Nombre del Cliente" << endl;
            cout << "  2. Por Fecha del Pedido" << endl;
            cout << "  3. Ver historial actual (sin ordenar)" << endl;
            cout << "  4. Volver" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(30);
            cout << AMARILLO << "Seleccione criterio: " << BLANCO;

            opcion = leerOpcion();

            if (opcion >= 1 && opcion <= 2) {
                // Submenu para elegir orden ascendente o descendente
                int orden = submenuOrden();

                if (orden == 0) continue; // Usuario canceló

                bool ascendente = (orden == 1);

                // Aplicar ordenamiento según la opción
                limpiarPantalla();
                cout << ROJO << "        ORDENANDO PEDIDOS..." << RESET << endl;
                imprimirSeparadorRojoBlanco(20);

                switch (opcion) {
                case 1:
                    GestorPedidos::ordenarPorNombre(historialPedidos, ascendente);
                    mostrarHistorialOrdenado("NOMBRE DEL CLIENTE", ascendente);
                    break;
                case 2:
                    GestorPedidos::ordenarPorFecha(historialPedidos, ascendente);
                    mostrarHistorialOrdenado("FECHA DEL PEDIDO", ascendente);
                    break;
                }

                pausar();
            }
            else if (opcion == 3) {
                verHistorialSinOrdenar();
            }
            else if (opcion == 4) {
                break;
            }
            else {
                cout << ROJO_BRILLANTE << "\nOpcion invalida." << RESET << endl;
                pausar();
            }

        } while (opcion != 4);
    }

    int submenuOrden() {
        limpiarPantalla();
        cout << ROJO << "        TIPO DE ORDENAMIENTO" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << BLANCO;
        cout << "\n  ¿En qué orden desea ver los pedidos?" << endl;
        cout << endl;
        cout << "  1. Ascendente  (A-Z, menor a mayor)" << endl;
        cout << "  2. Descendente (Z-A, mayor a menor)" << endl;
        cout << "  0. Cancelar" << endl;
        cout << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << AMARILLO << "Opción: " << BLANCO;

        int orden = leerOpcion();

        if (orden < 0 || orden > 2) {
            cout << ROJO << "Opción inválida. Cancelando..." << RESET << endl;
            _sleep(1000);
            return 0;
        }

        return orden;
    }

    void mostrarHistorialOrdenado(const string& criterio, bool ascendente) {
        cout << endl;
        cout << VERDE << "? Pedidos ordenados exitosamente" << RESET << endl;
        cout << "Criterio: " << criterio << endl;
        cout << "Orden: " << (ascendente ? "Ascendente" : "Descendente") << endl;
        cout << endl;
        imprimirSeparadorRojoBlanco(40);

        // Encabezados de tabla
        cout << left
            << setw(8) << "POS"
            << setw(10) << "PEDIDO#"
            << setw(12) << "FECHA"
            << setw(25) << "CLIENTE"
            << setw(12) << "TOTAL"
            << "ESTADO" << endl;
        imprimirSeparadorRojoBlanco(40);

        // Mostrar todos los pedidos ordenados
        for (int i = 0; i < historialPedidos.gettamano(); i++) {
            Pedido* p = historialPedidos.obtenerEnPosicion(i);

            cout << left
                << setw(8) << ("[" + to_string(i + 1) + "]")
                << setw(10) << p->getNumeroPedido()
                << setw(12) << p->getFechaPedido()
                << setw(25) << (p->getNombreCliente().length() > 23 ?
                    p->getNombreCliente().substr(0, 20) + "..." :
                    p->getNombreCliente())
                << "S/" << right << setw(9) << fixed << setprecision(2)
                << p->calcularTotal()
                << "  " << p->getEstado() << endl;
        }

        imprimirSeparadorRojoBlanco(40);
        cout << "Total: " << historialPedidos.gettamano() << " pedidos" << endl;
        cout << endl;

        // Opción para ver detalles de un pedido
        cout << AMARILLO << "¿Desea ver detalles de algún pedido? (s/n): " << RESET;
        char ver;
        cin >> ver;
        cin.ignore(10000, '\n');

        if (ver == 's' || ver == 'S') {
            cout << "Ingrese posición del pedido [1-" << historialPedidos.gettamano() << "]: ";
            int pos = leerOpcion();

            if (pos > 0 && pos <= historialPedidos.gettamano()) {
                limpiarPantalla();
                Pedido* p = historialPedidos.obtenerEnPosicion(pos - 1);

                cout << ROJO << "        DETALLE DEL PEDIDO" << RESET << endl;
                imprimirSeparadorRojoBlanco(30);
                p->mostrarResumen();
                imprimirSeparadorRojoBlanco(30);

                // Opción para ver comprobante
                if (p->getComprobante() != nullptr) {
                    cout << "\n¿Ver comprobante? (s/n): ";
                    char verComp;
                    cin >> verComp;
                    cin.ignore(10000, '\n');

                    if (verComp == 's' || verComp == 'S') {
                        limpiarPantalla();
                        p->getComprobante()->mostrarComprobante();
                    }
                }
            }
            else {
                cout << ROJO << "Posición inválida." << RESET << endl;
            }
            pausar();
        }
    }

    // En Fridays.h

    void verHistorialSinOrdenar() {
        limpiarPantalla();
        cout << ROJO << "        HISTORIAL DE PEDIDOS (SIN ORDENAR)" << RESET << endl;
        imprimirSeparadorRojoBlanco(40);

        cout << left
            << setw(8) << "POS"
            << setw(10) << "PEDIDO#"
            << setw(12) << "FECHA"
            << setw(25) << "CLIENTE"
            << setw(12) << "TOTAL"
            << "ESTADO" << endl;
        imprimirSeparadorRojoBlanco(40);

        for (int i = 0; i < historialPedidos.gettamano(); i++) {
            Pedido* p = historialPedidos.obtenerEnPosicion(i);

            cout << left
                << setw(8) << ("[" + to_string(i + 1) + "]")
                << setw(10) << p->getNumeroPedido()
                << setw(12) << p->getFechaPedido()
                << setw(25) << (p->getNombreCliente().length() > 23 ?
                    p->getNombreCliente().substr(0, 20) + "..." :
                    p->getNombreCliente())
                << "S/" << right << setw(9) << fixed << setprecision(2)
                << p->calcularTotal()
                << "  " << p->getEstado() << endl;
        }

        imprimirSeparadorRojoBlanco(40);
        cout << "Total: " << historialPedidos.gettamano() << " pedidos" << endl;

        pausar();
    }

    void menuGestionMenu() {
        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        GESTION DE MENU" << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << BLANCO;
            cout << "\n  1. Ver menu completo" << endl;
            cout << "  2. Ordenar productos por precio" << endl;
            cout << "  3. Agregar producto" << endl;
            cout << "  4. Eliminar producto" << endl;
            cout << "  5. Buscar producto" << endl;
            cout << "  6. Modificar producto" << endl;
            cout << "  7. Volver al menu principal" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << AMARILLO << "Opcion: " << BLANCO;

            opcion = leerOpcion();

            switch (opcion) {
            case 1:
                verMenuCompleto();
                break;
            case 2:
                ordenarProductosPorPrecio();
                break;
            case 3:
                agregarProducto();
                break;
            case 4:
                eliminarProducto();
                break;
            case 5:
                buscarProducto();
                break;
            case 6:
                modificarProducto();
                break;
            case 7:
                break;
            default:
                cout << ROJO_BRILLANTE << "\nOpcion invalida. Intente de nuevo." << RESET << endl;
                pausar();
                break;
            }
        } while (opcion != 7);
    }

    void menuGestionReservas() {
        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        GESTION DE RESERVAS" << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << BLANCO;
            cout << "\n  1. Ver todas las reservas" << endl;
            cout << "  2. Buscar reserva por codigo" << endl;
            cout << "  3. Reservas del dia" << endl;
            cout << "  4. Cancelar reserva" << endl;
            cout << "  5. Estadisticas de reservas" << endl;
            cout << "  6. Ver todas las mesas" << endl;
            cout << "  7. Cargar data set de reservas" << endl;
            cout << "  8. Ver lista de espera (Treap)" << endl;
            cout << "  9. Procesar siguiente en espera" << endl;
            cout << " 10. Cargar dataset de lista de espera" << endl;
            cout << " 11. Ver mapa del restaurante" << endl;
            cout << " 12. Calcular ruta entre mesas" << endl;
            cout << " 13. Sugerir mesa optima" << endl;
            cout << " 14. Estadisticas del grafo" << endl;
            cout << endl;
            cout << " 15. Volver al menu principal" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << AMARILLO << "Opcion: " << BLANCO;

            opcion = leerOpcion();

            switch (opcion) {
            case 1:
                limpiarPantalla();
                gestorReservas->mostrarTodasReservas();
                pausar();
                break;
            case 2:
                buscarReservaPorCodigo();
                break;
            case 3:
                verReservasPorFecha();
                break;
            case 4:
                cancelarReserva();
                break;
            case 5:
                limpiarPantalla();
                gestorReservas->mostrarEstadisticas();
                pausar();
                break;
            case 6:
                limpiarPantalla();
                gestorReservas->listarTodasMesas();
                pausar();
                break;
            case 7:
                cargarDatasetReservas();
                break;
            case 8:
                limpiarPantalla();
                gestorReservas->verListaEspera();
                pausar();
                break;
            case 9:
                limpiarPantalla();
                gestorReservas->procesarSiguienteEnEspera();
                pausar();
                break;
            case 10:
                cargarDatasetListaEspera();
                break;

            case 11:
                gestorReservas->verMapaRestaurante();
                pausar();
                break;

            case 12: {
                limpiarPantalla();
                cout << ROJO << "CALCULAR RUTA ENTRE MESAS" << RESET << endl;
                imprimirSeparadorRojoBlanco(20);

                int origen, destino;
                cout << "\nMesa origen (1-25): ";
                cin >> origen;
                cin.ignore();

                cout << "Mesa destino (1-25): ";
                cin >> destino;
                cin.ignore();

                if (origen >= 1 && origen <= 25 && destino >= 1 && destino <= 25) {
                    gestorReservas->mostrarRutaEntreMesas(origen, destino);
                }
                else {
                    cout << ROJO << "Números de mesa inválidos" << RESET << endl;
                }
                pausar();
                break;
            }

            case 13: {
                limpiarPantalla();
                cout << ROJO << "SUGERIR MESA ÓPTIMA" << RESET << endl;
                imprimirSeparadorRojoBlanco(20);

                int personas;
                string zona;

                cout << "\nNúmero de personas: ";
                cin >> personas;
                cin.ignore();

                cout << "\nZonas disponibles:" << endl;
                cout << "  1. Interior" << endl;
                cout << "  2. Terraza" << endl;
                cout << "  3. VIP" << endl;
                cout << "\nSeleccione zona: ";

                int opcionZona = leerOpcion();

                switch (opcionZona) {
                case 1: zona = "Interior"; break;
                case 2: zona = "Terraza"; break;
                case 3: zona = "VIP"; break;
                default: zona = "Interior";
                }

                gestorReservas->sugerirMesaOptima(personas, zona);
                pausar();
                break;
            }

            case 14:
                gestorReservas->verEstadisticasGrafo();
                pausar();
                break;

            case 15:
                break;

            default:
                cout << ROJO_BRILLANTE << "\nOpcion invalida. Intente de nuevo."
                    << RESET << endl;
                pausar();
                break;
            }
        } while (opcion != 15);
    }
    void menuGestionClientes() {
        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        GESTION DE CLIENTES" << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << BLANCO;
            cout << endl;
            cout << "  1. Ver todos los clientes" << endl;
            cout << "  2. Buscar cliente por DNI" << endl;
            cout << "  3. Ver clientes VIP/Premium" << endl;
            cout << "  4. Cargar dataset de clientes" << endl;
            cout << "  5. Ordenar clientes por ID" << endl;
            cout << "  6. Volver" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << AMARILLO << "Opcion: " << BLANCO;

            opcion = leerOpcion();

            switch (opcion) {
            case 1:
                limpiarPantalla();
                GestorClientes::listarTodosClientes();
                pausar();
                break;
            case 2:
                buscarClientePorDNI();
                break;
            case 3:
                verClientesVIPPremium();
                break;
            case 4:
                cargarDatasetClientes();
                break;
            case 5:
                ordenarClientesPorIDMenu();
                break;
            case 6:
                break;
            default:
                cout << ROJO_BRILLANTE << "\nOpcion invalida." << RESET << endl;
                pausar();
                break;
            }

        } while (opcion != 6);
    }

    // ==================== MÉTODOS DE GESTIÓN DE PEDIDOS ====================
    void verColaPedidos() {
        limpiarPantalla();
        cout << "COLA DE PEDIDOS PENDIENTES " << endl;
        imprimirSeparadorRojoBlanco(20);

        if (colaPedidos.estaVacia()) {
            cout << "\nNo hay pedidos pendientes." << endl;
        }
        else {
            cout << "\nPedidos en espera: " << colaPedidos.gettamano() << "\n" << endl;

            for (int i = 0; i < colaPedidos.gettamano(); i++) {
                Pedido* p = colaPedidos.obtenerEnPosicion(i);
                cout << "[" << (i + 1) << "] Pedido #" << p->getNumeroPedido()
                    << " - Cliente: " << p->getNombreCliente()
                    << " - Total: S/" << fixed << setprecision(2) << p->calcularTotal()
                    << " - Estado: " << p->getEstado()
                    << " - Tipo: " << p->getTipoServicio();

                if (p->getComprobante() != nullptr) {
                    cout << " - " << p->getComprobante()->getTipoComprobante();
                }
                cout << endl;
            }
        }
        pausar();
    }

    void procesarSiguientePedido() {
        limpiarPantalla();
        cout << ROJO << "         PROCESAR SIGUIENTE PEDIDO" << RESET << endl;
        imprimirSeparadorRojoBlanco(25);

        if (colaPedidos.estaVacia()) {
            cout << "\nNo hay pedidos para procesar." << endl;
        }
        else {
            Pedido* pedido = colaPedidos.desencolar();
            cout << "\nProcesando pedido #" << pedido->getNumeroPedido() << "..." << endl;
            pedido->mostrarResumen();
            _sleep(3000);
            pedido->cambiarEstado("completado");
            historialPedidos.apilar(pedido);
            GestorPedidos::guardarHistorial(historialPedidos);
            imprimirSeparadorRojoBlanco(25);

            cout << "\nPedido completado y movido al historial!" << endl;
        }
        pausar();
    }

    void verHistorialPedidos() {
        limpiarPantalla();
        cout << ROJO << "                   HISTORIAL DE PEDIDOS " << RESET << endl;
        imprimirSeparadorRojoBlanco(42);

        if (historialPedidos.estaVacia()) {
            cout << "\nNo hay pedidos en el historial." << endl;
        }
        else {
            cout << "\nPedidos completados: " << historialPedidos.gettamano() << "\n" << endl;
            imprimirSeparadorRojoBlanco(42);
            for (int i = 0; i < historialPedidos.gettamano(); i++) {
                Pedido* p = historialPedidos.obtenerEnPosicion(i);
                cout << "[" << (i + 1) << "] Pedido #" << p->getNumeroPedido()
                    << " - Cliente: " << p->getNombreCliente()
                    << " - Total: S/" << fixed << setprecision(2) << p->calcularTotal()
                    << " - Estado: " << p->getEstado();

                if (p->getComprobante() != nullptr) {
                    cout << " - Comprobante: " << p->getComprobante()->getTipoComprobante();
                }
                cout << endl;
            }
            cout << endl;
            imprimirSeparadorRojoBlanco(42);
            cout << "\nDesea ver el comprobante de algun pedido? (s/n): ";
            char ver;
            cin >> ver;
            cin.ignore(10000, '\n');

            if (ver == 's' || ver == 'S') {
                int numPedido;
                cout << "Ingrese numero de pedido [1-" << historialPedidos.gettamano() << "]: ";
                cin >> numPedido;
                cin.ignore(10000, '\n');

                if (numPedido > 0 && numPedido <= historialPedidos.gettamano()) {
                    Pedido* p = historialPedidos.obtenerEnPosicion(numPedido - 1);
                    if (p->getComprobante() != nullptr) {
                        limpiarPantalla();
                        p->getComprobante()->mostrarComprobante();
                    }
                    else {
                        cout << "Este pedido no tiene comprobante." << endl;
                    }
                }
            }
        }
        pausar();
    }

    void cargarDatasetPedidos() {
        limpiarPantalla();
        cout << ROJO << "          CARGAR DATASET DE PEDIDOS" << endl;
        imprimirSeparadorRojoBlanco(20);

        cout << "\n¿Continuar con la carga? (s/n): ";
        char resp;
        cin >> resp;
        cin.ignore();

        if (resp == 's' || resp == 'S') {
            int pedidosNuevos = GeneradorDataset::cargarDatasetPedidos();

            if (pedidosNuevos > 0) {
                while (!historialPedidos.estaVacia()) {
                    historialPedidos.desapilar();
                }
                GestorPedidos::cargarHistorial(historialPedidos, contadorPedidos);
                cout << VERDE << "\n CARGA EXITOSA!" << RESET << endl;
            }
            else {
                cout << ROJO << "\n No se cargaron pedidos." << RESET << endl;
            }
        }
        pausar();
    }

    // ==================== MÉTODOS DE GESTIÓN DE MENÚ ====================
    void verMenuCompleto() {
        limpiarPantalla();
        menuPrincipal->mostrarMenu();
        pausar();
    }

    void ordenarProductosPorPrecio() {
        limpiarPantalla();
        cout << "Ordenar productos por precio:\n";
        imprimirSeparadorRojoBlanco(20);
        cout << "1. Menor a mayor\n";
        cout << "2. Mayor a menor\n";
        cout << AMARILLO << "Opcion: " << BLANCO;
        int orden = leerOpcion();
        menuPrincipal->ordenarPorPrecio(orden == 1);
        cout << VERDE << "\nProductos ordenados exitosamente!" << RESET << endl;
        pausar();
    }

    void agregarProducto() {
        limpiarPantalla();
        cout << ROJO << "AGREGAR PRODUCTO" << endl;
        imprimirSeparadorRojoBlanco(20);

        int id;
        string nombre, categoria;
        double precio;

        cout << "\nIngrese ID del producto: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (menuPrincipal->buscarPlato(id) != nullptr) {
            cout << ROJO << "\n Error: Ya existe un producto con ese ID" << RESET << endl;
            pausar();
            return;
        }

        cout << "Ingrese nombre del producto: ";
        getline(cin, nombre);

        cout << "Ingrese precio: S/";
        cin >> precio;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Ingrese categoria (plato/bebida/postre): ";
        getline(cin, categoria);

        Producto nuevo(id, nombre, precio, categoria, true);
        menuPrincipal->agregarAlMenu(nuevo);
        menuPrincipal->guardarEnArchivo();

        cout << VERDE << "\nProducto agregado exitosamente!" << RESET << endl;
        pausar();
    }

    void eliminarProducto() {
        limpiarPantalla();
        cout << ROJO << "ELIMINAR PRODUCTO" << endl;
        imprimirSeparadorRojoBlanco(40);

        menuPrincipal->mostrarMenu();

        int id;
        cout << "\nIngrese ID del producto a eliminar (0 para cancelar): ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (id == 0) {
            cout << "Operacion cancelada." << endl;
            pausar();
            return;
        }

        Producto* producto = menuPrincipal->buscarPlato(id);
        if (producto == nullptr) {
            cout << ROJO << "\n Error: Producto no encontrado" << RESET << endl;
            pausar();
            return;
        }

        cout << "\nProducto encontrado: " << producto->getNombre()
            << " - S/" << producto->getPrecio() << endl;
        cout << "¿Confirmar eliminacion? (s/n): ";
        char confirmar;
        cin >> confirmar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (confirmar == 's' || confirmar == 'S') {
            menuPrincipal->removerDelMenu(id);
            menuPrincipal->guardarEnArchivo();
            cout << VERDE << "\n Producto eliminado exitosamente!" << RESET << endl;
        }
        else {
            cout << "\nOperacion cancelada." << endl;
        }

        delete producto;
        pausar();
    }

    void buscarProducto() {
        limpiarPantalla();
        cout << ROJO << "BUSCAR PRODUCTO" << endl;
        imprimirSeparadorRojoBlanco(20);

        cout << AMARILLO << "\nBuscar por:" << endl;
        cout << BLANCO;
        cout << "1. ID " << endl;
        cout << "2. Nombre exacto " << endl;
        cout << "3. Nombre parcial " << endl;
        cout << "4. Categoria" << endl;
        cout << "5. Ver estadisticas de busqueda" << endl;
        cout << "Opcion: ";
        int opcionBusqueda = leerOpcion();

        switch (opcionBusqueda) {
        case 1:
            buscarProductoPorID();
            break;
        case 2:
            buscarProductoPorNombreExacto();
            break;
        case 3:
            buscarProductoPorNombreParcial();
            break;
        case 4:
            buscarProductoPorCategoria();
            break;
        case 5:
            limpiarPantalla();
            menuPrincipal->mostrarEstadisticasHashTable();
            break;
        default:
            cout << ROJO << "\n Opcion invalida" << RESET << endl;
        }
        pausar();
    }

    void buscarProductoPorID() {
        int id;
        cout << "\nIngrese ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Producto* p = menuPrincipal->buscarPlato(id);

        if (p != nullptr) {
            cout << VERDE << "\n Producto encontrado!" << RESET << endl;
            cout << "\nID: " << p->getId() << endl;
            cout << "Nombre: " << p->getNombre() << endl;
            cout << "Precio: S/" << fixed << setprecision(2) << p->getPrecio() << endl;
            cout << "Categoria: " << p->getCategoria() << endl;
            cout << "Disponible: " << (p->isDisponible() ? "Si" : "No") << endl;
            delete p;
        }
        else {
            cout << ROJO << "\n Producto no encontrado" << RESET << endl;
        }
    }

    void buscarProductoPorNombreExacto() {
        string nombre;
        cout << "\nIngrese nombre exacto del producto: ";
        getline(cin, nombre);

        Producto* p = menuPrincipal->buscarPorNombre(nombre);

        if (p != nullptr) {
            cout << VERDE << "\n Producto encontrado!" << RESET << endl;
            cout << "\nID: " << p->getId() << endl;
            cout << "Nombre: " << p->getNombre() << endl;
            cout << "Precio: S/" << fixed << setprecision(2) << p->getPrecio() << endl;
            cout << "Categoria: " << p->getCategoria() << endl;
            cout << "Disponible: " << (p->isDisponible() ? "Si" : "No") << endl;
            delete p;
        }
        else {
            cout << ROJO << "\n Producto no encontrado" << RESET << endl;
            cout << AMARILLO << "Consejo: Use busqueda parcial (opcion 3)" << RESET << endl;
        }
    }

    void buscarProductoPorNombreParcial() {
        string texto;
        cout << "\nIngrese parte del nombre: ";
        getline(cin, texto);

        Lista<Producto> resultados = menuPrincipal->buscarPorNombreParcial(texto);

        if (!resultados.estaVacia()) {
            cout << VERDE << "\n>>> Resultados (" << resultados.gettamano() << "):" << RESET << endl;
            imprimirSeparadorRojoBlanco(30);

            for (int i = 0; i < resultados.gettamano(); i++) {
                Producto p = resultados.obtenerEnPosicion(i);
                cout << "ID: " << p.getId() << " - " << p.getNombre()
                    << " - S/" << fixed << setprecision(2) << p.getPrecio()
                    << " (" << p.getCategoria() << ")" << endl;
            }
            imprimirSeparadorRojoBlanco(30);
        }
        else {
            cout << ROJO << " No se encontraron productos" << RESET << endl;
        }
    }

    void buscarProductoPorCategoria() {
        string categoria;
        cout << "\nIngrese categoria (plato/bebida/postre): ";
        getline(cin, categoria);
        limpiarPantalla();
        menuPrincipal->mostrarPorCategoria(categoria);
    }

    void modificarProducto() {
        limpiarPantalla();
        cout << ROJO << "MODIFICAR PRODUCTO" << endl;
        imprimirSeparadorRojoBlanco(20);

        int id;
        cout << "\nIngrese ID del producto a modificar: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Producto* producto = menuPrincipal->buscarPlato(id);
        if (producto == nullptr) {
            cout << ROJO << "\n Error: Producto no encontrado" << RESET << endl;
            pausar();
            return;
        }

        cout << VERDE << "\n>>> Producto encontrado:" << RESET << endl;
        cout << "ID: " << producto->getId() << endl;
        cout << "Nombre actual: " << producto->getNombre() << endl;
        cout << "Precio actual: S/" << fixed << setprecision(2) << producto->getPrecio() << endl;
        cout << "Categoria actual: " << producto->getCategoria() << endl;
        cout << "Disponible: " << (producto->isDisponible() ? "Si" : "No") << endl;
        cout << endl;

        string nuevoNombre;
        double nuevoPrecio;
        string nuevaCategoria;
        char disponible;

        cout << AMARILLO << ">>> Ingrese los nuevos datos:" << RESET << endl;
        cout << "Nuevo nombre (Enter para mantener actual): ";
        getline(cin, nuevoNombre);
        if (nuevoNombre.empty()) {
            nuevoNombre = producto->getNombre();
        }

        cout << "Nuevo precio (0 para mantener actual): ";
        cin >> nuevoPrecio;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (nuevoPrecio <= 0) {
            nuevoPrecio = producto->getPrecio();
        }

        cout << "Nueva categoria (plato/bebida/postre, Enter para mantener actual): ";
        getline(cin, nuevaCategoria);
        if (nuevaCategoria.empty()) {
            nuevaCategoria = producto->getCategoria();
        }

        cout << "¿Disponible? (s/n): ";
        cin >> disponible;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        bool nuevaDisponibilidad = (disponible == 's' || disponible == 'S');

        cout << "\n¿Confirmar modificacion? (s/n): ";
        char confirmar;
        cin >> confirmar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (confirmar == 's' || confirmar == 'S') {
            bool exito = menuPrincipal->modificarProducto(id, nuevoNombre, nuevoPrecio,
                nuevaCategoria, nuevaDisponibilidad);
            if (exito) {
                menuPrincipal->guardarEnArchivo();
                cout << VERDE << "\n Producto modificado exitosamente!" << RESET << endl;
            }
            else {
                cout << ROJO << "\n Error al modificar producto" << RESET << endl;
            }
        }
        else {
            cout << "\nOperacion cancelada." << endl;
        }

        delete producto;
        pausar();
    }

    // ==================== MÉTODOS DE GESTIÓN DE RESERVAS ====================
    void buscarReservaPorCodigo() {
        limpiarPantalla();
        cout << ROJO << "BUSCAR RESERVA" << endl;
        imprimirSeparadorRojoBlanco(20);
        string codigo;
        cout << "\nIngrese codigo de reserva: ";
        getline(cin, codigo);
        limpiarPantalla();
        gestorReservas->buscarReservaPorCodigo(codigo);
        pausar();
    }

    void verReservasPorFecha() {
        limpiarPantalla();
        cout << ROJO << "RESERVAS DEL DIA" << endl;
        imprimirSeparadorRojoBlanco(20);
        string fecha;
        cout << "\nIngrese fecha (DD/MM/YYYY): ";
        getline(cin, fecha);
        gestorReservas->listarReservasPorFecha(fecha);
        pausar();
    }

    void cancelarReserva() {
        limpiarPantalla();
        cout << ROJO << "CANCELAR RESERVA" << endl;
        imprimirSeparadorRojoBlanco(20);
        string codigo;
        cout << "\nIngrese codigo de reserva a cancelar: ";
        getline(cin, codigo);
        gestorReservas->cancelarReserva(codigo);
        pausar();
    }

    void cargarDatasetReservas() {
        limpiarPantalla();
        cout << ROJO << "          CARGAR DATASET DE RESERVAS" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "\n¿Continuar con la carga? (s/n): ";
        char resp;
        cin >> resp;
        cin.ignore();

        if (resp == 's' || resp == 'S') {
            int reservasNuevas = GeneradorDataset::cargarDatasetReservas();
            if (reservasNuevas > 0) {
                cout << VERDE << "\n CARGA EXITOSA!" << RESET << endl;
                cout << "  Agregadas: " << reservasNuevas << " reservas" << endl;
                gestorReservas->recargarReservas();
            }
            else {
                cout << ROJO << "\n No se cargaron reservas." << RESET << endl;
            }
        }
        pausar();
    }

    void cargarDatasetListaEspera() {
        limpiarPantalla();
        cout << ROJO << "          CARGAR DATASET DE LISTA DE ESPERA" << endl;
        imprimirSeparadorRojoBlanco(20);

        cout << "\n¿Continuar con la carga? (s/n): ";
        char resp;
        cin >> resp;
        cin.ignore();

        if (resp == 's' || resp == 'S') {
            int solicitudesNuevas = GeneradorDataset::cargarDatasetListaEspera();

            if (solicitudesNuevas > 0) {
                cout << VERDE << "\n CARGA EXITOSA!" << RESET << endl;
                cout << "  Agregadas: " << solicitudesNuevas << " solicitudes a la lista de espera" << endl;

                // Recargar lista de espera
                gestorReservas->cargarListaEsperaDesdeArchivo();
            }
            else {
                cout << ROJO << "\n No se cargaron solicitudes." << RESET << endl;
            }
        }
        pausar();
    }





    // ==================== MÉTODOS DE GESTIÓN DE CLIENTES ====================
    void buscarClientePorDNI() {
        limpiarPantalla();
        cout << ROJO << "BUSCAR CLIENTE" << endl;
        imprimirSeparadorRojoBlanco(20);
        string dni;
        cout << "\nIngrese DNI: ";
        getline(cin, dni);

        Cliente* cliente = GestorClientes::buscarClientePorDNI(dni);
        if (cliente != nullptr) {
            cout << endl;
            limpiarPantalla();
            cliente->mostrarInfo();
            delete cliente;
        }
        else {
            cout << ROJO << "\nCliente no encontrado" << RESET << endl;
        }
        pausar();
    }

    void verClientesVIPPremium() {
        limpiarPantalla();
        cout << ROJO << "    CLIENTES VIP Y PREMIUM    " << RESET << endl;
        imprimirSeparadorRojoBlanco(20);

        ifstream file("Data/clientes.txt");
        if (!file.is_open()) {
            cout << "No hay clientes registrados" << endl;
        }
        else {
            string linea;
            int contVIP = 0;

            while (getline(file, linea)) {
                if (linea.empty() || linea[0] == '#') continue;

                if (linea.find(",VIP,") != string::npos ||
                    linea.find(",Premium,") != string::npos) {

                    stringstream ss(linea);
                    string i, nombre, apellido, tel, dni, dir, cat, pts;
                    getline(ss, i, ',');
                    getline(ss, nombre, ',');
                    getline(ss, apellido, ',');
                    getline(ss, tel, ',');
                    getline(ss, dni, ',');
                    getline(ss, dir, ',');
                    getline(ss, cat, ',');
                    getline(ss, pts);

                    string color = (cat == "VIP") ? VERDE : AMARILLO;
                    cout << color << cat << RESET << " - "
                        << nombre << " " << apellido
                        << " (" << pts << " pts)" << endl;
                    contVIP++;
                }
            }
            file.close();

            imprimirSeparadorRojoBlanco(20);
            cout << "Total VIP/Premium: " << contVIP << endl;
        }
        pausar();
    }

    void cargarDatasetClientes() {
        limpiarPantalla();
        cout << ROJO << "          CARGAR DATASET DE CLIENTES" << endl;
        imprimirSeparadorRojoBlanco(20);

        cout << "\n¿Continuar con la carga? (s/n): ";
        char resp;
        cin >> resp;
        cin.ignore();

        if (resp == 's' || resp == 'S') {
            int clientesNuevos = GeneradorDataset::cargarDatasetClientes();

            if (clientesNuevos > 0) {
                cout << VERDE << "\n CARGA EXITOSA!" << RESET << endl;
                cout << "  Agregados: " << clientesNuevos << " clientes" << endl;
            }
            else {
                cout << ROJO << "\n No se cargaron clientes." << RESET << endl;
            }
        }
        pausar();
    }

    void ordenarClientesPorIDMenu() {
        limpiarPantalla();
        cout << ROJO << "          ORDENAR CLIENTES POR ID" << endl;
        imprimirSeparadorRojoBlanco(20);

        cout << "\nSeleccione el tipo de ordenamiento:" << endl;
        cout << "  1. Ascendente (ID menor a mayor)" << endl;
        cout << "  2. Descendente (ID mayor a menor)" << endl;
        cout << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << AMARILLO << "Opcion: " << BLANCO;

        int opcion = leerOpcion();

        limpiarPantalla();

        if (opcion == 1) {
            GestorClientes::ordenarClientesPorID(true);
        }
        else {
            GestorClientes::ordenarClientesPorID(false);
        }
        pausar();
    }

    // ==================== OTROS MÉTODOS ====================
    void mostrarInfoRestaurante() {
        limpiarPantalla();
        cout << ROJO << "INFORMACION DEL RESTAURANTE" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "Nombre: Fridays Restaurant" << endl;
        cout << "Direccion: Av. Principal 123" << endl;
        cout << "Telefono: 555-1234" << endl;
        pausar();
    }


};