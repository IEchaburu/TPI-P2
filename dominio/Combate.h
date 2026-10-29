#pragma once
#include <string>
#include "Personaje.h"
#include "Enemigo.h"

/// Resuelve los ataques entre el jugador y un enemigo. Determinista: sin azar.
class Combate
{
public:
    Combate();

    /// el jugador choca contra un enemigo. Si lo derrota, suma su experiencia.
    /// Devuelve true si el enemigo quedo derrotado.
    bool atacarEnemigo(Personaje &jugador, Enemigo &enemigo);

    /// un enemigo pegado al jugador ataca en el turno de los enemigos
    void atacarJugador(Enemigo &enemigo, Personaje &jugador);

    std::string getUltimoMensaje();   /// para el HUD, ej.: "Teseo hizo 3 de danio"

private:
    std::string _ultimoMensaje;
};
