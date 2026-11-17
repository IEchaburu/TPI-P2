#pragma once
#include <ctime>
#include <string>
#include "Enumeradores.h"
#include "Pantalla.h"
#include "Sprite.h"
#include "Fuente.h"
#include "Menu.h"
#include "Validador.h"
#include "Combate.h"
#include "Personaje.h"
#include "Nivel.h"
#include "Item.h"
#include "RegistroJugador.h"
#include "RegistroNivel.h"
#include "RegistroPartida.h"

const int CANTIDAD_TIPOS_CELDA = 6;   /// PARED, PISO, ENTRADA, ESCALERA, COFRE, PUERTA
const int CANTIDAD_SPRITES = 100;     /// images/sprites/<id>.png, con id de 0 a 99
const int CANTIDAD_RANKING = 10;      /// las mejores partidas que se muestran

/// Controlador principal: lleva el EstadoJuego y conecta menu, niveles, combate y dibujado.
/// Cada vuelta del bucle: leer teclas -> procesarlas segun el estado -> dibujar el estado.
/// La partida en curso (Teseo y el piso) es memoria dinamica: no copiar un Juego.
class Juego
{
public:
    Juego();
    ~Juego();     /// libera la partida si quedo una a medias

    void ejecutar();

private:
    bool cargarGraficos();

    /// teclas: una funcion por estado
    void procesarTecla(Tecla tecla);
    void procesarMenu(Tecla tecla);
    void procesarJugando(Tecla tecla);
    void procesarAcertijo(Tecla tecla);
    void procesarPausa(Tecla tecla);
    void procesarVolverAlMenu(Tecla tecla);   /// fin de partida, ranking y creditos

    void administrar();                       /// el ABM por consola, con la ventana oculta

    /// lee Partidas.dat, las ordena por puntaje y se queda con las mejores (con el nombre
    /// de cada jugador, de Jugadores.dat). Se llama al entrar al ranking, no en cada dibujo.
    void cargarRanking();

    /// partida
    bool elegirPerfil();                      /// por consola, con la ventana oculta
    bool iniciarPartida();
    bool buscarPiso(int pisoAnterior, RegistroNivel &receta);   /// el siguiente de Niveles.dat
    bool prepararPiso(RegistroNivel receta);
    void moverTeseo(Direccion direccion);     /// mover, atacar, abrir cofre o bajar la escalera
    void recoger(Item *item);                 /// queda a cargo del inventario o se libera
    void usarCuracion();
    void abrirAcertijo();
    void turnoEnemigos();
    void bajarPiso();
    bool esPisoJefeSinJefe();                 /// arena sin jefe: no hay como salir (se gana)
    void terminarPartida(ResultadoPartida resultado);
    bool guardarPartida();                    /// alta en Partidas.dat y perfil actualizado
    void liberarPartida();

    /// dibujo: una funcion por pantalla
    void dibujar();
    void dibujarMenu();
    void dibujarPartida();                    /// laberinto + enemigos + Teseo + HUD
    void dibujarLaberinto();
    void dibujarEntidades();
    void dibujarHud();
    void dibujarAcertijo();
    void dibujarPausa();
    void dibujarFinPartida();
    void dibujarRanking();
    void dibujarCreditos();

    /// ayudas de dibujo
    void dibujarSprite(int idSprite, int x, int y);
    void dibujarBarra(int x, int y, int ancho, int valor, int maximo, int rojo, int verde, int azul);
    void dibujarRecuadro(int x, int y, int ancho, int alto);
    void dibujarCentrado(Fuente &fuente, std::string texto, int y, int rojo, int verde, int azul);
    int dibujarParrafo(std::string texto, int x, int y, int anchoMaximo);   /// devuelve la y siguiente
    int getMargenX();                         /// para centrar el laberinto del piso

    /// los atributos se destruyen en orden inverso: Pantalla va primero para destruirse
    /// ultima, despues de los Sprite y las Fuente que dependen de ella
    Pantalla _pantalla;
    Sprite _spritesCeldas[CANTIDAD_TIPOS_CELDA];   /// en el orden del enum TipoCelda
    Sprite _sprites[CANTIDAD_SPRITES];             /// personajes e items, por idSprite
    Fuente _fuente;                                /// chica: HUD y textos largos
    Fuente _fuenteGrande;                          /// titulos y menus

    Menu _menuPrincipal;
    Menu _menuPausa;
    Menu _menuAcertijo;
    Validador _validador;
    Combate _combate;
    EstadoJuego _estado;
    bool _salir;
    std::string _aviso;                            /// un error para mostrar en el menu

    /// la partida en curso
    RegistroJugador _jugador;
    int _posicionJugador;                          /// su posicion en Jugadores.dat
    Personaje *_teseo;                             /// new al empezar, delete al terminar
    Nivel *_nivel;                                 /// new por piso, delete al bajar la escalera
    bool _respondiendoAcertijo;
    int _pisoAlcanzado;
    int _enemigosDerrotados;
    int _experienciaGanada;
    time_t _horaInicio;
    std::string _mensaje;                          /// lo que hizo Teseo este turno
    std::string _mensajeEnemigos;                  /// lo que le hicieron los enemigos

    /// la ultima partida terminada (pantalla final)
    ResultadoPartida _resultado;
    int _puntaje;
    int _duracionSegundos;
    bool _esMejorPuntaje;
    bool _guardadoOk;

    /// el ranking (lo llena cargarRanking): vectores estaticos de tamanio constante
    RegistroPartida _ranking[CANTIDAD_RANKING];
    std::string _nombresRanking[CANTIDAD_RANKING]; /// el nombre del jugador de cada partida
    int _cantidadRanking;                          /// cuantas hay (puede haber menos de 10)
};
