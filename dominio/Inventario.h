#pragma once
#include "Item.h"

/// Arreglo dinamico de Item*. El Inventario es duenio de los items que contiene:
/// los libera en el destructor. No copiarlo ni pasarlo por valor.
class Inventario
{
public:
    Inventario(int capacidad = 10);
    ~Inventario();

    bool agregar(Item *item);     /// false si esta lleno (el item sigue a cargo de quien llamo)
    Item *quitar(int indice);     /// lo saca sin liberarlo: pasa a cargo de quien llamo
    Item *getItem(int indice);    /// nullptr si el indice no existe
    int getCantidad();
    int getCapacidad();

private:
    Item **_items;
    int _capacidad;
    int _cantidad;
};
