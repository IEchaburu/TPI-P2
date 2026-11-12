#include <cstring>
#include "RegistroAcertijo.h"

RegistroAcertijo::RegistroAcertijo()
{
    _idAcertijo = 0;
    _enunciado[0] = '\0';
    _opcionA[0] = '\0';
    _opcionB[0] = '\0';
    _opcionC[0] = '\0';
    _correcta = 0;
    _dificultad = 0;
    _idItemPremio = 0;
    _eliminado = false;
}

int RegistroAcertijo::getIdAcertijo(){ return _idAcertijo; }
void RegistroAcertijo::setIdAcertijo(int idAcertijo){ _idAcertijo = idAcertijo; }

std::string RegistroAcertijo::getEnunciado(){
    return std::string(_enunciado);
}

void RegistroAcertijo::setEnunciado(std::string enunciado){
    /// se corta en 199 para dejar lugar al '\0'
    if(enunciado.size() <= 199){
        strcpy(_enunciado, enunciado.c_str());
    }
    else{
        strcpy(_enunciado, enunciado.substr(0, 199).c_str());
    }
}

std::string RegistroAcertijo::getOpcionA(){
    return std::string(_opcionA);
}

void RegistroAcertijo::setOpcionA(std::string opcionA){
    /// se corta en 59 para dejar lugar al '\0'
    if(opcionA.size() <= 59){
        strcpy(_opcionA, opcionA.c_str());
    }
    else{
        strcpy(_opcionA, opcionA.substr(0, 59).c_str());
    }
}

std::string RegistroAcertijo::getOpcionB(){
    return std::string(_opcionB);
}

void RegistroAcertijo::setOpcionB(std::string opcionB){
    /// se corta en 59 para dejar lugar al '\0'
    if(opcionB.size() <= 59){
        strcpy(_opcionB, opcionB.c_str());
    }
    else{
        strcpy(_opcionB, opcionB.substr(0, 59).c_str());
    }
}

std::string RegistroAcertijo::getOpcionC(){
    return std::string(_opcionC);
}

void RegistroAcertijo::setOpcionC(std::string opcionC){
    /// se corta en 59 para dejar lugar al '\0'
    if(opcionC.size() <= 59){
        strcpy(_opcionC, opcionC.c_str());
    }
    else{
        strcpy(_opcionC, opcionC.substr(0, 59).c_str());
    }
}

int RegistroAcertijo::getCorrecta(){ return _correcta; }
void RegistroAcertijo::setCorrecta(int correcta){ _correcta = correcta; }

int RegistroAcertijo::getDificultad(){ return _dificultad; }
void RegistroAcertijo::setDificultad(int dificultad){ _dificultad = dificultad; }

int RegistroAcertijo::getIdItemPremio(){ return _idItemPremio; }
void RegistroAcertijo::setIdItemPremio(int idItemPremio){ _idItemPremio = idItemPremio; }

bool RegistroAcertijo::getEliminado(){ return _eliminado; }
void RegistroAcertijo::setEliminado(bool eliminado){ _eliminado = eliminado; }
