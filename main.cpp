#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

int main(int argc, char* argv[])
{
    // initialize SDL video, PNG loading and TTF fonts
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("Unable to init SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    if (TTF_Init() < 0)
    {
        printf("Unable to init SDL_ttf: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // create a window and a renderer to draw on it
    SDL_Window* window = SDL_CreateWindow("TPI-P2",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          640, 480, 0);
    SDL_Renderer* renderer = nullptr;
    if (window)
    {
        renderer = SDL_CreateRenderer(window, -1,
                                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }
    if (!renderer)
    {
        printf("Unable to create window: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // load an image (IMG_LoadTexture also accepts PNG files)
    SDL_Texture* image = IMG_LoadTexture(renderer, "images/UTN.bmp");
    if (!image)
    {
        printf("Unable to load image: %s\n", IMG_GetError());
    }

    // render a line of text with a TTF font
    SDL_Texture* text = nullptr;
    TTF_Font* font = TTF_OpenFont("fuentes/PressStart2P-Regular.ttf", 16);
    if (font)
    {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, "ESC para salir", white);
        if (textSurface)
        {
            text = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
        }
        TTF_CloseFont(font);
    }
    else
    {
        printf("Unable to load font: %s\n", TTF_GetError());
    }

    // centre the image on screen, text near the bottom
    SDL_Rect imageRect = {0, 0, 0, 0};
    SDL_QueryTexture(image, nullptr, nullptr, &imageRect.w, &imageRect.h);
    imageRect.x = (640 - imageRect.w) / 2;
    imageRect.y = (480 - imageRect.h) / 2;

    SDL_Rect textRect = {0, 0, 0, 0};
    SDL_QueryTexture(text, nullptr, nullptr, &textRect.w, &textRect.h);
    textRect.x = (640 - textRect.w) / 2;
    textRect.y = 480 - textRect.h - 20;

    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // exit if ESCAPE is pressed
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                break;
            }
        }

        // DRAWING STARTS HERE

        // clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw image and text
        SDL_RenderCopy(renderer, image, nullptr, &imageRect);
        SDL_RenderCopy(renderer, text, nullptr, &textRect);

        // DRAWING ENDS HERE

        // finally, show what was drawn
        SDL_RenderPresent(renderer);
    }

    // free everything in reverse order
    SDL_DestroyTexture(text);
    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    printf("Exited cleanly\n");
    return 0;
}
