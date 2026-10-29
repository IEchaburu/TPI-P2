#pragma once
#include "Laberinto.h"
#include "RegistroNivel.h"

/// Arma el Laberinto a partir de la receta del RegistroNivel.
class GeneradorLaberinto
{
public:
    /// Pasillos y paredes segun la semilla (misma semilla = mismo laberinto).
    /// Ubica la ENTRADA, getCantidadItems() COFRE y la ESCALERA (salvo en el piso JEFE).
    /// En el piso DESAFIO pone una PUERTA que corta el unico camino a la escalera.
    void generar(Laberinto &laberinto, RegistroNivel receta);
};
