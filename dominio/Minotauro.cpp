#include "Minotauro.h"

const int TURNOS_ENTRE_EMBESTIDAS = 3;

Minotauro::Minotauro(RegistroEnemigo registro, Posicion posicion) : Enemigo(registro, posicion)
{
    _turnosDesdeEmbestida = 0;
}

int Minotauro::atacar(Entidad &objetivo)
{
    /// primero el ataque comun, el mismo que el de cualquier entidad
    int danio = Entidad::atacar(objetivo);

    /// cada 3 ataques, embestida: el mismo golpe se aplica dos veces
    _turnosDesdeEmbestida++;
    if(_turnosDesdeEmbestida == TURNOS_ENTRE_EMBESTIDAS){
        objetivo.recibirDanio(danio);
        danio = danio * 2;
        _turnosDesdeEmbestida = 0;
    }

    return danio;
}
