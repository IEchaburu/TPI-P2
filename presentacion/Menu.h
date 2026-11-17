#pragma once
#include <string>
#include "Enumeradores.h"
#include "Pantalla.h"
#include "Fuente.h"

/// Menu grafico: una lista de opciones que se recorre con las flechas y se elige con Enter.
/// Lo usan el menu principal, la pausa y las respuestas del acertijo.
/// Opciones en un vector dinamico, como el Menu de la catedra: no copiarlo.
class Menu
{
public:
    Menu(int cantidadOpciones);
    ~Menu();

    void setOpcion(int indice, std::string texto);   /// indice desde 1

    /// vuelve a marcar la primera opcion (al abrir el menu otra vez)
    void reiniciar();

    /// ARRIBA y ABAJO mueven la seleccion. Con ACEPTAR devuelve la opcion elegida (desde 1);
    /// mientras no se elija nada devuelve 0.
    int procesarTecla(Tecla tecla);

    /// cada opcion centrada en la ventana, una debajo de la otra desde y
    void dibujar(Pantalla &pantalla, Fuente &fuente, int y, int altoLinea);

private:
    std::string *_vOpciones;
    int _cantOpciones;
    int _seleccionada;
};
