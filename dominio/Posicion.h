#pragma once
#include "Enumeradores.h"

/// Una celda de la grilla. Fila 0 arriba, columna 0 a la izquierda.
class Posicion
{
public:
    Posicion();
    Posicion(int fila, int columna);

    int getFila();
    void setFila(int fila);

    int getColumna();
    void setColumna(int columna);

    /// la posicion vecina en esa direccion (no valida contra el laberinto)
    Posicion desplazar(Direccion direccion);

    bool esIgual(Posicion otra);

private:
    int _fila;
    int _columna;
};
