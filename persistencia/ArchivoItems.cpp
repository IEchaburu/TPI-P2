#include <cstdio>
#include "ArchivoItems.h"

using namespace std;

ArchivoItems::ArchivoItems(std::string fileName)
{
    _fileName = fileName;
}

ArchivoError ArchivoItems::guardar(RegistroItem reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "ab");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    escritura = fwrite(&reg, sizeof(RegistroItem), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo escribir el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

RegistroItem ArchivoItems::leer(int indice){
    FILE *pFile;
    RegistroItem reg;
    reg.setIdItem(-1);

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return reg;
    }

    fseek(pFile, indice * sizeof(RegistroItem), SEEK_SET);

    size_t lectura = fread(&reg, sizeof(RegistroItem), 1, pFile);

    fclose(pFile);

    if(lectura != 1){
        _errorMessage = "No se pudo leer el registro";
        reg.setIdItem(-1);
        return reg;
    }

    _errorMessage = "";
    return reg;
}

ArchivoError ArchivoItems::leerTodos(RegistroItem reg[], int cant){
    FILE *pFile;
    size_t lecturas;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    lecturas = fread(reg, sizeof(RegistroItem), cant, pFile);
    fclose(pFile);

    if(lecturas != (size_t)cant){
        _errorMessage = "No se pudo leer los registros";
        return ArchivoError::LEER;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

ArchivoError ArchivoItems::actualizar(int indice, RegistroItem reg){
    FILE *pFile;
    size_t escritura;
    const int CANTIDAD_REGISTROS = 1;

    pFile = fopen(_fileName.c_str(), "rb+");   /// rb+ : lee y escribe SIN borrar el archivo

    if(pFile == nullptr){
        _errorMessage = "No se pudo abrir el archivo " + _fileName;
        return ArchivoError::ABRIR;
    }

    fseek(pFile, indice * sizeof(RegistroItem), SEEK_SET);

    escritura = fwrite(&reg, sizeof(RegistroItem), CANTIDAD_REGISTROS, pFile);

    fclose(pFile);

    if(escritura != CANTIDAD_REGISTROS){
        _errorMessage = "No se pudo actualizar el registro";
        return ArchivoError::ESCRIBIR;
    }

    _errorMessage = "";
    return ArchivoError::OK;
}

ArchivoError ArchivoItems::eliminar(int indice){
    RegistroItem reg = leer(indice);

    if(reg.getIdItem() == -1){
        return ArchivoError::LEER;
    }

    reg.setEliminado(true);
    return actualizar(indice, reg);
}
int ArchivoItems::buscarByID(int id){
    FILE *pFile;
    RegistroItem reg;
    int posicion;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return -1;
    }

    posicion = 0;
    while(fread(&reg, sizeof(RegistroItem), 1, pFile) == 1){
        if(reg.getIdItem() == id){
            fclose(pFile);
            return posicion;
        }
        posicion++;
    }

    fclose(pFile);
    return -1;
}

int ArchivoItems::getCantidadRegistros(){
    FILE *pFile;

    pFile = fopen(_fileName.c_str(), "rb");

    if(pFile == nullptr){
        return 0;
    }

    fseek(pFile, 0, SEEK_END);
    int cantidad = ftell(pFile) / sizeof(RegistroItem);

    fclose(pFile);

    return cantidad;
}

int ArchivoItems::getNewID(){
    /// funciona porque nunca se borra fisicamente (solo borrado logico)
    return getCantidadRegistros() + 1;
}

std::string ArchivoItems::getLastError(){
    return _errorMessage;
}