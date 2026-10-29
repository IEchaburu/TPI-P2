#pragma once
#include <string>

/// Registro de Acertijos.dat
class RegistroAcertijo
{
public:
    RegistroAcertijo();

    int getIdAcertijo();
    void setIdAcertijo(int idAcertijo);

    std::string getEnunciado();
    void setEnunciado(std::string enunciado);

    std::string getOpcionA();
    void setOpcionA(std::string opcionA);

    std::string getOpcionB();
    void setOpcionB(std::string opcionB);

    std::string getOpcionC();
    void setOpcionC(std::string opcionC);

    int getCorrecta();                    /// 1, 2 o 3
    void setCorrecta(int correcta);

    int getDificultad();
    void setDificultad(int dificultad);

    int getIdItemPremio();                /// relacion con Items.dat
    void setIdItemPremio(int idItemPremio);

    bool getEliminado();
    void setEliminado(bool eliminado);

private:
    int _idAcertijo;
    char _enunciado[200];
    char _opcionA[60];
    char _opcionB[60];
    char _opcionC[60];
    int _correcta;
    int _dificultad;
    int _idItemPremio;
    bool _eliminado;
};
