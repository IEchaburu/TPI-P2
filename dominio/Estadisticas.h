#pragma once

/// Composicion: toda Entidad TIENE UNAS Estadisticas.
class Estadisticas
{
public:
    Estadisticas();
    Estadisticas(int vidaMaxima, int ataque, int defensa);   /// arranca con vida = vidaMaxima

    int getVida();
    void setVida(int vida);               /// queda entre 0 y vidaMaxima

    int getVidaMaxima();
    void setVidaMaxima(int vidaMaxima);

    int getAtaque();
    void setAtaque(int ataque);

    int getDefensa();
    void setDefensa(int defensa);

private:
    int _vida;
    int _vidaMaxima;
    int _ataque;
    int _defensa;
};
