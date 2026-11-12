#include <cstring>
#include "RegistroNivel.h"

RegistroNivel::RegistroNivel()
{
    _idNivel = 0;
    _nombre[0] = '\0';
    _numeroPiso = 0;
    _tipo = TipoNivel::COMBATE;
    _filas = 0;
    _columnas = 0;
    _cantidadEnemigos = 0;
    _cantidadItems = 0;
    _dificultad = 0;
    _semilla = 0;
    _eliminado = false;
}

int RegistroNivel::getIdNivel(){ return _idNivel; }
void RegistroNivel::setIdNivel(int idNivel){ _idNivel = idNivel; }

std::string RegistroNivel::getNombre(){
    return std::string(_nombre);
}

void RegistroNivel::setNombre(std::string nombre){
    /// se corta en 29 para dejar lugar al '\0'
    if(nombre.size() <= 29){
        strcpy(_nombre, nombre.c_str());
    }
    else{
        strcpy(_nombre, nombre.substr(0, 29).c_str());
    }
}

int RegistroNivel::getNumeroPiso(){ return _numeroPiso; }
void RegistroNivel::setNumeroPiso(int numeroPiso){ _numeroPiso = numeroPiso; }

TipoNivel RegistroNivel::getTipo(){ return _tipo; }
void RegistroNivel::setTipo(TipoNivel tipo){ _tipo = tipo; }

int RegistroNivel::getFilas(){ return _filas; }
void RegistroNivel::setFilas(int filas){ _filas = filas; }

int RegistroNivel::getColumnas(){ return _columnas; }
void RegistroNivel::setColumnas(int columnas){ _columnas = columnas; }

int RegistroNivel::getCantidadEnemigos(){ return _cantidadEnemigos; }
void RegistroNivel::setCantidadEnemigos(int cantidadEnemigos){ _cantidadEnemigos = cantidadEnemigos; }

int RegistroNivel::getCantidadItems(){ return _cantidadItems; }
void RegistroNivel::setCantidadItems(int cantidadItems){ _cantidadItems = cantidadItems; }

int RegistroNivel::getDificultad(){ return _dificultad; }
void RegistroNivel::setDificultad(int dificultad){ _dificultad = dificultad; }

int RegistroNivel::getSemilla(){ return _semilla; }
void RegistroNivel::setSemilla(int semilla){ _semilla = semilla; }

bool RegistroNivel::getEliminado(){ return _eliminado; }
void RegistroNivel::setEliminado(bool eliminado){ _eliminado = eliminado; }
