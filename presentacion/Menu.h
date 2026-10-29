#pragma once
#include <string>
#include "Enumeradores.h"
#include "Pantalla.h"
#include "Fuente.h"

/// Menu grafico (principal: Jugar, Administrar, Ranking, Creditos, Salir).
/// Opciones en un vector dinamico, como el Menu de la catedra: no copiarlo.
class Menu
{
public:
    Menu(int cantidadOpciones);
    ~Menu();

    void setOpcion(int indice, std::string texto);   /// indice desde 1

    /// ARRIBA y ABAJO mueven la seleccion. Con ACEPTAR devuelve la opcion elegida (desde 1);
    /// mientras no se elija nada devuelve 0.
    int procesarTecla(Tecla tecla);

    void dibujar(Pantalla &pantalla, Fuente &fuente);

private:
    std::string *_vOpciones;
    int _cantOpciones;
    int _seleccionada;
};
