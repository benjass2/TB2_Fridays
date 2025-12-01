#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class GestorBackup {
public:
    //Backup de pedidos
    static bool crearBackupPedidos(const string& archivoOriginal = "Data/historialPedidos.txt",
        const string& archivoBackup = "Backup/historialPedidos_backup.txt") {
        return copiarArchivo(archivoOriginal, archivoBackup);
    }

    static bool restaurarBackupPedidos(const string& archivoBackup = "Backup/historialPedidos_backup.txt",
        const string& archivoOriginal = "Data/historialPedidos.txt") {
        if (restaurarArchivo(archivoBackup, archivoOriginal)) {
            return true;
        }
        return false;
    }


    //Backup de reservas
    static bool crearBackupReservas(const string& archivoOriginal = "Data/reservas.txt",
        const string& archivoBackup = "Backup/reservas_backup.txt") {
        return copiarArchivo(archivoOriginal, archivoBackup);
    }

    static bool restaurarBackupReservas(const string& archivoBackup = "Backup/reservas_backup.txt",
        const string& archivoOriginal = "Data/reservas.txt") {
        if (restaurarArchivo(archivoBackup, archivoOriginal)) {
            return true;
        }
        return false;
    }

    //restaurar todo
    static void restaurarTodo() {
        bool pedidos = restaurarBackupPedidos();
        bool reservas = restaurarBackupReservas();
    }


    // CREAR BACKUPS DE TODO
    static void crearBackupsTotales() {
        crearBackupPedidos();
        crearBackupReservas();
    }


    static bool existeBackupPedidos() {
        return existeArchivo("Backup/historialPedidos_backup.txt");
    }

    static bool existeBackupReservas() {
        return existeArchivo("Backup/reservas_backup.txt");
    }


private:

    static bool copiarArchivo(const string& origen, const string& destino) {
        ifstream archivoOrigen(origen);
        ofstream archivoDestino(destino);

        if (!archivoOrigen.is_open() || !archivoDestino.is_open()) {
            return false;
        }

        string linea;
        while (getline(archivoOrigen, linea)) {
            archivoDestino << linea << "\n";
        }

        archivoOrigen.close();
        archivoDestino.close();
        return true;
    }

    static bool restaurarArchivo(const string& backup, const string& original) {
        ifstream archivoBackup(backup);
        ofstream archivoOriginal(original);

        if (!archivoBackup.is_open() || !archivoOriginal.is_open()) {
            return false;
        }

        string linea;
        while (getline(archivoBackup, linea)) {
            archivoOriginal << linea << "\n";
        }

        archivoBackup.close();
        archivoOriginal.close();
        return true;
    }

    static bool existeArchivo(const string& archivo) {
        ifstream file(archivo);
        bool existe = file.is_open();
        if (existe) file.close();
        return existe;
    }
};