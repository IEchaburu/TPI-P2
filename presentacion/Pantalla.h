#pragma once
#include <string>
#include "Enumeradores.h"

/// declaraciones adelantadas: asi quien incluye Pantalla.h no ve SDL.h
struct SDL_Window;
struct SDL_Renderer;

/// Inicia SDL (video, SDL_image y SDL_ttf), tiene la ventana y traduce el teclado a Tecla.
/// Se destruye ultima: los Sprite y Fuente tienen que liberarse antes.
class Pantalla
{
public:
    Pantalla();
    ~Pantalla();

    bool iniciar(std::string titulo, int ancho, int alto);

    void limpiar();               /// pinta todo de negro
    void presentar();             /// muestra lo dibujado desde el ultimo limpiar()

    void dibujarRectangulo(int x, int y, int ancho, int alto, int rojo, int verde, int azul);

    /// la siguiente tecla pendiente, o NINGUNA si no hay mas eventos
    Tecla leerTecla();

    int getAncho();
    int getAlto();

    SDL_Renderer *getRenderer();  /// solo para Sprite y Fuente
    std::string getLastError();

private:
    void cerrar();

    SDL_Window *_ventana;
    SDL_Renderer *_renderer;
    int _ancho;
    int _alto;
    bool _iniciada;
    std::string _errorMessage;
};
