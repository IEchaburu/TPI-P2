#pragma once
#include "Enumeradores.h"
#include "Celda.h"
#include "Posicion.h"

/// Grilla dinamica de filas x columnas: un solo new Celda[filas * columnas],
/// la celda (f, c) esta en _celdas[f * _columnas + c]. No copiarlo ni pasarlo por valor.
class Laberinto
{
public:
    Laberinto(int filas, int columnas);
    ~Laberinto();

    int getFilas();
    int getColumnas();

    bool estaDentro(Posicion posicion);
    bool esTransitable(Posicion posicion);        /// dentro de la grilla y la celda es transitable

    TipoCelda getTipoCelda(Posicion posicion);    /// PARED si esta afuera
    void setTipoCelda(Posicion posicion, TipoCelda tipo);

    /// primera celda de ese tipo (ej.: ENTRADA). Posicion(-1, -1) si no hay.
    Posicion buscar(TipoCelda tipo);

private:
    Celda *_celdas;
    int _filas;
    int _columnas;
};
