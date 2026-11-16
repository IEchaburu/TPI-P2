#include "Combate.h"

using namespace std;

Combate::Combate()
{
    _ultimoMensaje = "";
}

bool Combate::atacarEnemigo(Personaje &jugador, Enemigo &enemigo)
{
    int danio = jugador.atacar(enemigo);

    if(enemigo.estaVivo()){
        _ultimoMensaje = jugador.getNombre() + " le hizo " + to_string(danio) + " a " + enemigo.getNombre();
        return false;
    }

    int nivelAntes = jugador.getNivel();
    jugador.ganarExperiencia(enemigo.getExperienciaOtorgada());

    _ultimoMensaje = jugador.getNombre() + " derroto a " + enemigo.getNombre()
                   + " (+" + to_string(enemigo.getExperienciaOtorgada()) + " exp)";
    if(jugador.getNivel() > nivelAntes){
        _ultimoMensaje += ". Nivel " + to_string(jugador.getNivel()) + "!";
    }
    return true;
}

void Combate::atacarJugador(Enemigo &enemigo, Personaje &jugador)
{
    /// enemigo es una referencia: si en realidad es un Minotauro, se ejecuta su atacar
    int danio = enemigo.atacar(jugador);
    _ultimoMensaje = enemigo.getNombre() + " le hizo " + to_string(danio) + " a " + jugador.getNombre();
}

std::string Combate::getUltimoMensaje()
{
    return _ultimoMensaje;
}
