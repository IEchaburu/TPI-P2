#include <cstdio>
#include "ArchivoPartidas.h"

using namespace std;

ArchivoPartidas::ArchivoPartidas(std::string fileName)
{
    _fileName = fileName;
}

ArchivoError ArchivoPartidas::guardar(RegistroPartida reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "ab");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    escritura = fwrite(&reg, sizeof(RegistroPartida), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo escribir el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

RegistroPartida ArchivoPartidas::leer(int indice){
    FILE *pFile;
    RegistroPartida reg;
    reg.setIdPartida(-1);

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return reg;
    }

    fseek(pFile, indice * sizeof(RegistroPartida), SEEK_SET);

    size_t lectura = fread(&reg, sizeof(RegistroPartida), 1, pFile);

    fclose(pFile);

    if(lectura != 1){
        _errorMessage = "No se pudo leer el registro";
        reg.setIdPartida(-1);
        return reg;
    }

    _errorMessage = "";
    return reg;
}

ArchivoError ArchivoPartidas::leerTodos(RegistroPartida reg[], int cant){
    FILE *pFile;
    size_t lecturas;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    lecturas = fread(reg, sizeof(RegistroPartida), cant, pFile);
    fclose(pFile);

    if(lecturas != (size_t)cant){
        _errorMessage = "No se pudo leer los registros";
        return ArchivoError::LEER;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

ArchivoError ArchivoPartidas::actualizar(int indice, RegistroPartida reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "rb+");   /// rb+ : lee y escribe SIN borrar el archivo

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    fseek(pFile, indice * sizeof(RegistroPartida), SEEK_SET);

    escritura = fwrite(&reg, sizeof(RegistroPartida), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo actualizar el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

int ArchivoPartidas::buscarByID(int id){
    FILE *pFile;
    RegistroPartida reg;
    int posicion;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return -1;
    }

    posicion = 0;
    while(fread(&reg, sizeof(RegistroPartida), 1, pFile) == 1){
        if(reg.getIdPartida() == id){
            fclose(pFile);
            return posicion;
        }
        posicion++;
    }

    fclose(pFile);
    return -1;
}

int ArchivoPartidas::getCantidadRegistros(){
    FILE *pFile;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return 0;
    }

    fseek(pFile, 0, SEEK_END);
    int cantidad = ftell(pFile) / sizeof(RegistroPartida);

    fclose(pFile);

    return cantidad;
}

int ArchivoPartidas::getNewID(){
    /// funciona porque nunca se borra fisicamente (solo borrado logico)
    return getCantidadRegistros() + 1;
}

std::string ArchivoPartidas::getLastError(){
    return _errorMessage;
}