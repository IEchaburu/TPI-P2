#pragma once
#include <string>
#include "Enumeradores.h"
#include "RegistroItem.h"

class Personaje;   /// declaracion adelantada: Personaje incluye Inventario, que incluye Item

/// Base comun de los objetos. Abstracta. Se construye desde un RegistroItem.
class Item
{
public:
    Item();
    Item(RegistroItem registro);
    virtual ~Item();

    int getIdItem();
    std::string getNombre();
    TipoItem getTipo();
    int getPoder();
    int getIdSprite();

    /// el efecto del item. Arma, Armadura y Reliquia: al recogerlo. Curacion: al usarlo.
    virtual void aplicar(Personaje &personaje) = 0;

    /// true si al recogerlo va al Inventario en vez de aplicarse (solo Curacion)
    virtual bool seGuardaEnInventario();

private:
    int _idItem;
    std::string _nombre;
    TipoItem _tipo;
    int _poder;
    int _idSprite;
};
