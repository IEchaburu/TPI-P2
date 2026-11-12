#include <cstdio>
#include "ArchivoEnemigos.h"

using namespace std;

ArchivoEnemigos::ArchivoEnemigos(std::string fileName)
{
    _fileName = fileName;
}

ArchivoError ArchivoEnemigos::guardar(RegistroEnemigo reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "ab");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    escritura = fwrite(&reg, sizeof(RegistroEnemigo), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo escribir el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

RegistroEnemigo ArchivoEnemigos::leer(int indice){
    FILE *pFile;
    RegistroEnemigo reg;
    reg.setIdEnemigo(-1);

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return reg;
    }

    fseek(pFile, indice * sizeof(RegistroEnemigo), SEEK_SET);

    size_t lectura = fread(&reg, sizeof(RegistroEnemigo), 1, pFile);

    fclose(pFile);

    if(lectura != 1){
        _errorMessage = "No se pudo leer el registro";
        reg.setIdEnemigo(-1);
        return reg;
    }

    _errorMessage = "";
    return reg;
}

ArchivoError ArchivoEnemigos::leerTodos(RegistroEnemigo reg[], int cant){
    FILE *pFile;
    size_t lecturas;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    lecturas = fread(reg, sizeof(RegistroEnemigo), cant, pFile);
    fclose(pFile);

    if(lecturas != (size_t)cant){
        _errorMessage = "No se pudo leer los registros";
        return ArchivoError::LEER;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

ArchivoError ArchivoEnemigos::actualizar(int indice, RegistroEnemigo reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "rb+");   /// rb+ : lee y escribe SIN borrar el archivo

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    fseek(pFile, indice * sizeof(RegistroEnemigo), SEEK_SET);

    escritura = fwrite(&reg, sizeof(RegistroEnemigo), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo actualizar el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

ArchivoError ArchivoEnemigos::eliminar(int indice){
    RegistroEnemigo reg = leer(indice);

    if(reg.getIdEnemigo() == -1){
        return ArchivoError::LEER;
    }

    reg.setEliminado(true);
    return actualizar(indice, reg);
}
int ArchivoEnemigos::buscarByID(int id){
    FILE *pFile;
    RegistroEnemigo reg;
    int posicion;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return -1;
    }

    posicion = 0;
    while(fread(&reg, sizeof(RegistroEnemigo), 1, pFile) == 1){
        if(reg.getIdEnemigo() == id){
            fclose(pFile);
            return posicion;
        }
        posicion++;
    }

    fclose(pFile);
    return -1;
}

int ArchivoEnemigos::getCantidadRegistros(){
    FILE *pFile;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return 0;
    }

    fseek(pFile, 0, SEEK_END);
    int cantidad = ftell(pFile) / sizeof(RegistroEnemigo);

    fclose(pFile);

    return cantidad;
}

int ArchivoEnemigos::getNewID(){
    /// funciona porque nunca se borra fisicamente (solo borrado logico)
    return getCantidadRegistros() + 1;
}

std::string ArchivoEnemigos::getLastError(){
    return _errorMessage;
}