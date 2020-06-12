#ifndef CUCURUCHO_CONEXIONSERVIDOR_H
#define CUCURUCHO_CONEXIONSERVIDOR_H


#include <sys/socket.h>
#include "../protocols/protocolo.h"
#include "../../libraries/json/json.hpp"
#include "ConexionUtils.h"
#include "Conexion.h"


class ConexionServidor : public Conexion {
public:
	ConexionServidor(int client_socket);

	nlohmann::json recibirMensaje();
	void enviarMensaje(nlohmann::json mensaje);
	void enviarEstadoLogin(struct EstadoLogin estadoLogin);
	void cerrar();

    const std::string &getUsuario() const;

    void setUsuario(const std::string &usuario);

private:
	int client_socket;
	std::string usuario;
};


#endif //CUCURUCHO_CONEXIONSERVIDOR_H
