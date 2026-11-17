#pragma once
#include "Validador.h"
#include "ArchivoEnemigos.h"
#include "ArchivoItems.h"
#include "ArchivoAcertijos.h"
#include "ArchivoNiveles.h"

/// ABM de los catalogos por consola: alta, baja logica, modificacion y listado
/// de Enemigos, Items, Acertijos y Niveles. Todo ingreso pasa por el Validador.
/// Por cada catalogo: un submenu, una accion por metodo, y dos ayudas:
/// mostrar (un registro en pantalla) y pedirDatos (lo usan el alta y la modificacion).
class MenuAdministracion
{
public:
    void ejecutar();

private:
    ///Enemigos.dat
    void menuEnemigos();
    void listarEnemigos();
    void altaEnemigo();
    void modificarEnemigo();
    void bajaEnemigo();
    void mostrarEnemigo(RegistroEnemigo registro);
    void pedirDatosEnemigo(RegistroEnemigo &registro);

    ///  Items.dat 
    void menuItems();
    void listarItems();
    void altaItem();
    void modificarItem();
    void bajaItem();
    void mostrarItem(RegistroItem registro);
    void pedirDatosItem(RegistroItem &registro);

    /// Acertijos.dat 
    void menuAcertijos();
    void listarAcertijos();
    void altaAcertijo();
    void modificarAcertijo();
    void bajaAcertijo();
    void mostrarAcertijo(RegistroAcertijo registro);
    void pedirDatosAcertijo(RegistroAcertijo &registro);
    bool existeItem(int idItem);                  /// en Items.dat y no dado de baja

    ///  Niveles.dat 
    void menuNiveles();
    void listarNiveles();
    void altaNivel();
    void modificarNivel();
    void bajaNivel();
    void mostrarNivel(RegistroNivel registro);
    void pedirDatosNivel(RegistroNivel &registro);
    bool pisoOcupado(int numeroPiso, int idPropio);   /// otro nivel activo con ese numero
    int pedirImpar(std::string mensaje, int minimo, int maximo);

    /// las 5 opciones de cada submenu (1 a 4 y 0 para volver)
    int pedirAccion(std::string catalogo);

    Validador _validador;
    ArchivoEnemigos _archivoEnemigos;
    ArchivoItems _archivoItems;
    ArchivoAcertijos _archivoAcertijos;
    ArchivoNiveles _archivoNiveles;
};
