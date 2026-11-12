#include "Posicion.h"

Posicion::Posicion()
{
    _fila = 0;
    _columna = 0;
}

Posicion::Posicion(int fila, int columna)
{
    setFila(fila);
    setColumna(columna);
}

int Posicion::getFila(){ return _fila; }
void Posicion::setFila(int fila){ _fila = fila; }

int Posicion::getColumna(){ return _columna; }
void Posicion::setColumna(int columna){ _columna = columna; }

Posicion Posicion::desplazar(Direccion direccion)
{
    Posicion vecina(_fila, _columna);

    switch(direccion){
        case Direccion::ARRIBA:    vecina.setFila(_fila - 1); break;
        case Direccion::ABAJO:     vecina.setFila(_fila + 1); break;
        case Direccion::IZQUIERDA: vecina.setColumna(_columna - 1); break;
        case Direccion::DERECHA:   vecina.setColumna(_columna + 1); break;
    }

    return vecina;
}

bool Posicion::esIgual(Posicion otra)
{
    return _fila == otra.getFila() && _columna == otra.getColumna();
}
