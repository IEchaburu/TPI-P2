#include "Armadura.h"
#include "Personaje.h"

Armadura::Armadura(RegistroItem registro) : Item(registro)
{
}

void Armadura::aplicar(Personaje &personaje)
{
    Estadisticas estadisticas = personaje.getEstadisticas();
    estadisticas.setDefensa(estadisticas.getDefensa() + getPoder());
    personaje.setEstadisticas(estadisticas);
}
