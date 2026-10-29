# TPI-P2 · El Laberinto del Minotauro

Trabajo práctico integrador de Programación II (UTN FRGP, TUP). Videojuego en C++ con
**SDL 2** (+ SDL_image para PNG y SDL_ttf para fuentes).

## Cómo abrirlo

1. Instalar **Code::Blocks 25.03 con MinGW** (el instalador `codeblocks-25.03mingw-setup.exe`).
   No hace falta configurar nada más: SDL 2 ya viene dentro del proyecto.
2. Abrir `TPI-P2.cbp`.
3. **Build and run** (F9). Tiene que aparecer una grilla con un cuadrado dorado (Teseo)
   que se mueve con las flechas. ESC cierra. Es una demo provisoria dentro de `Juego`.

## Estructura

| Carpeta | Qué hay | Regla |
|---|---|---|
| `dominio/` | Las reglas del juego: `Entidad` y `Item` con sus hijas, `Laberinto`, `Nivel`, `Combate`, `Desafio`… y `Enumeradores.h` con **todos** los enums | **Nunca incluye `SDL.h`** |
| `persistencia/` | Los `Registro*` (lo que se graba) y los `Archivo*` (los únicos con `fopen`/`fread`/`fwrite`) | Registros planos: `char[]`, sin punteros, sin virtuales |
| `presentacion/` | `Pantalla`, `Sprite`, `Fuente`, `Menu`, `MenuAdministracion`, `Validador`, `Juego` | Lo único que dibuja o lee el teclado |

**Todas las clases ya existen**, con su `.h` y su `.cpp`, y ya están en `TPI-P2.cbp`. Cada uno
implementa los `.cpp` que tiene asignados (los que dicen `TODO`). Los `.h` son el acuerdo entre
nosotros: **si necesitás cambiar un `.h` ajeno, avisá antes** (en el issue o al líder), porque
otro puede estar usándolo.

## Cómo colaborar

1. Tomá un issue de la pestaña *Issues* (están por semana, en *Milestones*).
2. Trabajá en **una rama propia** (o en un fork del repositorio), nunca directo en `main`.
3. Cuando compile sin errores, abrí un **Pull Request** hacia `main` y nombrá el issue
   (`Cierra #12`). Solo el líder del proyecto (@IEchaburu) mergea a `main`.

Si nunca usaste git, instalá **GitHub Desktop**: clonar, crear rama, commitear y abrir el PR
se hace con botones.

## Reglas para trabajar en el proyecto

- **Cada `.cpp` o `.h` nuevo hay que agregarlo al proyecto** (*Project → Add files…*),
  si no Code::Blocks no lo compila. Debería hacer falta poco: las clases ya están creadas.
- Imágenes en `images/` (PNG o BMP) y fuentes en `fuentes/`. Se cargan con rutas relativas
  a la carpeta del proyecto (`"images/UTN.bmp"`), que es desde donde Code::Blocks ejecuta.
- **No tocar `SDL2/`** ni las DLL de la raíz: son la biblioteca tal cual la publica SDL.
- `bin/` y `obj/` los genera Code::Blocks; no se suben al repositorio.
- Estilo de la cátedra: atributos privados `_camelCase`, getters y setters, `enum class`,
  `nullptr`, `using namespace std;` solo en los `.cpp`, `#pragma once` en los `.h`. Todo
  `new` tiene su `delete` en el destructor. Sin `std::vector` (la memoria dinámica se
  demuestra con `new`/`delete[]`).
- Para ver cómo se usa SDL 2 (cargar una imagen, escribir texto), mirá el `main.cpp` del
  primer commit del repositorio.

## Para entregar o ejecutar fuera de Code::Blocks

El `.exe` necesita al lado: `SDL2.dll`, `SDL2_image.dll`, `SDL2_ttf.dll`, las carpetas
`images/` y `fuentes/`, y los `.dat`.

## Versiones y licencias

- SDL 2.32.10, SDL_image 2.8.12, SDL_ttf 2.24.0 (64 bits, MinGW). Licencia zlib, en
  `SDL2/licencias/`.
- Fuente *Press Start 2P* (SIL Open Font License, en `fuentes/OFL.txt`).
