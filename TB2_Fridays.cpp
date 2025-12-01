#include <iostream>
#include "locale.h"
#include "pch.h"
#include "Fridays.h"  
#include "ControladoraCliente.h"
#include "ControladoraAdmin.h"

int main() {
    setlocale(LC_ALL, "Spanish");
    Fridays sistema;
    sistema.ejecutar();
    return 0;
}