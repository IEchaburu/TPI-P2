/**
    Herramienta: escribe los .dat con el contenido inicial del juego.

    NO es parte del juego (no esta en el .cbp). Usa las mismas clases Archivo* del juego,
    asi que los registros quedan exactamente como el juego los lee. Borra y reescribe
    Niveles.dat, Enemigos.dat, Items.dat y Acertijos.dat; no toca Jugadores.dat ni
    Partidas.dat (esos los crea el juego al jugar).

    Se compila con todos los .cpp de la carpeta persistencia mas dominio/Fecha.cpp, y se
    ejecuta desde la carpeta TPI-P2, que es donde el juego busca los .dat. Por ejemplo,
    desde la terminal, parado en TPI-P2:

        g++ -std=c++17 -Idominio -Ipersistencia herramientas/CargarDatos.cpp
            persistencia/Registro*.cpp persistencia/Archivo*.cpp dominio/Fecha.cpp
            -o cargar_datos.exe
        cargar_datos.exe

    Despues de esto, los datos se modifican desde el ABM del juego.
*/
#include <iostream>
#include <cstdio>
#include "ArchivoNiveles.h"
#include "ArchivoEnemigos.h"
#include "ArchivoItems.h"
#include "ArchivoAcertijos.h"

using namespace std;

void cargarNivel(ArchivoNiveles &archivo, string nombre, int piso, TipoNivel tipo,
                 int cantidadEnemigos, int cantidadItems)
{
    const int FILAS = 13;
    const int COLUMNAS = 19;

    RegistroNivel reg;
    reg.setIdNivel(archivo.getNewID());
    reg.setNombre(nombre);
    reg.setNumeroPiso(piso);
    reg.setTipo(tipo);
    reg.setFilas(FILAS);
    reg.setColumnas(COLUMNAS);
    reg.setCantidadEnemigos(cantidadEnemigos);
    reg.setCantidadItems(cantidadItems);
    reg.setDificultad(piso);
    reg.setSemilla(0);                  /// 0 = otro laberinto en cada partida
    reg.setEliminado(false);
    archivo.guardar(reg);
}

void cargarEnemigo(ArchivoEnemigos &archivo, string nombre, TipoEnemigo tipo, int vida, int ataque,
                   int defensa, int experiencia, int dificultadMinima, int dificultadMaxima, int idSprite)
{
    RegistroEnemigo reg;
    reg.setIdEnemigo(archivo.getNewID());
    reg.setNombre(nombre);
    reg.setTipo(tipo);
    reg.setVidaMaxima(vida);
    reg.setAtaque(ataque);
    reg.setDefensa(defensa);
    reg.setExperienciaOtorgada(experiencia);
    reg.setDificultadMinima(dificultadMinima);
    reg.setDificultadMaxima(dificultadMaxima);
    reg.setIdSprite(idSprite);
    reg.setEliminado(false);
    archivo.guardar(reg);
}

void cargarItem(ArchivoItems &archivo, string nombre, TipoItem tipo, int poder, int dificultadMinima, int idSprite)
{
    RegistroItem reg;
    reg.setIdItem(archivo.getNewID());
    reg.setNombre(nombre);
    reg.setTipo(tipo);
    reg.setPoder(poder);
    reg.setDificultadMinima(dificultadMinima);
    reg.setIdSprite(idSprite);
    reg.setEliminado(false);
    archivo.guardar(reg);
}

void cargarAcertijo(ArchivoAcertijos &archivo, string enunciado, string a, string b, string c,
                    int correcta, int idItemPremio)
{
    const int DIFICULTAD = 3;   /// los acertijos son del piso 3 (DESAFIO)

    RegistroAcertijo reg;
    reg.setIdAcertijo(archivo.getNewID());
    reg.setEnunciado(enunciado);
    reg.setOpcionA(a);
    reg.setOpcionB(b);
    reg.setOpcionC(c);
    reg.setCorrecta(correcta);
    reg.setDificultad(DIFICULTAD);
    reg.setIdItemPremio(idItemPremio);
    reg.setEliminado(false);
    archivo.guardar(reg);
}

int main()
{
    remove("Niveles.dat");
    remove("Enemigos.dat");
    remove("Items.dat");
    remove("Acertijos.dat");

    ArchivoNiveles niveles;
    cargarNivel(niveles, "Pasillo de Inicio",       1, TipoNivel::COMBATE, 5, 3);
    cargarNivel(niveles, "Escaleras al Fin",        2, TipoNivel::COMBATE, 6, 3);
    cargarNivel(niveles, "La Guardia de la Puerta", 3, TipoNivel::DESAFIO, 6, 3);
    cargarNivel(niveles, "Guarida del Minotauro",   4, TipoNivel::JEFE,    1, 1);

    ArchivoEnemigos enemigos;
    ///                           nombre              tipo                vida at def  exp min max sprite
    cargarEnemigo(enemigos, "Slime",             TipoEnemigo::COMUN,    8,  3,  0,  20, 1, 1, 10);
    cargarEnemigo(enemigos, "Arania",            TipoEnemigo::COMUN,    6,  4,  1,  25, 1, 1, 11);
    cargarEnemigo(enemigos, "Murcielago",        TipoEnemigo::COMUN,   10,  6,  1,  35, 2, 2, 12);
    cargarEnemigo(enemigos, "Lobo",              TipoEnemigo::COMUN,   16,  7,  2,  45, 2, 3, 13);
    cargarEnemigo(enemigos, "Ciclope",           TipoEnemigo::ELITE,   40, 10,  4, 120, 2, 2, 14);
    cargarEnemigo(enemigos, "Esqueleto armado",  TipoEnemigo::COMUN,   22,  9,  4,  60, 3, 3, 15);
    cargarEnemigo(enemigos, "Lobo feroz",        TipoEnemigo::COMUN,   24, 10,  3,  65, 3, 3, 16);
    cargarEnemigo(enemigos, "Guardia acorazado", TipoEnemigo::COMUN,   30,  9,  7,  80, 3, 3, 17);
    cargarEnemigo(enemigos, "Minotauro",         TipoEnemigo::JEFE,    80, 14,  5,   0, 4, 4, 18);

    ArchivoItems items;
    ///                    nombre             tipo                 poder min sprite
    cargarItem(items, "Espada",           TipoItem::ARMA,      2,  1, 30);   /// id 1
    cargarItem(items, "Botas",            TipoItem::ARMADURA,  1,  1, 31);   /// id 2
    cargarItem(items, "Carne de jabali",  TipoItem::CURACION, 15,  1, 32);   /// id 3
    cargarItem(items, "Capa",             TipoItem::RELIQUIA, 10,  2, 33);   /// id 4
    cargarItem(items, "Hacha doble",      TipoItem::ARMA,      3,  2, 34);   /// id 5
    cargarItem(items, "Escudo de bronce", TipoItem::ARMADURA,  2,  3, 35);   /// id 6
    cargarItem(items, "Ambrosia",         TipoItem::CURACION, 30,  3, 36);   /// id 7

    const int ID_ESCUDO = 6;
    const int ID_AMBROSIA = 7;

    ArchivoAcertijos acertijos;
    cargarAcertijo(acertijos,
        "Que camina en cuatro patas al amanecer, en dos al mediodia y en tres al atardecer?",
        "El hombre", "El Minotauro", "La Esfinge", 1, ID_AMBROSIA);
    cargarAcertijo(acertijos,
        "Quien construyo el laberinto de Creta por orden del rey Minos?",
        "Teseo", "Dedalo", "Icaro", 2, ID_ESCUDO);
    cargarAcertijo(acertijos,
        "Que le dio Ariadna a Teseo para no perderse en el laberinto?",
        "Una espada", "Un mapa", "Un ovillo de hilo", 3, ID_AMBROSIA);
    cargarAcertijo(acertijos,
        "Cuanto mas le sacas, mas grande se hace. Que es?",
        "Un pozo", "Una deuda", "Una sombra", 1, ID_ESCUDO);
    cargarAcertijo(acertijos,
        "De que isla era rey Minos, el que encerro al Minotauro?",
        "Creta", "Rodas", "Itaca", 1, ID_AMBROSIA);

    /// se relee todo para comprobar que quedo bien escrito
    cout << "Niveles.dat (" << niveles.getCantidadRegistros() << ")" << endl;
    for(int i = 0; i < niveles.getCantidadRegistros(); i++){
        RegistroNivel r = niveles.leer(i);
        cout << "  " << r.getIdNivel() << ". Piso " << r.getNumeroPiso() << " - " << r.getNombre()
             << " (" << r.getFilas() << "x" << r.getColumnas() << ", enemigos " << r.getCantidadEnemigos()
             << ", cofres " << r.getCantidadItems() << ")" << endl;
    }

    cout << "Enemigos.dat (" << enemigos.getCantidadRegistros() << ")" << endl;
    for(int i = 0; i < enemigos.getCantidadRegistros(); i++){
        RegistroEnemigo r = enemigos.leer(i);
        cout << "  " << r.getIdEnemigo() << ". " << r.getNombre() << " - vida " << r.getVidaMaxima()
             << ", at " << r.getAtaque() << ", def " << r.getDefensa() << ", exp " << r.getExperienciaOtorgada()
             << ", dificultad " << r.getDificultadMinima() << "-" << r.getDificultadMaxima()
             << ", sprite " << r.getIdSprite() << endl;
    }

    cout << "Items.dat (" << items.getCantidadRegistros() << ")" << endl;
    for(int i = 0; i < items.getCantidadRegistros(); i++){
        RegistroItem r = items.leer(i);
        cout << "  " << r.getIdItem() << ". " << r.getNombre() << " - poder " << r.getPoder()
             << ", dificultad minima " << r.getDificultadMinima() << ", sprite " << r.getIdSprite() << endl;
    }

    cout << "Acertijos.dat (" << acertijos.getCantidadRegistros() << ")" << endl;
    for(int i = 0; i < acertijos.getCantidadRegistros(); i++){
        RegistroAcertijo r = acertijos.leer(i);
        cout << "  " << r.getIdAcertijo() << ". " << r.getEnunciado() << endl
             << "     1) " << r.getOpcionA() << "  2) " << r.getOpcionB() << "  3) " << r.getOpcionC()
             << "  -> correcta " << r.getCorrecta() << ", premio item " << r.getIdItemPremio() << endl;
    }

    return 0;
}
