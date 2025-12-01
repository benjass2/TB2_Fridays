#pragma once
#include "Funcionalidades.h"
#include "Carta.h"
#include "CartaLocal.h"
#include "CartaDelivery.h"
#include "Cliente.h"
#include "Pedido.h"
#include "Cola.h"
#include "GestorReserva.h"
#include "GestorClientes.h"
#include "PagoEfectivo.h"
#include "PagoTarjeta.h"
#include "Boleta.h"
#include "Factura.h"

using namespace UtilidadesConsola;
using namespace std;

class ControladoraCliente {
private:
    Carta* menuPrincipal;
    GestorReserva* gestorReservas;
    Cola<Pedido*>& colaPedidos;
    int& contadorPedidos;
    int& contadorClientes;

public:
    ControladoraCliente(Carta* menu, GestorReserva* gestor,
        Cola<Pedido*>& cola, int& contPed, int& contCli)
        : menuPrincipal(menu), gestorReservas(gestor),
        colaPedidos(cola), contadorPedidos(contPed),
        contadorClientes(contCli) {
    }

    void modoCliente() {

        leerTipoServicio();

        int tipoServicio = leerOpcion();

        if (tipoServicio == 1) {
            hacerReservaCliente();
            return;
        }

        if (tipoServicio == 4) return;

        bool esDelivery = (tipoServicio == 3);
        string servicioTexto = esDelivery ? "delivery" : "local";

        Cliente* cliente = registrarCliente(esDelivery);

        Carta* carta = nullptr;
        if (esDelivery) {
            CartaDelivery* cartaDelivery = new CartaDelivery();
            cartaDelivery->cargarProductos(menuPrincipal->getProductos());
            carta = cartaDelivery;
        }
        else {
            CartaLocal* cartaLocal = new CartaLocal();
            cartaLocal->cargarProductos(menuPrincipal->getProductos());
            carta = cartaLocal;
        }

        Pedido* pedidoActual = new Pedido(contadorPedidos, cliente, servicioTexto);

        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        MENU FRIDAYS - " << (esDelivery ? "DELIVERY" : "LOCAL") << RESET << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << endl;
            cout << "  1. Ver solo PLATOS PRINCIPALES" << endl;
            cout << "  2. Ver solo BEBIDAS" << endl;
            cout << "  3. Ver solo POSTRES" << endl;
            cout << endl;
            cout << "  4. Ver MENU COMPLETO" << endl;
            cout << "  5. Mi Carrito (" << pedidoActual->getCantidadProductos() << " productos)" << endl;
            cout << "  6. Confirmar Pedido y Pagar" << endl;
            cout << "  7. Cancelar y Volver" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << AMARILLO;
            cout << "Opcion: ";
            cout << BLANCO;

            opcion = leerOpcion();

            switch (opcion) {
            case 1:
            case 2:
            case 3:
            case 4: {
                string categoria;
                switch (opcion) {
                case 1: categoria = "plato"; break;
                case 2: categoria = "bebida"; break;
                case 3: categoria = "postre"; break;
                case 4: categoria = "completo"; break;
                }

                mostrarMenuPorCategoria(carta, categoria);
                imprimirSeparadorRojoBlanco(40);
                cout << "\nDesea agregar un producto a su bolsa? (s/n): ";
                char resp;
                cin >> resp;
                cin.ignore(10000, '\n');

                if (resp == 's' || resp == 'S') {
                    Producto* p = seleccionarProductoPorID(carta);
                    if (p != nullptr) {
                        pedidoActual->agregarProducto(p);
                        cout << VERDE << "Producto agregado al carrito!" << endl;
                    }
                    else {
                        cout << "Producto no encontrado." << endl;
                    }
                }
                pausar();
                break;
            }

            case 5:
                limpiarPantalla();
                cout << ROJO << "                  MI BOLSA" << endl; cout << RESET;
                cout << endl;
                imprimirSeparadorRojoBlanco(30);
                pedidoActual->mostrarProductos();
                cout << "\nSubtotal: S/" << fixed << setprecision(2) << pedidoActual->calcularTotal() << endl;
                if (esDelivery) {
                    cout << "Tarifa delivery: S/8.90" << endl;
                    cout << "Total: S/" << fixed << setprecision(2) << (pedidoActual->calcularTotal() + 8.90) << endl;
                }
                else {
                    cout << "Total: S/" << fixed << setprecision(2) << pedidoActual->calcularTotal() << endl;
                }
                cout << endl;
                imprimirSeparadorRojoBlanco(30);
                cout << "\nDesea eliminar un producto? (s/n): ";
                {
                    char resp;
                    cin >> resp;
                    cin.ignore(10000, '\n');
                    if (resp == 's' || resp == 'S') {
                        if (pedidoActual->estaVacio()) {
                            cout << "Carrito vacio. No hay productos para eliminar." << endl;
                        }
                        else {
                            int pos;
                            cout << "Ingrese el numero del producto a eliminar: ";
                            cin >> pos;
                            cin.ignore(10000, '\n');
                            if (!pedidoActual->quitarProducto(pos - 1)) {
                                cout << "No se pudo eliminar el producto." << endl;
                            }
                        }
                    }
                }
                pausar();
                break;

            case 6:
                if (pedidoActual->estaVacio()) {
                    cout << "\nCarrito vacio. Agregue productos primero." << endl;
                    pausar();
                }
                else {
                    limpiarPantalla();
                    cout << ROJO << "          CONFIRMAR PEDIDO" << endl;
                    imprimirSeparadorRojoBlanco(20);
                    pedidoActual->mostrarResumen();
                    imprimirSeparadorRojoBlanco(20);

                    cout << AMARILLO << "\nDesea proceder al pago? (s/n): ";
                    char confirmar;
                    cin >> confirmar;
                    cin.ignore(10000, '\n');

                    if (confirmar == 's' || confirmar == 'S') {
                        bool pagoExitoso = procesarPagoPedido(pedidoActual, esDelivery);

                        if (pagoExitoso) {
                            colaPedidos.encolar(pedidoActual);
                            contadorPedidos++;
                            cout << BLANCO;
                            system("cls");
                            imprimirSeparadorRojoBlanco(50);
                            cout << "\n Pedido #" << pedidoActual->getNumeroPedido() << " confirmado!" << endl;
                            cout << " Estado: En cola de cocina" << endl;
                            cout << " Tiempo estimado: " << (esDelivery ? "45" : "20") << " minutos" << endl;
                            cout << endl;
                            cout << "El administrador gestionara su pedido pronto. Gracias por elegir Fridays!" << endl;
                            cout << endl;
                            imprimirSeparadorRojoBlanco(50);
                            pausar();

                            delete carta;
                            return;
                        }
                        else {
                            cout << "\nPago no completado. Puede volver a intentar." << endl;
                            pausar();
                        }
                    }
                }
                break;

            case 7:
                delete pedidoActual;
                delete cliente;
                delete carta;
                return;
            }

        } while (opcion != 7);
    }

    void hacerReservaCliente() {
        limpiarPantalla();
        cout << ROJO << "          HACER RESERVA" << endl;
        imprimirSeparadorRojoBlanco(20);

        Cliente* cliente = registrarCliente(false);

        string fecha, hora, observaciones;
        int numeroPersonas, numeroMesa;
        limpiarPantalla();
        cout << BLANCO;
        cout << ROJO << "DATOS DE LA RESERVA" << RESET << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "Fecha (DD/MM/YYYY): ";
        getline(cin, fecha);

        cout << "Hora (HH:MM): ";
        getline(cin, hora);

        cout << "Numero de personas: ";
        cin >> numeroPersonas;
        cin.ignore();

        gestorReservas->mostrarMesasDisponibles(numeroPersonas, fecha, hora);

        cout << "\nSeleccione numero de mesa: ";
        cin >> numeroMesa;
        cin.ignore();

        cout << "Observaciones (opcional): ";
        getline(cin, observaciones);

        gestorReservas->crearReserva(*cliente, numeroMesa, fecha, hora, numeroPersonas, observaciones);

        delete cliente;
        pausar();
    }

private:
    bool procesarPagoPedido(Pedido* pedido, bool esDelivery) {
        double totalPedido = pedido->calcularTotal();
        double tarifaDelivery = esDelivery ? 8.90 : 0.0;
        double totalFinal = totalPedido + tarifaDelivery;
        bool pagoExitoso = false;

        limpiarPantalla();
        cout << ROJO << "                 PROCESAR PAGO" << endl;
        imprimirSeparadorRojoBlanco(30);

        pedido->mostrarProductos();

        cout << "SUBTOTAL:" << setw(20) << right << "S/" << fixed << setprecision(2) << setw(10) << totalPedido << endl;
        if (esDelivery) {
            cout << "DELIVERY:" << setw(20) << right << "S/" << setw(10) << tarifaDelivery << endl;
        }

        cout << endl;
        cout << VERDE << "TOTAL A PAGAR:" << setw(15) << right << "S/" << setw(10) << totalFinal << endl;
        cout << RESET;
        imprimirSeparadorRojoBlanco(30);

        cout << AMARILLO << "\nMETODOS DE PAGO DISPONIBLES"; cout << RESET << endl;
        cout << "1. Efectivo" << endl;
        cout << "2. Tarjeta de Credito" << endl;
        cout << "3. Tarjeta de Debito" << endl;
        cout << "0. Cancelar pago" << endl;
        cout << "---------------------------" << endl;
        cout << AMARILLO << "Seleccione metodo de pago: "; cout << BLANCO;

        int metodoPago = leerOpcion();

        if (metodoPago == 0) {
            cout << "\n Pago cancelado. El pedido no ha sido confirmado." << endl;
            pausar();
            return false;
        }

        string fecha = obtenerFechaActual();

        switch (metodoPago) {
        case 1: {
            limpiarPantalla();
            cout << ROJO << "PAGO EN EFECTIVO" << endl;
            imprimirSeparadorRojoBlanco(20);
            double montoPagado;
            cout << "Total a Pagar: S/" << fixed << setprecision(2) << totalFinal << endl;
            cout << "\nIngrese monto con el que paga: S/";
            cin >> montoPagado;
            cin.ignore(10000, '\n');

            PagoEfectivo pago(totalFinal, fecha, montoPagado);
            cout << "\n>>> Procesando pago en efectivo..." << endl;
            _sleep(1000);
            limpiarPantalla();

            string resultado = pago.procesarPago();
            cout << resultado << endl;

            if (montoPagado >= totalFinal) {
                pago.mostrarDetalle();
                pedido->cambiarEstado("pagado");
                pagoExitoso = true;
            }
            else {
                cout << "\n PAGO RECHAZADO - Monto insuficiente" << endl;
                cout << "   Falta: S/" << fixed << setprecision(2) << (totalFinal - montoPagado) << endl;
            }
            break;
        }

        case 2:
        case 3: {
            string numeroTarjeta, banco;
            string tipoTarjeta = (metodoPago == 2) ? "Credito" : "Debito";
            system("cls");
            cout << ROJO << "PAGO CON TARJETA DE " << tipoTarjeta << endl;
            imprimirSeparadorRojoBlanco(30);
            cout << "\nIngrese numero de tarjeta (16 digitos): ";
            numeroTarjeta = leerTarjetaConAsteriscos(16);

            cout << "Ingrese nombre del banco: ";
            getline(cin, banco);

            if (numeroTarjeta.length() < 16) {
                cout << "\n ERROR: Numero de tarjeta invalido (minimo 16 digitos)" << endl;
                pausar();
                return false;
            }

            PagoTarjeta pago(totalFinal, fecha, numeroTarjeta, tipoTarjeta, banco);
            cout << endl;

            cout << ">>>Procesando pago con tarjeta de " << tipoTarjeta << "..." << endl;
            _sleep(1500);
            cout << ">>>Conectando con el banco: " << banco << "..." << endl;
            _sleep(1500);
            cout << ">>>Verificando datos de la tarjeta..." << endl;
            _sleep(2000);

            string resultado = pago.procesarPago();
            cout << "\n>>> " << resultado << endl;
            _sleep(1000);

            if (resultado.find("ERROR") == string::npos && numeroTarjeta.length() >= 16) {
                limpiarPantalla();
                cout << VERDE << "\n PAGO PROCESADO EXITOSAMENTE " << RESET << endl;
                pago.mostrarDetalle();
                pedido->cambiarEstado("pagado");
                pagoExitoso = true;
            }
            else {
                cout << ROJO << "\n PAGO RECHAZADO - Por favor, intente con otro metodo." << RESET << endl;
            }
            break;
        }

        default:
            cout << "\n Opción invalida" << endl;
            pausar();
            return false;
        }

        if (pagoExitoso) {
            pausar();

            Comprobante* comprobante = generarComprobante(pedido, totalFinal, fecha);
            pedido->setComprobante(comprobante);

            limpiarPantalla();
            comprobante->mostrarComprobante();
            pausar();
            limpiarPantalla();

            Cliente* clientePedido = pedido->getCliente();
            string dniCliente = clientePedido->getDni();

            Cliente* clienteActualizar = GestorClientes::buscarClientePorDNI(dniCliente);

            if (clienteActualizar != nullptr) {
                clienteActualizar->agregarPuntos(20);
                clienteActualizar->actualizarCategoria();
                GestorClientes::actualizarCliente(clienteActualizar);

                cout << VERDE << "\n¡FELICIDADES!" << RESET << endl;
                cout << AMARILLO << "+20 puntos de lealtad agregados a tu cuenta!" << RESET << endl;
                cout << "Puntos totales: " << clienteActualizar->getPuntosLealtad() << endl;
                cout << "Categoria actual: " << clienteActualizar->getCategoria() << endl;

                delete clienteActualizar;
                /* pausar();*/

            }
        }

        pausar();
        return pagoExitoso;
    }

    Comprobante* generarComprobante(Pedido* pedido, double totalFinal, string fecha) {
        limpiarPantalla();
        cout << BLANCO;
        cout << ROJO << "              COMPROBANTE" << RESET << endl;
        imprimirSeparadorRojoBlanco(20);

        cout << "1.BOLETA" << endl;
        cout << "2.FACTURA" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << AMARILLO << "\nSeleccione su tipo de comprobante: " << RESET;

        int tipoComprobante = leerOpcion();
        Comprobante* comprobante = nullptr;

        if (tipoComprobante == 1) {
            Cliente* cliente = pedido->getCliente();
            comprobante = new Boleta(
                fecha,
                totalFinal,
                cliente->getNombreCompleto(),
                cliente->getDni(),
                pedido->getProductos()
            );
        }
        else if (tipoComprobante == 2) {
            string ruc, razonSocial;

            limpiarPantalla();
            cout << "\nDATOS PARA FACTURA " << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << "Ingrese RUC (11 digitos): ";
            getline(cin, ruc);

            cout << "Ingrese Razon Social: ";
            getline(cin, razonSocial);

            if (ruc.length() != 11) {
                cout << "\n ERROR: El RUC debe tener 11 digitos" << endl;
                cout << " Se generara una Boleta en su lugar" << endl;
                pausar();

                Cliente* cliente = pedido->getCliente();
                comprobante = new Boleta(
                    fecha,
                    totalFinal,
                    cliente->getNombreCompleto(),
                    cliente->getDni(),
                    pedido->getProductos()
                );
            }
            else {
                comprobante = new Factura(
                    fecha,
                    totalFinal,
                    razonSocial,
                    ruc,
                    pedido->getProductos()
                );

                cout << "\n Factura generada para: " << razonSocial << endl;
                cout << " RUC: " << ruc << endl;
            }
        }
        else {
            cout << "\n Opcion invalida. Se generara Boleta por defecto." << endl;
            Cliente* cliente = pedido->getCliente();
            comprobante = new Boleta(
                fecha,
                totalFinal,
                cliente->getNombreCompleto(),
                cliente->getDni(),
                pedido->getProductos()
            );
        }

        pausar();
        return comprobante;
    }

    Cliente* registrarCliente(bool esDelivery) {
        limpiarPantalla();
        cout << ROJO << "REGISTRO DE CLIENTE" << endl;
        imprimirSeparadorRojoBlanco(20);

        string dni;
        cout << "Ingrese DNI: ";
        getline(cin, dni);

        Cliente* clienteExistente = GestorClientes::buscarClientePorDNI(dni);

        if (clienteExistente != nullptr) {
            cout << VERDE << "\n¡Bienvenido de nuevo, "
                << clienteExistente->getNombreCompleto() << "!" << RESET << endl;

            string colorCat = (clienteExistente->getCategoria() == "Premium") ? VERDE :
                (clienteExistente->getCategoria() == "VIP") ? AMARILLO : BLANCO;

            cout << "Categoria: " << colorCat << clienteExistente->getCategoria() << RESET << endl;
            cout << "Puntos de lealtad: " << clienteExistente->getPuntosLealtad() << endl;

            pausar();
            return clienteExistente;
        }

        cout << AMARILLO << "\nCliente nuevo detectado" << RESET << endl;
        string nombre, apellido, telefono, direccion = "";

        cout << "Ingrese nombre: ";
        getline(cin, nombre);

        cout << "Ingrese apellido: ";
        getline(cin, apellido);

        cout << "Ingrese telefono: ";
        getline(cin, telefono);

        if (esDelivery) {
            cout << "Ingrese direccion de entrega: ";
            getline(cin, direccion);
        }

        Cliente* cliente = new Cliente(contadorClientes++, nombre, apellido,
            telefono, dni, direccion, "Regular", 0);

        GestorClientes::guardarCliente(cliente);

        cout << VERDE << "\n¡Cliente registrado exitosamente!" << RESET << endl;
        cout << "Categoria inicial: Regular (0 puntos)" << endl;
        pausar();

        return cliente;
    }

    void mostrarMenuPorCategoria(Carta* carta, const string& categoria) {
        limpiarPantalla();

        if (categoria == "completo") {
            carta->mostrarMenu();
        }
        else {
            carta->mostrarPorCategoria(categoria);
        }
    }

    Producto* seleccionarProductoPorID(Carta* carta) {
        int id;
        cout << "\nIngrese ID del producto: ";
        cin >> id;
        cin.ignore(10000, '\n');

        Producto* p = carta->buscarPlato(id);
        if (p != nullptr && !p->isDisponible()) {
            cout << ROJO << "\n Lo sentimos, el producto '" << p->getNombre()
                << "' NO esta disponible en este momento." << RESET << endl;
            delete p;
            return nullptr;
        }

        return p;
    }

    string obtenerFechaActual() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[11];
        sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
        return string(buffer);
    }


};