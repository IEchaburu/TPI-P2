#include <cstdio>
#include "ArchivoAcertijos.h"

using namespace std;

ArchivoAcertijos::ArchivoAcertijos(std::string fileName)
{
    _fileName = fileName;
}

ArchivoError ArchivoAcertijos::guardar(RegistroAcertijo reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "ab");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    escritura = fwrite(&reg, sizeof(RegistroAcertijo), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo escribir el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

RegistroAcertijo ArchivoAcertijos::leer(int indice){
    FILE *pFile;
    RegistroAcertijo reg;
    reg.setIdAcertijo(-1);

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return reg;
    }

    fseek(pFile, indice * sizeof(RegistroAcertijo), SEEK_SET);

    size_t lectura = fread(&reg, sizeof(RegistroAcertijo), 1, pFile);

    fclose(pFile);

    if(lectura != 1){
        _errorMessage = "No se pudo leer el registro";
        reg.setIdAcertijo(-1);
        return reg;
    }

    _errorMessage = "";
    return reg;
}

ArchivoError ArchivoAcertijos::leerTodos(RegistroAcertijo reg[], int cant){
    FILE *pFile;
    size_t lecturas;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    lecturas = fread(reg, sizeof(RegistroAcertijo), cant, pFile);
    fclose(pFile);

    if(lecturas != (size_t)cant){
        _errorMessage = "No se pudo leer los registros";
        return ArchivoError::LEER;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

ArchivoError ArchivoAcertijos::actualizar(int indice, RegistroAcertijo reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "rb+");   /// rb+ : lee y escribe SIN borrar el archivo

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    fseek(pFile, indice * sizeof(RegistroAcertijo), SEEK_SET);

    escritura = fwrite(&reg, sizeof(RegistroAcertijo), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo actualizar el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

ArchivoError ArchivoAcertijos::eliminar(int indice){
    RegistroAcertijo reg = leer(indice);

    if(reg.getIdAcertijo() == -1){
        return ArchivoError::LEER;
    }

    reg.setEliminado(true);
    return actualizar(indice, reg);
}
int ArchivoAcertijos::buscarByID(int id){
    FILE *pFile;
    RegistroAcertijo reg;
    int posicion;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return -1;
    }

    posicion = 0;
    while(fread(&reg, sizeof(RegistroAcertijo), 1, pFile) == 1){
        if(reg.getIdAcertijo() == id){
            fclose(pFile);
            return posicion;
        }
        posicion++;
    }

    fclose(pFile);
    return -1;
}

int ArchivoAcertijos::getCantidadRegistros(){
    FILE *pFile;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return 0;
    }

    fseek(pFile, 0, SEEK_END);
    int cantidad = ftell(pFile) / sizeof(RegistroAcertijo);

    fclose(pFile);

    return cantidad;
}

int ArchivoAcertijos::getNewID(){
    /// funciona porque nunca se borra fisicamente (solo borrado logico)
    return getCantidadRegistros() + 1;
}

std::string ArchivoAcertijos::getLastError(){
    return _errorMessage;
}