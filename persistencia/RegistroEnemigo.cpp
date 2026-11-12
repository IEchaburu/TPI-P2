#include <cstring>
#include "RegistroEnemigo.h"

RegistroEnemigo::RegistroEnemigo()
{
    _idEnemigo = 0;
    _nombre[0] = '\0';
    _tipo = TipoEnemigo::COMUN;
    _vidaMaxima = 0;
    _ataque = 0;
    _defensa = 0;
    _experienciaOtorgada = 0;
    _dificultadMinima = 0;
    _dificultadMaxima = 0;
    _idSprite = 0;
    _eliminado = false;
}

int RegistroEnemigo::getIdEnemigo(){ return _idEnemigo; }
void RegistroEnemigo::setIdEnemigo(int idEnemigo){ _idEnemigo = idEnemigo; }

std::string RegistroEnemigo::getNombre(){
    return std::string(_nombre);
}

void RegistroEnemigo::setNombre(std::string nombre){
    /// se corta en 29 para dejar lugar al '\0'
    if(nombre.size() <= 29){
        strcpy(_nombre, nombre.c_str());
    }
    else{
        strcpy(_nombre, nombre.substr(0, 29).c_str());
    }
}

TipoEnemigo RegistroEnemigo::getTipo(){ return _tipo; }
void RegistroEnemigo::setTipo(TipoEnemigo tipo){ _tipo = tipo; }

int RegistroEnemigo::getVidaMaxima(){ return _vidaMaxima; }
void RegistroEnemigo::setVidaMaxima(int vidaMaxima){ _vidaMaxima = vidaMaxima; }

int RegistroEnemigo::getAtaque(){ return _ataque; }
void RegistroEnemigo::setAtaque(int ataque){ _ataque = ataque; }

int RegistroEnemigo::getDefensa(){ return _defensa; }
void RegistroEnemigo::setDefensa(int defensa){ _defensa = defensa; }

int RegistroEnemigo::getExperienciaOtorgada(){ return _experienciaOtorgada; }
void RegistroEnemigo::setExperienciaOtorgada(int experienciaOtorgada){ _experienciaOtorgada = experienciaOtorgada; }

int RegistroEnemigo::getDificultadMinima(){ return _dificultadMinima; }
void RegistroEnemigo::setDificultadMinima(int dificultadMinima){ _dificultadMinima = dificultadMinima; }

int RegistroEnemigo::getDificultadMaxima(){ return _dificultadMaxima; }
void RegistroEnemigo::setDificultadMaxima(int dificultadMaxima){ _dificultadMaxima = dificultadMaxima; }

int RegistroEnemigo::getIdSprite(){ return _idSprite; }
void RegistroEnemigo::setIdSprite(int idSprite){ _idSprite = idSprite; }

bool RegistroEnemigo::getEliminado(){ return _eliminado; }
void RegistroEnemigo::setEliminado(bool eliminado){ _eliminado = eliminado; }
