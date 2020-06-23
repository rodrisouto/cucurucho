//
// Created by camix on 10/6/20.
//

#ifndef CUCURUCHO_PARTIDA_H
#define CUCURUCHO_PARTIDA_H

#include <SDL_types.h>
#include <SDL_system.h>
#include "../../commons/protocols/protocolo.h"
#include "../../commons/connections/ConexionCliente.h"
#include "ManagerJuego.h"
#include "../../commons/connections/IniciadorComunicacion.h"
#include "view/Titulo.h"
#include "../../commons/utils/Constantes.h"
#include "../../commons/connections/IniciadorComunicacion.h"
#include "HiloConexionCliente.h"
#include "GraphicRenderer.h"
#include "view/Titulo.h"
#include "view/ManagerVista.h"
#include "ManagerJuego.h"

class Partida {
public:
    Partida();

    void play(const char *ip_address, int port);

private:
    Titulo* pantallaPrincipal;
    ManagerJuego* manager;
    HiloConexionCliente* hiloConexionCliente;
    IniciadorComunicacion* iniciadorComunicacion;
    ColaBloqueante<nlohmann::json>* colaMensajes;
    struct EstadoLogin estadoLogin;
    bool validarLogin;

    void conexionLoop(const Uint8 *currentKeyStates);

    void pantallaInicioLoop(std::string inputText, const Uint8 *currentKeyStates);

    bool eventLoop(std::string *inputText);

    void renderLoop();

    void autenticarServidor();

    void comenzarHilo();

    void reiniciarInstanciaHilo();

    void setEstadoTick(nlohmann::json mensaje);

    void setInformacionNivel(nlohmann::json mensaje);

    void setEstadoLogin(nlohmann::json mensaje);
};


#endif //CUCURUCHO_PARTIDA_H