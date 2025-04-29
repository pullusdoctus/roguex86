#include "menu.h"
#include <iostream>

const int ANCHO_VENTANA = 800;
const int ALTO_VENTANA = 600;
const char* RUTA_FUENTE = "assets/fonts/PressStart2P-Regular.ttf";
const int TAMANO_FUENTE = 24;

int main(int argc, char* argv[]) {
    SDL_Window* ventana = nullptr;
    SDL_Renderer* renderizador = nullptr;

    if (!iniciarSDL(&ventana, &renderizador, ANCHO_VENTANA, ALTO_VENTANA)) {
        std::cout << "Fallo al iniciar SDL.\n";
        return 1;
    }

    TTF_Font* fuente = cargarFuente(RUTA_FUENTE, TAMANO_FUENTE);
    if (!fuente) {
        std::cout << "Fallo al cargar la fuente.\n";
        limpiar(ventana, renderizador, nullptr);
        return 1;
    }

    mostrarMenu(renderizador, fuente);

    limpiar(ventana, renderizador, fuente);

    return 0;
}