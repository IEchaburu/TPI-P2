#pragma once
#include "Enumeradores.h"
#include "Pantalla.h"

/// Controlador principal: lleva el EstadoJuego y conecta menu, niveles, combate y dibujado.
class Juego
{
public:
    Juego();

    void ejecutar();

private:
    /// los atributos se destruyen en orden inverso: Pantalla va primero para destruirse ultima
    Pantalla _pantalla;
    EstadoJuego _estado;
};
