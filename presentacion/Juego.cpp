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

    if(!cargarGraficos()){
        cout << "Hay que ejecutar el juego desde la carpeta del proyecto (TPI-P2)." << endl;
        return;
    }

    /// DEMO PROVISORIA (adelanto del issue #9): un laberinto generado de verdad, dibujado
    /// con sus imagenes, y Teseo moviendose sin atravesar paredes. Enter genera otro y pasa
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

        string tipoPiso = "COMBATE";
        if(receta.getTipo() == TipoNivel::DESAFIO){
            tipoPiso = "DESAFIO";
        }
        else if(receta.getTipo() == TipoNivel::JEFE){
            tipoPiso = "JEFE";
        }
        _fuente.dibujar(_pantalla, "Piso: " + tipoPiso + "   Enter: otro laberinto   Esc: salir",
                        MARGEN_X, MARGEN_Y + FILAS_LABERINTO * TAMANIO_CELDA + 16, 230, 220, 190);

        _pantalla.presentar();
    }

    delete laberinto;
    _estado = EstadoJuego::FIN_PARTIDA;
}

bool Juego::cargarGraficos()
{
    /// en el mismo orden que el enum TipoCelda, asi el tipo sirve de indice
    string nombresCeldas[CANTIDAD_TIPOS_CELDA] = {"pared", "piso", "entrada", "escalera", "cofre", "puerta"};

    for(int i = 0; i < CANTIDAD_TIPOS_CELDA; i++){
        string ruta = "images/celdas/" + nombresCeldas[i] + ".png";
        if(!_spritesCeldas[i].cargar(_pantalla, ruta)){
            cout << "No se pudo cargar " << ruta << endl;
            return false;
        }
    }

    if(!_spriteTeseo.cargar(_pantalla, "images/sprites/1.png")){
        cout << "No se pudo cargar images/sprites/1.png" << endl;
        return false;
    }

    if(!_fuente.abrir("fuentes/PressStart2P-Regular.ttf", 8)){
        cout << "No se pudo abrir fuentes/PressStart2P-Regular.ttf" << endl;
        return false;
    }

    return true;
}

void Juego::dibujarLaberinto(Laberinto &laberinto, Posicion posicionTeseo)
{
    int indicePiso = (int)TipoCelda::PISO;

    for(int f = 0; f < laberinto.getFilas(); f++){
        for(int c = 0; c < laberinto.getColumnas(); c++){
            int x = MARGEN_X + c * TAMANIO_CELDA;
            int y = MARGEN_Y + f * TAMANIO_CELDA;
            TipoCelda tipo = laberinto.getTipoCelda(Posicion(f, c));

            /// todo lo que no es pared se apoya sobre el piso (el cofre tiene fondo transparente)
            if(tipo != TipoCelda::PARED){
                _spritesCeldas[indicePiso].dibujar(_pantalla, x, y);
            }
            if(tipo != TipoCelda::PISO){
                _spritesCeldas[(int)tipo].dibujar(_pantalla, x, y);
            }
        }
    }

    _spriteTeseo.dibujar(_pantalla, MARGEN_X + posicionTeseo.getColumna() * TAMANIO_CELDA,
                         MARGEN_Y + posicionTeseo.getFila() * TAMANIO_CELDA);
}
