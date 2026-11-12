#include <cstdlib>
#include "Enemigo.h"

Enemigo::Enemigo() : Entidad()
{
    _idEnemigo = 0;
    _tipo = TipoEnemigo::COMUN;
    _experienciaOtorgada = 0;
    _idSprite = 0;
}

/// se construye desde el registro del archivo: el nombre y las estadisticas pasan a la base
Enemigo::Enemigo(RegistroEnemigo registro, Posicion posicion)
    : Entidad(registro.getNombre(),
              Estadisticas(registro.getVidaMaxima(), registro.getAtaque(), registro.getDefensa()),
              posicion)
{
    _idEnemigo = registro.getIdEnemigo();
    _tipo = registro.getTipo();
    _experienciaOtorgada = registro.getExperienciaOtorgada();
    _idSprite = registro.getIdSprite();
}

int Enemigo::getIdEnemigo(){ return _idEnemigo; }
TipoEnemigo Enemigo::getTipo(){ return _tipo; }
int Enemigo::getExperienciaOtorgada(){ return _experienciaOtorgada; }

Direccion Enemigo::elegirDireccion(Posicion posicionJugador)
{
    int diferenciaFilas = posicionJugador.getFila() - getPosicion().getFila();
    int diferenciaColumnas = posicionJugador.getColumna() - getPosicion().getColumna();

    /// se acerca por el eje en el que esta mas lejos del jugador
    if(abs(diferenciaFilas) >= abs(diferenciaColumnas)){
        return (diferenciaFilas > 0) ? Direccion::ABAJO : Direccion::ARRIBA;
    }
    return (diferenciaColumnas > 0) ? Direccion::DERECHA : Direccion::IZQUIERDA;
}

int Enemigo::getIdSprite()
{
    return _idSprite;
}
