#pragma once
#include "Item.h"

/// Al recogerlo: vidaMaxima += poder y vida += poder (permanente). Ej.: Capa.
class Reliquia : public Item
{
public:
    Reliquia(RegistroItem registro);

    void aplicar(Personaje &personaje) override;
};
