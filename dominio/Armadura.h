#pragma once
#include "Item.h"

/// Al recogerlo: defensa += poder (permanente). Ej.: Botas.
class Armadura : public Item
{
public:
    Armadura(RegistroItem registro);

    void aplicar(Personaje &personaje) override;
};
