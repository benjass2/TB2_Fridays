#pragma once
#include "Carta.h"
#include "Funcionalidades.h"

class CartaDelivery : public Carta {
private:
    double tarifaDelivery;
    int tiempoEstimadoMin;
    double montoMinimoDelivery;

public:
    CartaDelivery() : Carta("delivery", "Menu para Delivery", "Data/menu.txt") {
        tarifaDelivery = 8.90;
        tiempoEstimadoMin = 35;
        montoMinimoDelivery = 25.00;
    }

    ~CartaDelivery() {}

    void cargarProductos(const Lista<Producto>& productosExternos) override {
        productos = Lista<Producto>();
        for (int i = 0; i < productosExternos.gettamano(); i++) {
            Producto p = productosExternos.obtenerEnPosicion(i);
            if (esAptoParaDelivery(p)) {
                productos.insertarFinal(p);
            }
        }
        actualizarIndices();
        cout << ">>> Carta Delivery: " << productos.gettamano() << " productos cargados (filtrados)" << endl;
    }

    double getTarifaDelivery() const { return tarifaDelivery; }
    int getTiempoEstimado() const { return tiempoEstimadoMin; }
    double getMontoMinimo() const { return montoMinimoDelivery; }

    void setTarifaDelivery(double tarifa) {
        if (tarifa >= 0) tarifaDelivery = tarifa;
    }

    void setTiempoEstimado(int minutos) {
        if (minutos > 0) tiempoEstimadoMin = minutos;
    }

private:
    string convertirMinusculas(string texto) const {
        for (size_t i = 0; i < texto.length(); i++) {
            if (texto[i] >= 'A' && texto[i] <= 'Z') {
                texto[i] = texto[i] + ('a' - 'A');
            }
        }
        return texto;
    }

    bool esAptoParaDelivery(const Producto& p) const {
        string nombre = convertirMinusculas(p.getNombre());

        // ============================================================
        // 1. EXCLUIR BEBIDAS ALCOHÓLICAS Y CÓCTELES
        // ============================================================
        const string alcohol[] = {
            "cerveza", "vino", "whisky", "ron", "vodka",
            "mojito", "chardonay", "tequila",
            "daiquiri", "pisco", "margarita", "gin"
        };

        for (const string& palabra : alcohol) {
            if (nombre.find(palabra) != string::npos)
                return false;
        }

        // ============================================================
        // 2. EXCLUIR LÍQUIDOS CALIENTES O FÁCILES DE DERRAMAR
        // ============================================================
        const string calientes[] = {
            "sopa",
            "te caliente",
            "té caliente", // por si acaso usan tilde
            "cafe",
            "café"
        };

        for (const string& palabra : calientes) {
            if (nombre.find(palabra) != string::npos)
                return false;
        }

        // ============================================================
        // 3. EXCLUIR PRODUCTOS FROZEN / QUE SE DERRITEN
        // ============================================================
        const string frozen[] = {
            "helado", "mousse", "frappuccino",
            "smoothie", "malteada", "batido",
            "capuccino", "capuchino"
        };

        for (const string& palabra : frozen) {
            if (nombre.find(palabra) != string::npos)
                return false;
        }

        // ============================================================
        // 4. EXCLUIR ALIMENTOS FRITOS / CRUJIENTES
        // ============================================================
        const string fritos[] = {
            "papas fritas",
            "churros",
            "nachos",
            "aros de cebolla"
        };

        for (const string& palabra : fritos) {
            if (nombre.find(palabra) != string::npos)
                return false;
        }

        // ============================================================
        // Si no entró a ninguna restricción, es Apto.
        // ============================================================
        return true;
    }


};
