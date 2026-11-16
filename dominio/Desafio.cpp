#include "Desafio.h"

Desafio::Desafio()
{
    /// _acertijo se construye solo con el constructor por defecto de RegistroAcertijo
}

Desafio::Desafio(RegistroAcertijo acertijo)
{
    _acertijo = acertijo;
}

std::string Desafio::getEnunciado()
{
    return _acertijo.getEnunciado();
}

std::string Desafio::getOpcion(int numero)
{
    switch(numero){
        case 1: return _acertijo.getOpcionA();
        case 2: return _acertijo.getOpcionB();
        case 3: return _acertijo.getOpcionC();
        default: return "";
    }
}

bool Desafio::responder(int numero)
{
    return numero == _acertijo.getCorrecta();
}

int Desafio::getIdItemPremio()
{
    return _acertijo.getIdItemPremio();
}
