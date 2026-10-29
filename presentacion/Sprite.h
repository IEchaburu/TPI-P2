#pragma once
#include <string>
#include "Pantalla.h"

struct SDL_Texture;

/// Una imagen (PNG o BMP) cargada como textura. La libera en el destructor: no copiarlo.
/// Referencia para implementarla: el main.cpp del primer commit (IMG_LoadTexture, SDL_RenderCopy).
class Sprite
{
public:
    Sprite();
    ~Sprite();

    bool cargar(Pantalla &pantalla, std::string ruta);

    void dibujar(Pantalla &pantalla, int x, int y);

    /// sobrecarga: dibuja solo un recorte (un tile de una hoja de sprites)
    void dibujar(Pantalla &pantalla, int x, int y, int recorteX, int recorteY, int ancho, int alto);

    int getAncho();
    int getAlto();

private:
    SDL_Texture *_textura;
    int _ancho;
    int _alto;
};
