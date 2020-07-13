//
// Created by javier on 24/4/20.
//

#ifndef CUCURUCHO_TEXTOVISTA_H
#define CUCURUCHO_TEXTOVISTA_H

#include <SDL_render.h>
#include "../../../commons/utils/Vector.h"

enum {
    TEXTO_NULO,
    TEXTO_COLOR_AZUL,
    TEXTO_COLOR_ROJO,
    TEXTO_COLOR_ROSA,
    TEXTO_COLOR_TURQUESA,
    TEXTO_COLOR_VERDE,
    TEXTO_COLOR_NARANJA
};

const int ALINEACION_IZQUIERDA = 1;
const int ALINEACION_DERECHA = 0;
const int ALINEACION_CENTRO = 2;

const int LETRA_ALTO = 24;
const int LETRA_ANCHO = 24;
const int ASCII_OFFSET = 30;

class TextoVista {
public:
	TextoVista(std::string* texto, Vector posicion, const int color, int alineacion);
	static void eRender(std::string texto, Vector posicion, const int color, int alineacion);
    void render();

    void setTexto(const std::string texto);
	void setPosicion(Vector posicion);

	std::string *getTexto() const;

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
