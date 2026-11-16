#pragma once
#include "Enumeradores.h"
#include "Pantalla.h"
#include "Laberinto.h"
#include "Posicion.h"

/// Controlador principal: lleva el EstadoJuego y conecta menu, niveles, combate y dibujado.
class Juego
{
public:
    Juego();

    void ejecutar();

private:
    /// dibuja cada celda segun su tipo y a Teseo encima (por ahora con rectangulos de colores)
    void dibujarLaberinto(Laberinto &laberinto, Posicion posicionTeseo);

    /// los atributos se destruyen en orden inverso: Pantalla va primero para destruirse ultima
    Pantalla _pantalla;
    EstadoJuego _estado;
};
