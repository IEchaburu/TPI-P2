#include "Reliquia.h"
#include "Personaje.h"

Reliquia::Reliquia(RegistroItem registro) : Item(registro)
{
}

void Reliquia::aplicar(Personaje &personaje)
{
    Estadisticas estadisticas = personaje.getEstadisticas();
    /// primero sube la maxima: si no, setVida cortaria la vida en la maxima vieja
    estadisticas.setVidaMaxima(estadisticas.getVidaMaxima() + getPoder());
    estadisticas.setVida(estadisticas.getVida() + getPoder());
    personaje.setEstadisticas(estadisticas);
}
