//
// Created by rodrigosouto on 30/5/20.
//

#ifndef CUCURUCHO_INICIADORCOMUNICACION_H
#define CUCURUCHO_INICIADORCOMUNICACION_H


#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "ConexionServidor.h"
#include "ConexionCliente.h"


class IniciadorComunicacion {
public:
	IniciadorComunicacion(char* ip_address, int port);

	ConexionCliente* conectar();

private:
	char* ip_address;
	int port;
};



#endif //CUCURUCHO_INICIADORCOMUNICACION_H
