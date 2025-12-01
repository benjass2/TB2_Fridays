#pragma once
#include "Funcionalidades.h"
using namespace UtilidadesConsola;

class Cliente {
private:
    int id;
    string nombre;
    string apellido;
    string telefono;
    string dni;
    string direccion;
    string categoria;
    int puntosLealtad;

public:

    Cliente() : id(0), nombre(""), apellido(""), telefono(""), dni(""), direccion(""), categoria("Regular"), puntosLealtad(0) {}


    Cliente(int id, string nombre, string apellido, string telefono, string dni, string direccion = "", string categoria = "Regular", int puntos = 0)
        : id(id), nombre(nombre), apellido(apellido), telefono(telefono), dni(dni), direccion(direccion), categoria(categoria), puntosLealtad(puntos) {
    }


    ~Cliente() {}


    int getId() const { return id; }
    string getNombre() const { return nombre; }
    string getApellido() const { return apellido; }
    string getNombreCompleto() const { return nombre + " " + apellido; }
    string getTelefono() const { return telefono; }
    string getDni() const { return dni; }
    string getDireccion() const { return direccion; }
    string getCategoria() const { return categoria; }
    int getPuntosLealtad() const { return puntosLealtad; }
    bool esClientePreferente() const { return categoria == "VIP" || categoria == "Premium"; }


    void setId(int id) { this->id = id; }
    void setNombre(const string& nombre) { this->nombre = nombre; }
    void setApellido(const string& apellido) { this->apellido = apellido; }
    void setTelefono(const string& telefono) { this->telefono = telefono; }
    void setDni(const string& dni) { this->dni = dni; }
    void setDireccion(const string& direccion) { this->direccion = direccion; }
    void setCategoria(const string& cat) { categoria = cat; }
    void setPuntosLealtad(int puntos) { puntosLealtad = puntos; }



    void agregarPuntos(int puntos) {
        puntosLealtad += puntos;
    }

    void actualizarCategoria() {
        if (puntosLealtad >= 200) {
            categoria = "VIP";
        }
        else if (puntosLealtad >= 100) {
            categoria = "Premium";
        }
        else {
            categoria = "Regular";
        }
    }


    // Mostrar información del cliente
    void mostrarInfo() const {

        cout << "DATOS DEL CLIENTE" << endl;
        imprimirSeparadorRojoBlanco(30);
        cout << "ID: " << id << endl;
        cout << "Nombre: " << getNombreCompleto() << endl;
        cout << "DNI: " << dni << endl;
        cout << "Telefono: " << telefono << endl;
        if (!direccion.empty()) {
            cout << "Direccion: " << direccion << endl;
        }
        cout << "Categoria: " << categoria << endl;
        cout << "Puntos de lealtad: " << puntosLealtad << endl;
        imprimirSeparadorRojoBlanco(30);
    }


    bool datosCompletos() const {
        return !nombre.empty() && !apellido.empty() && !telefono.empty() && !dni.empty();
    }


    bool operator==(const Cliente& otro) const {
        return dni == otro.dni;
    }

    bool operator!=(const Cliente& otro) const {
        return dni != otro.dni;
    }
};