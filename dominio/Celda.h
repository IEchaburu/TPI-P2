#pragma once
#include "Enumeradores.h"

class Celda
{
public:
    Celda();                      /// PARED
    Celda(TipoCelda tipo);

    TipoCelda getTipo();
    void setTipo(TipoCelda tipo);

    bool esTransitable();         /// false para PARED y PUERTA (cerrada)

private:
    TipoCelda _tipo;
};
