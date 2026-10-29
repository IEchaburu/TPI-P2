#include <iostream>
#include "Juego.h"

using namespace std;

Juego::Juego()
{
    _estado = EstadoJuego::MENU;
}

void Juego::ejecutar()
{
    const int ANCHO_VENTANA = 640;
    const int ALTO_VENTANA = 480;
    const int TAMANIO_CELDA = 32;

    if(!_pantalla.iniciar("El Laberinto del Minotauro", ANCHO_VENTANA, ALTO_VENTANA)){
        cout << _pantalla.getLastError() << endl;
        return;
    }

    /// DEMO PROVISORIA: Teseo es un cuadrado que se mueve de a una celda.
    /// Se reemplaza cuando esten Laberinto, Personaje, Sprite y Fuente.
    const int FILAS = ALTO_VENTANA / TAMANIO_CELDA;
    const int COLUMNAS = ANCHO_VENTANA / TAMANIO_CELDA;
    int fila = 0;
    int columna = 0;
    bool salir = false;

    _estado = EstadoJuego::JUGANDO;

    while(!salir){
        Tecla tecla = _pantalla.leerTecla();

        while(tecla != Tecla::NINGUNA){
            switch(tecla){
                case Tecla::ARRIBA:    if(fila > 0) fila--; break;
                case Tecla::ABAJO:     if(fila < FILAS - 1) fila++; break;
                case Tecla::IZQUIERDA: if(columna > 0) columna--; break;
                case Tecla::DERECHA:   if(columna < COLUMNAS - 1) columna++; break;
                case Tecla::CANCELAR:
                case Tecla::CERRAR:    salir = true; break;
                default: break;
            }
            tecla = _pantalla.leerTecla();
        }

        _pantalla.limpiar();

        for(int f = 0; f < FILAS; f++){
            for(int c = 0; c < COLUMNAS; c++){
                _pantalla.dibujarRectangulo(c * TAMANIO_CELDA + 1, f * TAMANIO_CELDA + 1,
                                            TAMANIO_CELDA - 2, TAMANIO_CELDA - 2, 40, 40, 48);
            }
        }
        _pantalla.dibujarRectangulo(columna * TAMANIO_CELDA + 4, fila * TAMANIO_CELDA + 4,
                                    TAMANIO_CELDA - 8, TAMANIO_CELDA - 8, 230, 180, 40);

        _pantalla.presentar();
    }

    _estado = EstadoJuego::FIN_PARTIDA;
}
