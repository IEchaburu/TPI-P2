#pragma once
#include <string>
#include "Enumeradores.h"
#include "RegistroJugador.h"

/// Es la UNICA clase que abre Jugadores.dat (fopen, fread, fwrite, fseek, fclose).
/// Mismo patron que MovimientoArchivo de la catedra (guia de estilo, seccion 7.3).
class ArchivoJugadores
{
public:
    ArchivoJugadores(std::string fileName = "Jugadores.dat");

    ArchivoError guardar(RegistroJugador reg);  /// agrega al final
    RegistroJugador leer(int indice);  /// lee uno (id -1 si fallo)
    ArchivoError leerTodos(RegistroJugador reg[], int cant);  /// lee varios en un vector
    ArchivoError actualizar(int indice, RegistroJugador reg);  /// sobreescribe uno existente
    ArchivoError eliminar(int indice);  /// borrado logico
    int buscarByID(int id);  /// posicion o -1

    int getCantidadRegistros();
    int getNewID();
    std::string getLastError();

private:
    std::string _fileName;
    std::string _errorMessage;
};