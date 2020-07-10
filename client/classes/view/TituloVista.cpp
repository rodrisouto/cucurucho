//
// Created by javier on 10/5/20.
//
#include "TituloVista.h"
#include "../../../commons/utils/Vector.h"
#include "../GeneradorDeTexturas.h"
#include "../../../commons/utils/Utils.h"
#include "TextoVista.h"
#include "Titulo.h"
#include <string>

const int PARTICULA_ALTO = 21;
const int TITULO_ANCHO = 825;
const int TITULO_ALTO = 123;
const float PARTICULA_VELOCIDAD = 4.5;


class TituloParticula {
    Vector posicion;
    Vector velocidad;
    int contador;


public:
    explicit TituloParticula(const Vector &posicion, const Vector &velocidad) : velocidad(velocidad), posicion(posicion), contador(0) {}

    int getContador() const {
        return contador;
    }

    Vector getPosicion() const {
        return posicion;
    }

    void render(SDL_Renderer* gRenderer, SDL_Texture* texture) {
        int size = contador / 40;
        if (size > 3) size = 3;
        posicion = posicion + velocidad;
        SDL_Rect srcrect = {size * PARTICULA_ALTO, 0, PARTICULA_ALTO, PARTICULA_ALTO};
        SDL_Rect dstrect = {(int) posicion.getX(), (int) posicion.getY(), PARTICULA_ALTO, PARTICULA_ALTO};
        SDL_RenderCopy(gRenderer, texture, &srcrect, &dstrect);
        contador++;
    }

    bool fueraDePantalla(int ancho, int alto) {
        return posicion.getX() < - PARTICULA_ALTO
               || posicion.getX() > ancho + PARTICULA_ALTO
               || posicion.getY() < - PARTICULA_ALTO
               || posicion.getY() > alto + PARTICULA_ALTO;
    }
};

TituloVista::TituloVista(int ancho, int alto, bool conexionPerdida) {
    TituloVista::texturaTitulo = GeneradorDeTexturas::getInstance()->generarTextura("title.png");
    TituloVista::texturaParticulas = GeneradorDeTexturas::getInstance()->generarTextura("title_bg.png");
    TituloVista::alto = alto;
    TituloVista::ancho = ancho;
    TituloVista::conexionPerdida = conexionPerdida;
    TituloVista::contador = 0;
    TituloVista::contadorActivada = 0;
    TituloVista::gRenderer = GraphicRenderer::getInstance();

    TituloVista::audioInicio = Audio::getInstante();
    audioInicio->generarSoundEffect("sfx-26.wav");
    TituloVista::audioError = Audio::getInstante();
    audioError->generarSoundEffect("sfx-30.wav");
    TituloVista::audioErrorConexion = Audio::getInstante();
    audioError->generarAudio("audioNivel6.mp3");

}

void TituloVista::nuevaParticula() {
    float angulo = ((float) std::rand() / (RAND_MAX)) * 2 * PI;
    float velocidadEscalar = ((std::rand() % 3) + 1) * PARTICULA_VELOCIDAD;
    Vector velocidad = Vector(velocidadEscalar * cos(angulo), velocidadEscalar * sin(angulo));
    auto* particula = new TituloParticula(Vector(ancho / 2, alto / 2), velocidad);
    particulas.insert(particula);
}

void TituloVista::renderParticulas() {
    nuevaParticula();

    for (auto it = particulas.begin(); it != particulas.end(); ) {
        (*it)->render(gRenderer, texturaParticulas);
        if ((*it)->fueraDePantalla(ancho, alto)) {
            it = particulas.erase(it);
        } else {
            ++it;
        }
    }

    contador++;
}

void TituloVista::renderTextos(bool activa) {
    if (contador % (120 - (activa * 100)) < 60 - (activa * 50)) {
        TextoVista::eRender(std::string("PRESS ENTER"), Vector(ancho / 2, alto * 2 / 3), TEXTO_COLOR_AZUL, ALINEACION_CENTRO);
    }

    TextoVista::eRender(std::string("TALLER DE PROGRAMACION I"), Vector(ancho / 2, alto * 5 / 6), TEXTO_COLOR_AZUL, ALINEACION_CENTRO);
    TextoVista::eRender(std::string("2020 1C            "), Vector(ancho / 2, alto * 9 / 10), TEXTO_COLOR_ROJO, ALINEACION_CENTRO);
    TextoVista::eRender(std::string("        CUCURUCHO++"), Vector(ancho / 2, alto * 9 / 10), TEXTO_COLOR_NARANJA, ALINEACION_CENTRO);
}

void TituloVista::renderTitulo() {
    SDL_Rect srcrect = {0, 0, TITULO_ANCHO, TITULO_ALTO};
    SDL_Rect dstrect = {ancho / 2 - TITULO_ANCHO / 2, alto / 5, TITULO_ANCHO, TITULO_ALTO};
    SDL_RenderCopy(gRenderer, texturaTitulo, &srcrect, &dstrect);
}

void TituloVista::renderFadeout(bool activada) {
    if (!activada) return;

    int alpha = contadorActivada * 25;
    if (alpha > 255) alpha = 255;

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, alpha);
    SDL_Rect rect = {0, 0, ancho, alto};
    SDL_RenderFillRect(gRenderer, &rect);
    contadorActivada++;
}

void TituloVista::renderInput(std::string username, std::string password, bool seleccionadoUsuario) {
    Vector posicionUserLabel = Vector(ancho * 8 / 19, alto * 8 / 12);
    Vector posicionPassLabel = Vector(ancho * 8 / 19, alto * 9 / 12);

    Vector posicionUserInput = Vector(ancho * 9 / 19, alto * 8 / 12);
    Vector posicionPassInput = Vector(ancho * 9 / 19, alto * 9 / 12);

    std::string passwordInput = "";
    for (int i = 0; i < password.length(); i++) passwordInput += "*";

    TextoVista::eRender("USUARIO:", posicionUserLabel, TEXTO_COLOR_AZUL, ALINEACION_DERECHA);
    TextoVista::eRender("PASSWORD:", posicionPassLabel, TEXTO_COLOR_AZUL, ALINEACION_DERECHA);

    TextoVista::eRender(seleccionadoUsuario ? username + " <" : username, posicionUserInput, TEXTO_COLOR_ROJO, ALINEACION_IZQUIERDA);
    TextoVista::eRender(!seleccionadoUsuario ? passwordInput + " <" : passwordInput, posicionPassInput, TEXTO_COLOR_ROJO, ALINEACION_IZQUIERDA);

    if (conexionPerdida) {
        TextoVista::eRender(std::string("CONEXION PERDIDA"), Vector(ancho / 2, alto / 2), TEXTO_COLOR_ROJO, ALINEACION_CENTRO);
    }
}

void TituloVista::renderInfo(int estado, int estadoLogin) {
    Vector posicion = Vector(ancho / 2, alto * 5 / 6);
    std::string texto = "";
    if (estadoLogin == LOGIN_ERROR_PASS_INVALIDA) {
        texto = "ERROR DE AUTENTICACION";
    } else if (estadoLogin == LOGIN_ERROR_USUARIO_INEXISTENTE) {
        texto = "USUARIO NO REGISTRADO";
    } else if (estadoLogin == LOGIN_ERROR_USUARIO_EN_JUEGO) {
        texto = "USUARIO YA SE ENCUENTRA CONECTADO";
    } else if (estadoLogin == LOGIN_ERROR_EN_PARTIDA) {
        texto = "PARTIDA EN CURSO";
    } else if (estadoLogin == LOGIN_SIN_CONEXION) {
        audioError->playMusic(50);
        texto = "PROBLEMA DE CONEXION";
    } else if (estadoLogin == LOGIN_ESPERANDO_RESPUESTA) {
        texto = "ESPERANDO RESPUESTA...";
    } else if (estadoLogin > 0) {
        audioInicio->playSoundEffect(70);
        texto = "INICIANDO JUEGO";
    }
    TextoVista::eRender(texto, posicion, TEXTO_COLOR_ROJO, ALINEACION_CENTRO);
}

void
TituloVista::render(int estado, int estadoLogin, std::string username, std::string password, bool seleccionadoUsuario) {
    SDL_Rect posCampo = { 0, 0, ancho, alto };
    SDL_RenderSetViewport(GraphicRenderer::getInstance(), &posCampo);
    renderParticulas();
    renderTitulo();
    renderInfo(estado, estadoLogin);
    if (estado == TITULO_VACIO) renderTextos(false);
    else renderInput(username, password, seleccionadoUsuario);
    renderFadeout(estadoLogin > 0);
    contador++;
}