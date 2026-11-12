#include "Entidad.h"

Entidad::Entidad()
{
    _nombre = "";
}

Entidad::Entidad(std::string nombre, Estadisticas estadisticas, Posicion posicion)
{
    setNombre(nombre);
    setEstadisticas(estadisticas);
    setPosicion(posicion);
}

Entidad::~Entidad()
{
}

std::string Entidad::getNombre(){ return _nombre; }
void Entidad::setNombre(std::string nombre){ _nombre = nombre; }

Estadisticas Entidad::getEstadisticas(){ return _estadisticas; }
void Entidad::setEstadisticas(Estadisticas estadisticas){ _estadisticas = estadisticas; }

Posicion Entidad::getPosicion(){ return _posicion; }
void Entidad::setPosicion(Posicion posicion){ _posicion = posicion; }

bool Entidad::estaVivo()
{
    return _estadisticas.getVida() > 0;
}

void Entidad::recibirDanio(int danio)
{
    if(danio < 0){
        danio = 0;
    }
    _estadisticas.setVida(_estadisticas.getVida() - danio);   /// setVida no la deja bajar de 0
}

int Entidad::atacar(Entidad &objetivo)
{
    int danio = _estadisticas.getAtaque() - objetivo.getEstadisticas().getDefensa();

    /// max(1, ...): si la defensa supera al ataque, igual pega 1. Sin esto habria
    /// enemigos invulnerables y el combate quedaria trabado.
    if(danio < 1){
        danio = 1;
    }

    objetivo.recibirDanio(danio);
    return danio;
}
