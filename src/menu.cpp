#include "menu.h"
#include <iostream>

bool iniciarSDL(SDL_Window** ventana, SDL_Renderer** renderizador, const int ancho, const int alto) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cout << "Error al inicializar SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    *ventana = SDL_CreateWindow("Menu del Juego", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ancho, alto, SDL_WINDOW_SHOWN);
    if (!*ventana) {
        std::cout << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        return false;
    }

    *renderizador = SDL_CreateRenderer(*ventana, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderizador) {
        std::cout << "Error al crear el renderizador: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

TTF_Font* cargarFuente(const char* rutaFuente, int tamano) {
    TTF_Font* fuente = TTF_OpenFont(rutaFuente, tamano);
    if (!fuente) {
        std::cout << "Error al cargar la fuente: " << TTF_GetError() << std::endl;
    }
    return fuente;
}

SDL_Texture* renderizarTexto(const char* mensaje, TTF_Font* fuente, SDL_Color color, SDL_Renderer* renderizador) {
    SDL_Surface* superficie = TTF_RenderText_Solid(fuente, mensaje, color);
    if (!superficie) {
        std::cout << "Error al crear la superficie de texto: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderizador, superficie);
    SDL_FreeSurface(superficie);

    if (!textura) {
        std::cout << "Error al crear la textura de texto: " << SDL_GetError() << std::endl;
    }

    return textura;
}

void mostrarMenu(SDL_Renderer* renderizador, TTF_Font* fuente) {
    bool ejecutando = true;
    SDL_Event evento;

    SDL_Color colorTexto = {255, 255, 255, 255}; // Color blanco

    // Creamos los textos que queremos mostrar
    SDL_Texture* titulo = renderizarTexto("roguex86", fuente, colorTexto, renderizador);
    SDL_Texture* opcion1 = renderizarTexto("options", fuente, colorTexto, renderizador);
    SDL_Texture* opcion2 = renderizarTexto("new game", fuente, colorTexto, renderizador);
    SDL_Texture* opcion3 = renderizarTexto("EXIT", fuente, colorTexto, renderizador);

    // Si cualquiera falla, salimos
    if (!titulo || !opcion1 || !opcion2 || !opcion3) return;

    int texW, texH;
    int ancho_ventana = 800; // Ancho de la ventana
    int alto_ventana = 600;  // Alto de la ventana
    
    // Título centrado
    SDL_QueryTexture(titulo, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstTitulo = { (ancho_ventana - texW) / 2, 100, texW, texH };

    // Opciones horizontales
    SDL_QueryTexture(opcion1, nullptr, nullptr, &texW, &texH);
    int texW1 = texW;

    SDL_QueryTexture(opcion2, nullptr, nullptr, &texW, &texH);
    int texW2 = texW;

    SDL_QueryTexture(opcion3, nullptr, nullptr, &texW, &texH);
    int texW3 = texW;

    int espaciado = 50; // Espacio entre opciones
    int inicioX = (ancho_ventana - (texW1 + texW2 + texW3 + 2 * espaciado)) / 2;

    SDL_Rect dstOpcion1 = { inicioX, 200, texW1, texH };
    SDL_Rect dstOpcion2 = { inicioX + texW1 + espaciado, 200, texW2, texH };
    SDL_Rect dstOpcion3 = { inicioX + texW1 + texW2 + 2 * espaciado, 200, texW3, texH };

    while (ejecutando) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                ejecutando = false;
            }
            if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_ESCAPE) {
                ejecutando = false;
            }
        }

        SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255); // Fondo negro
        SDL_RenderClear(renderizador);

        // Dibujamos cada opción
        SDL_RenderCopy(renderizador, titulo, nullptr, &dstTitulo);
        SDL_RenderCopy(renderizador, opcion1, nullptr, &dstOpcion1);
        SDL_RenderCopy(renderizador, opcion2, nullptr, &dstOpcion2);
        SDL_RenderCopy(renderizador, opcion3, nullptr, &dstOpcion3);

        SDL_RenderPresent(renderizador);
    }

    // Liberamos todos los textos
    SDL_DestroyTexture(titulo);
    SDL_DestroyTexture(opcion1);
    SDL_DestroyTexture(opcion2);
    SDL_DestroyTexture(opcion3);
}

void limpiar(SDL_Window* ventana, SDL_Renderer* renderizador, TTF_Font* fuente) {
    if (fuente) TTF_CloseFont(fuente);
    if (renderizador) SDL_DestroyRenderer(renderizador);
    if (ventana) SDL_DestroyWindow(ventana);

    TTF_Quit();
    SDL_Quit();
}