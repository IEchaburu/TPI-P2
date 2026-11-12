#include "RegistroPartida.h"

RegistroPartida::RegistroPartida()
{
    _idPartida = 0;
    _idJugador = 0;
    _fecha = Fecha();
    _nivelAlcanzado = 0;
    _enemigosDerrotados = 0;
    _puntaje = 0;
    _resultado = ResultadoPartida::ABANDONO;
    _duracionSegundos = 0;
}

int RegistroPartida::getIdPartida(){ return _idPartida; }
void RegistroPartida::setIdPartida(int idPartida){ _idPartida = idPartida; }

int RegistroPartida::getIdJugador(){ return _idJugador; }
void RegistroPartida::setIdJugador(int idJugador){ _idJugador = idJugador; }

Fecha RegistroPartida::getFecha(){ return _fecha; }
void RegistroPartida::setFecha(Fecha fecha){ _fecha = fecha; }

int RegistroPartida::getNivelAlcanzado(){ return _nivelAlcanzado; }
void RegistroPartida::setNivelAlcanzado(int nivelAlcanzado){ _nivelAlcanzado = nivelAlcanzado; }

int RegistroPartida::getEnemigosDerrotados(){ return _enemigosDerrotados; }
void RegistroPartida::setEnemigosDerrotados(int enemigosDerrotados){ _enemigosDerrotados = enemigosDerrotados; }

int RegistroPartida::getPuntaje(){ return _puntaje; }
void RegistroPartida::setPuntaje(int puntaje){ _puntaje = puntaje; }

ResultadoPartida RegistroPartida::getResultado(){ return _resultado; }
void RegistroPartida::setResultado(ResultadoPartida resultado){ _resultado = resultado; }

int RegistroPartida::getDuracionSegundos(){ return _duracionSegundos; }
void RegistroPartida::setDuracionSegundos(int duracionSegundos){ _duracionSegundos = duracionSegundos; }
