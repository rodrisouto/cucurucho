#include "HudVista.h"
#include "../GeneradorDeTexturas.h"
#include "../GraphicRenderer.h"
#include "ManagerVista.h"
#include <string>
#include "../../../commons/utils/Constantes.h"


HudVista::HudVista() {
	HudVista::gRenderer = GraphicRenderer::getInstance();
    GeneradorDeTexturas *generadorDeTexturas = GeneradorDeTexturas::getInstance();
    HudVista::textura = generadorDeTexturas->generarTextura("hud.png");

    this->nuevoTexto(new std::string("HI"), Vector(24, 24), TEXTO_COLOR_ROJO, true);
    this->nuevoTexto(new std::string("10000"), Vector(240, 24), TEXTO_COLOR_ROJO, false);
    this->nuevoTexto(new std::string("00"), Vector(240, 48), TEXTO_COLOR_NARANJA, false);

    this->nuevoTexto(new std::string("MOV NORMAL"), Vector(312, 24), TEXTO_COLOR_AZUL, true);

    this->nuevoTexto(new std::string("GUN TWIN"), Vector(312, 48), TEXTO_COLOR_NARANJA, true);

    this->nuevoTexto(new std::string("SPEED"), Vector(624, 24), TEXTO_COLOR_AZUL, true);
    this->nuevoTexto(new std::string(">"), Vector(624, 48), TEXTO_COLOR_ROJO, true);

    this->nuevoTexto(new std::string("NORMAL"), Vector(792, 24), TEXTO_COLOR_AZUL, true);

}

void HudVista::render() {
    SDL_Rect hudrect = {(PANTALLA_ANCHO - HUD_SRC_ANCHO) / 2, 0, HUD_SRC_ANCHO, HUD_SRC_ALTO};
	SDL_RenderSetViewport(gRenderer, &hudrect);

	SDL_Rect srcrect = {0, 0, HUD_SRC_ANCHO, HUD_SRC_ALTO};
	SDL_Rect dstrect = {0, 0, HUD_SRC_ANCHO, HUD_SRC_ALTO};
	SDL_RenderCopy(gRenderer, textura, &srcrect, &dstrect);

    for (TextoVista* c : textos){
        c->render();
    }

    if (toast != nullptr) {
        toast->render();
    }
}

void HudVista::nuevoTexto(std::string* texto, Vector posicion, int color, bool alineacionIzq) {
	textos.emplace_back(new TextoVista(texto, posicion, color, alineacionIzq));
}
