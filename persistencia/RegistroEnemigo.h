#pragma once
#include <string>
#include "Enumeradores.h"

/// Registro de Enemigos.dat. Plano: sin metodos virtuales, sin punteros, sin std::string adentro.
class RegistroEnemigo
{
public:
    RegistroEnemigo();

    int getIdEnemigo();
    void setIdEnemigo(int idEnemigo);

    std::string getNombre();
    void setNombre(std::string nombre);

    TipoEnemigo getTipo();
    void setTipo(TipoEnemigo tipo);

    int getVidaMaxima();
    void setVidaMaxima(int vidaMaxima);

    int getAtaque();
    void setAtaque(int ataque);

    int getDefensa();
    void setDefensa(int defensa);

    int getExperienciaOtorgada();
    void setExperienciaOtorgada(int experienciaOtorgada);

    int getDificultadMinima();
    void setDificultadMinima(int dificultadMinima);

    int getDificultadMaxima();
    void setDificultadMaxima(int dificultadMaxima);

    int getIdSprite();
    void setIdSprite(int idSprite);

    bool getEliminado();
    void setEliminado(bool eliminado);

private:
    int _idEnemigo;
    char _nombre[30];
    TipoEnemigo _tipo;
    int _vidaMaxima;
    int _ataque;
    int _defensa;
    int _experienciaOtorgada;
    int _dificultadMinima;
    int _dificultadMaxima;
    int _idSprite;
    bool _eliminado;
};
