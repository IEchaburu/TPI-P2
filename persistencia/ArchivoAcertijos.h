#pragma once
#include <string>
#include "Enumeradores.h"
#include "RegistroAcertijo.h"

/// Es la UNICA clase que abre Acertijos.dat (fopen, fread, fwrite, fseek, fclose).
/// Mismo patron que MovimientoArchivo de la catedra (guia de estilo, seccion 7.3).
class ArchivoAcertijos
{
public:
    ArchivoAcertijos(std::string fileName = "Acertijos.dat");

    ArchivoError guardar(RegistroAcertijo reg);  /// agrega al final
    RegistroAcertijo leer(int indice);  /// lee uno (id -1 si fallo)
    ArchivoError leerTodos(RegistroAcertijo reg[], int cant);  /// lee varios en un vector
    ArchivoError actualizar(int indice, RegistroAcertijo reg);  /// sobreescribe uno existente
    ArchivoError eliminar(int indice);  /// borrado logico
    int buscarByID(int id);  /// posicion o -1

    int getCantidadRegistros();
    int getNewID();
    std::string getLastError();

private:
    std::string _fileName;
    std::string _errorMessage;
};