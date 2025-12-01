#pragma once
#include "Carta.h"
#include "Funcionalidades.h"

class CartaLocal : public Carta {
private:
    int mesasDisponibles;
    string horarioAtencion;

public:

    CartaLocal() : Carta("local", "Menu para Salon", "Data/menu.txt") {
        mesasDisponibles = 20;
        horarioAtencion = "11:00 - 23:00";
    }


    ~CartaLocal() {}



    string getHorarioAtencion() const { return horarioAtencion; }


    void setHorarioAtencion(const string& horario) {
        horarioAtencion = horario;
    }
};
