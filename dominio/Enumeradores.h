#pragma once

/// Todos los valores fijos del juego. Ninguno depende de SDL.

enum class TipoEnemigo
{
    COMUN,
    ELITE,
    JEFE        /// se instancia como Minotauro
};

enum class TipoItem
{
    ARMA,       /// ataque += poder
    ARMADURA,   /// defensa += poder
    RELIQUIA,   /// vidaMaxima += poder
    CURACION    /// va al inventario; vida += poder al usarla
};

enum class TipoNivel
{
    COMBATE,
    DESAFIO,    /// la PUERTA se abre resolviendo un acertijo
    JEFE
};

enum class TipoCelda
{
    PARED,
    PISO,
    ENTRADA,
    ESCALERA,
    COFRE,
    PUERTA      /// piso DESAFIO: cerrada hasta resolver el acertijo
};

enum class Direccion
{
    ARRIBA,
    ABAJO,
    IZQUIERDA,
    DERECHA
};

enum class ResultadoPartida
{
    VICTORIA,
    DERROTA,
    ABANDONO
};

enum class EstadoJuego
{
    MENU,
    JUGANDO,
    PAUSA,
    ADMINISTRACION,
    CREDITOS,
    RANKING,
    FIN_PARTIDA
};

/// Lo que devuelven las clases Archivo* (patron de la catedra)
enum class ArchivoError
{
    OK = 0,
    LEER,
    ABRIR,
    ESCRIBIR
};

/// Lo que Pantalla traduce de los eventos de SDL, para que el resto no conozca SDL
enum class Tecla
{
    NINGUNA,
    ARRIBA,
    ABAJO,
    IZQUIERDA,
    DERECHA,
    ACEPTAR,    /// Enter
    CANCELAR,   /// Escape
    CERRAR      /// se cerro la ventana
};
