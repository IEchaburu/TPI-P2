#pragma once
#include "Entidad.h"
#include "Enumeradores.h"
#include "RegistroEnemigo.h"

/// Se construye desde un RegistroEnemigo. La especie (slime, lobo...) es solo el nombre del registro.
class Enemigo : public Entidad
{
public:
    Enemigo();
    Enemigo(RegistroEnemigo registro, Posicion posicion);

    int getIdEnemigo();
    TipoEnemigo getTipo();
    int getExperienciaOtorgada();

    /// IA: hacia donde quiere moverse este turno. Nivel decide si el paso es posible.
    virtual Direccion elegirDireccion(Posicion posicionJugador);

    int getIdSprite() override;

private:
    int _idEnemigo;
    TipoEnemigo _tipo;
    int _experienciaOtorgada;
    int _idSprite;
};
