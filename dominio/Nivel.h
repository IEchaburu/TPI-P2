#pragma once
#include <string>
#include "Enumeradores.h"
#include "Laberinto.h"
#include "Enemigo.h"
#include "Item.h"
#include "RegistroNivel.h"

/// Un piso en juego. Compone su Laberinto, sus enemigos y los items de sus cofres,
/// todo en memoria dinamica y liberado en el destructor. No copiarlo ni pasarlo por valor.
class Nivel
{
public:
    Nivel(RegistroNivel receta);
    ~Nivel();

    /// Genera el laberinto y lo puebla con enemigos e items de Enemigos.dat e Items.dat
    /// filtrados por la dificultad del piso. false si no se pudieron leer los archivos.
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

    /// saca el item del cofre en esa celda (pasa a cargo de quien llamo), o nullptr
    Item *abrirCofre(Posicion posicion);

    /// piso DESAFIO: la PUERTA pasa a ser PISO
    void abrirPuerta();

private:
    Enemigo *crearEnemigo(RegistroEnemigo registro, Posicion posicion);   /// Minotauro si es JEFE
    Item *crearItem(RegistroItem registro);                               /// la subclase segun el tipo

    RegistroNivel _receta;
    Laberinto _laberinto;
    Enemigo **_enemigos;
    int _cantidadEnemigos;
    Item **_itemsCofres;
    Posicion *_posicionesCofres;
    int _cantidadCofres;
};
