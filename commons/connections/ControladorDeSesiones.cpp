//
// Created by ailen-magali on 3/6/20.
//

#include "ControladorDeSesiones.h"


ControladorDeSesiones::ControladorDeSesiones(ConexionServidor* conexionServidor){
    this->servidor = conexionServidor;
    ifstream archivo(JSON_USUARIOS, ifstream::binary);
    archivo >> this->jsonUsuarios;
    this->contrasenias = jsonUsuarios["usuariosRegistrados"];
}

bool ControladorDeSesiones::iniciarSesion(){

    bool ok = true;

    //pedirle un usuario y contraseña al cliente
    struct Login login;
    login = pedirCredenciales();

    char* usuario;
    usuario = login.usuario;
    char* contrasenia;
    contrasenia = login.contrasenia;

    //verifico que el usuario esté registrado
    if(!usuarioEstaRegistrado(usuario, contrasenia)){
        //TODO se le informa al cliente que no se le permitirá jugar
        this->servidor->cerrar();
        ok = false;
    } else {
        this->usuarioConectado = (string)usuario;
    }

    return ok;
}

bool ControladorDeSesiones::usuarioEstaRegistrado(char* usuario, char* contrasenia)
{
    bool usuarioRegistrado;
    bool contraseniaCorrecta;
    char* nuevaContrasenia;

    //chequeo si el usuario está registrado
    usuarioRegistrado = !(this->contrasenias[usuario].empty());

    //si está registrado, verifico la contraseña
    if(usuarioRegistrado){
        contraseniaCorrecta = (this->contrasenias[usuario] == contrasenia);
        while(!contraseniaCorrecta){
            this->servidor->enviarEstadoLogin(contraseniaCorrecta);
            nuevaContrasenia = pedirCredenciales().contrasenia;
            contraseniaCorrecta = (this->contrasenias[usuario] == nuevaContrasenia);
        }
    }
    this->servidor->enviarEstadoLogin(contraseniaCorrecta);

    return usuarioRegistrado;
}

struct Login ControladorDeSesiones::pedirCredenciales(){
    nlohmann::json mensajeJson = this->servidor->recibirMensaje();
    struct Login login;
    strcpy(login.usuario, std::string(mensajeJson["usuario"]).c_str());
    strcpy(login.contrasenia, std::string(mensajeJson["contrasenia"]).c_str());

    return login;
}

void ControladorDeSesiones::setServidor(ConexionServidor *servidor) {
    this->servidor = servidor;
}

string ControladorDeSesiones::userConectado(){
    return this->usuarioConectado;
}

void ControladorDeSesiones::controlarQueNoIngreseUsuarioYaEnJuego(map<string, bool> &jugadoresConectados){
    string usuario;
    usuario = this->usuarioConectado;
    map<string, bool>::iterator i = jugadoresConectados.find(usuario);

    if(i != jugadoresConectados.end()){ //si existe ese usuario en el map
        if(jugadoresConectados[usuario]){//si ese usuario está jugando
            //TODO informar que ya se encuentra en juego alguien con ese nombre de usuario
            cout<<"Ya se encuentra en juego alguien con ese nombre de usuario"<<endl;
            this->servidor->cerrar();
        } else if(!jugadoresConectados[usuario]){ //TODO si ese usuario se conectó en esta partida pero se fue, ponerle false
            cout<<"Te desconectaste y volviste"<<endl;
            jugadoresConectados[usuario] = true; //ver reconexión
        }
    } else { //si no se conectó en esta partida alguien con ese nombre de usuario
        cout<<"No se encuentra en juego todavía alguien con ese nombre de usuario"<<endl;
        jugadoresConectados[usuario] = true;
    }
}

