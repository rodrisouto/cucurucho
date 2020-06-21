#include "Conexion.h"
#include "ConexionUtils.h"
#include "ConexionExcepcion.h"

void Conexion::enviarData2(int client_socket, nlohmann::json dataJson) {

    std::string mensajeStr = dataJson.dump();
	uint32_t tamanioMensaje = mensajeStr.size();

    if (_enviarData(&client_socket, &tamanioMensaje, sizeof(tamanioMensaje)) < 0) {
		perror("Send Data Error 1");
		throw ConexionExcepcion();
	}
	if (_enviarData(&client_socket, &mensajeStr[0], tamanioMensaje) < 0) {
		perror("Send Data Error 2");
        throw ConexionExcepcion();
	}
}

nlohmann::json Conexion::recibirData2(int client_socket) {
    uint32_t tamanioMensaje;

    if (_recibirData<uint32_t>(&client_socket, &tamanioMensaje, sizeof(uint32_t)) < 0) {
        perror("recibirMensaje. Error cuando se recibe tamanioMensaje");
        throw ConexionExcepcion();
        // !!!! exit(1);
    }
    l->debug("conexionTamanio: " + std::to_string(tamanioMensaje));

    std::string mensajeStr(tamanioMensaje, '\0');
    if (_recibirData<char>(&client_socket, &mensajeStr[0], tamanioMensaje) < 0) {
        perror("recibirMensaje. Error cuando se recibe mensaje json");
        throw ConexionExcepcion();
        // !!!! exit(1);
    }

    l->debug("Conexion recibio:  " + mensajeStr);

    if (mensajeStr.empty()) {
        l->error("Conexion recibio un mensaje vacio");
        throw ConexionExcepcion();
    }

    nlohmann::json mensajeJson = nlohmann::json::parse(mensajeStr);
    return mensajeJson;
}
