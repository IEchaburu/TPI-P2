#include "Item.h"

Item::Item()
{
    _idItem = 0;
    _nombre = "";
    _tipo = TipoItem::ARMA;
    _poder = 0;
    _idSprite = 0;
}

/// se construye desde el registro del archivo
Item::Item(RegistroItem registro)
{
    _idItem = registro.getIdItem();
    _nombre = registro.getNombre();
    _tipo = registro.getTipo();
    _poder = registro.getPoder();
    _idSprite = registro.getIdSprite();
}

/// virtual: al hacer delete de un Item* se destruye la subclase completa
Item::~Item()
{
}

int Item::getIdItem(){ return _idItem; }
std::string Item::getNombre(){ return _nombre; }
TipoItem Item::getTipo(){ return _tipo; }
int Item::getPoder(){ return _poder; }
int Item::getIdSprite(){ return _idSprite; }

bool Item::seGuardaEnInventario()
{
    return false;   /// por defecto se aplica al recogerlo; Curacion lo redefine
}
