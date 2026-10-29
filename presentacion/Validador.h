#pragma once
#include <string>

/// Todo ingreso del usuario pasa por aca (criterio obligatorio). Por consola.
class Validador
{
public:
    /// repite el pedido hasta que el numero este entre minimo y maximo
    int pedirEntero(std::string mensaje, int minimo, int maximo);

    /// repite el pedido hasta que el texto no este vacio y tenga como mucho longitudMaxima
    std::string pedirTexto(std::string mensaje, int longitudMaxima);

    /// S o N
    bool pedirConfirmacion(std::string mensaje);
};
