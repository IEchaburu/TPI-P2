#include <SDL.h>
#include <SDL_ttf.h>
#include "Fuente.h"

Fuente::Fuente()
{
    _fuente = nullptr;
}

Fuente::~Fuente()
{
    if(_fuente != nullptr){
        TTF_CloseFont(_fuente);
    }
}

bool Fuente::abrir(std::string ruta, int tamanio)
{
    if(_fuente != nullptr){
        TTF_CloseFont(_fuente);
    }

    _fuente = TTF_OpenFont(ruta.c_str(), tamanio);
    return _fuente != nullptr;
}

void Fuente::dibujar(Pantalla &pantalla, std::string texto, int x, int y, int rojo, int verde, int azul)
{
    /// SDL_ttf no puede dibujar un texto vacio
    if(_fuente == nullptr || texto.empty()){
        return;
    }

    /// 1) el texto se dibuja en una superficie (memoria comun)
    SDL_Color color = {(Uint8)rojo, (Uint8)verde, (Uint8)azul, 255};
    SDL_Surface *superficie = TTF_RenderUTF8_Blended(_fuente, texto.c_str(), color);
    if(superficie == nullptr){
        return;
    }

    /// 2) la superficie se convierte en textura (placa de video) y ya no hace falta
    SDL_Texture *textura = SDL_CreateTextureFromSurface(pantalla.getRenderer(), superficie);
    SDL_Rect destino = {x, y, superficie->w, superficie->h};
    SDL_FreeSurface(superficie);
    if(textura == nullptr){
        return;
    }

    /// 3) se copia a la ventana y se libera: cada texto vive solo lo que dura este dibujo
    SDL_RenderCopy(pantalla.getRenderer(), textura, nullptr, &destino);
    SDL_DestroyTexture(textura);
}

int Fuente::medirAncho(std::string texto)
{
    int ancho = 0;
    int alto = 0;

    if(_fuente != nullptr && !texto.empty()){
        TTF_SizeUTF8(_fuente, texto.c_str(), &ancho, &alto);
    }
    return ancho;
}
