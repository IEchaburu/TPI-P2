#include <iostream>
#include <cstdlib>
#include "Nivel.h"
#include "Minotauro.h"
#include "Arma.h"
#include "Armadura.h"
#include "Reliquia.h"
#include "Curacion.h"
#include "GeneradorLaberinto.h"
#include "ArchivoEnemigos.h"
#include "ArchivoItems.h"
#include "ArchivoAcertijos.h"

using namespace std;

const int DISTANCIA_MINIMA_A_LA_ENTRADA = 4;   /// para no empezar con un enemigo encima
const int RANGO_DE_PERSECUCION = 6;            /// mas lejos que esto, el enemigo no se mueve

/// la lista de inicializacion construye el laberinto con el tamanio de la receta
Nivel::Nivel(RegistroNivel receta) : _receta(receta), _laberinto(receta.getFilas(), receta.getColumnas())
{
    _enemigos = nullptr;
    _cantidadEnemigos = 0;
    _itemsCofres = nullptr;
    _posicionesCofres = nullptr;
    _cantidadCofres = 0;
    _premioDesafio = nullptr;
}

Nivel::~Nivel()
{
    /// cada enemigo es un new: primero ellos, despues el vector de punteros
    for(int i = 0; i < _cantidadEnemigos; i++){
        delete _enemigos[i];
    }
    delete [] _enemigos;

    /// los cofres ya abiertos tienen nullptr: delete de nullptr no hace nada
    for(int i = 0; i < _cantidadCofres; i++){
        delete _itemsCofres[i];
    }
    delete [] _itemsCofres;
    delete [] _posicionesCofres;

    delete _premioDesafio;
}

bool Nivel::preparar()
{
    GeneradorLaberinto generador;
    generador.generar(_laberinto, _receta);   /// tambien fija la semilla de rand()

    if(!ubicarEnemigos()){
        return false;
    }
    if(!llenarCofres()){
        return false;
    }
    if(_receta.getTipo() == TipoNivel::DESAFIO){
        elegirAcertijo();
    }
    return true;
}

bool Nivel::ubicarEnemigos()
{
    ArchivoEnemigos archivo;
    int cantidadRegistros = archivo.getCantidadRegistros();
    if(cantidadRegistros == 0){
        return false;
    }

    RegistroEnemigo *registros = new RegistroEnemigo[cantidadRegistros];
    if(registros == nullptr){
        return false;
    }
    if(archivo.leerTodos(registros, cantidadRegistros) != ArchivoError::OK){
        delete [] registros;
        return false;
    }

    /// candidatos: no eliminados y con la dificultad del piso dentro de su rango.
    /// Se guardan las posiciones del vector de registros, separadas por tipo.
    int *comunes = new int[cantidadRegistros];
    if(comunes == nullptr){
        delete [] registros;
        return false;
    }
    int cantidadComunes = 0;
    int indiceElite = -1;
    int indiceJefe = -1;
    int dificultad = _receta.getDificultad();

    for(int i = 0; i < cantidadRegistros; i++){
        RegistroEnemigo r = registros[i];
        if(r.getEliminado() || dificultad < r.getDificultadMinima() || dificultad > r.getDificultadMaxima()){
            continue;
        }
        if(r.getTipo() == TipoEnemigo::COMUN){
            comunes[cantidadComunes] = i;
            cantidadComunes++;
        }
        else if(r.getTipo() == TipoEnemigo::ELITE && indiceElite == -1){
            indiceElite = i;
        }
        else if(r.getTipo() == TipoEnemigo::JEFE && indiceJefe == -1){
            indiceJefe = i;
        }
    }

    int cantidadPedida = _receta.getCantidadEnemigos();
    _enemigos = new Enemigo*[cantidadPedida > 0 ? cantidadPedida : 1];
    if(_enemigos == nullptr){
        delete [] registros;
        delete [] comunes;
        return false;
    }

    if(_receta.getTipo() == TipoNivel::JEFE){
        /// el jefe solo, arriba al centro de la arena (la entrada esta abajo)
        if(indiceJefe != -1){
            Posicion lugar(1, _laberinto.getColumnas() / 2);
            _enemigos[0] = crearEnemigo(registros[indiceJefe], lugar);
            _cantidadEnemigos = 1;
        }
    }
    else{
        for(int i = 0; i < cantidadPedida; i++){
            /// el primer lugar es para el ELITE si el piso tiene uno; el resto, COMUN al azar
            int elegido = -1;
            if(i == 0 && indiceElite != -1){
                elegido = indiceElite;
            }
            else if(cantidadComunes > 0){
                elegido = comunes[rand() % cantidadComunes];
            }

            Posicion lugar = elegirLugarParaEnemigo();
            if(elegido == -1 || lugar.getFila() == -1){
                continue;
            }
            _enemigos[_cantidadEnemigos] = crearEnemigo(registros[elegido], lugar);
            _cantidadEnemigos++;
        }
    }

    delete [] registros;
    delete [] comunes;
    return true;
}

bool Nivel::llenarCofres()
{
    /// cuantos cofres dejo el generador
    for(int f = 0; f < _laberinto.getFilas(); f++){
        for(int c = 0; c < _laberinto.getColumnas(); c++){
            if(_laberinto.getTipoCelda(Posicion(f, c)) == TipoCelda::COFRE){
                _cantidadCofres++;
            }
        }
    }

    int tamanio = (_cantidadCofres > 0) ? _cantidadCofres : 1;
    _itemsCofres = new Item*[tamanio];
    _posicionesCofres = new Posicion[tamanio];
    if(_itemsCofres == nullptr || _posicionesCofres == nullptr){
        _cantidadCofres = 0;   /// asi el destructor no recorre vectores que no existen
        return false;
    }

    ArchivoItems archivo;
    int cantidadRegistros = archivo.getCantidadRegistros();
    RegistroItem *registros = nullptr;
    int *candidatos = nullptr;
    int cantidadCandidatos = 0;

    if(cantidadRegistros > 0){
        registros = new RegistroItem[cantidadRegistros];
        candidatos = new int[cantidadRegistros];
        if(registros == nullptr || candidatos == nullptr
           || archivo.leerTodos(registros, cantidadRegistros) != ArchivoError::OK){
            delete [] registros;
            delete [] candidatos;
            for(int i = 0; i < _cantidadCofres; i++){
                _itemsCofres[i] = nullptr;
            }
            return false;
        }

        for(int i = 0; i < cantidadRegistros; i++){
            if(!registros[i].getEliminado() && registros[i].getDificultadMinima() <= _receta.getDificultad()){
                candidatos[cantidadCandidatos] = i;
                cantidadCandidatos++;
            }
        }
    }

    /// cada cofre, en el orden de la grilla, recibe un item al azar entre los candidatos
    int cofre = 0;
    for(int f = 0; f < _laberinto.getFilas(); f++){
        for(int c = 0; c < _laberinto.getColumnas(); c++){
            Posicion posicion(f, c);
            if(_laberinto.getTipoCelda(posicion) == TipoCelda::COFRE){
                _posicionesCofres[cofre] = posicion;
                if(cantidadCandidatos > 0){
                    _itemsCofres[cofre] = crearItem(registros[candidatos[rand() % cantidadCandidatos]]);
                }
                else{
                    _itemsCofres[cofre] = nullptr;   /// cofre vacio
                }
                cofre++;
            }
        }
    }

    delete [] registros;
    delete [] candidatos;
    return true;
}

void Nivel::elegirAcertijo()
{
    ArchivoAcertijos archivo;
    int cantidadRegistros = archivo.getCantidadRegistros();

    /// se elige al azar entre los no eliminados con dificultad hasta la del piso
    int validos = 0;
    for(int i = 0; i < cantidadRegistros; i++){
        RegistroAcertijo r = archivo.leer(i);
        if(!r.getEliminado() && r.getDificultad() <= _receta.getDificultad()){
            validos++;
        }
    }

    if(validos == 0){
        abrirPuerta();   /// sin acertijos la puerta no se podria abrir nunca: se deja abierta
        return;
    }

    int elegido = rand() % validos;
    for(int i = 0; i < cantidadRegistros; i++){
        RegistroAcertijo r = archivo.leer(i);
        if(!r.getEliminado() && r.getDificultad() <= _receta.getDificultad()){
            if(elegido == 0){
                _desafio = Desafio(r);
                return;
            }
            elegido--;
        }
    }
}

Enemigo *Nivel::crearEnemigo(RegistroEnemigo registro, Posicion posicion)
{
    /// polimorfismo: los dos se guardan como Enemigo*, pero el JEFE es un Minotauro
    if(registro.getTipo() == TipoEnemigo::JEFE){
        return new Minotauro(registro, posicion);
    }
    return new Enemigo(registro, posicion);
}

Item *Nivel::crearItem(RegistroItem registro)
{
    switch(registro.getTipo()){
        case TipoItem::ARMA:     return new Arma(registro);
        case TipoItem::ARMADURA: return new Armadura(registro);
        case TipoItem::RELIQUIA: return new Reliquia(registro);
        case TipoItem::CURACION: return new Curacion(registro);
    }
    return nullptr;
}

Posicion Nivel::elegirLugarParaEnemigo()
{
    Posicion entrada = _laberinto.buscar(TipoCelda::ENTRADA);

    /// primero se cuentan los lugares validos, despues se elige el n-esimo al azar
    int validos = 0;
    for(int vuelta = 0; vuelta < 2; vuelta++){
        int elegido = (vuelta == 1) ? rand() % validos : -1;

        for(int f = 0; f < _laberinto.getFilas(); f++){
            for(int c = 0; c < _laberinto.getColumnas(); c++){
                Posicion posicion(f, c);
                int distancia = abs(f - entrada.getFila()) + abs(c - entrada.getColumna());
                if(_laberinto.getTipoCelda(posicion) == TipoCelda::PISO
                   && distancia >= DISTANCIA_MINIMA_A_LA_ENTRADA
                   && getEnemigoEn(posicion) == nullptr){
                    if(vuelta == 0){
                        validos++;
                    }
                    else if(elegido == 0){
                        return posicion;
                    }
                    else{
                        elegido--;
                    }
                }
            }
        }

        if(validos == 0){
            break;
        }
    }
    return Posicion(-1, -1);
}

bool Nivel::puedeOcupar(Posicion destino, Posicion posicionJugador)
{
    return _laberinto.getTipoCelda(destino) == TipoCelda::PISO
        && !destino.esIgual(posicionJugador)
        && getEnemigoEn(destino) == nullptr;
}

void Nivel::moverEnemigos(Posicion posicionJugador)
{
    for(int i = 0; i < _cantidadEnemigos; i++){
        Enemigo *enemigo = _enemigos[i];
        if(!enemigo->estaVivo()){
            continue;
        }

        Posicion actual = enemigo->getPosicion();
        int diferenciaFilas = posicionJugador.getFila() - actual.getFila();
        int diferenciaColumnas = posicionJugador.getColumna() - actual.getColumna();
        int distancia = abs(diferenciaFilas) + abs(diferenciaColumnas);

        /// pegado al jugador no se mueve (ataca, lo resuelve Combate); muy lejos tampoco
        if(distancia <= 1 || distancia > RANGO_DE_PERSECUCION){
            continue;
        }

        Direccion direccion = enemigo->elegirDireccion(posicionJugador);
        Posicion destino = actual.desplazar(direccion);

        if(!puedeOcupar(destino, posicionJugador)){
            /// bloqueado: se prueba acercarse por el otro eje
            bool eraVertical = (direccion == Direccion::ARRIBA || direccion == Direccion::ABAJO);
            if(eraVertical && diferenciaColumnas != 0){
                destino = actual.desplazar(diferenciaColumnas > 0 ? Direccion::DERECHA : Direccion::IZQUIERDA);
            }
            else if(!eraVertical && diferenciaFilas != 0){
                destino = actual.desplazar(diferenciaFilas > 0 ? Direccion::ABAJO : Direccion::ARRIBA);
            }
            else{
                continue;
            }
            if(!puedeOcupar(destino, posicionJugador)){
                continue;
            }
        }

        enemigo->setPosicion(destino);
    }
}

Item *Nivel::abrirCofre(Posicion posicion)
{
    for(int i = 0; i < _cantidadCofres; i++){
        if(_posicionesCofres[i].esIgual(posicion) && _laberinto.getTipoCelda(posicion) == TipoCelda::COFRE){
            Item *item = _itemsCofres[i];
            _itemsCofres[i] = nullptr;                      /// el nivel deja de ser su duenio
            _laberinto.setTipoCelda(posicion, TipoCelda::PISO);
            return item;
        }
    }
    return nullptr;
}

void Nivel::abrirPuerta()
{
    Posicion puerta = _laberinto.buscar(TipoCelda::PUERTA);
    if(puerta.getFila() != -1){
        _laberinto.setTipoCelda(puerta, TipoCelda::PISO);
    }
}

Desafio &Nivel::getDesafio()
{
    return _desafio;
}

bool Nivel::responderDesafio(int opcion)
{
    if(!_desafio.responder(opcion)){
        return false;
    }

    abrirPuerta();

    /// el premio se busca por id en Items.dat
    ArchivoItems archivo;
    int posicion = archivo.buscarByID(_desafio.getIdItemPremio());
    if(posicion != -1 && _premioDesafio == nullptr){
        RegistroItem registro = archivo.leer(posicion);
        if(!registro.getEliminado()){
            _premioDesafio = crearItem(registro);
        }
    }
    return true;
}

Item *Nivel::tomarPremioDesafio()
{
    Item *premio = _premioDesafio;
    _premioDesafio = nullptr;
    return premio;
}

std::string Nivel::getNombre(){ return _receta.getNombre(); }
int Nivel::getNumeroPiso(){ return _receta.getNumeroPiso(); }
TipoNivel Nivel::getTipo(){ return _receta.getTipo(); }
int Nivel::getDificultad(){ return _receta.getDificultad(); }

Laberinto &Nivel::getLaberinto(){ return _laberinto; }

int Nivel::getCantidadEnemigos(){ return _cantidadEnemigos; }

Enemigo *Nivel::getEnemigo(int indice)
{
    if(indice < 0 || indice >= _cantidadEnemigos){
        return nullptr;
    }
    return _enemigos[indice];
}

Enemigo *Nivel::getEnemigoEn(Posicion posicion)
{
    for(int i = 0; i < _cantidadEnemigos; i++){
        if(_enemigos[i]->estaVivo() && _enemigos[i]->getPosicion().esIgual(posicion)){
            return _enemigos[i];
        }
    }
    return nullptr;
}
