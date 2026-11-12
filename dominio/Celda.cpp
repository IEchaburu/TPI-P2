#include "Celda.h"

Celda::Celda()
{
    _tipo = TipoCelda::PARED;
}

Celda::Celda(TipoCelda tipo)
{
    setTipo(tipo);
}

TipoCelda Celda::getTipo(){ return _tipo; }
void Celda::setTipo(TipoCelda tipo){ _tipo = tipo; }

bool Celda::esTransitable()
{
    return _tipo != TipoCelda::PARED && _tipo != TipoCelda::PUERTA;
}
