#ifndef CUCURUCHO_CONEXIONSERVIDOR_H
#define CUCURUCHO_CONEXIONSERVIDOR_H


#include <sys/socket.h>
#include "../protocols/protocolo.h"

class ConexionServidor {
public:
	ConexionServidor(int client_socket);

	struct Comando recibirMensaje();
	int enviarEstadoTick(struct EstadoTick* estadoTick);
	int enviarInformacionNivel(struct InformacionNivel* header);
    int sincronizarInicio();
    void cerrarConexion();
	//para logueo
    Logueo recibirDatosDeLogueo();
	void enviarSiContraseniaEsCorrecta(bool esCorrecta);
	int getClientSocket();

private:
	int client_socket;
	int receiveData(int* client_socket, struct Comando* comando);
	int sendDataEstadoTick(int* client_socket, struct EstadoTick* estadoTick);
	int sendDataInformacionNivel(int* client_socket, InformacionNivel *estadoTick);
	//para login
    int recibirUsuarioYContrasenia(int *client_socket, Logueo *logueo);
    int sendBool(int *client_socket, bool esCorrecta);
};


#endif //CUCURUCHO_CONEXIONSERVIDOR_H
