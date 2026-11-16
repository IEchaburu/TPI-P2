#include <iostream>
#include "Juego.h"
#include "GeneradorLaberinto.h"
#include "RegistroNivel.h"

using namespace std;

const int ANCHO_VENTANA = 640;
const int ALTO_VENTANA = 480;
const int TAMANIO_CELDA = 32;
const int FILAS_LABERINTO = 13;
const int COLUMNAS_LABERINTO = 19;
const int MARGEN_X = (ANCHO_VENTANA - COLUMNAS_LABERINTO * TAMANIO_CELDA) / 2;
const int MARGEN_Y = 16;

Juego::Juego()
{
    _estado = EstadoJuego::MENU;
}

void Juego::ejecutar()
{
    if(!_pantalla.iniciar("El Laberinto del Minotauro", ANCHO_VENTANA, ALTO_VENTANA)){
        cout << _pantalla.getLastError() << endl;
        return;
    }

    /// DEMO PROVISORIA (adelanto del issue #9): un laberinto generado de verdad, dibujado
    /// con rectangulos, y Teseo moviendose sin atravesar paredes. Enter genera otro y pasa
    /// al tipo de piso siguiente. Se reemplaza en el paso 8 por el juego completo.
    GeneradorLaberinto generador;
    RegistroNivel receta;
    receta.setFilas(FILAS_LABERINTO);
    receta.setColumnas(COLUMNAS_LABERINTO);
    receta.setTipo(TipoNivel::COMBATE);
    receta.setCantidadItems(3);
    receta.setSemilla(0);   /// 0 = al azar: otro laberinto cada vez

    Laberinto *laberinto = new Laberinto(receta.getFilas(), receta.getColumnas());
    generador.generar(*laberinto, receta);
    Posicion teseo = laberinto->buscar(TipoCelda::ENTRADA);

    bool salir = false;
    _estado = EstadoJuego::JUGANDO;

    while(!salir){
        Tecla tecla = _pantalla.leerTecla();

        while(tecla != Tecla::NINGUNA){
            if(tecla == Tecla::CANCELAR || tecla == Tecla::CERRAR){
                salir = true;
            }
            else if(tecla == Tecla::ACEPTAR){
                /// el tipo siguiente: COMBATE -> DESAFIO -> JEFE -> COMBATE
                if(receta.getTipo() == TipoNivel::COMBATE){
                    receta.setTipo(TipoNivel::DESAFIO);
                }
                else if(receta.getTipo() == TipoNivel::DESAFIO){
                    receta.setTipo(TipoNivel::JEFE);
                }
                else{
                    receta.setTipo(TipoNivel::COMBATE);
                }

                /// el laberinto viejo se libera antes de pedir el nuevo
                delete laberinto;
                laberinto = new Laberinto(receta.getFilas(), receta.getColumnas());
                generador.generar(*laberinto, receta);
                teseo = laberinto->buscar(TipoCelda::ENTRADA);
            }
            else{
                Direccion direccion = Direccion::ARRIBA;
                switch(tecla){
                    case Tecla::ABAJO:     direccion = Direccion::ABAJO; break;
                    case Tecla::IZQUIERDA: direccion = Direccion::IZQUIERDA; break;
                    case Tecla::DERECHA:   direccion = Direccion::DERECHA; break;
                    default: break;
                }

                /// se mueve solo si la celda de destino se puede pisar
                Posicion destino = teseo.desplazar(direccion);
                if(laberinto->esTransitable(destino)){
                    teseo = destino;
                }
            }
            tecla = _pantalla.leerTecla();
        }

        _pantalla.limpiar();
        dibujarLaberinto(*laberinto, teseo);
        _pantalla.presentar();
    }

    delete laberinto;
    _estado = EstadoJuego::FIN_PARTIDA;
}

void Juego::dibujarLaberinto(Laberinto &laberinto, Posicion posicionTeseo)
{
    for(int f = 0; f < laberinto.getFilas(); f++){
        for(int c = 0; c < laberinto.getColumnas(); c++){
            int x = MARGEN_X + c * TAMANIO_CELDA;
            int y = MARGEN_Y + f * TAMANIO_CELDA;
            TipoCelda tipo = laberinto.getTipoCelda(Posicion(f, c));

            if(tipo == TipoCelda::PARED){
                /// bloque de piedra: borde oscuro y centro mas claro
                _pantalla.dibujarRectangulo(x, y, TAMANIO_CELDA, TAMANIO_CELDA, 60, 52, 45);
                _pantalla.dibujarRectangulo(x + 2, y + 2, TAMANIO_CELDA - 4, TAMANIO_CELDA - 4, 95, 84, 72);
                continue;
            }

            /// todo lo que no es pared se apoya sobre el piso
            _pantalla.dibujarRectangulo(x, y, TAMANIO_CELDA, TAMANIO_CELDA, 32, 30, 28);

            switch(tipo){
                case TipoCelda::ENTRADA:
                    _pantalla.dibujarRectangulo(x + 6, y + 6, TAMANIO_CELDA - 12, TAMANIO_CELDA - 12, 40, 120, 60);
                    break;
                case TipoCelda::ESCALERA:
                    /// tres escalones
                    for(int i = 0; i < 3; i++){
                        _pantalla.dibujarRectangulo(x + 4 + i * 4, y + 6 + i * 8, TAMANIO_CELDA - 8 - i * 8, 5, 170, 170, 180);
                    }
                    break;
                case TipoCelda::COFRE:
                    _pantalla.dibujarRectangulo(x + 6, y + 9, TAMANIO_CELDA - 12, TAMANIO_CELDA - 16, 120, 70, 30);
                    _pantalla.dibujarRectangulo(x + 6, y + 15, TAMANIO_CELDA - 12, 3, 220, 180, 50);
                    break;
                case TipoCelda::PUERTA:
                    _pantalla.dibujarRectangulo(x + 2, y, TAMANIO_CELDA - 4, TAMANIO_CELDA, 110, 45, 30);
                    _pantalla.dibujarRectangulo(x + 2, y + 13, TAMANIO_CELDA - 4, 5, 60, 25, 15);
                    break;
                default:
                    break;
            }
        }
    }

    /// Teseo: un cuadrado dorado sobre su celda
    int x = MARGEN_X + posicionTeseo.getColumna() * TAMANIO_CELDA;
    int y = MARGEN_Y + posicionTeseo.getFila() * TAMANIO_CELDA;
    _pantalla.dibujarRectangulo(x + 7, y + 7, TAMANIO_CELDA - 14, TAMANIO_CELDA - 14, 230, 180, 40);
}
