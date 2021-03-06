//
// Created by camix on 16/4/20.
//

#ifndef CUCURUCHO_LOG_H
#define CUCURUCHO_LOG_H
#include <chrono>
#include <string>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <experimental/filesystem>

class Log {
private:
	std::string homePath;
    std::string nivel;
    char logEntrada [40]{};
    bool std_out;

public:
    Log(std::string basePath);

    void error(const std::string& string);
    void debug(const std::string& string);
    void info(const std::string& string);

    void setConf(std::string string);

    void output(const std::string& estado_log, const std::string& mensaje);
    void cargar_log(std::string log, time_t timestamp, const std::string& estado, const std::string& msj);

    static bool confValida(std::string nivel);

    void set_stdout(bool std_out);
};

extern Log* l;


#endif //CUCURUCHO_LOG_H
