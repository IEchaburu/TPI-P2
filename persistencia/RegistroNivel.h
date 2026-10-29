#pragma once
#include <string>
#include "Enumeradores.h"

/// Registro de Niveles.dat: la RECETA de un piso, no su layout (lo arma GeneradorLaberinto)
class RegistroNivel
{
public:
    RegistroNivel();

    int getIdNivel();
    void setIdNivel(int idNivel);

    std::string getNombre();
    void setNombre(std::string nombre);

    int getNumeroPiso();
    void setNumeroPiso(int numeroPiso);

    TipoNivel getTipo();
    void setTipo(TipoNivel tipo);

    int getFilas();
    void setFilas(int filas);

    int getColumnas();
    void setColumnas(int columnas);

    int getCantidadEnemigos();
    void setCantidadEnemigos(int cantidadEnemigos);

    int getCantidadItems();               /// = cantidad de cofres del piso
    void setCantidadItems(int cantidadItems);

    int getDificultad();
    void setDificultad(int dificultad);

    int getSemilla();
    void setSemilla(int semilla);

    bool getEliminado();
    void setEliminado(bool eliminado);

private:
    int _idNivel;
    char _nombre[30];
    int _numeroPiso;
    TipoNivel _tipo;
    int _filas;
    int _columnas;
    int _cantidadEnemigos;
    int _cantidadItems;
    int _dificultad;
    int _semilla;
    bool _eliminado;
};
