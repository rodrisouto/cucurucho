#ifndef CUCURUCHO_CONFIGURACION_H
#define CUCURUCHO_CONFIGURACION_H

#include <string>
#include <queue>
#include <jsoncpp/json/json.h>
#include "NivelConfiguracion.h"

class Configuracion {
public:
    Configuracion(int altoPantalla, int anchoPantalla, int escalaPantalla, std::string nivelLog,
    		std::list<NivelConfiguracion*> niveles);

    int getAltoPantalla();
    int getAnchoPantalla();
    int getEscalaPantalla();
    std::string getNivelLog();
	const std::list<NivelConfiguracion *> &getNiveles() const;
	int getCantidadJugadores() { return 2; };

private:
    int altoPantalla;
    int anchoPantalla;
    int escalaPantalla;
    std::string nivelLog;
	std::list<NivelConfiguracion*> niveles;
};


#endif //CUCURUCHO_CONFIGURACION_H
