#include "VidaEnemigo2.h"
#include "../../../../commons/utils/Log.h"

#define ENEMIGO2_ENERGIA_INICIAL 200

VidaEnemigo2::VidaEnemigo2() : VidaEntidad(ENEMIGO2_ENERGIA_INICIAL) {
}

int VidaEnemigo2::danioColisionDisparoJugador() {
	return 50;
}
