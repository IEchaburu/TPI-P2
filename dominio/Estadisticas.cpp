#include "Estadisticas.h"

Estadisticas::Estadisticas()
{
    _vida = 0;
    _vidaMaxima = 0;
    _ataque = 0;
    _defensa = 0;
}

Estadisticas::Estadisticas(int vidaMaxima, int ataque, int defensa)
{
    _vida = 0;                  /// setVidaMaxima la compara: tiene que tener un valor antes
    setVidaMaxima(vidaMaxima);
    setAtaque(ataque);
    setDefensa(defensa);
    setVida(vidaMaxima);        /// se arranca con la vida completa
}

int Estadisticas::getVida(){ return _vida; }

void Estadisticas::setVida(int vida)
{
    /// la vida nunca baja de 0 ni pasa la maxima
    if(vida < 0){
        _vida = 0;
    }
    else if(vida > _vidaMaxima){
        _vida = _vidaMaxima;
    }
    else{
        _vida = vida;
    }
}

int Estadisticas::getVidaMaxima(){ return _vidaMaxima; }

void Estadisticas::setVidaMaxima(int vidaMaxima)
{
    if(vidaMaxima < 0){
        vidaMaxima = 0;
    }
    _vidaMaxima = vidaMaxima;

    /// si la maxima baja, la vida actual no puede quedar por encima
    if(_vida > _vidaMaxima){
        _vida = _vidaMaxima;
    }
}

int Estadisticas::getAtaque(){ return _ataque; }

void Estadisticas::setAtaque(int ataque)
{
    _ataque = (ataque < 0) ? 0 : ataque;
}

int Estadisticas::getDefensa(){ return _defensa; }

void Estadisticas::setDefensa(int defensa)
{
    _defensa = (defensa < 0) ? 0 : defensa;
}
