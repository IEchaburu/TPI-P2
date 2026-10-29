#pragma once
#include <string>
#include "Enumeradores.h"
#include "RegistroItem.h"

/// Es la UNICA clase que abre Items.dat (fopen, fread, fwrite, fseek, fclose).
/// Mismo patron que MovimientoArchivo de la catedra (guia de estilo, seccion 7.3).
class ArchivoItems
{
public:
    ArchivoItems(std::string fileName = "Items.dat");

    ArchivoError guardar(RegistroItem reg);  /// agrega al final
    RegistroItem leer(int indice);  /// lee uno (id -1 si fallo)
    ArchivoError leerTodos(RegistroItem reg[], int cant);  /// lee varios en un vector
    ArchivoError actualizar(int indice, RegistroItem reg);  /// sobreescribe uno existente
    ArchivoError eliminar(int indice);  /// borrado logico
    int buscarByID(int id);  /// posicion o -1

    int getCantidadRegistros();
    int getNewID();
    std::string getLastError();

private:
    std::string _fileName;
    std::string _errorMessage;
};