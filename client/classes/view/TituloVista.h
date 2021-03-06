//
// Created by javier on 10/5/20.
//

#ifndef CUCURUCHO_TITULOVISTA_H
#define CUCURUCHO_TITULOVISTA_H

#include <SDL_render.h>
#include <SDL_rect.h>
#include <set>
#include <string>
#include "../Audio.h"

class TituloParticula;

class TituloVista {
    SDL_Texture* texturaTitulo;
    SDL_Texture* texturaParticulas;
    int alto;
    int ancho;
    int contador;
    int contadorActivada;
    bool conexionPerdida;
    SDL_Renderer* gRenderer;
    std::set<TituloParticula*> particulas;

    Audio *audio;
    std::string cancion;

public:
    TituloVista(int ancho, int alto, bool conexionPerdida);
    void
    render(int estado, int estadoLogin, std::string username, std::string password, bool seleccionadoUsuario);

    void nuevaParticula();

    void renderParticulas();

    void renderTitulo();

    void renderTextos(bool activa);

    void renderFadeout(bool activada);

    void renderInput(std::string username, std::string password, bool seleccionadoUsuario);

    void renderInfo(int estado, int i);


};


#endif //CUCURUCHO_TITULOVISTA_H
