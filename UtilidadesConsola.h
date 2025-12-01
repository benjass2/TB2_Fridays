#pragma once
#include "Funcionalidades.h"
using namespace ConsolaColor;

namespace UtilidadesConsola {


    void imprimirSeparadorRojoBlanco(int longitud_pares) {

        std::cout << NEGRITA;

        for (int i = 0; i < longitud_pares; ++i) {
            std::cout << ROJO << "-" << BLANCO << "-";
        }

        std::cout << RESET << std::endl;
    }


    void mostrarLogoPrincipal() {
        cout << ConsolaColor::ROJO << ConsolaColor::ROJO_BRILLANTE;

        cout << endl;
        cout << ROJO << "                                /$$$$$$$$        /$$       /$$                              " << endl;
        cout << BLANCO << "                               | $$_____/       |__/      | $$                              " << endl;
        cout << ROJO << "                               | $$     /$$$$$$  /$$  /$$$$$$$  /$$$$$$  /$$   /$$  /$$$$$$$" << endl;
        cout << BLANCO << "                               | $$$$$ /$$__  $$| $$ /$$__  $$ |____  $$| $$  | $$ /$$_____/" << endl;
        cout << ROJO << "                               | $$__/| $$  \\__/| $$| $$  | $$  /$$$$$$$| $$  | $$|  $$$$$$ " << endl;
        cout << BLANCO << "                               | $$   | $$      | $$| $$  | $$ /$$__  $$| $$  | $$ \\____  $$" << endl;
        cout << ROJO << "                               | $$   | $$      | $$|  $$$$$$$|  $$$$$$$|  $$$$$$$ /$$$$$$$/" << endl;
        cout << BLANCO << "                               |__/   |__/      |__/ \\_______/ \\_______/ \\____  $$|_______/ " << endl;
        cout << ROJO << "                                                                         /$$  | $$          " << endl;
        cout << BLANCO << "                                              T.G.I     Restaurante     |  $$$$$$/     " << endl;
        cout << ROJO << "                                                                          \\______/           " << endl;

        cout << ConsolaColor::BLANCO;
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



    string obtenerFechaActual() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[11];
        sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
        return string(buffer);
    }

    // ==================== MÉTODOS DE INTERFAZ DE CONSOLA ====================

    //UI DE CLIENTE
    void leerTipoServicio()
    {
        limpiarPantalla();
        cout << ConsolaColor::ROJO;
        cout << "          TIPO DE SERVICIO" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << ConsolaColor::AMARILLO;
        cout << "\n  ->Como deseas usar Friday's?<-\n" << endl;
        cout << ConsolaColor::BLANCO;
        cout << "  1. HACER UNA RESERVA (Mesa para fecha/hora)" << endl;
        cout << "  2. COMER EN LOCAL (Salon Fridays)" << endl;
        cout << "  3. DELIVERY (Entrega a domicilio)" << endl;
        cout << "  4. Volver" << endl;
        cout << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "  Opcion: ";
    }


    void leerMetodoPago()
    {
        cout << AMARILLO << "\nMETODOS DE PAGO DISPONIBLES"; cout << RESET << endl;
        cout << "1. Efectivo" << endl;
        cout << "2. Tarjeta de Credito" << endl;
        cout << "3. Tarjeta de Debito" << endl;
        cout << "0. Cancelar pago" << endl;
        cout << "---------------------------" << endl;
        cout << AMARILLO << "Seleccione metodo de pago: "; cout << BLANCO;
    }



    //UI DE ADMINISTRADOR
    void leerPanelAdmin()
    {
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
    }



    string leerTarjetaConAsteriscos(int maxDigitos = 16) {
        string numero = "";
        char c;

        while (true) {
            c = _getch(); // Lee tecla sin mostrarla

            if (c >= '0' && c <= '9') { // Solo números
                if (numero.size() < maxDigitos) {
                    numero += c;
                    cout << "*"; // Muestra asterisco
                }
            }
            else if (c == 8 && !numero.empty()) {
                // Tecla Backspace
                numero.pop_back();
                cout << "\b \b"; // Borra el último asterisco
            }
            else if (c == 13) {
                // Enter
                break;
            }
        }

        cout << endl;
        return numero;
    }
}