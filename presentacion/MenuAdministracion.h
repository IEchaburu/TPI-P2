#pragma once
#include "Validador.h"
#include "ArchivoEnemigos.h"
#include "ArchivoItems.h"
#include "ArchivoAcertijos.h"
#include "ArchivoNiveles.h"

/// ABM de los catalogos por consola: alta, baja logica, modificacion y listado
/// de Enemigos, Items, Acertijos y Niveles. Todo ingreso pasa por el Validador.
/// Quien la implemente agrega los metodos privados que necesite (uno por accion).
class MenuAdministracion
{
public:
    void ejecutar();

private:
    Validador _validador;
    ArchivoEnemigos _archivoEnemigos;
    ArchivoItems _archivoItems;
    ArchivoAcertijos _archivoAcertijos;
    ArchivoNiveles _archivoNiveles;
};
