#pragma once
#include <string>

/// Registro de Jugadores.dat
class RegistroJugador
{
public:
    RegistroJugador();

    int getIdJugador();
    void setIdJugador(int idJugador);

    std::string getNombre();
    void setNombre(std::string nombre);

    int getPartidasJugadas();
    void setPartidasJugadas(int partidasJugadas);

    int getMejorPuntaje();
    void setMejorPuntaje(int mejorPuntaje);

    int getNivelMaximoAlcanzado();
    void setNivelMaximoAlcanzado(int nivelMaximoAlcanzado);

    bool getEliminado();
    void setEliminado(bool eliminado);

private:
    int _idJugador;
    char _nombre[20];
    int _partidasJugadas;
    int _mejorPuntaje;
    int _nivelMaximoAlcanzado;
    bool _eliminado;
};
