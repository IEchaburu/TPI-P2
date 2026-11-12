#include "Personaje.h"

const int CAPACIDAD_INVENTARIO = 6;
const int ID_SPRITE_TESEO = 1;

/// la lista de inicializacion construye la base (Entidad) y el Inventario con su capacidad
Personaje::Personaje() : Entidad(), _inventario(CAPACIDAD_INVENTARIO)
{
    _nivel = 1;
    _experiencia = 0;
}

Personaje::Personaje(std::string nombre, Estadisticas estadisticas, Posicion posicion)
    : Entidad(nombre, estadisticas, posicion), _inventario(CAPACIDAD_INVENTARIO)
{
    _nivel = 1;
    _experiencia = 0;
}

int Personaje::getNivel(){ return _nivel; }
int Personaje::getExperiencia(){ return _experiencia; }

void Personaje::ganarExperiencia(int experiencia)
{
    if(experiencia <= 0){
        return;
    }

    _experiencia += experiencia;

    /// con mucha experiencia de golpe se puede subir mas de un nivel.
    /// La experiencia usada se descuenta, asi cada nivel cuesta mas que el anterior.
    while(_experiencia >= _nivel * 100){
        _experiencia -= _nivel * 100;
        _nivel++;

        Estadisticas estadisticas = getEstadisticas();
        estadisticas.setVidaMaxima(estadisticas.getVidaMaxima() + 10);
        estadisticas.setVida(estadisticas.getVida() + 10);
        estadisticas.setAtaque(estadisticas.getAtaque() + 2);
        estadisticas.setDefensa(estadisticas.getDefensa() + 1);
        setEstadisticas(estadisticas);
    }
}

Inventario &Personaje::getInventario()
{
    return _inventario;
}

bool Personaje::usarItem(int indice)
{
    Item *item = _inventario.quitar(indice);   /// el item deja de ser del inventario...

    if(item == nullptr){
        return false;
    }

    item->aplicar(*this);   /// polimorfismo: se ejecuta el aplicar de la subclase
    delete item;            /// ...y como ya se uso, lo libera el personaje
    return true;
}

int Personaje::getIdSprite()
{
    return ID_SPRITE_TESEO;
}
