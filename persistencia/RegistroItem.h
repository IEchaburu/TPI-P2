#pragma once
#include <string>
#include "Enumeradores.h"

/// Registro de Items.dat
class RegistroItem
{
public:
    RegistroItem();

    int getIdItem();
    void setIdItem(int idItem);

    std::string getNombre();
    void setNombre(std::string nombre);

    TipoItem getTipo();
    void setTipo(TipoItem tipo);

    int getPoder();
    void setPoder(int poder);

    int getDificultadMinima();
    void setDificultadMinima(int dificultadMinima);

    int getIdSprite();
    void setIdSprite(int idSprite);

    bool getEliminado();
    void setEliminado(bool eliminado);

private:
    int _idItem;
    char _nombre[30];
    TipoItem _tipo;
    int _poder;
    int _dificultadMinima;
    int _idSprite;
    bool _eliminado;
};
