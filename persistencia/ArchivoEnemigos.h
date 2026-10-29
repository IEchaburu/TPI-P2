#pragma once
#include <string>
#include "Enumeradores.h"
#include "RegistroEnemigo.h"

/// Es la UNICA clase que abre Enemigos.dat (fopen, fread, fwrite, fseek, fclose).
/// Mismo patron que MovimientoArchivo de la catedra (guia de estilo, seccion 7.3).
class ArchivoEnemigos
{
public:
    ArchivoEnemigos(std::string fileName = "Enemigos.dat");

    ArchivoError guardar(RegistroEnemigo reg);  /// agrega al final
    RegistroEnemigo leer(int indice);  /// lee uno (id -1 si fallo)
    ArchivoError leerTodos(RegistroEnemigo reg[], int cant);  /// lee varios en un vector
    ArchivoError actualizar(int indice, RegistroEnemigo reg);  /// sobreescribe uno existente
    ArchivoError eliminar(int indice);  /// borrado logico
    int buscarByID(int id);  /// posicion o -1

    int getCantidadRegistros();
    int getNewID();
    std::string getLastError();

private:
    std::string _fileName;
    std::string _errorMessage;
};