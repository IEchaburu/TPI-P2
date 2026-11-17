#include <iostream>
#include "Validador.h"

using namespace std;

const int MAXIMO_DIGITOS = 9;   /// asi el numero siempre entra en un int

/// Todas las lecturas son con getline (una linea entera): si se mezcla con cin >> queda
/// un Enter colgado en el buffer y la siguiente lectura sale vacia.

int Validador::pedirEntero(std::string mensaje, int minimo, int maximo)
{
    while(true){
        cout << mensaje;
        string linea;
        if(!getline(cin, linea)){
            /// se cerro la entrada (Ctrl+Z): no hay a quien seguir preguntando
            cin.clear();
            return minimo;
        }

        /// se aceptan espacios alrededor, un signo menos adelante y despues solo digitos
        int inicio = 0;
        int fin = (int)linea.size() - 1;
        while(inicio <= fin && linea[inicio] == ' '){
            inicio++;
        }
        while(fin >= inicio && linea[fin] == ' '){
            fin--;
        }

        bool negativo = false;
        if(inicio <= fin && linea[inicio] == '-'){
            negativo = true;
            inicio++;
        }

        int cantidadDigitos = fin - inicio + 1;
        bool esNumero = (cantidadDigitos >= 1 && cantidadDigitos <= MAXIMO_DIGITOS);
        int numero = 0;
        for(int i = inicio; i <= fin && esNumero; i++){
            if(linea[i] < '0' || linea[i] > '9'){
                esNumero = false;
            }
            else{
                numero = numero * 10 + (linea[i] - '0');
            }
        }

        if(!esNumero){
            cout << "Tiene que ser un numero entero." << endl;
            continue;
        }
        if(negativo){
            numero = -numero;
        }
        if(numero < minimo || numero > maximo){
            cout << "Tiene que estar entre " << minimo << " y " << maximo << "." << endl;
            continue;
        }
        return numero;
    }
}

std::string Validador::pedirTexto(std::string mensaje, int longitudMaxima)
{
    while(true){
        cout << mensaje;
        string linea;
        if(!getline(cin, linea)){
            /// se cerro la entrada: es el unico caso en que devuelve un texto vacio
            cin.clear();
            return "";
        }

        /// sin espacios al principio ni al final
        int inicio = 0;
        int fin = (int)linea.size() - 1;
        while(inicio <= fin && linea[inicio] == ' '){
            inicio++;
        }
        while(fin >= inicio && linea[fin] == ' '){
            fin--;
        }
        string texto = linea.substr(inicio, fin - inicio + 1);

        if(texto.empty()){
            cout << "No puede quedar vacio." << endl;
            continue;
        }
        if((int)texto.size() > longitudMaxima){
            cout << "Puede tener como mucho " << longitudMaxima << " caracteres." << endl;
            continue;
        }

        /// solo caracteres comunes: la consola de Windows no manda las tildes ni la enie
        /// en UTF-8, y la fuente del juego las dibujaria mal
        bool sonComunes = true;
        for(int i = 0; i < (int)texto.size(); i++){
            if(texto[i] < 32 || texto[i] > 126){
                sonComunes = false;
            }
        }
        if(!sonComunes){
            cout << "Sin tildes ni enie: solo letras comunes, numeros y signos." << endl;
            continue;
        }
        return texto;
    }
}

bool Validador::pedirConfirmacion(std::string mensaje)
{
    while(true){
        cout << mensaje << " (S/N): ";
        string linea;
        if(!getline(cin, linea)){
            cin.clear();
            return false;
        }
        if(linea == "S" || linea == "s"){
            return true;
        }
        if(linea == "N" || linea == "n"){
            return false;
        }
        cout << "Responder S o N." << endl;
    }
}
