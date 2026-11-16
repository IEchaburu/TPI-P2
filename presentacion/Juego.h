#pragma once
#include "Enumeradores.h"
#include "Pantalla.h"
#include "Sprite.h"
#include "Fuente.h"
#include "Laberinto.h"
#include "Posicion.h"

const int CANTIDAD_TIPOS_CELDA = 6;   /// PARED, PISO, ENTRADA, ESCALERA, COFRE, PUERTA

/// Controlador principal: lleva el EstadoJuego y conecta menu, niveles, combate y dibujado.
class Juego
{
public:
    Juego();

    void ejecutar();

private:
    /// carga las imagenes de las celdas, la de Teseo y la fuente. false si falta alguna.
    bool cargarGraficos();

    /// dibuja cada celda con su imagen y a Teseo encima
    void dibujarLaberinto(Laberinto &laberinto, Posicion posicionTeseo);

    /// los atributos se destruyen en orden inverso: Pantalla va primero para destruirse
    /// ultima, despues de los Sprite y la Fuente que dependen de ella
    Pantalla _pantalla;
    Sprite _spritesCeldas[CANTIDAD_TIPOS_CELDA];   /// en el orden del enum TipoCelda
    Sprite _spriteTeseo;
    Fuente _fuente;
    EstadoJuego _estado;
};
