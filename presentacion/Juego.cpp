#include <iostream>
#include <cstdlib>
#include "Juego.h"
#include "Estadisticas.h"
#include "Enemigo.h"
#include "Fecha.h"
#include "RegistroPartida.h"
#include "ArchivoJugadores.h"
#include "ArchivoPartidas.h"
#include "ArchivoNiveles.h"

using namespace std;

/// ventana: el laberinto (13 x 19 celdas de 32 px) arriba y el HUD abajo
const int ANCHO_VENTANA = 640;
const int ALTO_VENTANA = 560;
const int TAMANIO_CELDA = 32;
const int MARGEN_Y = 8;
const int HUD_Y = 432;
const int ALTO_LINEA = 14;            /// renglones con la fuente chica (8 px)
const int TAMANIO_FUENTE = 8;
const int TAMANIO_FUENTE_GRANDE = 16;

/// Teseo al empezar la partida
const int VIDA_INICIAL = 30;
const int ATAQUE_INICIAL = 5;
const int DEFENSA_INICIAL = 2;

const int CASTIGO_ACERTIJO = 5;       /// vida que cuesta responder mal
const int PUNTOS_POR_PISO = 100;
const int PUNTOS_POR_VICTORIA = 500;
const int LARGO_NOMBRE_JUGADOR = 19;  /// char[20] en RegistroJugador, menos el '\0'
const int ID_SPRITE_TESEO = 1;        /// el mismo que devuelve Personaje::getIdSprite
const int ID_SPRITE_MINOTAURO = 18;   /// solo para la portada del menu

/// funciones auxiliares (no necesitan nada del Juego)

/// dos posiciones pegadas en horizontal o vertical
bool sonVecinas(Posicion a, Posicion b)
{
    return abs(a.getFila() - b.getFila()) + abs(a.getColumna() - b.getColumna()) == 1;
}

/// lo que hace un item que se aplica al recogerlo, para el mensaje del HUD
string describirEfecto(Item *item)
{
    string poder = to_string(item->getPoder());
    switch(item->getTipo()){
        case TipoItem::ARMA:     return " (ataque +" + poder + ")";
        case TipoItem::ARMADURA: return " (defensa +" + poder + ")";
        case TipoItem::RELIQUIA: return " (vida maxima +" + poder + ")";
        case TipoItem::CURACION: return " (cura " + poder + ")";
    }
    return "";
}

Fecha fechaDeHoy()
{
    time_t ahora = time(nullptr);
    tm *local = localtime(&ahora);
    return Fecha(local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
}

bool existeJugador(RegistroJugador jugadores[], int cantidad, string nombre)
{
    for(int i = 0; i < cantidad; i++){
        if(!jugadores[i].getEliminado() && jugadores[i].getNombre() == nombre){
            return true;
        }
    }
    return false;
}

/// construccion

/// los Menu no tienen constructor por defecto: se construyen en la lista de inicializacion
Juego::Juego() : _menuPrincipal(5), _menuPausa(2), _menuAcertijo(3)
{
    _menuPrincipal.setOpcion(1, "Jugar");
    _menuPrincipal.setOpcion(2, "Administrar");
    _menuPrincipal.setOpcion(3, "Ranking");
    _menuPrincipal.setOpcion(4, "Creditos");
    _menuPrincipal.setOpcion(5, "Salir");

    _menuPausa.setOpcion(1, "Continuar");
    _menuPausa.setOpcion(2, "Abandonar partida");

    _estado = EstadoJuego::MENU;
    _salir = false;
    _posicionJugador = -1;
    _teseo = nullptr;
    _nivel = nullptr;
    _respondiendoAcertijo = false;
    _pisoAlcanzado = 0;
    _enemigosDerrotados = 0;
    _experienciaGanada = 0;
    _horaInicio = 0;
    _resultado = ResultadoPartida::ABANDONO;
    _puntaje = 0;
    _duracionSegundos = 0;
    _esMejorPuntaje = false;
    _guardadoOk = false;
}

Juego::~Juego()
{
    liberarPartida();
}

void Juego::ejecutar()
{
    if(!_pantalla.iniciar("El Laberinto del Minotauro", ANCHO_VENTANA, ALTO_VENTANA)){
        cout << _pantalla.getLastError() << endl;
        return;
    }

    if(!cargarGraficos()){
        cout << "Hay que ejecutar el juego desde la carpeta del proyecto (TPI-P2)." << endl;
        return;
    }

    /// el bucle del juego: primero todas las teclas pendientes, despues un dibujo.
    /// presentar() espera el refresco del monitor, asi que da unas 60 vueltas por segundo.
    while(!_salir){
        Tecla tecla = _pantalla.leerTecla();
        while(tecla != Tecla::NINGUNA && !_salir){
            procesarTecla(tecla);
            tecla = _pantalla.leerTecla();
        }

        if(!_salir){
            _pantalla.limpiar();
            dibujar();
            _pantalla.presentar();
        }
    }
}

bool Juego::cargarGraficos()
{
    /// en el mismo orden que el enum TipoCelda, asi el tipo sirve de indice
    string nombresCeldas[CANTIDAD_TIPOS_CELDA] = {"pared", "piso", "entrada", "escalera", "cofre", "puerta"};

    for(int i = 0; i < CANTIDAD_TIPOS_CELDA; i++){
        string ruta = "images/celdas/" + nombresCeldas[i] + ".png";
        if(!_spritesCeldas[i].cargar(_pantalla, ruta)){
            cout << "No se pudo cargar " << ruta << endl;
            return false;
        }
    }

    /// los sprites van por id: se intenta cargar cada uno y los que no existen quedan vacios
    /// (un enemigo dado de alta en el ABM puede usar cualquier id que tenga imagen)
    for(int id = 0; id < CANTIDAD_SPRITES; id++){
        _sprites[id].cargar(_pantalla, "images/sprites/" + to_string(id) + ".png");
    }

    if(_sprites[ID_SPRITE_TESEO].getAncho() == 0){
        cout << "No se pudo cargar la imagen de Teseo en images/sprites/" << endl;
        return false;
    }

    if(!_fuente.abrir("fuentes/PressStart2P-Regular.ttf", TAMANIO_FUENTE)
       || !_fuenteGrande.abrir("fuentes/PressStart2P-Regular.ttf", TAMANIO_FUENTE_GRANDE)){
        cout << "No se pudo abrir fuentes/PressStart2P-Regular.ttf" << endl;
        return false;
    }

    return true;
}

/// teclas

void Juego::procesarTecla(Tecla tecla)
{
    /// cerrar la ventana corta todo; si habia una partida, cuenta como abandonada
    if(tecla == Tecla::CERRAR){
        if(_teseo != nullptr){
            terminarPartida(ResultadoPartida::ABANDONO);
        }
        _salir = true;
        return;
    }

    switch(_estado){
        case EstadoJuego::MENU:
            procesarMenu(tecla);
            break;
        case EstadoJuego::JUGANDO:
            if(_respondiendoAcertijo){
                procesarAcertijo(tecla);
            }
            else{
                procesarJugando(tecla);
            }
            break;
        case EstadoJuego::PAUSA:
            procesarPausa(tecla);
            break;
        case EstadoJuego::FIN_PARTIDA:
        case EstadoJuego::ADMINISTRACION:
        case EstadoJuego::RANKING:
        case EstadoJuego::CREDITOS:
            procesarVolverAlMenu(tecla);
            break;
    }
}

void Juego::procesarMenu(Tecla tecla)
{
    if(tecla == Tecla::CANCELAR){
        _salir = true;
        return;
    }

    int opcion = _menuPrincipal.procesarTecla(tecla);
    switch(opcion){
        case 1:
            _aviso = "";
            if(elegirPerfil() && iniciarPartida()){
                _estado = EstadoJuego::JUGANDO;
            }
            break;
        case 2:
            _estado = EstadoJuego::ADMINISTRACION;   /// provisorio hasta el paso 9
            break;
        case 3:
            _estado = EstadoJuego::RANKING;          /// provisorio hasta el paso 10
            break;
        case 4:
            _estado = EstadoJuego::CREDITOS;         /// provisorio hasta el paso 10
            break;
        case 5:
            _salir = true;
            break;
        default:
            break;   /// 0: se movio la seleccion, no se eligio nada
    }
}

void Juego::procesarJugando(Tecla tecla)
{
    switch(tecla){
        case Tecla::ARRIBA:    moverTeseo(Direccion::ARRIBA); break;
        case Tecla::ABAJO:     moverTeseo(Direccion::ABAJO); break;
        case Tecla::IZQUIERDA: moverTeseo(Direccion::IZQUIERDA); break;
        case Tecla::DERECHA:   moverTeseo(Direccion::DERECHA); break;
        case Tecla::ACEPTAR:   usarCuracion(); break;
        case Tecla::CANCELAR:
            _menuPausa.reiniciar();
            _estado = EstadoJuego::PAUSA;
            break;
        default:
            break;
    }
}

void Juego::procesarAcertijo(Tecla tecla)
{
    /// ESC: Teseo se aleja de la puerta sin responder (no cuesta nada)
    if(tecla == Tecla::CANCELAR){
        _respondiendoAcertijo = false;
        _mensaje = "Teseo se alejo de la puerta";
        return;
    }

    int opcion = _menuAcertijo.procesarTecla(tecla);
    if(opcion == 0){
        return;
    }

    if(_nivel->responderDesafio(opcion)){
        _respondiendoAcertijo = false;
        _mensajeEnemigos = "";
        recoger(_nivel->tomarPremioDesafio());
        _mensaje = "Correcto! Se abrio la puerta. " + _mensaje;
        return;
    }

    /// respuesta incorrecta: cuesta vida, pero se puede volver a intentar
    _teseo->recibirDanio(CASTIGO_ACERTIJO);
    _mensaje = "Incorrecto: Teseo pierde " + to_string(CASTIGO_ACERTIJO) + " de vida";
    if(!_teseo->estaVivo()){
        terminarPartida(ResultadoPartida::DERROTA);
    }
}

void Juego::procesarPausa(Tecla tecla)
{
    if(tecla == Tecla::CANCELAR){
        _estado = EstadoJuego::JUGANDO;
        return;
    }

    int opcion = _menuPausa.procesarTecla(tecla);
    if(opcion == 1){
        _estado = EstadoJuego::JUGANDO;
    }
    else if(opcion == 2){
        terminarPartida(ResultadoPartida::ABANDONO);
    }
}

void Juego::procesarVolverAlMenu(Tecla tecla)
{
    if(tecla == Tecla::ACEPTAR || tecla == Tecla::CANCELAR){
        _estado = EstadoJuego::MENU;
    }
}

///  partida 

bool Juego::elegirPerfil()
{
    /// el nombre se escribe en la consola: mientras tanto la ventana no se ve
    _pantalla.ocultar();

    ArchivoJugadores archivo;
    int cantidad = archivo.getCantidadRegistros();
    RegistroJugador *jugadores = nullptr;
    int *posiciones = nullptr;   /// para cada numero de la lista, su posicion en el archivo

    if(cantidad > 0){
        jugadores = new RegistroJugador[cantidad];
        posiciones = new int[cantidad];
        if(jugadores == nullptr || posiciones == nullptr){
            delete [] jugadores;
            delete [] posiciones;
            _pantalla.mostrar();
            _aviso = "No hay memoria para leer los jugadores";
            return false;
        }
        if(archivo.leerTodos(jugadores, cantidad) != ArchivoError::OK){
            cout << archivo.getLastError() << endl;
            cantidad = 0;
        }
    }

    cout << endl << "========== ELEGIR JUGADOR ==========" << endl;
    int activos = 0;
    for(int i = 0; i < cantidad; i++){
        if(!jugadores[i].getEliminado()){
            posiciones[activos] = i;
            activos++;
            cout << activos << ". " << jugadores[i].getNombre()
                 << "  (partidas: " << jugadores[i].getPartidasJugadas()
                 << ", mejor puntaje: " << jugadores[i].getMejorPuntaje() << ")" << endl;
        }
    }
    if(activos == 0){
        cout << "Todavia no hay jugadores." << endl;
    }
    cout << activos + 1 << ". Crear un jugador nuevo" << endl;
    cout << "0. Volver al menu" << endl;

    int opcion = _validador.pedirEntero("Opcion: ", 0, activos + 1);
    bool elegido = false;

    if(opcion >= 1 && opcion <= activos){
        _posicionJugador = posiciones[opcion - 1];
        _jugador = jugadores[_posicionJugador];
        elegido = true;
    }
    else if(opcion == activos + 1){
        string nombre = _validador.pedirTexto("Nombre (hasta 19 letras): ", LARGO_NOMBRE_JUGADOR);
        while(!nombre.empty() && existeJugador(jugadores, cantidad, nombre)){
            cout << "Ya hay un jugador con ese nombre." << endl;
            nombre = _validador.pedirTexto("Nombre (hasta 19 letras): ", LARGO_NOMBRE_JUGADOR);
        }

        /// vacio solo si se cerro la entrada: se vuelve al menu
        if(!nombre.empty()){
            RegistroJugador nuevo;
            nuevo.setIdJugador(archivo.getNewID());
            nuevo.setNombre(nombre);

            if(archivo.guardar(nuevo) == ArchivoError::OK){
                _posicionJugador = archivo.getCantidadRegistros() - 1;   /// se agrego al final
                _jugador = nuevo;
                elegido = true;
            }
            else{
                cout << archivo.getLastError() << endl;
            }
        }
    }

    delete [] jugadores;
    delete [] posiciones;

    if(elegido){
        cout << "Jugador: " << _jugador.getNombre() << ". Volve a la ventana del juego." << endl;
    }
    _pantalla.mostrar();
    return elegido;
}

bool Juego::iniciarPartida()
{
    liberarPartida();

    RegistroNivel receta;
    if(!buscarPiso(0, receta)){
        _aviso = "No hay pisos en Niveles.dat";
        return false;
    }

    _teseo = new Personaje("Teseo", Estadisticas(VIDA_INICIAL, ATAQUE_INICIAL, DEFENSA_INICIAL), Posicion());
    if(_teseo == nullptr){
        _aviso = "No hay memoria para empezar la partida";
        return false;
    }

    _respondiendoAcertijo = false;
    _pisoAlcanzado = 0;
    _enemigosDerrotados = 0;
    _experienciaGanada = 0;
    _horaInicio = time(nullptr);

    if(!prepararPiso(receta)){
        _aviso = "No se pudo armar el piso: faltan Enemigos.dat o Items.dat";
        liberarPartida();
        return false;
    }
    return true;
}

bool Juego::buscarPiso(int pisoAnterior, RegistroNivel &receta)
{
    ArchivoNiveles archivo;
    int cantidad = archivo.getCantidadRegistros();
    if(cantidad == 0){
        return false;
    }

    RegistroNivel *niveles = new RegistroNivel[cantidad];
    if(niveles == nullptr){
        return false;
    }
    if(archivo.leerTodos(niveles, cantidad) != ArchivoError::OK){
        delete [] niveles;
        return false;
    }

    /// el de numeroPiso mas chico entre los que vienen despues del anterior (sin eliminados).
    /// Asi el orden lo da numeroPiso y no el orden en que se cargaron en el ABM.
    int elegido = -1;
    for(int i = 0; i < cantidad; i++){
        if(niveles[i].getEliminado() || niveles[i].getNumeroPiso() <= pisoAnterior){
            continue;
        }
        if(elegido == -1 || niveles[i].getNumeroPiso() < niveles[elegido].getNumeroPiso()){
            elegido = i;
        }
    }

    if(elegido != -1){
        receta = niveles[elegido];
    }
    delete [] niveles;
    return elegido != -1;
}

bool Juego::prepararPiso(RegistroNivel receta)
{
    /// el piso anterior se libera entero (laberinto, enemigos, cofres) antes de armar el nuevo
    delete _nivel;
    _nivel = new Nivel(receta);
    if(_nivel == nullptr){
        return false;
    }
    if(!_nivel->preparar()){
        return false;
    }

    _teseo->setPosicion(_nivel->getLaberinto().buscar(TipoCelda::ENTRADA));
    _pisoAlcanzado = _nivel->getNumeroPiso();
    _mensaje = "Piso " + to_string(_nivel->getNumeroPiso()) + ": " + _nivel->getNombre();
    _mensajeEnemigos = "";
    return true;
}

void Juego::moverTeseo(Direccion direccion)
{
    Posicion destino = _teseo->getPosicion().desplazar(direccion);
    Enemigo *enemigo = _nivel->getEnemigoEn(destino);

    if(enemigo != nullptr){
        /// hacia un enemigo: se lo ataca y Teseo se queda donde esta
        bool derrotado = _combate.atacarEnemigo(*_teseo, *enemigo);
        _mensaje = _combate.getUltimoMensaje();

        if(derrotado){
            _enemigosDerrotados++;
            _experienciaGanada += enemigo->getExperienciaOtorgada();
            if(enemigo->getTipo() == TipoEnemigo::JEFE){
                terminarPartida(ResultadoPartida::VICTORIA);
                return;
            }
        }
    }
    else{
        Laberinto &laberinto = _nivel->getLaberinto();
        TipoCelda tipo = laberinto.getTipoCelda(destino);

        if(tipo == TipoCelda::PUERTA){
            abrirAcertijo();
            return;
        }
        /// contra una pared no pasa nada y no se gasta el turno
        if(!laberinto.esTransitable(destino)){
            return;
        }

        _teseo->setPosicion(destino);
        _mensaje = "";

        if(tipo == TipoCelda::COFRE){
            recoger(_nivel->abrirCofre(destino));
        }
        else if(tipo == TipoCelda::ESCALERA){
            bajarPiso();   /// en el piso nuevo los enemigos todavia no juegan
            return;
        }
    }

    turnoEnemigos();
}

void Juego::recoger(Item *item)
{
    if(item == nullptr){
        _mensaje = "El cofre estaba vacio";
        return;
    }

    string nombre = item->getNombre();

    if(item->seGuardaEnInventario()){
        if(_teseo->getInventario().agregar(item)){
            _mensaje = "Teseo guardo " + nombre + describirEfecto(item);   /// ahora es del inventario
            return;
        }
        /// inventario lleno: se usa en el momento para no perderla
        item->aplicar(*_teseo);
        _mensaje = "Inventario lleno: Teseo uso " + nombre;
        delete item;
        return;
    }

    /// arma, armadura o reliquia: el efecto se aplica ya y el item no se guarda
    item->aplicar(*_teseo);
    _mensaje = "Teseo encontro " + nombre + describirEfecto(item);
    delete item;
}

void Juego::usarCuracion()
{
    Inventario &inventario = _teseo->getInventario();
    if(inventario.getCantidad() == 0){
        _mensaje = "No hay curaciones en el inventario";
        return;
    }

    Estadisticas antes = _teseo->getEstadisticas();
    if(antes.getVida() == antes.getVidaMaxima()){
        _mensaje = "La vida ya esta completa";
        return;
    }

    string nombre = inventario.getItem(0)->getNombre();
    _teseo->usarItem(0);   /// aplica la curacion y la libera

    int curado = _teseo->getEstadisticas().getVida() - antes.getVida();
    _mensaje = "Teseo uso " + nombre + " (+" + to_string(curado) + " de vida)";

    turnoEnemigos();   /// curarse gasta el turno
}

void Juego::abrirAcertijo()
{
    Desafio &desafio = _nivel->getDesafio();
    for(int i = 1; i <= 3; i++){
        _menuAcertijo.setOpcion(i, desafio.getOpcion(i));
    }
    _menuAcertijo.reiniciar();
    _respondiendoAcertijo = true;
    _mensaje = "";
    _mensajeEnemigos = "";
}

void Juego::turnoEnemigos()
{
    Posicion posicionTeseo = _teseo->getPosicion();
    int vidaAntes = _teseo->getEstadisticas().getVida();
    int atacantes = 0;
    _mensajeEnemigos = "";   /// se borra solo cuando hay un turno nuevo (chocar una pared no lo es)

    /// primero atacan los que ya estaban pegados a Teseo...
    for(int i = 0; i < _nivel->getCantidadEnemigos() && _teseo->estaVivo(); i++){
        Enemigo *enemigo = _nivel->getEnemigo(i);
        if(enemigo->estaVivo() && sonVecinas(enemigo->getPosicion(), posicionTeseo)){
            _combate.atacarJugador(*enemigo, *_teseo);
            atacantes++;
        }
    }

    /// ...despues se mueven los demas (los que llegan al lado atacan en el turno siguiente)
    _nivel->moverEnemigos(posicionTeseo);

    if(atacantes == 1){
        _mensajeEnemigos = _combate.getUltimoMensaje();
    }
    else if(atacantes > 1){
        int danio = vidaAntes - _teseo->getEstadisticas().getVida();
        _mensajeEnemigos = to_string(atacantes) + " enemigos le hicieron " + to_string(danio) + " a Teseo";
    }

    if(!_teseo->estaVivo()){
        terminarPartida(ResultadoPartida::DERROTA);
    }
}

void Juego::bajarPiso()
{
    RegistroNivel receta;

    /// sin piso siguiente (por ejemplo, si en el ABM se borro el del Minotauro) se gano
    if(!buscarPiso(_nivel->getNumeroPiso(), receta)){
        terminarPartida(ResultadoPartida::VICTORIA);
        return;
    }

    if(!prepararPiso(receta)){
        terminarPartida(ResultadoPartida::ABANDONO);
        _aviso = "No se pudo armar el piso siguiente";
    }
}

void Juego::terminarPartida(ResultadoPartida resultado)
{
    _resultado = resultado;
    _duracionSegundos = (int)(time(nullptr) - _horaInicio);

    _puntaje = _experienciaGanada + PUNTOS_POR_PISO * _pisoAlcanzado;
    if(resultado == ResultadoPartida::VICTORIA){
        _puntaje += PUNTOS_POR_VICTORIA;
    }

    _esMejorPuntaje = _puntaje > _jugador.getMejorPuntaje();
    _guardadoOk = guardarPartida();

    liberarPartida();
    _respondiendoAcertijo = false;
    _estado = EstadoJuego::FIN_PARTIDA;
}

bool Juego::guardarPartida()
{
    ArchivoPartidas archivoPartidas;
    RegistroPartida partida;
    partida.setIdPartida(archivoPartidas.getNewID());
    partida.setIdJugador(_jugador.getIdJugador());
    partida.setFecha(fechaDeHoy());
    partida.setNivelAlcanzado(_pisoAlcanzado);
    partida.setEnemigosDerrotados(_enemigosDerrotados);
    partida.setPuntaje(_puntaje);
    partida.setResultado(_resultado);
    partida.setDuracionSegundos(_duracionSegundos);
    bool partidaOk = archivoPartidas.guardar(partida) == ArchivoError::OK;

    /// el perfil se actualiza en su misma posicion del archivo
    _jugador.setPartidasJugadas(_jugador.getPartidasJugadas() + 1);
    if(_puntaje > _jugador.getMejorPuntaje()){
        _jugador.setMejorPuntaje(_puntaje);
    }
    if(_pisoAlcanzado > _jugador.getNivelMaximoAlcanzado()){
        _jugador.setNivelMaximoAlcanzado(_pisoAlcanzado);
    }
    ArchivoJugadores archivoJugadores;
    bool jugadorOk = archivoJugadores.actualizar(_posicionJugador, _jugador) == ArchivoError::OK;

    return partidaOk && jugadorOk;
}

void Juego::liberarPartida()
{
    /// delete de nullptr no hace nada: se puede llamar aunque no haya partida
    delete _nivel;
    _nivel = nullptr;
    delete _teseo;
    _teseo = nullptr;
}

///  dibujo

void Juego::dibujar()
{
    switch(_estado){
        case EstadoJuego::MENU:
            dibujarMenu();
            break;
        case EstadoJuego::JUGANDO:
            dibujarPartida();
            if(_respondiendoAcertijo){
                dibujarAcertijo();
            }
            break;
        case EstadoJuego::PAUSA:
            dibujarPartida();
            dibujarPausa();
            break;
        case EstadoJuego::FIN_PARTIDA:
            dibujarFinPartida();
            break;
        case EstadoJuego::ADMINISTRACION:
            dibujarProvisoria("ADMINISTRACION");
            break;
        case EstadoJuego::RANKING:
            dibujarProvisoria("RANKING");
            break;
        case EstadoJuego::CREDITOS:
            dibujarProvisoria("CREDITOS");
            break;
    }
}

void Juego::dibujarMenu()
{
    dibujarCentrado(_fuenteGrande, "EL LABERINTO", 90, 240, 200, 90);
    dibujarCentrado(_fuenteGrande, "DEL MINOTAURO", 118, 240, 200, 90);

    /// Teseo frente al Minotauro, como portada
    dibujarSprite(ID_SPRITE_TESEO, ANCHO_VENTANA / 2 - 48, 160);
    dibujarSprite(ID_SPRITE_MINOTAURO, ANCHO_VENTANA / 2 + 16, 160);

    _menuPrincipal.dibujar(_pantalla, _fuenteGrande, 240, 32);

    if(!_aviso.empty()){
        dibujarCentrado(_fuente, _aviso, 420, 230, 90, 80);
    }
    dibujarCentrado(_fuente, "Flechas: elegir   Enter: aceptar   Esc: salir", 520, 120, 110, 100);
}

void Juego::dibujarPartida()
{
    dibujarLaberinto();
    dibujarEntidades();
    dibujarHud();
}

int Juego::getMargenX()
{
    return (ANCHO_VENTANA - _nivel->getLaberinto().getColumnas() * TAMANIO_CELDA) / 2;
}

void Juego::dibujarLaberinto()
{
    Laberinto &laberinto = _nivel->getLaberinto();
    int margenX = getMargenX();
    int indicePiso = (int)TipoCelda::PISO;

    for(int f = 0; f < laberinto.getFilas(); f++){
        for(int c = 0; c < laberinto.getColumnas(); c++){
            int x = margenX + c * TAMANIO_CELDA;
            int y = MARGEN_Y + f * TAMANIO_CELDA;
            TipoCelda tipo = laberinto.getTipoCelda(Posicion(f, c));

            /// todo lo que no es pared se apoya sobre el piso (el cofre tiene fondo transparente)
            if(tipo != TipoCelda::PARED){
                _spritesCeldas[indicePiso].dibujar(_pantalla, x, y);
            }
            if(tipo != TipoCelda::PISO){
                _spritesCeldas[(int)tipo].dibujar(_pantalla, x, y);
            }
        }
    }
}

void Juego::dibujarEntidades()
{
    int margenX = getMargenX();

    for(int i = 0; i < _nivel->getCantidadEnemigos(); i++){
        Enemigo *enemigo = _nivel->getEnemigo(i);
        if(enemigo->estaVivo()){
            Posicion p = enemigo->getPosicion();
            dibujarSprite(enemigo->getIdSprite(), margenX + p.getColumna() * TAMANIO_CELDA,
                          MARGEN_Y + p.getFila() * TAMANIO_CELDA);
        }
    }

    Posicion p = _teseo->getPosicion();
    dibujarSprite(_teseo->getIdSprite(), margenX + p.getColumna() * TAMANIO_CELDA,
                  MARGEN_Y + p.getFila() * TAMANIO_CELDA);
}

void Juego::dibujarHud()
{
    const int X_IZQUIERDA = 16;
    const int X_DERECHA = 400;
    const int ANCHO_BARRA = 180;
    Estadisticas e = _teseo->getEstadisticas();

    /// columna izquierda: piso, barras y numeros
    _fuente.dibujar(_pantalla, "Piso " + to_string(_nivel->getNumeroPiso()) + " - " + _nivel->getNombre(),
                    X_IZQUIERDA, HUD_Y + 4, 240, 200, 90);

    int y = HUD_Y + 4 + ALTO_LINEA + 4;
    _fuente.dibujar(_pantalla, "Vida", X_IZQUIERDA, y, 220, 210, 200);
    dibujarBarra(X_IZQUIERDA + 48, y - 1, ANCHO_BARRA, e.getVida(), e.getVidaMaxima(), 200, 40, 40);
    _fuente.dibujar(_pantalla, to_string(e.getVida()) + "/" + to_string(e.getVidaMaxima()),
                    X_IZQUIERDA + 56 + ANCHO_BARRA, y, 220, 210, 200);

    y += ALTO_LINEA + 2;
    int experienciaParaSubir = _teseo->getNivel() * 100;   /// la regla de Personaje::ganarExperiencia
    _fuente.dibujar(_pantalla, "Exp", X_IZQUIERDA, y, 220, 210, 200);
    dibujarBarra(X_IZQUIERDA + 48, y - 1, ANCHO_BARRA, _teseo->getExperiencia(), experienciaParaSubir, 60, 110, 220);
    _fuente.dibujar(_pantalla, to_string(_teseo->getExperiencia()) + "/" + to_string(experienciaParaSubir),
                    X_IZQUIERDA + 56 + ANCHO_BARRA, y, 220, 210, 200);

    y += ALTO_LINEA + 2;
    _fuente.dibujar(_pantalla, "Nivel " + to_string(_teseo->getNivel()) + "   Ataque " + to_string(e.getAtaque())
                    + "   Defensa " + to_string(e.getDefensa()), X_IZQUIERDA, y, 220, 210, 200);

    /// columna derecha: las curaciones del inventario, una imagen por item
    Inventario &inventario = _teseo->getInventario();
    _fuente.dibujar(_pantalla, "Curaciones " + to_string(inventario.getCantidad()) + "/"
                    + to_string(inventario.getCapacidad()), X_DERECHA, HUD_Y + 4, 220, 210, 200);
    for(int i = 0; i < inventario.getCantidad(); i++){
        dibujarSprite(inventario.getItem(i)->getIdSprite(), X_DERECHA + i * (TAMANIO_CELDA + 2), HUD_Y + 20);
    }

    /// lo que paso en el ultimo turno
    y = HUD_Y + 74;
    _fuente.dibujar(_pantalla, _mensaje, X_IZQUIERDA, y, 240, 235, 220);
    _fuente.dibujar(_pantalla, _mensajeEnemigos, X_IZQUIERDA, y + ALTO_LINEA + 2, 230, 110, 90);

    dibujarCentrado(_fuente, "Flechas: mover/atacar   Enter: curarse   Esc: pausa",
                    ALTO_VENTANA - 16, 120, 110, 100);
}

void Juego::dibujarAcertijo()
{
    const int X = 32;
    const int Y = 80;
    const int ANCHO = ANCHO_VENTANA - 2 * X;
    dibujarRecuadro(X, Y, ANCHO, 300);

    dibujarCentrado(_fuenteGrande, "LA PUERTA ESTA CERRADA", Y + 20, 240, 200, 90);

    Desafio &desafio = _nivel->getDesafio();
    int y = dibujarParrafo(desafio.getEnunciado(), X + 24, Y + 60, ANCHO - 48);

    _menuAcertijo.dibujar(_pantalla, _fuenteGrande, y + 20, 30);

    /// el resultado de la ultima respuesta (si fue incorrecta)
    dibujarCentrado(_fuente, _mensaje, Y + 240, 230, 110, 90);
    dibujarCentrado(_fuente, "Flechas y Enter: responder   Esc: alejarse", Y + 270, 120, 110, 100);
}

void Juego::dibujarPausa()
{
    dibujarRecuadro(160, 150, 320, 140);
    dibujarCentrado(_fuenteGrande, "PAUSA", 175, 240, 200, 90);
    _menuPausa.dibujar(_pantalla, _fuenteGrande, 215, 30);
}

void Juego::dibujarFinPartida()
{
    switch(_resultado){
        case ResultadoPartida::VICTORIA:
            dibujarCentrado(_fuenteGrande, "VICTORIA", 70, 240, 200, 90);
            dibujarCentrado(_fuente, "Teseo vencio al Minotauro", 100, 220, 210, 200);
            break;
        case ResultadoPartida::DERROTA:
            dibujarCentrado(_fuenteGrande, "DERROTA", 70, 220, 60, 50);
            dibujarCentrado(_fuente, "Teseo cayo en el laberinto", 100, 220, 210, 200);
            break;
        case ResultadoPartida::ABANDONO:
            dibujarCentrado(_fuenteGrande, "PARTIDA ABANDONADA", 70, 180, 170, 160);
            break;
    }

    int minutos = _duracionSegundos / 60;
    int segundos = _duracionSegundos % 60;
    string textoSegundos = (segundos < 10 ? "0" : "") + to_string(segundos);

    int y = 150;
    dibujarCentrado(_fuente, "Jugador: " + _jugador.getNombre(), y, 220, 210, 200);
    dibujarCentrado(_fuente, "Piso alcanzado: " + to_string(_pisoAlcanzado), y + 24, 220, 210, 200);
    dibujarCentrado(_fuente, "Enemigos derrotados: " + to_string(_enemigosDerrotados), y + 48, 220, 210, 200);
    dibujarCentrado(_fuente, "Experiencia ganada: " + to_string(_experienciaGanada), y + 72, 220, 210, 200);
    dibujarCentrado(_fuente, "Duracion: " + to_string(minutos) + ":" + textoSegundos, y + 96, 220, 210, 200);

    dibujarCentrado(_fuenteGrande, "Puntaje: " + to_string(_puntaje), y + 140, 240, 200, 90);
    if(_esMejorPuntaje){
        dibujarCentrado(_fuente, "Nuevo mejor puntaje!", y + 170, 120, 200, 120);
    }
    if(!_guardadoOk){
        dibujarCentrado(_fuente, "No se pudo guardar la partida", y + 200, 230, 90, 80);
    }

    dibujarCentrado(_fuente, "Enter: volver al menu", 520, 120, 110, 100);
}

void Juego::dibujarProvisoria(std::string titulo)
{
    dibujarCentrado(_fuenteGrande, titulo, 200, 240, 200, 90);
    dibujarCentrado(_fuente, "En construccion", 250, 220, 210, 200);
    dibujarCentrado(_fuente, "Enter o Esc: volver al menu", 520, 120, 110, 100);
}

/// ayudas de dibujo

void Juego::dibujarSprite(int idSprite, int x, int y)
{
    /// sin imagen para ese id: un cuadrado violeta, para que se note pero no se rompa nada
    if(idSprite < 0 || idSprite >= CANTIDAD_SPRITES || _sprites[idSprite].getAncho() == 0){
        _pantalla.dibujarRectangulo(x + 4, y + 4, TAMANIO_CELDA - 8, TAMANIO_CELDA - 8, 200, 0, 200);
        return;
    }
    _sprites[idSprite].dibujar(_pantalla, x, y);
}

void Juego::dibujarBarra(int x, int y, int ancho, int valor, int maximo, int rojo, int verde, int azul)
{
    const int ALTO_BARRA = 10;
    _pantalla.dibujarRectangulo(x, y, ancho, ALTO_BARRA, 60, 55, 50);   /// el fondo, gris

    /// la parte llena es proporcional: ancho * valor / maximo (sin dividir por cero)
    if(maximo > 0 && valor > 0){
        int lleno = ancho * valor / maximo;
        if(lleno > ancho){
            lleno = ancho;
        }
        _pantalla.dibujarRectangulo(x, y, lleno, ALTO_BARRA, rojo, verde, azul);
    }
}

void Juego::dibujarRecuadro(int x, int y, int ancho, int alto)
{
    const int BORDE = 3;
    _pantalla.dibujarRectangulo(x, y, ancho, alto, 150, 110, 50);   /// borde bronce
    _pantalla.dibujarRectangulo(x + BORDE, y + BORDE, ancho - 2 * BORDE, alto - 2 * BORDE, 20, 16, 12);
}

void Juego::dibujarCentrado(Fuente &fuente, std::string texto, int y, int rojo, int verde, int azul)
{
    int x = (ANCHO_VENTANA - fuente.medirAncho(texto)) / 2;
    fuente.dibujar(_pantalla, texto, x, y, rojo, verde, azul);
}

int Juego::dibujarParrafo(std::string texto, int x, int y, int anchoMaximo)
{
    /// se arma cada renglon palabra por palabra mientras entre en el ancho
    string renglon = "";
    string palabra = "";

    for(int i = 0; i <= (int)texto.size(); i++){
        bool finDePalabra = (i == (int)texto.size() || texto[i] == ' ');
        if(!finDePalabra){
            palabra += texto[i];
            continue;
        }
        if(palabra.empty()){
            continue;
        }

        string probado = renglon.empty() ? palabra : renglon + " " + palabra;
        if(!renglon.empty() && _fuente.medirAncho(probado) > anchoMaximo){
            _fuente.dibujar(_pantalla, renglon, x, y, 230, 220, 190);
            y += ALTO_LINEA;
            renglon = palabra;
        }
        else{
            renglon = probado;
        }
        palabra = "";
    }

    if(!renglon.empty()){
        _fuente.dibujar(_pantalla, renglon, x, y, 230, 220, 190);
        y += ALTO_LINEA;
    }
    return y;
}
