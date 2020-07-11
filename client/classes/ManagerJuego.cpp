//
// Created by camix on 10/6/20.
//

#include <iostream>
#include "ManagerJuego.h"
#include "../../commons/protocols/protocolo.h"
#include "../../commons/utils/Constantes.h"

ManagerJuego::ManagerJuego() {
    ManagerJuego::informacionNivel = {0}; // TODO patch para race conditions
    ManagerJuego::managerVista = new ManagerVista({}, 0, PANTALLA_ANCHO, PANTALLA_ALTO);
    ManagerJuego::estadoLogin = {LOGIN_PENDIENTE};
    ManagerJuego::estadoTick.numeroNivel = 0;
    ManagerJuego::estadoTick.nuevoNivel = 0;
}

void ManagerJuego::render(){
    // Render texture to screen
    managerVista->render(estadoTick, estadoLogin, username);
}

bool ManagerJuego::terminoJuego() {
    return estadoTick.nuevoNivel == FIN_DE_JUEGO || estadoTick.numeroNivel == FIN_DE_JUEGO;
}

// TODO: che tampoco da
bool ManagerJuego::enJuego() {
    return estadoLogin.estadoLogin == LOGIN_FIN;
}

void ManagerJuego::setInformacionNivel(const InformacionNivel &informacionNivel) {
    ManagerJuego::informacionNivel = informacionNivel;
	managerVista->setInformacionNivel(informacionNivel, estadoTick);
}

void ManagerJuego::setUsername(const std::string &username) {
    ManagerJuego::username = username;
}

void ManagerJuego::setEstadoTick(const EstadoTick &estadoTick) {
    if (ManagerJuego::estadoTick.numeroNivel == FIN_DE_JUEGO) return;
    ManagerJuego::estadoTick = estadoTick;
}

void ManagerJuego::setEstadoLogin(const EstadoLogin &estadoLogin) {
    ManagerJuego::estadoLogin = estadoLogin;
}
