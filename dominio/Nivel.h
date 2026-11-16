#pragma once
#include <string>
#include "Enumeradores.h"
#include "Laberinto.h"
#include "Enemigo.h"
#include "Item.h"
#include "Desafio.h"
#include "RegistroNivel.h"

/// Un piso en juego. Compone su Laberinto, sus enemigos, los items de sus cofres y, en el
/// piso DESAFIO, su Desafio. Lo dinamico se libera en el destructor: no copiarlo.
class Nivel
{
public:
    Nivel(RegistroNivel receta);
    ~Nivel();

    /// Genera el laberinto y lo puebla con enemigos e items de Enemigos.dat e Items.dat
    /// filtrados por la dificultad del piso; en DESAFIO elige un acertijo de Acertijos.dat.
    /// false si no se pudieron leer los archivos.
    bool preparar();

    std::string getNombre();
    int getNumeroPiso();
    TipoNivel getTipo();
    int getDificultad();

    Laberinto &getLaberinto();

    int getCantidadEnemigos();
    Enemigo *getEnemigo(int indice);
    Enemigo *getEnemigoEn(Posicion posicion);   /// el enemigo vivo en esa celda, o nullptr

    /// mueve cada enemigo vivo segun su IA, sin atravesar paredes ni a otros enemigos
    void moverEnemigos(Posicion posicionJugador);

    /// saca el item del cofre en esa celda (pasa a cargo de quien llamo), o nullptr.
    /// El cofre queda abierto: la celda pasa a ser PISO.
    Item *abrirCofre(Posicion posicion);

    /// piso DESAFIO: la PUERTA pasa a ser PISO
    void abrirPuerta();

    Desafio &getDesafio();

    /// con la respuesta correcta abre la puerta y prepara el premio. false si es incorrecta.
    bool responderDesafio(int opcion);

    /// el item premio del desafio (pasa a cargo de quien llamo), o nullptr si no hay
    Item *tomarPremioDesafio();

private:
    Enemigo *crearEnemigo(RegistroEnemigo registro, Posicion posicion);   /// Minotauro si es JEFE
    Item *crearItem(RegistroItem registro);                               /// la subclase segun el tipo

    bool ubicarEnemigos();
    bool llenarCofres();
    void elegirAcertijo();

    /// una celda de PISO libre y lejos de la entrada, elegida al azar; (-1,-1) si no hay
    Posicion elegirLugarParaEnemigo();

    /// un enemigo puede pararse ahi: PISO, sin otro enemigo y sin el jugador
    bool puedeOcupar(Posicion destino, Posicion posicionJugador);

    RegistroNivel _receta;
    Laberinto _laberinto;
    Enemigo **_enemigos;
    int _cantidadEnemigos;
    Item **_itemsCofres;
    Posicion *_posicionesCofres;
    int _cantidadCofres;
    Desafio _desafio;
    Item *_premioDesafio;
};
