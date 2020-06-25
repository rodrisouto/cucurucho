#include "HiloOrquestadorPartida.h"
#include "../../commons/utils/Log.h"
#include "HiloConexionServidor.h"
#include "../../commons/ColaBloqueante.h"

void initializeData(struct EstadoTick* estadoTick);
bool processData(Partida *partida, Comando comandos[], EstadoTick *estadoTick, InformacionNivel *informacionNivel,
                 std::list<HiloConexionServidor *> *pList);
int esperarConexiones(int puerto, Configuracion* config);


HiloOrquestadorPartida::HiloOrquestadorPartida(Configuracion *config, std::list<HiloConexionServidor*>* hilosConexionesServidores) {
	this->partida = new Partida(config);
	this->hilosConexionesServidores = hilosConexionesServidores;
}


void receiveData(std::list<HiloConexionServidor*>* hilosConexionesServidores, struct Comando *comandos) {
    hilosConexionesServidores->reverse();

    std::string usuarioPerdido;

    try {
        // Uno de los clientes se queda esperando al resto, por eso siempre unos se ven mejor y otros peor.
        for (auto* hiloConexionServidor : *(hilosConexionesServidores)) {
            // Solo opero sobre hilos activos
            if(hiloConexionServidor->activo){
                auto* colaReceptora = hiloConexionServidor->colaReceptora;
                nlohmann::json mensajeJson;

                while (colaReceptora->size() > MAX_COLA_RECEPTORA_SERVIDOR) {
                    mensajeJson = colaReceptora->pop();
                }

                if(colaReceptora->size() > 0 && hiloConexionServidor->activo){
                    mensajeJson = colaReceptora->pop();

                    if (mensajeJson["_t"] == COMANDO) {
                        struct Comando comando = {mensajeJson["nroJugador"], mensajeJson["arriba"], mensajeJson["abajo"], mensajeJson["izquierda"], mensajeJson["derecha"]};
                        comandos[comando.nroJugador-1] = comando;
                    } else {
                        l->error("HiloOrquestadorPartida. Recibiendo mensaje invalido");
                    }
                }
            }
        }
    }
    catch(const std::exception &e) {
        // TODO: Algo mas para hacer tras eliminar al jugador de la lista y marcarlo como no presente?
        l->error("Ocurrio un error al recibir los movimientos de los jugadores");
        l->error(e.what());
    }


    // !!!! Dejo esta linea aca porque es muy buena
	/*
		struct Comando comando = comandos[i];
		if (comando.arriba || comando.abajo || comando.izquierda || comando.derecha) {
			huboMovimientos = true;
			l->error("!!!!! " + std::to_string(comando.nroJugador) + " | " + std::to_string(comando.arriba) + " - " + std::to_string(comando.abajo) + " - " + std::to_string(comando.izquierda) + " - " + std::to_string(comando.derecha));
			*/
}

void sendData(std::list<HiloConexionServidor*>* hilosConexionesServidores, struct InformacionNivel* informacionNivel, struct EstadoTick* estadoTick, int* nuevoNivel) {
	if (*nuevoNivel) {
		l->debug("Nuevo nivel enviando : " + std::to_string(informacionNivel->numeroNivel));
	}
	if (*nuevoNivel) {
		for (auto *hiloConexionServidor : *(hilosConexionesServidores)) {
			hiloConexionServidor->enviarInformacionNivel(informacionNivel);
		}
		*nuevoNivel = false;
	} else {
		for (auto *hiloConexionServidor : *(hilosConexionesServidores)) {
            hiloConexionServidor->enviarEstadoTick(estadoTick);
		}
		*nuevoNivel = estadoTick->nuevoNivel;
	}
}

void HiloOrquestadorPartida::run() {
	l->info("Comenzando a correr HiloOrquestadorPartida con " + std::to_string(hilosConexionesServidores->size()) + " jugadores.");
	bool quit = false;
	struct Comando comandos[hilosConexionesServidores->size()];
	struct InformacionNivel informacionNivel;
	struct EstadoTick estadoTick;
	bool terminoNivelActual = false;
	clock_t t2, t1 = clock();

	int commands_count = 0;
	initializeData(&estadoTick);
	// Todo esto es necesario?
	informacionNivel.numeroNivel = 0;
	// Comunicacion inicial.
	int nuevoNivel = 1;

	//keep communicating with client
	try {
		while (!quit) {

			t2 = clock();
			if ((t2 - t1) > 1000 * 1000 / 60) { // TODO jugar con estos valores afecta la performance, yo toco el ultimo nro para que sea divisor de 1 tick cada 60 sec.
			} else {
				continue;
			}

			for (int i = 0; i < hilosConexionesServidores->size(); i++) {
				comandos[i] = {0, 0, 0, 0, 0};
			}
			// Receive data (command)
			receiveData(hilosConexionesServidores, comandos);
            //--------------------
			// Process model
            quit |= processData(partida, comandos, &estadoTick, &informacionNivel, hilosConexionesServidores);
            if (quit) {
				break;
            }
            //--------------------
			// Send data (view)
            sendData(hilosConexionesServidores, &informacionNivel, &estadoTick, &nuevoNivel);
			//--------------------

			t1 = clock();
		}
	}
	catch (const std::exception &e) {
        // TODO stoppear hilosConexionesServidores
        // Si se llegó aca, quiere decir que no se pudo catchear la desconexion dentro de receiveData
        l->error("HiloOrquestadorPartida. Ocurrio un error en el main loop");
        l->error(e.what());
	}

	l->error("Esperando que terminen los hilosConexionesServidores");
	for (auto* hiloConexionServidor : *(hilosConexionesServidores)) {
		hiloConexionServidor->join();
	}

	l->info("Terminando de correr HiloOrquestadorPartida");
}

bool processData(Partida *partida, Comando comandos[], EstadoTick *estadoTick, InformacionNivel *informacionNivel,
                 std::list<HiloConexionServidor *> *conexiones) {
	EstadoInternoNivel estadoInternoNivel = partida->state(informacionNivel);
	partida->tick(comandos);

	if (partida->termino()) {
		l->error("!!!! La partida finalizo");
		return true;
	}

	// Seteando estadoTick
	estadoTick->nuevoNivel = estadoInternoNivel.nuevoNivel;
	EstadoInternoCampoMovil estadoCampoMovil = estadoInternoNivel.estadoCampoMovil;
	int i = 0;
	for (EstadoJugador estadoJugador : estadoCampoMovil.estadosJugadores) {
		estadoTick->estadosJugadores[i] = estadoJugador;
		i++;
	}

	for (HiloConexionServidor* h : *conexiones) {
	    estadoTick->estadosJugadores[h->jugador - 1].presente = h->activo;
	}

	i = 0;
	for (EstadoEnemigo estadoEnemigo : estadoCampoMovil.estadosEnemigos) {
		estadoTick->estadosEnemigos[i] = estadoEnemigo;
		i++;
	}
	// !!!! hardcodeadisimo
	for (; i < MAX_ENEMIGOS; i++) {
		estadoTick->estadosEnemigos[i].clase = 0;
	}

	return false;
}


void initializeData(struct EstadoTick* estadoTick) {
	for (int i = 0; i < MAX_JUGADORES; i++) {
		estadoTick->estadosJugadores[i].posicionX = -1000;
		estadoTick->estadosJugadores[i].posicionY = -1000;
        estadoTick->estadosJugadores[i].presente = 0;
        estadoTick->estadosJugadores[i].helper1.posicionX = -1000;
		estadoTick->estadosJugadores[i].helper1.posicionY = -1000;
		estadoTick->estadosJugadores[i].helper2.posicionX = -1000;
		estadoTick->estadosJugadores[i].helper2.posicionY = -1000;
	}

	for (int i = 0; i < MAX_ENEMIGOS; i++) {
		estadoTick->estadosEnemigos[i].clase = 0;
	}
}
