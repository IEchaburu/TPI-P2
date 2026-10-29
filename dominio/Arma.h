#pragma once
#include "Item.h"

/// Al recogerlo: ataque += poder (permanente). Ej.: Espada.
class Arma : public Item
{
public:
    Arma(RegistroItem registro);

    void aplicar(Personaje &personaje) override;
};
