#pragma once
#include <string>
#include "Enumeradores.h"
#include "RegistroNivel.h"

/// Es la UNICA clase que abre Niveles.dat (fopen, fread, fwrite, fseek, fclose).
/// Mismo patron que MovimientoArchivo de la catedra (guia de estilo, seccion 7.3).
class ArchivoNiveles
{
public:
    ArchivoNiveles(std::string fileName = "Niveles.dat");

    ArchivoError guardar(RegistroNivel reg);  /// agrega al final
    RegistroNivel leer(int indice);  /// lee uno (id -1 si fallo)
    ArchivoError leerTodos(RegistroNivel reg[], int cant);  /// lee varios en un vector
    ArchivoError actualizar(int indice, RegistroNivel reg);  /// sobreescribe uno existente
    ArchivoError eliminar(int indice);  /// borrado logico
    int buscarByID(int id);  /// posicion o -1

    int getCantidadRegistros();
    int getNewID();
    std::string getLastError();

private:
    std::string _fileName;
    std::string _errorMessage;
};