#include <cstring>
#include "RegistroJugador.h"

RegistroJugador::RegistroJugador()
{
    _idJugador = 0;
    _nombre[0] = '\0';
    _partidasJugadas = 0;
    _mejorPuntaje = 0;
    _nivelMaximoAlcanzado = 0;
    _eliminado = false;
}

int RegistroJugador::getIdJugador(){ return _idJugador; }
void RegistroJugador::setIdJugador(int idJugador){ _idJugador = idJugador; }

std::string RegistroJugador::getNombre(){
    return std::string(_nombre);
}

void RegistroJugador::setNombre(std::string nombre){
    /// se corta en 19 para dejar lugar al '\0'
    if(nombre.size() <= 19){
        strcpy(_nombre, nombre.c_str());
    }
    else{
        strcpy(_nombre, nombre.substr(0, 19).c_str());
    }
}

int RegistroJugador::getPartidasJugadas(){ return _partidasJugadas; }
void RegistroJugador::setPartidasJugadas(int partidasJugadas){ _partidasJugadas = partidasJugadas; }

int RegistroJugador::getMejorPuntaje(){ return _mejorPuntaje; }
void RegistroJugador::setMejorPuntaje(int mejorPuntaje){ _mejorPuntaje = mejorPuntaje; }

int RegistroJugador::getNivelMaximoAlcanzado(){ return _nivelMaximoAlcanzado; }
void RegistroJugador::setNivelMaximoAlcanzado(int nivelMaximoAlcanzado){ _nivelMaximoAlcanzado = nivelMaximoAlcanzado; }

bool RegistroJugador::getEliminado(){ return _eliminado; }
void RegistroJugador::setEliminado(bool eliminado){ _eliminado = eliminado; }
