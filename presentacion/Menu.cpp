#include <iostream>
#include <cstdlib>
#include "Menu.h"

using namespace std;

Menu::Menu(int cantidadOpciones)
{
    _cantOpciones = (cantidadOpciones < 1) ? 1 : cantidadOpciones;
    _seleccionada = 1;

    _vOpciones = new string[_cantOpciones];

    if(_vOpciones == nullptr){
        cout << "No se pudo pedir memoria para el menu..." << endl;
        exit(-100);
    }
}

Menu::~Menu()
{
    delete [] _vOpciones;
}

void Menu::setOpcion(int indice, std::string texto)
{
    /// el indice es desde 1, el vector desde 0
    if(indice >= 1 && indice <= _cantOpciones){
        _vOpciones[indice - 1] = texto;
    }
}

void Menu::reiniciar()
{
    _seleccionada = 1;
}

int Menu::procesarTecla(Tecla tecla)
{
    switch(tecla){
        case Tecla::ARRIBA:
            /// de la primera se pasa a la ultima
            _seleccionada--;
            if(_seleccionada < 1){
                _seleccionada = _cantOpciones;
            }
            break;
        case Tecla::ABAJO:
            /// de la ultima se vuelve a la primera
            _seleccionada++;
            if(_seleccionada > _cantOpciones){
                _seleccionada = 1;
            }
            break;
        case Tecla::ACEPTAR:
            return _seleccionada;
        default:
            break;
    }
    return 0;
}

void Menu::dibujar(Pantalla &pantalla, Fuente &fuente, int y, int altoLinea)
{
    for(int i = 0; i < _cantOpciones; i++){
        int ancho = fuente.medirAncho(_vOpciones[i]);
        int x = (pantalla.getAncho() - ancho) / 2;
        int yOpcion = y + i * altoLinea;

        if(i + 1 == _seleccionada){
            /// la elegida, en dorado y con una flecha a la izquierda
            fuente.dibujar(pantalla, ">", x - fuente.medirAncho("> "), yOpcion, 240, 200, 90);
            fuente.dibujar(pantalla, _vOpciones[i], x, yOpcion, 240, 200, 90);
        }
        else{
            fuente.dibujar(pantalla, _vOpciones[i], x, yOpcion, 150, 140, 130);
        }
    }
}
