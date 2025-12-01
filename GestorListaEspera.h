#pragma once
#include "ArbolTreap.h"
#include "Cliente.h"
#include "Mesa.h"
#include "Funcionalidades.h"
using namespace ConsolaColor;

struct SolicitudEspera {
    Cliente cliente;
    string horaDeseada;
    string fechaDeseada;
    int numeroPersonas;
    string timestamp;
    int numeroMesaDeseada;
    int prioridadCalculada;

    // Constructor
    SolicitudEspera() : numeroPersonas(0), numeroMesaDeseada(0), prioridadCalculada(0) {}

    SolicitudEspera(const Cliente& cli, string fecha, string hora, int numPers, int numMesa, int prio)
        : cliente(cli), fechaDeseada(fecha), horaDeseada(hora),
        numeroPersonas(numPers), numeroMesaDeseada(numMesa), prioridadCalculada(prio) {
        timestamp = obtenerHoraActual();
    }

    // Operadores para BST (ordena por fecha/hora)
    bool operator<(const SolicitudEspera& otra) const {
        if (fechaDeseada != otra.fechaDeseada) {
            return compararFechas(fechaDeseada, otra.fechaDeseada) < 0;
        }
        return horaDeseada < otra.horaDeseada;
    }

    bool operator>(const SolicitudEspera& otra) const {
        if (fechaDeseada != otra.fechaDeseada) {
            return compararFechas(fechaDeseada, otra.fechaDeseada) > 0;
        }
        return horaDeseada > otra.horaDeseada;
    }

    bool operator==(const SolicitudEspera& otra) const {
        return cliente.getDni() == otra.cliente.getDni() &&
            fechaDeseada == otra.fechaDeseada &&
            horaDeseada == otra.horaDeseada;
    }

    static string obtenerHoraActual() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[6];
        sprintf(buffer, "%02d:%02d", ltm->tm_hour, ltm->tm_min);
        return string(buffer);
    }

    static int compararFechas(const string& f1, const string& f2) {
        if (f1.length() < 10 || f2.length() < 10) return 0;

        try {
            int dia1 = stoi(f1.substr(0, 2));
            int mes1 = stoi(f1.substr(3, 2));
            int anio1 = stoi(f1.substr(6, 4));

            int dia2 = stoi(f2.substr(0, 2));
            int mes2 = stoi(f2.substr(3, 2));
            int anio2 = stoi(f2.substr(6, 4));

            if (anio1 != anio2) return (anio1 < anio2) ? -1 : 1;
            if (mes1 != mes2) return (mes1 < mes2) ? -1 : 1;
            if (dia1 != dia2) return (dia1 < dia2) ? -1 : 1;

            return 0;
        }
        catch (...) {
            return 0;
        }
    }
};

class GestorListaEspera {
private:
    ArbolTreap<SolicitudEspera> listaEspera;

    // CALCULAR PRIORIDAD
    int calcularPrioridad(const Cliente& cliente, int numeroPersonas) const {
        int prioridad = 0;

        // 1. CATEGORÍA DEL CLIENTE (40 puntos max)
        string categoria = cliente.getCategoria();
        if (categoria == "VIP") {
            prioridad += 40;
        }
        else if (categoria == "Premium") {
            prioridad += 25;
        }
        else {
            prioridad += 5;  // Regular
        }

        // 2. TAMAÑO DEL GRUPO (30 puntos max)
        if (numeroPersonas >= 8) {
            prioridad += 30;
        }
        else if (numeroPersonas >= 5) {
            prioridad += 20;
        }
        else if (numeroPersonas >= 3) {
            prioridad += 10;
        }
        else {
            prioridad += 5;
        }

        // 3. PUNTOS DE LEALTAD (20 puntos max)
        int puntosBonus = min(cliente.getPuntosLealtad() / 10, 20);
        prioridad += puntosBonus;

        // 4. BONUS ALEATORIO (5 puntos max) - para desempatar
  /*      prioridad += (rand() % 6);*/

        return prioridad;
    }

public:
    GestorListaEspera() {}

    // AGREGAR A LISTA DE ESPERA
    bool agregarAListaEspera(const Cliente& cliente, const string& fecha,
        const string& hora, int numeroPersonas, int numeroMesa) {

        // Calcular prioridad
        int prioridad = calcularPrioridad(cliente, numeroPersonas);

        // Crear solicitud
        SolicitudEspera solicitud(cliente, fecha, hora, numeroPersonas, numeroMesa, prioridad);

        // Insertar en ArbolTreap
        listaEspera.insertar(solicitud, prioridad);

        cout << VERDE << "\n¡Agregado a lista de espera!" << RESET << endl;

        cout << "Cliente: " << cliente.getNombreCompleto() << endl;
        cout << "Categoria: " << AMARILLO << cliente.getCategoria() << RESET << endl;
        cout << "Prioridad asignada: " << AMARILLO << prioridad << RESET << " puntos" << endl;
        cout << "Posicion estimada: Dependera de las cancelaciones" << endl;


        return true;
    }

    // MOSTRAR LISTA DE ESPERA (Por prioridad)
    void mostrarListaEspera() const {
        if (listaEspera.estaVacio()) {
            cout << "\n>>> No hay solicitudes en lista de espera." << endl;
            return;
        }

        cout << ROJO << "\n        LISTA DE ESPERA (Orden de Prioridad)        " << RESET << endl;


        cout << left << setw(4) << "#"
            << setw(25) << "CLIENTE"
            << setw(12) << "FECHA"
            << setw(8) << "HORA"
            << setw(6) << "PERS"
            << setw(12) << "CATEGORIA"
            << setw(10) << "PRIORIDAD" << endl;


        int posicion = 1;
        listaEspera.recorrerPorPrioridad([&](SolicitudEspera s, int prio) {
            string colorCat = (s.cliente.getCategoria() == "Premium") ? VERDE :
                (s.cliente.getCategoria() == "VIP") ? AMARILLO : BLANCO;

            cout << left
                << setw(4) << posicion
                << setw(25) << s.cliente.getNombreCompleto()
                << setw(12) << s.fechaDeseada
                << setw(8) << s.horaDeseada
                << setw(6) << s.numeroPersonas
                << colorCat << setw(12) << s.cliente.getCategoria() << RESET
                << AMARILLO << prio << RESET << " pts" << endl;
            posicion++;
            });


        cout << "Total en espera: " << listaEspera.getTamano() << " solicitudes" << endl;
    }


    SolicitudEspera obtenerSiguiente() {
        if (listaEspera.estaVacio()) {
            throw runtime_error("Lista de espera vacia");
        }

        SolicitudEspera siguiente = listaEspera.obtenerMayorPrioridad();
        listaEspera.eliminar(siguiente);

        return siguiente;
    }

    // PROCESAR SIGUIENTE
    void procesarSiguiente() {
        if (listaEspera.estaVacio()) {
            cout << ROJO << "\n>>> No hay nadie en lista de espera" << RESET << endl;
            return;
        }

        SolicitudEspera siguiente = obtenerSiguiente();

        cout << VERDE << "\n>>> PROCESANDO SOLICITUD DE MAYOR PRIORIDAD" << RESET << endl;
        /*imprimirSeparadorRojoBlanco(40);*/
        cout << "Cliente: " << siguiente.cliente.getNombreCompleto() << endl;
        cout << "DNI: " << siguiente.cliente.getDni() << endl;
        cout << "Telefono: " << siguiente.cliente.getTelefono() << endl;
        cout << "Fecha/Hora: " << siguiente.fechaDeseada << " " << siguiente.horaDeseada << endl;
        cout << "Personas: " << siguiente.numeroPersonas << endl;
        cout << "Mesa deseada: " << siguiente.numeroMesaDeseada << endl;
        cout << "Prioridad: " << AMARILLO << siguiente.prioridadCalculada << RESET << " pts" << endl;

        cout << AMARILLO << "\nLlamar al cliente para confirmar la reserva" << RESET << endl;
    }

    bool hayPersonasEsperando() const {
        return !listaEspera.estaVacio();
    }

    int getTotalEsperando() const {
        return listaEspera.getTamano();
    }


    template<typename Funcion>
    void recorrerPorPrioridad(Funcion func) const {
        listaEspera.recorrerPorPrioridad(func);
    }


};