#pragma once
#include "Laberinto.h"
#include "RegistroNivel.h"

/// Arma el Laberinto a partir de la receta del RegistroNivel.
class GeneradorLaberinto
{
public:
    /// Pasillos y paredes segun la semilla (misma semilla = mismo laberinto; 0 = al azar).
    /// Ubica la ENTRADA, getCantidadItems() COFRE y la ESCALERA (salvo en el piso JEFE).
    /// En el piso DESAFIO pone una PUERTA que corta el unico camino a la escalera.
    void generar(Laberinto &laberinto, RegistroNivel receta);

private:
    /// backtracker con una pila: entre dos celdas queda un unico camino.
    /// Ubica la ENTRADA, la ESCALERA (lejos por camino y en linea recta) y, si conPuerta,
    /// la PUERTA en el camino hacia la escalera.
    void tallarPasillos(Laberinto &laberinto, bool conPuerta);

    /// piso del jefe: todo abierto salvo el borde
    void armarArena(Laberinto &laberinto);

    /// rompe paredes sueltas entre dos pasillos para que haya mas de un camino
    void abrirAtajos(Laberinto &laberinto, int cantidad);

    /// convierte celdas de PISO elegidas al azar en celdas del tipo indicado
    void ubicarAlAzar(Laberinto &laberinto, TipoCelda tipo, int cantidad);
};
