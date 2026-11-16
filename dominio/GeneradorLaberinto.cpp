#include <iostream>
#include <cstdlib>
#include <ctime>
#include "GeneradorLaberinto.h"

using namespace std;

const int CANTIDAD_ATAJOS = 6;
const int CANTIDAD_DIRECCIONES = 4;

void GeneradorLaberinto::generar(Laberinto &laberinto, RegistroNivel receta)
{
    /// misma semilla = misma secuencia de rand() = mismo laberinto
    if(receta.getSemilla() == 0){
        srand(time(nullptr));
    }
    else{
        srand(receta.getSemilla());
    }

    if(receta.getTipo() == TipoNivel::JEFE){
        armarArena(laberinto);
        laberinto.setTipoCelda(Posicion(laberinto.getFilas() - 2, laberinto.getColumnas() / 2),
                               TipoCelda::ENTRADA);
    }
    else{
        tallarPasillos(laberinto, receta.getTipo() == TipoNivel::DESAFIO);

        /// en el DESAFIO no: un atajo permitiria rodear la puerta
        if(receta.getTipo() == TipoNivel::COMBATE){
            abrirAtajos(laberinto, CANTIDAD_ATAJOS);
        }
    }

    ubicarAlAzar(laberinto, TipoCelda::COFRE, receta.getCantidadItems());
}

void GeneradorLaberinto::tallarPasillos(Laberinto &laberinto, bool conPuerta)
{
    Direccion direcciones[CANTIDAD_DIRECCIONES] = {
        Direccion::ARRIBA, Direccion::ABAJO, Direccion::IZQUIERDA, Direccion::DERECHA
    };

    /// las "habitaciones" son las celdas de fila y columna impares
    int maximoHabitaciones = ((laberinto.getFilas() - 1) / 2) * ((laberinto.getColumnas() - 1) / 2);

    if(maximoHabitaciones < 2){
        armarArena(laberinto);   /// grilla demasiado chica para un laberinto
        return;
    }

    int columnas = laberinto.getColumnas();
    int totalCeldas = laberinto.getFilas() * columnas;

    /// pila: el camino desde la entrada hasta la celda actual.
    /// profundidad: a cuantas habitaciones de la entrada quedo cada celda (-1 = no visitada).
    /// anterior: desde que celda se llego a cada una (para reconstruir el camino despues).
    /// camino: el camino reconstruido de la entrada a la escalera.
    Posicion *pila = new Posicion[maximoHabitaciones];
    int *profundidad = new int[totalCeldas];
    int *anterior = new int[totalCeldas];
    Posicion *camino = new Posicion[maximoHabitaciones];

    if(pila == nullptr || profundidad == nullptr || anterior == nullptr || camino == nullptr){
        delete [] pila;            /// delete [] de un nullptr no hace nada: es seguro
        delete [] profundidad;
        delete [] anterior;
        delete [] camino;
        cout << "No se pudo pedir memoria para generar el laberinto..." << endl;
        exit(-100);
    }

    for(int i = 0; i < totalCeldas; i++){
        profundidad[i] = -1;
        anterior[i] = -1;
    }

    Posicion inicio(1, 1);
    laberinto.setTipoCelda(inicio, TipoCelda::PISO);
    pila[0] = inicio;
    int tope = 1;
    profundidad[inicio.getFila() * columnas + inicio.getColumna()] = 1;
    int profundidadMaxima = 1;

    while(tope > 0){
        Posicion actual = pila[tope - 1];

        /// direcciones hacia una habitacion vecina (a 2 celdas) que todavia es pared
        Direccion opciones[CANTIDAD_DIRECCIONES];
        int cantidadOpciones = 0;
        for(int i = 0; i < CANTIDAD_DIRECCIONES; i++){
            Posicion vecina = actual.desplazar(direcciones[i]).desplazar(direcciones[i]);
            bool enElInterior = vecina.getFila() >= 1 && vecina.getFila() <= laberinto.getFilas() - 2
                             && vecina.getColumna() >= 1 && vecina.getColumna() <= laberinto.getColumnas() - 2;
            if(enElInterior && laberinto.getTipoCelda(vecina) == TipoCelda::PARED){
                opciones[cantidadOpciones] = direcciones[i];
                cantidadOpciones++;
            }
        }

        if(cantidadOpciones > 0){
            /// avanzar: romper la pared del medio y pasar a la vecina
            Direccion elegida = opciones[rand() % cantidadOpciones];
            Posicion pared = actual.desplazar(elegida);
            Posicion vecina = pared.desplazar(elegida);
            laberinto.setTipoCelda(pared, TipoCelda::PISO);
            laberinto.setTipoCelda(vecina, TipoCelda::PISO);
            pila[tope] = vecina;
            tope++;

            /// la altura de la pila es la distancia por camino desde la entrada
            int indiceVecina = vecina.getFila() * columnas + vecina.getColumna();
            profundidad[indiceVecina] = tope;
            anterior[indiceVecina] = actual.getFila() * columnas + actual.getColumna();
            if(tope > profundidadMaxima){
                profundidadMaxima = tope;
            }
        }
        else{
            tope--;   /// callejon sin salida: volver atras
        }
    }

    /// escalera: entre las habitaciones del ultimo cuarto del recorrido (lejos por camino),
    /// la mas lejana en linea recta. Asi no queda pegada a la entrada del otro lado de una pared.
    int profundidadMinima = profundidadMaxima * 3 / 4;
    int indiceEscalera = -1;
    int mejorDistancia = -1;
    for(int i = 0; i < totalCeldas; i++){
        if(profundidad[i] >= profundidadMinima){
            int fila = i / columnas;
            int columna = i % columnas;
            int distanciaRecta = abs(fila - inicio.getFila()) + abs(columna - inicio.getColumna());
            if(distanciaRecta > mejorDistancia){
                mejorDistancia = distanciaRecta;
                indiceEscalera = i;
            }
        }
    }

    /// camino de la entrada a la escalera: se sigue "anterior" desde la escalera hacia atras
    /// y se guarda al reves, para que camino[0] sea la entrada
    int largoCamino = profundidad[indiceEscalera];
    int indice = indiceEscalera;
    for(int i = largoCamino - 1; i >= 0; i--){
        camino[i] = Posicion(indice / columnas, indice % columnas);
        indice = anterior[indice];
    }

    laberinto.setTipoCelda(inicio, TipoCelda::ENTRADA);
    laberinto.setTipoCelda(camino[largoCamino - 1], TipoCelda::ESCALERA);

    /// el camino es unico: una puerta en el medio deja la escalera del otro lado
    if(conPuerta && largoCamino >= 3){
        int indicePuerta = largoCamino * 2 / 3;
        if(indicePuerta >= largoCamino - 1){
            indicePuerta = largoCamino - 2;
        }
        laberinto.setTipoCelda(camino[indicePuerta], TipoCelda::PUERTA);
    }

    delete [] pila;
    delete [] profundidad;
    delete [] anterior;
    delete [] camino;
}

void GeneradorLaberinto::armarArena(Laberinto &laberinto)
{
    for(int f = 1; f < laberinto.getFilas() - 1; f++){
        for(int c = 1; c < laberinto.getColumnas() - 1; c++){
            laberinto.setTipoCelda(Posicion(f, c), TipoCelda::PISO);
        }
    }
}

void GeneradorLaberinto::abrirAtajos(Laberinto &laberinto, int cantidad)
{
    const int INTENTOS_POR_ATAJO = 50;
    int abiertos = 0;
    int intentos = 0;

    while(abiertos < cantidad && intentos < cantidad * INTENTOS_POR_ATAJO){
        intentos++;
        Posicion candidata(1 + rand() % (laberinto.getFilas() - 2),
                           1 + rand() % (laberinto.getColumnas() - 2));

        if(laberinto.getTipoCelda(candidata) != TipoCelda::PARED){
            continue;
        }

        /// una pared "fina": pasillo a los dos lados en una direccion y pared en la otra
        bool horizontal = laberinto.esTransitable(candidata.desplazar(Direccion::IZQUIERDA))
                       && laberinto.esTransitable(candidata.desplazar(Direccion::DERECHA))
                       && !laberinto.esTransitable(candidata.desplazar(Direccion::ARRIBA))
                       && !laberinto.esTransitable(candidata.desplazar(Direccion::ABAJO));
        bool vertical = laberinto.esTransitable(candidata.desplazar(Direccion::ARRIBA))
                     && laberinto.esTransitable(candidata.desplazar(Direccion::ABAJO))
                     && !laberinto.esTransitable(candidata.desplazar(Direccion::IZQUIERDA))
                     && !laberinto.esTransitable(candidata.desplazar(Direccion::DERECHA));

        if(horizontal || vertical){
            laberinto.setTipoCelda(candidata, TipoCelda::PISO);
            abiertos++;
        }
    }
}

void GeneradorLaberinto::ubicarAlAzar(Laberinto &laberinto, TipoCelda tipo, int cantidad)
{
    /// cuantas celdas de PISO hay para elegir
    int libres = 0;
    for(int f = 0; f < laberinto.getFilas(); f++){
        for(int c = 0; c < laberinto.getColumnas(); c++){
            if(laberinto.getTipoCelda(Posicion(f, c)) == TipoCelda::PISO){
                libres++;
            }
        }
    }

    for(int i = 0; i < cantidad && libres > 0; i++){
        /// se elige la n-esima celda de PISO, recorriendo la grilla en orden
        int elegida = rand() % libres;
        bool ubicada = false;
        for(int f = 0; f < laberinto.getFilas() && !ubicada; f++){
            for(int c = 0; c < laberinto.getColumnas() && !ubicada; c++){
                Posicion posicion(f, c);
                if(laberinto.getTipoCelda(posicion) == TipoCelda::PISO){
                    if(elegida == 0){
                        laberinto.setTipoCelda(posicion, tipo);
                        ubicada = true;
                    }
                    elegida--;
                }
            }
        }
        libres--;
    }
}
