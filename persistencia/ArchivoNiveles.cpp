#include <cstdio>
#include "ArchivoNiveles.h"

using namespace std;

ArchivoNiveles::ArchivoNiveles(std::string fileName)
{
    _fileName = fileName;
}

ArchivoError ArchivoNiveles::guardar(RegistroNivel reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "ab");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    escritura = fwrite(&reg, sizeof(RegistroNivel), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo escribir el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

RegistroNivel ArchivoNiveles::leer(int indice){
    FILE *pFile;
    RegistroNivel reg;
    reg.setIdNivel(-1);

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return reg;
    }

    fseek(pFile, indice * sizeof(RegistroNivel), SEEK_SET);

    size_t lectura = fread(&reg, sizeof(RegistroNivel), 1, pFile);

    fclose(pFile);

    if(lectura != 1){
        _errorMessage = "No se pudo leer el registro";
        reg.setIdNivel(-1);
        return reg;
    }

    _errorMessage = "";
    return reg;
}

ArchivoError ArchivoNiveles::leerTodos(RegistroNivel reg[], int cant){
    FILE *pFile;
    size_t lecturas;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    lecturas = fread(reg, sizeof(RegistroNivel), cant, pFile);
    fclose(pFile);

    if(lecturas != (size_t)cant){
        _errorMessage = "No se pudo leer los registros";
        return ArchivoError::LEER;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

ArchivoError ArchivoNiveles::actualizar(int indice, RegistroNivel reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "rb+");   /// rb+ : lee y escribe SIN borrar el archivo

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    fseek(pFile, indice * sizeof(RegistroNivel), SEEK_SET);

    escritura = fwrite(&reg, sizeof(RegistroNivel), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo actualizar el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

ArchivoError ArchivoNiveles::eliminar(int indice){
    RegistroNivel reg = leer(indice);

    if(reg.getIdNivel() == -1){
        return ArchivoError::LEER;
    }

    reg.setEliminado(true);
    return actualizar(indice, reg);
}
int ArchivoNiveles::buscarByID(int id){
    FILE *pFile;
    RegistroNivel reg;
    int posicion;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return -1;
    }

    posicion = 0;
    while(fread(&reg, sizeof(RegistroNivel), 1, pFile) == 1){
        if(reg.getIdNivel() == id){
            fclose(pFile);
            return posicion;
        }
        posicion++;
    }

    fclose(pFile);
    return -1;
}

int ArchivoNiveles::getCantidadRegistros(){
    FILE *pFile;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return 0;
    }

    fseek(pFile, 0, SEEK_END);
    int cantidad = ftell(pFile) / sizeof(RegistroNivel);

    fclose(pFile);

    return cantidad;
}

int ArchivoNiveles::getNewID(){
    /// funciona porque nunca se borra fisicamente (solo borrado logico)
    return getCantidadRegistros() + 1;
}

std::string ArchivoNiveles::getLastError(){
    return _errorMessage;
}