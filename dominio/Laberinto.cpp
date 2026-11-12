#include <iostream>
#include <cstdlib>
#include "Laberinto.h"

using namespace std;

Laberinto::Laberinto(int filas, int columnas)
{
    /// una grilla tiene al menos una celda
    _filas = (filas < 1) ? 1 : filas;
    _columnas = (columnas < 1) ? 1 : columnas;

    /// un solo vector para toda la grilla; todas las celdas arrancan como PARED
    _celdas = new Celda[_filas * _columnas];

    if(_celdas == nullptr){
        cout << "No se pudo pedir memoria para el laberinto..." << endl;
        exit(-100);
    }
}

Laberinto::~Laberinto()
{
    delete [] _celdas;
}

int Laberinto::getFilas(){ return _filas; }
int Laberinto::getColumnas(){ return _columnas; }

int Laberinto::getIndice(Posicion posicion)
{
    return posicion.getFila() * _columnas + posicion.getColumna();
}

bool Laberinto::estaDentro(Posicion posicion)
{
    return posicion.getFila() >= 0 && posicion.getFila() < _filas
        && posicion.getColumna() >= 0 && posicion.getColumna() < _columnas;
}

bool Laberinto::esTransitable(Posicion posicion)
{
    if(!estaDentro(posicion)){
        return false;
    }
    return _celdas[getIndice(posicion)].esTransitable();
}

TipoCelda Laberinto::getTipoCelda(Posicion posicion)
{
    /// fuera de la grilla se comporta como una pared
    if(!estaDentro(posicion)){
        return TipoCelda::PARED;
    }
    return _celdas[getIndice(posicion)].getTipo();
    
}

void Laberinto::setTipoCelda(Posicion posicion, TipoCelda tipo)
{
    if(estaDentro(posicion)){
        _celdas[getIndice(posicion)].setTipo(tipo);
    }
}

Posicion Laberinto::buscar(TipoCelda tipo)
{
    for(int f = 0; f < _filas; f++){
        for(int c = 0; c < _columnas; c++){
            Posicion posicion(f, c);
            if(_celdas[getIndice(posicion)].getTipo() == tipo){
                return posicion;
            }
        }
    }
    return Posicion(-1, -1);
}
