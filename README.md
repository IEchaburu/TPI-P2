# TPI-P2 · El Laberinto del Minotauro

Trabajo práctico integrador de Programación II (UTN FRGP, TUP). Videojuego en C++ con
**SDL 2** (+ SDL_image para PNG y SDL_ttf para fuentes).

## Cómo abrirlo

1. Instalar **Code::Blocks 25.03 con MinGW** (el instalador `codeblocks-25.03mingw-setup.exe`).
   No hace falta configurar nada más: SDL 2 ya viene dentro del proyecto.
2. Abrir `TPI-P2.cbp`.
3. **Build and run** (F9). Tiene que aparecer una ventana con el logo de la UTN y el texto
   "ESC para salir".

## Reglas para trabajar en el proyecto

- **Cada `.cpp` o `.h` nuevo hay que agregarlo al proyecto** (*Project → Add files…*),
  si no Code::Blocks no lo compila.
- Imágenes en `images/` (BMP o PNG) y fuentes en `fuentes/`. Se cargan con rutas relativas
  a la carpeta del proyecto (`"images/UTN.bmp"`), que es desde donde Code::Blocks ejecuta.
- **No tocar `SDL2/`** ni las DLL de la raíz: son la biblioteca tal cual la publica SDL.
- `bin/` y `obj/` los genera Code::Blocks; no se suben al repositorio.

## Para entregar o ejecutar fuera de Code::Blocks

El `.exe` necesita al lado: `SDL2.dll`, `SDL2_image.dll`, `SDL2_ttf.dll` y las carpetas
`images/` y `fuentes/`.

## Versiones y licencias

- SDL 2.32.10, SDL_image 2.8.12, SDL_ttf 2.24.0 (64 bits, MinGW). Licencia zlib, en
  `SDL2/licencias/`.
- Fuente *Press Start 2P* (SIL Open Font License, en `fuentes/OFL.txt`).
