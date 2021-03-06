#include "NivelConfiguracion.h"
#include "../../../commons/utils/Constantes.h"
#include <string>
#include <iostream>
#include <cstring>

NivelConfiguracion::NivelConfiguracion(std::list<FondoConfiguracion *> fondos, EnemigosConfiguracion *enemigos, const char* finalNivel,const char* audioNivel, float velocidad, float largo)
		: fondos(fondos), enemigos(enemigos), velocidad(velocidad), largo(largo) {
    strcpy(NivelConfiguracion::finalNivel, finalNivel);
    strcpy(NivelConfiguracion::audioNivel, audioNivel);

}

EnemigosConfiguracion *NivelConfiguracion::getEnemigos() const {
	return enemigos;
}

void NivelConfiguracion::getFinalNivel(char* final) const{
    strcpy(final, finalNivel);
}

void NivelConfiguracion::getAudioNivel(char* audio) const {
    strcpy(audio, audioNivel);
}

const char* NivelConfiguracion::getFinalNivel() const {
    return finalNivel;
}

float NivelConfiguracion::getVelocidad() const {
    return velocidad * (CAMPO_ANCHO / 960.0);
}

float NivelConfiguracion::getLargo() const {
    return largo;
}

const std::list<FondoConfiguracion *> &NivelConfiguracion::getFondos() const {
    return fondos;
}
