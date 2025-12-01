#pragma once
#include "ArbolAVL.h"
#include "Reserva.h"
#include "Mesa.h"
#include "Cliente.h"
#include "Lista.h"
#include "GestorArchivos.h"
#include "GestorListaEspera.h"
#include "GestorClientes.h"
#include "Funcionalidades.h"
#include "Grafo.h"
using namespace ConsolaColor;

class GestorReserva {
private:
    ArbolAVL<Reserva> arbolReservas;
    Lista<Mesa> mesas;
    GestorListaEspera listaEspera;
    GrafoMesas grafoMesas;  // ✨ NUEVO: Grafo para gestión de mesas

    const string HORARIO_APERTURA = "11:00";
    const string HORARIO_CIERRE = "23:00";

    // Función para validar horario
    bool validarHorario(const string& hora) {
        int h = stoi(hora.substr(0, 2));
        int m = stoi(hora.substr(3, 2));
        int minutosDesdeCero = h * 60 + m;
        int aperturaMinutos = 11 * 60;
        int cierreMinutos = 23 * 60;
        return (minutosDesdeCero >= aperturaMinutos && minutosDesdeCero <= cierreMinutos);
    }

public:
    GestorReserva() {
        mesas = GestorArchivos::cargarMesas();
        if (mesas.estaVacia()) {
            inicializarMesas();
            GestorArchivos::guardarMesas(mesas);
        }

        // ✨ NUEVO: Inicializar grafo de mesas
        grafoMesas.inicializarGrafoRestaurante(mesas);

        cargarListaEsperaDesdeArchivo();
    }

    ~GestorReserva() {
        guardarDatos();
    }

    void inicializarMesas() {
        // Mesas regulares - Interior (10 mesas)
        for (int i = 1; i <= 10; i++) {
            mesas.insertarFinal(Mesa(i, 4, "Interior", "Regular"));
        }

        // Mesas familiares - Interior (5 mesas)
        for (int i = 11; i <= 15; i++) {
            mesas.insertarFinal(Mesa(i, 6, "Interior", "Familiar"));
        }

        // Mesas terraza (5 mesas)
        for (int i = 16; i <= 20; i++) {
            mesas.insertarFinal(Mesa(i, 4, "Terraza", "Regular"));
        }

        // Mesas VIP (5 mesas)
        for (int i = 21; i <= 25; i++) {
            mesas.insertarFinal(Mesa(i, 4, "VIP", "VIP"));
        }

        cout << ">>> " << mesas.gettamano() << " mesas inicializadas." << endl;
    }

    void guardarDatos() {
        GestorArchivos::guardarReservas(arbolReservas);
        GestorArchivos::guardarMesas(mesas);
    }

    // ✨ NUEVA FUNCIÓN: Buscar mesas unibles para grupos grandes
    bool buscarYUnirMesas(Cliente& cliente, int numeroPersonas, string fecha,
        string hora, string observaciones = "") {

        cout << AMARILLO << "\n>>> Buscando combinación de mesas para "
            << numeroPersonas << " personas..." << RESET << endl;

        // Intentar desde cada mesa disponible
        for (int i = 0; i < mesas.gettamano(); i++) {
            Mesa mesaInicio = mesas.obtenerEnPosicion(i);

            if (!mesaDisponibleEnFechaHora(mesaInicio.getNumeroMesa(), fecha, hora)) {
                continue;
            }

            // Usar DFS para encontrar mesas unibles
            Lista<int> mesasUnibles = grafoMesas.encontrarMesasUnibles(
                mesaInicio.getNumeroMesa(), numeroPersonas);

            // Verificar que todas las mesas estén disponibles
            bool todasDisponibles = true;
            int capacidadTotal = 0;

            for (int j = 0; j < mesasUnibles.gettamano(); j++) {
                int numMesa = mesasUnibles.obtenerEnPosicion(j);

                if (!mesaDisponibleEnFechaHora(numMesa, fecha, hora)) {
                    todasDisponibles = false;
                    break;
                }

                Mesa* m = buscarMesa(numMesa);
                if (m) {
                    capacidadTotal += m->getCapacidad();
                    delete m;
                }
            }

            if (todasDisponibles && capacidadTotal >= numeroPersonas) {
                cout << VERDE << "\n>>> ¡Combinación encontrada!" << RESET << endl;
                cout << "Mesas a unir: ";

                for (int j = 0; j < mesasUnibles.gettamano(); j++) {
                    cout << mesasUnibles.obtenerEnPosicion(j);
                    if (j < mesasUnibles.gettamano() - 1) cout << " + ";
                }
                cout << endl;
                cout << "Capacidad total: " << capacidadTotal << " personas" << endl;

                // Crear reservas para cada mesa
                bool exito = true;
                for (int j = 0; j < mesasUnibles.gettamano(); j++) {
                    int numMesa = mesasUnibles.obtenerEnPosicion(j);
                    Mesa* mesa = buscarMesa(numMesa);

                    if (mesa) {
                        Reserva nuevaReserva(cliente, *mesa, fecha, hora,
                            numeroPersonas,
                            observaciones + " [MESAS UNIDAS]");
                        arbolReservas.insertar(nuevaReserva);
                        delete mesa;
                    }
                    else {
                        exito = false;
                        break;
                    }
                }

                if (exito) {
                    cliente.agregarPuntos(30); // Bonus por grupo grande
                    cliente.actualizarCategoria();
                    GestorClientes::actualizarCliente(&cliente);

                    guardarDatos();

                    cout << AMARILLO << "\n+30 puntos de lealtad (grupo grande)!"
                        << RESET << endl;
                    return true;
                }
            }
        }

        return false;
    }

    //Crear reserva (modificada)
    bool crearReserva(Cliente& cliente, int numeroMesa, string fecha, string hora,
        int numeroPersonas, string observaciones = "") {

        if (!validarHorario(hora)) {
            cout << ROJO << ">>> Error: La hora debe estar entre "
                << HORARIO_APERTURA << " y " << HORARIO_CIERRE << "."
                << RESET << endl;
            return false;
        }

        Mesa* mesaSeleccionada = buscarMesa(numeroMesa);

        if (mesaSeleccionada && !mesaSeleccionada->tieneCapacidadPara(numeroPersonas)) {
            delete mesaSeleccionada;

            cout << AMARILLO << "\n>>> La mesa individual no tiene capacidad suficiente"
                << RESET << endl;
            cout << "¿Desea que busquemos una combinación de mesas? (s/n): ";

            char resp;
            cin >> resp;
            cin.ignore();

            if (resp == 's' || resp == 'S') {
                return buscarYUnirMesas(cliente, numeroPersonas, fecha, hora, observaciones);
            }

            return false;
        }

        if (mesaSeleccionada == nullptr) {
            cout << ROJO << ">>> Error: Mesa no encontrada." << RESET << endl;
            ofrecerListaEspera(cliente, fecha, hora, numeroPersonas, numeroMesa);
            return false;
        }

        // Verificar disponibilidad
        if (!mesaDisponibleEnFechaHora(numeroMesa, fecha, hora)) {
            cout << ROJO << ">>> Error: La mesa " << numeroMesa
                << " ya está reservada para " << fecha << " a las " << hora
                << RESET << endl;

            delete mesaSeleccionada;
            ofrecerListaEspera(cliente, fecha, hora, numeroPersonas, numeroMesa);
            return false;
        }

        // Crear la reserva
        Reserva nuevaReserva(cliente, *mesaSeleccionada, fecha, hora,
            numeroPersonas, observaciones);
        arbolReservas.insertar(nuevaReserva);

        cout << VERDE << "\n>>> Reserva creada exitosamente!" << RESET << endl;
        cout << "Codigo de reserva: " << nuevaReserva.getCodigoReserva() << endl;

        // Actualizar puntos
        cliente.agregarPuntos(20);
        cliente.actualizarCategoria();
        GestorClientes::actualizarCliente(&cliente);

        cout << AMARILLO << "\n+20 puntos de lealtad agregados!" << RESET << endl;
        cout << "Puntos totales: " << cliente.getPuntosLealtad() << endl;
        cout << "Categoria actual: " << cliente.getCategoria() << endl;

        delete mesaSeleccionada;
        guardarDatos();
        return true;
    }

    // ✨ NUEVA FUNCIÓN: Sugerir mesa óptima usando BFS
    void sugerirMesaOptima(int numeroPersonas, const string& zonaPreferida) {
        cout << ROJO << "\n>>> SUGERENCIAS DE MESAS ÓPTIMAS" << RESET << endl;
        imprimirSeparadorRojoBlanco(40);

        cout << "Buscando en zona: " << zonaPreferida << endl;
        cout << "Capacidad requerida: " << numeroPersonas << " personas" << endl;
        cout << endl;

        // Buscar mesas en la zona usando BFS desde mesa 1
        Lista<int> mesasZona = grafoMesas.buscarMesasPorZona(1, zonaPreferida);

        if (mesasZona.estaVacia()) {
            cout << ROJO << "No hay mesas en la zona especificada" << RESET << endl;
            return;
        }

        cout << VERDE << "Mesas encontradas:" << RESET << endl;
        for (int i = 0; i < mesasZona.gettamano(); i++) {
            int numMesa = mesasZona.obtenerEnPosicion(i);
            Mesa* m = buscarMesa(numMesa);

            if (m && m->getCapacidad() >= numeroPersonas) {
                cout << "  Mesa " << numMesa << " - Capacidad: "
                    << m->getCapacidad() << " - Tipo: " << m->getTipoMesa();

                // Mostrar mesas cercanas
                Lista<int> vecinos = grafoMesas.obtenerVecinos(numMesa);
                if (!vecinos.estaVacia()) {
                    cout << " (Cercanas: ";
                    for (int j = 0; j < vecinos.gettamano() && j < 2; j++) {
                        cout << vecinos.obtenerEnPosicion(j);
                        if (j < vecinos.gettamano() - 1 && j < 1) cout << ", ";
                    }
                    cout << ")";
                }
                cout << endl;
                delete m;
            }
            else if (m) {
                delete m;
            }
        }
    }

    // ✨ NUEVA FUNCIÓN: Calcular ruta óptima entre mesas (para meseros)
    void mostrarRutaEntreMesas(int mesaOrigen, int mesaDestino) {
        cout << ROJO << "\n>>> RUTA ÓPTIMA ENTRE MESAS" << RESET << endl;
        imprimirSeparadorRojoBlanco(30);

        Lista<int> ruta = grafoMesas.rutaMasCorta(mesaOrigen, mesaDestino);

        if (ruta.estaVacia()) {
            cout << ROJO << "No hay ruta disponible entre las mesas" << RESET << endl;
            return;
        }

        cout << "De Mesa " << mesaOrigen << " a Mesa " << mesaDestino << ":" << endl;
        cout << endl;

        double distanciaTotal = 0;

        for (int i = 0; i < ruta.gettamano(); i++) {
            int mesa = ruta.obtenerEnPosicion(i);
            cout << "  " << (i + 1) << ". Mesa " << mesa;

            if (i < ruta.gettamano() - 1) {
                int siguiente = ruta.obtenerEnPosicion(i + 1);
                double dist = grafoMesas.calcularDistancia(mesa, siguiente);
                distanciaTotal += dist;
                cout << " --(" << fixed << setprecision(1) << dist << "m)--> ";
            }
            cout << endl;
        }

        cout << endl;
        cout << "Distancia total: " << fixed << setprecision(2)
            << distanciaTotal << " metros" << endl;
        imprimirSeparadorRojoBlanco(30);
    }

    void verMapaRestaurante() {
        limpiarPantalla();
        grafoMesas.mostrarMapaRestaurante();
    }

    void verEstadisticasGrafo() {
        limpiarPantalla();
        grafoMesas.mostrarEstadisticas();
    }

    void ofrecerListaEspera(Cliente& cliente, string fecha, string hora,
        int numeroPersonas, int numeroMesa) {
        cout << AMARILLO << "\n¿Desea entrar en lista de espera? (s/n): " << RESET;
        char resp;
        cin >> resp;
        cin.ignore();

        if (resp == 's' || resp == 'S') {
            bool tieneReservaEnEsaFecha = false;
            arbolReservas.inOrder([&](Reserva r) {
                if (r.getCliente().getDni() == cliente.getDni() &&
                    r.getFecha() == fecha &&
                    r.getEstado() != "Cancelada") {
                    tieneReservaEnEsaFecha = true;
                }
                });

            if (tieneReservaEnEsaFecha) {
                cout << ROJO << ">>> Ya tienes una reserva para esta fecha."
                    << RESET << endl;
            }
            else {
                listaEspera.agregarAListaEspera(cliente, fecha, hora,
                    numeroPersonas, numeroMesa);
            }
        }
    }

    // Resto de funciones originales...
    void mostrarTodasReservas() {
        if (arbolReservas.estaVacio()) {
            cout << "\n>>> No hay reservas registradas." << endl;
            return;
        }

        cout << ROJO << "                    RESERVAS                         " << endl;
        imprimirSeparadorRojoBlanco(40);

        cout << left << setw(10) << "CODIGO"
            << setw(25) << "CLIENTE"
            << setw(12) << "FECHA"
            << setw(8) << "HORA"
            << setw(8) << "MESA"
            << setw(6) << "PERS."
            << setw(12) << "ESTADO" << endl;

        imprimirSeparadorRojoBlanco(40);
        arbolReservas.inOrder([](Reserva r) {
            r.mostrarResumen();
            });

        cout << "Total de reservas: " << arbolReservas.gettamano() << endl;
    }

    void buscarReservaPorCodigo(string codigo) {
        bool encontrada = false;
        arbolReservas.inOrder([&](Reserva r) {
            if (r.getCodigoReserva() == codigo) {
                r.mostrarInfo();
                encontrada = true;
            }
            });

        if (!encontrada) {
            cout << "\nReserva no encontrada." << endl;
        }
    }

    void listarReservasPorFecha(string fecha) {
        int contador = 0;
        cout << ROJO << "\n>>> Reservas para el dia: " << fecha << RESET << endl;
        imprimirSeparadorRojoBlanco(30);

        arbolReservas.inOrder([&](Reserva r) {
            if (r.getFecha() == fecha) {
                r.mostrarResumen();
                contador++;
            }
            });

        if (contador == 0) {
            cout << ">>> No hay reservas para esta fecha." << endl;
        }
        else {
            imprimirSeparadorRojoBlanco(30);
            cout << "Total: " << contador << " reservas" << endl;
        }
    }

    void mostrarMesasDisponibles(int capacidadMinima, string fecha, string hora) {
        cout << ROJO << "\n>>> Mesas disponibles (capacidad >= "
            << capacidadMinima << "):" << RESET << endl;
        imprimirSeparadorRojoBlanco(30);

        cout << left << setw(6) << "MESA"
            << setw(12) << "CAPACIDAD"
            << setw(12) << "UBICACION"
            << setw(10) << "TIPO"
            << setw(10) << "ESTADO" << endl;
        imprimirSeparadorRojoBlanco(30);

        for (int i = 0; i < mesas.gettamano(); i++) {
            Mesa mesa = mesas.obtenerEnPosicion(i);
            if (mesa.getCapacidad() >= capacidadMinima) {
                mesa.mostrarResumen();
            }
        }
    }

    void cancelarReserva(string codigo) {
        bool encontrada = false;
        Reserva reservaEliminar;

        arbolReservas.inOrder([&](Reserva r) {
            if (r.getCodigoReserva() == codigo) {
                reservaEliminar = r;
                encontrada = true;
            }
            });

        if (encontrada) {
            arbolReservas.eliminar(reservaEliminar);
            cout << VERDE << ">>> Reserva " << codigo
                << " cancelada exitosamente." << RESET << endl;
            guardarDatos();
        }
        else {
            cout << ROJO << ">>> Reserva no encontrada." << endl;
        }
    }

    Mesa* buscarMesa(int numeroMesa) {
        for (int i = 0; i < mesas.gettamano(); i++) {
            Mesa mesa = mesas.obtenerEnPosicion(i);
            if (mesa.getNumeroMesa() == numeroMesa) {
                return new Mesa(mesa);
            }
        }
        return nullptr;
    }

    bool mesaDisponibleEnFechaHora(int numeroMesa, const string& fecha,
        const string& hora) {
        bool disponible = true;

        arbolReservas.inOrder([&](Reserva r) {
            if (r.getEstado() != "Cancelada" &&
                r.getMesa().getNumeroMesa() == numeroMesa &&
                r.getFecha() == fecha) {

                int horaReserva = stoi(r.getHora().substr(0, 2));
                int minReserva = stoi(r.getHora().substr(3, 2));
                int horaDeseada = stoi(hora.substr(0, 2));
                int minDeseada = stoi(hora.substr(3, 2));

                int minutosReserva = horaReserva * 60 + minReserva;
                int minutosDeseada = horaDeseada * 60 + minDeseada;

                if (abs(minutosReserva - minutosDeseada) < 120) {
                    disponible = false;
                }
            }
            });

        return disponible;
    }

    void cargarListaEsperaDesdeArchivo() {
        ifstream file("Data/lista_espera.txt");
        if (!file.is_open()) return;

        string linea;
        while (getline(file, linea)) {
            if (linea.empty()) continue;

            stringstream ss(linea);
            string dni, nombre, apellido, telefono, fecha, hora, numPers,
                numMesa, categoria, puntos;

            getline(ss, dni, ',');
            getline(ss, nombre, ',');
            getline(ss, apellido, ',');
            getline(ss, telefono, ',');
            getline(ss, fecha, ',');
            getline(ss, hora, ',');
            getline(ss, numPers, ',');
            getline(ss, numMesa, ',');
            getline(ss, categoria, ',');
            getline(ss, puntos);

            try {
                Cliente cliente(0, nombre, apellido, telefono, dni, "",
                    categoria, stoi(puntos));
                listaEspera.agregarAListaEspera(cliente, fecha, hora,
                    stoi(numPers), stoi(numMesa));
            }
            catch (...) {
                continue;
            }
        }

        file.close();
    }

    void mostrarEstadisticas() {
        cout << ROJO << "    ESTADÍSTICAS DEL SISTEMA           " << RESET << endl;
        imprimirSeparadorRojoBlanco(30);
        cout << " Total de mesas: " << mesas.gettamano() << endl;
        cout << " Total de reservas: " << arbolReservas.gettamano() << endl;
        cout << " Altura del árbol AVL: " << arbolReservas.getAltura() << endl;
    }

    void recargarReservas() {
        arbolReservas.limpiar();
        GestorArchivos::cargarReservas(arbolReservas);
    }

    void verListaEspera() {
        listaEspera.mostrarListaEspera();
    }

    void procesarSiguienteEnEspera() {
        if (!listaEspera.hayPersonasEsperando()) {
            cout << ROJO << "\n>>> No hay nadie en lista de espera" << RESET << endl;
            return;
        }

        SolicitudEspera siguiente = listaEspera.obtenerSiguiente();

        cout << VERDE << "\n>>> PROCESANDO SOLICITUD DE MAYOR PRIORIDAD"
            << RESET << endl;
        imprimirSeparadorRojoBlanco(40);
        cout << "Cliente: " << siguiente.cliente.getNombreCompleto() << endl;
        cout << "DNI: " << siguiente.cliente.getDni() << endl;
        cout << "Telefono: " << siguiente.cliente.getTelefono() << endl;
        cout << "Fecha/Hora: " << siguiente.fechaDeseada << " "
            << siguiente.horaDeseada << endl;
        cout << "Personas: " << siguiente.numeroPersonas << endl;
        cout << "Mesa deseada: " << siguiente.numeroMesaDeseada << endl;
        cout << "Categoria: " << siguiente.cliente.getCategoria() << endl;
        imprimirSeparadorRojoBlanco(40);

        cout << AMARILLO << "\n¿Desea crear la reserva automaticamente? (s/n): "
            << RESET;
        char resp;
        cin >> resp;
        cin.ignore();

        if (resp == 's' || resp == 'S') {
            if (!mesaDisponibleEnFechaHora(siguiente.numeroMesaDeseada,
                siguiente.fechaDeseada, siguiente.horaDeseada)) {
                cout << ROJO << ">>> La mesa aún no está disponible" << RESET << endl;
                return;
            }

            Mesa* mesa = buscarMesa(siguiente.numeroMesaDeseada);
            if (mesa == nullptr) {
                cout << ROJO << ">>> Error: Mesa no encontrada" << RESET << endl;
                return;
            }

            Reserva nuevaReserva(siguiente.cliente, *mesa,
                siguiente.fechaDeseada, siguiente.horaDeseada,
                siguiente.numeroPersonas);

            arbolReservas.insertar(nuevaReserva);

            cout << VERDE << "\n>>> ¡Reserva creada exitosamente!" << RESET << endl;
            cout << "Codigo de reserva: " << nuevaReserva.getCodigoReserva() << endl;

            Cliente clienteActualizar = siguiente.cliente;
            clienteActualizar.agregarPuntos(20);
            clienteActualizar.actualizarCategoria();
            GestorClientes::actualizarCliente(&clienteActualizar);

            cout << AMARILLO << "+20 puntos de lealtad agregados!" << RESET << endl;

            delete mesa;
            guardarDatos();
            guardarListaEsperaEnArchivo();
        }
    }

    int getTotalEnEspera() const {
        return listaEspera.getTotalEsperando();
    }

    void listarTodasMesas() {
        cout << ROJO << "           MESAS DEL RESTAURANTE FRIDAY'S              "
            << RESET << endl;
        imprimirSeparadorRojoBlanco(40);

        cout << left << setw(6) << "MESA"
            << setw(12) << "CAPACIDAD"
            << setw(12) << "UBICACION"
            << setw(10) << "TIPO"
            << setw(10) << "ESTADO" << endl;
        imprimirSeparadorRojoBlanco(40);

        for (int i = 0; i < mesas.gettamano(); i++) {
            Mesa mesa = mesas.obtenerEnPosicion(i);
            mesa.mostrarResumen();
        }
    }

    int getTotalReservas() const {
        return arbolReservas.gettamano();
    }

    int getTotalMesas() const {
        return mesas.gettamano();
    }

    void guardarListaEsperaEnArchivo() {
        ofstream file("Data/lista_espera.txt");
        if (!file.is_open()) return;

        listaEspera.recorrerPorPrioridad([&](SolicitudEspera s, int prio) {
            file << s.cliente.getDni() << ","
                << s.cliente.getNombre() << ","
                << s.cliente.getApellido() << ","
                << s.cliente.getTelefono() << ","
                << s.fechaDeseada << ","
                << s.horaDeseada << ","
                << s.numeroPersonas << ","
                << s.numeroMesaDeseada << ","
                << s.cliente.getCategoria() << ","
                << s.cliente.getPuntosLealtad() << "\n";
            });

        file.close();
    }

    bool convertirListaEsperaAReserva() {
        if (!listaEspera.hayPersonasEsperando()) {
            cout << ROJO << ">>> No hay personas en lista de espera" << RESET << endl;
            return false;
        }

        SolicitudEspera siguiente = listaEspera.obtenerSiguiente();
        cout << VERDE << "\n>>> Convirtiendo lista de espera a reserva..."
            << RESET << endl;

        if (!mesaDisponibleEnFechaHora(siguiente.numeroMesaDeseada,
            siguiente.fechaDeseada, siguiente.horaDeseada)) {
            cout << ROJO << ">>> La mesa aún no está disponible" << RESET << endl;
            return false;
        }

        Mesa* mesa = buscarMesa(siguiente.numeroMesaDeseada);
        if (mesa == nullptr) {
            cout << ROJO << ">>> Error: Mesa no encontrada" << RESET << endl;
            return false;
        }

        Reserva nuevaReserva(siguiente.cliente, *mesa,
            siguiente.fechaDeseada, siguiente.horaDeseada,
            siguiente.numeroPersonas);

        arbolReservas.insertar(nuevaReserva);

        cout << VERDE << ">>> ¡Reserva creada exitosamente!" << RESET << endl;
        cout << "Código de reserva: " << nuevaReserva.getCodigoReserva() << endl;

        delete mesa;
        guardarDatos();
        return true;
    }
};