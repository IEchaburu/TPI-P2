#include "Curacion.h"
#include "Personaje.h"

Curacion::Curacion(RegistroItem registro) : Item(registro)
{
}

void Curacion::aplicar(Personaje &personaje)
{
    Estadisticas estadisticas = personaje.getEstadisticas();
    estadisticas.setVida(estadisticas.getVida() + getPoder());   /// setVida no pasa la maxima
    personaje.setEstadisticas(estadisticas);
}

bool Curacion::seGuardaEnInventario()
{
    return true;
}
