#pragma once
#include "Entidad.h"
#include "Inventario.h"

/// El jugador (Teseo). Tiene un Inventario con memoria dinamica: no copiarlo ni pasarlo por valor.
class Personaje : public Entidad
{
public:
    Personaje();
    Personaje(std::string nombre, Estadisticas estadisticas, Posicion posicion);

    int getNivel();
    int getExperiencia();

    /// suma experiencia y sube de nivel mientras experiencia >= nivel * 100
    /// (nivel++, vidaMaxima += 10, ataque += 2, defensa += 1)
    void ganarExperiencia(int experiencia);

    Inventario &getInventario();

    /// usa la Curacion de esa posicion del inventario y la libera. false si no hay ninguna.
    bool usarItem(int indice);

    int getIdSprite() override;

private:
    int _nivel;
    int _experiencia;
    Inventario _inventario;       /// composicion: el Personaje TIENE UN Inventario
};
