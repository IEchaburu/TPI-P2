#include <SDL.h>
#include <SDL_image.h>
#include "Sprite.h"

Sprite::Sprite()
{
    _textura = nullptr;
    _ancho = 0;
    _alto = 0;
}

Sprite::~Sprite()
{
    if(_textura != nullptr){
        SDL_DestroyTexture(_textura);
    }
}

bool Sprite::cargar(Pantalla &pantalla, std::string ruta)
{
    /// si ya tenia una imagen, se libera antes de cargar la nueva
    if(_textura != nullptr){
        SDL_DestroyTexture(_textura);
        _textura = nullptr;
    }

    /// IMG_LoadTexture lee el archivo (PNG o BMP) y lo sube a la placa de video
    _textura = IMG_LoadTexture(pantalla.getRenderer(), ruta.c_str());
    if(_textura == nullptr){
        _ancho = 0;
        _alto = 0;
        return false;
    }

    SDL_QueryTexture(_textura, nullptr, nullptr, &_ancho, &_alto);
    return true;
}

void Sprite::dibujar(Pantalla &pantalla, int x, int y)
{
    if(_textura == nullptr){
        return;
    }

    SDL_Rect destino = {x, y, _ancho, _alto};
    SDL_RenderCopy(pantalla.getRenderer(), _textura, nullptr, &destino);
}

void Sprite::dibujar(Pantalla &pantalla, int x, int y, int recorteX, int recorteY, int ancho, int alto)
{
    if(_textura == nullptr){
        return;
    }

    /// origen: que parte de la imagen se copia; destino: donde va en la ventana
    SDL_Rect origen = {recorteX, recorteY, ancho, alto};
    SDL_Rect destino = {x, y, ancho, alto};
    SDL_RenderCopy(pantalla.getRenderer(), _textura, &origen, &destino);
}

int Sprite::getAncho(){ return _ancho; }
int Sprite::getAlto(){ return _alto; }
