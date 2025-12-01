#pragma once
#include "Producto.h"
#include "Cliente.h"
#include "Cola.h"
#include "Comprobante.h"
#include "Funcionalidades.h"

class Pedido {
private:
    int numeroPedido;
    Cliente* cliente;
    Cola<Producto*> productos;
    double total;
    string estado;
    string tipoServicio;
    Comprobante* comprobante;
    double totalManual;
    bool usarTotalManual;
    string fechaPedido;
public:
    Pedido(int numero = 0, Cliente* cli = nullptr, string tipo = "local")
        : numeroPedido(numero), cliente(cli), total(0.0),
        estado("pendiente"), tipoServicio(tipo), comprobante(nullptr), totalManual(0.0), usarTotalManual(false), fechaPedido("") {

        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[11];
        sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
        fechaPedido = string(buffer);
    }


    ~Pedido() {}


    void agregarProducto(Producto* producto) {
        productos.encolar(producto);
        total += producto->getPrecio();
        cout << ">>> Producto agregado: " << producto->getNombre() << endl;
    }


    bool quitarProducto(int posicion) {
        if (posicion < 0 || posicion >= productos.gettamano()) {
            cout << ">>> Posicion invalida" << endl;
            return false;
        }

        Producto* productoEliminado = productos.obtenerEnPosicion(posicion);
        total -= productoEliminado->getPrecio();

        if (productos.eliminarEnPosicion(posicion)) {
            cout << ">>> Producto eliminado: " << productoEliminado->getNombre() << endl;
            return true;
        }

        return false;
    }




    Producto* procesarSiguienteProducto() {
        if (productos.estaVacia()) {
            cout << ">>> No hay mas productos en el pedido" << endl;
            return nullptr;
        }

        Producto* producto = productos.desencolar();
        cout << ">>> Procesando: " << producto->getNombre() << endl;
        return producto;
    }

    Producto* verSiguienteProducto() const {
        if (productos.estaVacia()) {
            return nullptr;
        }
        return productos.verFrente();
    }


    double calcularTotal() const {
        if (usarTotalManual) {
            return totalManual;
        }

        double total = 0.0;
        for (int i = 0; i < productos.gettamano(); i++) {
            Producto* p = productos.obtenerEnPosicion(i);
            if (p != nullptr) {
                total += p->getPrecio();
            }
        }
        return total;
    }


    void mostrarResumen() const {

        cout << "PEDIDO #" << numeroPedido << endl;


        if (cliente != nullptr) {
            cout << "Cliente: " << cliente->getNombreCompleto() << endl;
            cout << "DNI: " << cliente->getDni() << endl;
            cout << "Telefono: " << cliente->getTelefono() << endl;
            if (tipoServicio == "delivery") {
                cout << "Direccion: " << cliente->getDireccion() << endl;
            }
        }

        cout << "Tipo: " << tipoServicio << endl;
        cout << "Estado: " << estado << endl;
        cout << "Productos en cola: " << productos.gettamano() << endl;
        cout << "Total: S/" << fixed << setprecision(2) << total << endl;

        if (comprobante != nullptr)
        {
            cout << "Comprobante: " << comprobante->getTipoComprobante() << " N" << comprobante->getNumeroComprobante() << endl;
        }


    }


    void mostrarProductos() const {
        cout << ">>> Productos del pedido #" << numeroPedido << " del cliente: " << getCliente()->getNombreCompleto() << endl;

        if (productos.estaVacia()) {
            cout << ">>> Pedido vacio" << endl;
            return;
        }

        for (int i = 0; i < productos.gettamano(); i++) {
            Producto* p = productos.obtenerEnPosicion(i);
            cout << (i + 1) << ". " << p->getNombre()
                << " - S/" << fixed << setprecision(2) << p->getPrecio() << endl;
        }
    }

    void cambiarEstado(const string& nuevoEstado) {
        estado = nuevoEstado;
    }

    bool estaVacio() const {
        return productos.estaVacia();
    }

    bool operator==(const Pedido& otro) const {
        return numeroPedido == otro.numeroPedido;
    }

    bool operator!=(const Pedido& otro) const {
        return numeroPedido != otro.numeroPedido;
    }

    void setTotalManual(double total) {
        totalManual = total;
        usarTotalManual = true;
    }

    // Getters
    int getNumeroPedido() const { return numeroPedido; }
    Cliente* getCliente() const { return cliente; }
    string getNombreCliente() const {
        return cliente ? cliente->getNombreCompleto() : "Sin cliente";
    }
    string getEstado() const { return estado; }
    string getTipoServicio() const { return tipoServicio; }
    string getFechaPedido() const { return fechaPedido; }
    int getCantidadProductos() const { return productos.gettamano(); }
    Cola<Producto*>* getProductos() { return &productos; }
    Comprobante* getComprobante() const { return comprobante; }

    // Setters
    void setCliente(Cliente* cli) { cliente = cli; }
    void setTipoServicio(const string& tipo) { tipoServicio = tipo; }
    void setComprobante(Comprobante* comp) { comprobante = comp; }
    void setFechaPedido(const string& f) { fechaPedido = f; }
};

