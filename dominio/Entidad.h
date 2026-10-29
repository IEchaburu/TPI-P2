#pragma once
#include <string>
#include "Estadisticas.h"
#include "Posicion.h"

/// Base comun de Personaje y Enemigo. Abstracta: no se instancia directamente.
/// Tiene metodos virtuales, asi que NUNCA se graba en disco: se construye desde un Registro*.
class Entidad
{
public:
    Entidad();
    Entidad(std::string nombre, Estadisticas estadisticas, Posicion posicion);
    virtual ~Entidad();

    std::string getNombre();
    void setNombre(std::string nombre);

    Estadisticas getEstadisticas();
    void setEstadisticas(Estadisticas estadisticas);

    Posicion getPosicion();
    void setPosicion(Posicion posicion);

    bool estaVivo();
    void recibirDanio(int danio);

    /// danio = max(1, ataque propio - defensa del objetivo).
    /// Se lo aplica al objetivo y lo devuelve. Minotauro lo redefine.
    virtual int atacar(Entidad &objetivo);

    /// cada clase concreta sabe con que imagen se dibuja
    virtual int getIdSprite() = 0;

private:
    std::string _nombre;          /// no se graba: puede ser std::string
    Estadisticas _estadisticas;
    Posicion _posicion;
};
