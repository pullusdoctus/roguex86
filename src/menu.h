#ifndef MENU_H
#define MENU_H

#include "SDL.h"
#include "SDL_ttf.h"

// Inicializa SDL y TTF, y crea ventana y renderer
bool iniciarSDL(SDL_Window** ventana, SDL_Renderer** renderizador, const int ancho, const int alto);

// Carga la fuente desde el archivo dado
TTF_Font* cargarFuente(const char* rutaFuente, int tamano);

// Renderiza un texto en pantalla
SDL_Texture* renderizarTexto(const char* mensaje, TTF_Font* fuente, SDL_Color color, SDL_Renderer* renderizador);

// Muestra el menú principal y maneja eventos
void mostrarMenu(SDL_Renderer* renderizador, TTF_Font* fuente);

// Libera todos los recursos y cierra SDL
void limpiar(SDL_Window* ventana, SDL_Renderer* renderizador, TTF_Font* fuente);

#endif