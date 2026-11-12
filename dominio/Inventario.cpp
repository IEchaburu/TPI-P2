

#include <iostream>
#include <cstdlib>
#include "Inventario.h"

using namespace std;

Inventario::Inventario(int capacidad)
{
    _capacidad = (capacidad < 1) ? 1 : capacidad;
    _cantidad = 0;

    /// vector dinamico de punteros: cada lugar apunta a un Item (de cualquier subclase)
    _items = new Item*[_capacidad];

    if(_items == nullptr){
        cout << "No se pudo pedir memoria para el inventario..." << endl;
        exit(-100);
    }

    for(int i = 0; i < _capacidad; i++){
        _items[i] = nullptr;
    }
}

Inventario::~Inventario()
{
    /// primero cada item que quedo guardado, despues el vector de punteros
    for(int i = 0; i < _cantidad; i++){
        delete _items[i];
    }
    delete [] _items;
}

bool Inventario::agregar(Item *item)
{
    if(item == nullptr || _cantidad >= _capacidad){
        return false;
    }

    _items[_cantidad] = item;
    _cantidad++;
    return true;
}

Item *Inventario::quitar(int indice)
{
    if(indice < 0 || indice >= _cantidad){
        return nullptr;
    }

    Item *item = _items[indice];

    /// se corren los de atras un lugar para no dejar huecos
    for(int i = indice; i < _cantidad - 1; i++){
        _items[i] = _items[i + 1];
    }
    _cantidad--;
    _items[_cantidad] = nullptr;

    return item;
}

Item *Inventario::getItem(int indice)
{
    if(indice < 0 || indice >= _cantidad){
        return nullptr;
    }
    return _items[indice];
}

int Inventario::getCantidad(){ return _cantidad; }
int Inventario::getCapacidad(){ return _capacidad; }
