#pragma once
#include "Enemigo.h"

/// Jefe final: el enemigo con tipo JEFE. Redefine el ataque.
class Minotauro : public Enemigo
{
public:
    Minotauro(RegistroEnemigo registro, Posicion posicion);

    /// ataque propio del jefe (por ejemplo, una embestida con mas danio cada algunos turnos)
    int atacar(Entidad &objetivo) override;

private:
    int _turnosDesdeEmbestida;
};
