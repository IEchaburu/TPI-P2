#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Pantalla.h"

using namespace std;

Pantalla::Pantalla()
{
    _ventana = nullptr;
    _renderer = nullptr;
    _ancho = 0;
    _alto = 0;
    _iniciada = false;
}

Pantalla::~Pantalla()
{
    cerrar();
}

bool Pantalla::iniciar(std::string titulo, int ancho, int alto)
{
    /// main.cpp no incluye SDL.h, asi que SDL no reemplaza main: hay que avisarle
    SDL_SetMainReady();

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        _errorMessage = string("No se pudo iniciar SDL: ") + SDL_GetError();
        return false;
    }
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        _errorMessage = string("No se pudo iniciar SDL_image: ") + IMG_GetError();
        SDL_Quit();
        return false;
    }
    if(TTF_Init() < 0){
        _errorMessage = string("No se pudo iniciar SDL_ttf: ") + TTF_GetError();
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    _iniciada = true;

    _ventana = SDL_CreateWindow(titulo.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                ancho, alto, 0);
    if(_ventana == nullptr){
        _errorMessage = string("No se pudo crear la ventana: ") + SDL_GetError();
        cerrar();
        return false;
    }

    _renderer = SDL_CreateRenderer(_ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(_renderer == nullptr){
        _errorMessage = string("No se pudo crear el renderer: ") + SDL_GetError();
        cerrar();
        return false;
    }

    _ancho = ancho;
    _alto = alto;
    _errorMessage = "";
    return true;
}

void Pantalla::cerrar()
{
    if(_renderer != nullptr){
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if(_ventana != nullptr){
        SDL_DestroyWindow(_ventana);
        _ventana = nullptr;
    }
    if(_iniciada){
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        _iniciada = false;
    }
}

void Pantalla::limpiar()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void Pantalla::presentar()
{
    SDL_RenderPresent(_renderer);
}

void Pantalla::dibujarRectangulo(int x, int y, int ancho, int alto, int rojo, int verde, int azul)
{
    SDL_Rect rectangulo = {x, y, ancho, alto};
    SDL_SetRenderDrawColor(_renderer, rojo, verde, azul, 255);
    SDL_RenderFillRect(_renderer, &rectangulo);
}

Tecla Pantalla::leerTecla()
{
    SDL_Event evento;

    while(SDL_PollEvent(&evento)){
        if(evento.type == SDL_QUIT){
            return Tecla::CERRAR;
        }
        if(evento.type == SDL_KEYDOWN){
            switch(evento.key.keysym.sym){
                case SDLK_UP:     return Tecla::ARRIBA;
                case SDLK_DOWN:   return Tecla::ABAJO;
                case SDLK_LEFT:   return Tecla::IZQUIERDA;
                case SDLK_RIGHT:  return Tecla::DERECHA;
                case SDLK_RETURN: return Tecla::ACEPTAR;
                case SDLK_ESCAPE: return Tecla::CANCELAR;
                default: break;   /// otra tecla: se ignora y se sigue leyendo
            }
        }
    }
    return Tecla::NINGUNA;
}

int Pantalla::getAncho(){ return _ancho; }
int Pantalla::getAlto(){ return _alto; }

SDL_Renderer *Pantalla::getRenderer(){ return _renderer; }

std::string Pantalla::getLastError(){ return _errorMessage; }
