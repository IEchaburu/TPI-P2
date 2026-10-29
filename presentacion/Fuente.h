#pragma once
#include <string>
#include "Pantalla.h"

struct _TTF_Font;   /// es el TTF_Font de SDL_ttf

/// Dibuja texto con una fuente TTF (fuentes/PressStart2P-Regular.ttf). La cierra en el destructor.
/// Referencia para implementarla: el main.cpp del primer commit (TTF_OpenFont, TTF_RenderUTF8_Blended).
class Fuente
{
public:
    Fuente();
    ~Fuente();

    bool abrir(std::string ruta, int tamanio);

    void dibujar(Pantalla &pantalla, std::string texto, int x, int y, int rojo, int verde, int azul);

    int medirAncho(std::string texto);    /// en pixeles, para centrar

private:
    _TTF_Font *_fuente;
};
