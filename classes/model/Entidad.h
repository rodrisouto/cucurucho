#ifndef CUCURUCHO_ENTIDAD_H
#define CUCURUCHO_ENTIDAD_H


class Entidad : public Ticker {
public:
	virtual int getAncho() = 0;
	virtual int getAlto() = 0;
	virtual Vector getPosicion() = 0;
};


#endif //CUCURUCHO_ENTIDAD_H
