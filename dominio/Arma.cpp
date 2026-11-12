#include "Arma.h"
#include "Personaje.h"

Arma::Arma(RegistroItem registro) : Item(registro)
{
}

void Arma::aplicar(Personaje &personaje)
{
    Estadisticas estadisticas = personaje.getEstadisticas();
    estadisticas.setAtaque(estadisticas.getAtaque() + getPoder());
    personaje.setEstadisticas(estadisticas);
}
