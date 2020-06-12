//
// Created by javier on 24/4/20.
//

#ifndef CUCURUCHO_TEXTOVISTA_H
#define CUCURUCHO_TEXTOVISTA_H

#include <SDL_render.h>
#include "../../../commons/utils/Vector.h"

const int TEXTO_COLOR_AZUL = 1;
const int TEXTO_COLOR_ROJO = 2;
const int TEXTO_COLOR_NARANJA = 3;
const int TEXTO_COLOR_VERDE = 4;
const int TEXTO_COLOR_ROSA = 5;
const int TEXTO_COLOR_TURQUESA = 6;

const int ALINEACION_IZQUIERDA = 1;
const int ALINEACION_DERECHA = 0;
const int ALINEACION_CENTRO = 2;

const int LETRA_ALTO = 24;
const int LETRA_ANCHO = 24;
const int ASCII_OFFSET = 32;

class TextoVista {
public:
	TextoVista(std::string* texto, Vector posicion, const int color, int alineacion);
	static void eRender(std::string texto, Vector posicion, const int color, int alineacion);
    void render();

    void setTexto(const std::string texto);
	void setPosicion(Vector posicion);

private:
    std::string* texto;
    Vector posicion;
    int color;
    int alineacion;
    SDL_Texture* textura;
    SDL_Renderer* gRenderer;

    void renderLetra(char c, int pos, int size);
};


#endif //CUCURUCHO_TEXTOVISTA_H
