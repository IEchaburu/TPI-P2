#include <cstring>
#include "RegistroItem.h"

RegistroItem::RegistroItem()
{
    _idItem = 0;
    _nombre[0] = '\0';
    _tipo = TipoItem::ARMA;
    _poder = 0;
    _dificultadMinima = 0;
    _idSprite = 0;
    _eliminado = false;
}

int RegistroItem::getIdItem(){ return _idItem; }
void RegistroItem::setIdItem(int idItem){ _idItem = idItem; }

std::string RegistroItem::getNombre(){
    return std::string(_nombre);
}

void RegistroItem::setNombre(std::string nombre){
    /// se corta en 29 para dejar lugar al '\0'
    if(nombre.size() <= 29){
        strcpy(_nombre, nombre.c_str());
    }
    else{
        strcpy(_nombre, nombre.substr(0, 29).c_str());
    }
}

TipoItem RegistroItem::getTipo(){ return _tipo; }
void RegistroItem::setTipo(TipoItem tipo){ _tipo = tipo; }

int RegistroItem::getPoder(){ return _poder; }
void RegistroItem::setPoder(int poder){ _poder = poder; }

int RegistroItem::getDificultadMinima(){ return _dificultadMinima; }
void RegistroItem::setDificultadMinima(int dificultadMinima){ _dificultadMinima = dificultadMinima; }

int RegistroItem::getIdSprite(){ return _idSprite; }
void RegistroItem::setIdSprite(int idSprite){ _idSprite = idSprite; }

bool RegistroItem::getEliminado(){ return _eliminado; }
void RegistroItem::setEliminado(bool eliminado){ _eliminado = eliminado; }
