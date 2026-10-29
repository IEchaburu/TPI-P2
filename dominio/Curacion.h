#pragma once
#include "Item.h"

/// Va al Inventario. Al usarla: vida = min(vidaMaxima, vida + poder). Ej.: Carne de jabali.
class Curacion : public Item
{
public:
    Curacion(RegistroItem registro);

    void aplicar(Personaje &personaje) override;
    bool seGuardaEnInventario() override;   /// true
};
