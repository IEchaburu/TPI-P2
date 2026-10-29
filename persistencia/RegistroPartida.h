#pragma once
#include "Enumeradores.h"
#include "Fecha.h"

/// Registro de Partidas.dat. Es un historial: no tiene borrado logico.
class RegistroPartida
{
public:
    RegistroPartida();

    int getIdPartida();
    void setIdPartida(int idPartida);

    int getIdJugador();                   /// relacion con Jugadores.dat
    void setIdJugador(int idJugador);

    Fecha getFecha();
    void setFecha(Fecha fecha);

    int getNivelAlcanzado();
    void setNivelAlcanzado(int nivelAlcanzado);

    int getEnemigosDerrotados();
    void setEnemigosDerrotados(int enemigosDerrotados);

    int getPuntaje();
    void setPuntaje(int puntaje);

    ResultadoPartida getResultado();
    void setResultado(ResultadoPartida resultado);

    int getDuracionSegundos();
    void setDuracionSegundos(int duracionSegundos);

private:
    int _idPartida;
    int _idJugador;
    Fecha _fecha;                         /// composicion: una Partida TIENE UNA Fecha
    int _nivelAlcanzado;
    int _enemigosDerrotados;
    int _puntaje;
    ResultadoPartida _resultado;
    int _duracionSegundos;
};
