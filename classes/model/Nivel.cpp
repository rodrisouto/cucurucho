#include <algorithm>
#include "Nivel.h"
#include "Enemigo2.h"
#include "Enemigo1.h"
#include "Jugador.h"
#include "../../server/classes/model/Nivel.h"

#include <queue>
#include <iterator>
#include <list>

Nivel::Nivel(NivelConfiguracion* nivelConfig, Jugador* jugador, struct InformacionFondo* informacionFondo) {
	Nivel::hud = new Hud();
	Nivel::velocidad = nivelConfig->getVelocidad();
	Nivel::ancho = nivelConfig->getLargo();
    Nivel::campo = crearCampo(nivelConfig, jugador, informacionFondo);
    Nivel::alto = campo->getAlto();
}

void Nivel::tick() {
    campo->tick();
	hud->actualizarHI(campo->getPosicion().getX());
	hud->tick();
	plantarSemillasEnCampo();
}

<<<<<<< HEAD
bool Nivel::termino() {
	bool verificacionPosicion = campo->verificarPosicion();
	if (verificacionPosicion) {
		l->debug("Termina el nivel? Evidentemente si.");
	}

	return verificacionPosicion;
}
=======
>>>>>>> develop_sokets_fondos

void Nivel::crearEnemigos(int cantClase1, int cantClase2) {
	crearEnemigosDeClase(2, cantClase2);
	crearEnemigosDeClase(1, cantClase1);
}

void Nivel::crearEnemigosDeClase(int tipoDeEnemigo, int cantDeEnemigos){
    for (int i = 0; i < cantDeEnemigos; i++) {
        int posInicialX = campo->getAncho();
        int posY = std::rand() % alto;

        int rangoEnemigos = (int) ancho - posInicialX;
        if (rangoEnemigos <= 0) {
            l->info("Ancho de pantalla mas grande que largo del nivel");
            rangoEnemigos = ancho;
        }
        int posXEnNivel = std::rand() % rangoEnemigos + posInicialX;
        float velocidadX = campo->getVelocidadX();

        Entidad* entidad;

        switch (tipoDeEnemigo) {
            case 1: {entidad = new Enemigo1(posXEnNivel, posY, velocidadX);}
            break;
            case 2: {entidad = new Enemigo2(posXEnNivel, posY, velocidadX);}
            break;
            // Todo después vemos
            default: {entidad = nullptr;};
        }

        SemillaEntidad* semillaEntidad = new SemillaEntidad(entidad, Vector(posXEnNivel, 0));
        // Esto se hace para que no queden parte fuera de la pantalla. Probablemente sólo para la primer entrega.
        if (campo->getAlto() < entidad->getPosicion().getY() + entidad->getAlto()) {
            i--;
            continue;
        }

        semillasEntidades.push_back(semillaEntidad);
    }
}

CampoMovil* Nivel::crearCampo(NivelConfiguracion* nivelConfig, Jugador* jugador, struct InformacionFondo* informacionFondo) {
	// TODO esto quedo muuuy sucio, venia asi desde antes, mientras la pantalla no sea configurable va como piña
	int inicioCampoEnEjeY = HUD_ALTO;
	auto* campo = new CampoMovil(jugador, PANTALLA_ANCHO, PANTALLA_ALTO - inicioCampoEnEjeY, inicioCampoEnEjeY, velocidad, ancho);

    for ( int i = 0; i < MAX_FONDOS ; i++ ){
        std::string str(informacionFondo[i].pFondo);
        campo->nuevoFondo(str, 0,0, informacionFondo[i].pVelocidad);
    }

	l->info("Se creo correctamente el nivel (Parallax)");
	return campo;
}

void Nivel::plantarSemillasEnCampo() {
	list<SemillaEntidad*> nuevasSemillasEntidades;

	while (!semillasEntidades.empty()) {
		SemillaEntidad* semillaEntidad = semillasEntidades.front();
		semillasEntidades.pop_front();

		int posXEnemigo = semillaEntidad->getPosicion().getX();
		int posicionXVentana = campo->getPosicion().getX() + campo->getAncho();

		if (posicionXVentana < posXEnemigo) {
			nuevasSemillasEntidades.push_back(semillaEntidad);
		} else {
			Entidad* entidad = semillaEntidad->getEntidad();
			campo->agregarEntidad(entidad);
		}
	}

	semillasEntidades = nuevasSemillasEntidades;
}

void Nivel::setEstado(std::list<EstadoEnemigo> estadosEnemigos) {
	campo->setEstadosEnemigos(estadosEnemigos);
}
