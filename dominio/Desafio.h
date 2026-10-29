#pragma once
#include <string>
#include "RegistroAcertijo.h"

/// El acertijo que abre la PUERTA del piso DESAFIO.
class Desafio
{
public:
    Desafio();
    Desafio(RegistroAcertijo acertijo);

    std::string getEnunciado();
    std::string getOpcion(int numero);    /// numero 1, 2 o 3
    bool responder(int numero);           /// true si es la correcta
    int getIdItemPremio();

private:
    RegistroAcertijo _acertijo;
};
