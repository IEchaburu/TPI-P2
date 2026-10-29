#pragma once
#include <string>
#include "Enumeradores.h"
#include "RegistroPartida.h"

/// Es la UNICA clase que abre Partidas.dat (fopen, fread, fwrite, fseek, fclose).
/// Mismo patron que MovimientoArchivo de la catedra (guia de estilo, seccion 7.3).
class ArchivoPartidas
{
public:
    ArchivoPartidas(std::string fileName = "Partidas.dat");

    ArchivoError guardar(RegistroPartida reg);  /// agrega al final
    RegistroPartida leer(int indice);  /// lee uno (id -1 si fallo)
    ArchivoError leerTodos(RegistroPartida reg[], int cant);  /// lee varios en un vector
    ArchivoError actualizar(int indice, RegistroPartida reg);  /// sobreescribe uno existente
    int buscarByID(int id);  /// posicion o -1

    int getCantidadRegistros();
    int getNewID();
    std::string getLastError();

private:
    std::string _fileName;
    std::string _errorMessage;
};