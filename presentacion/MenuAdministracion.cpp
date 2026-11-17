#include <iostream>
#include "MenuAdministracion.h"

using namespace std;

/// largos maximos de los textos: el tamanio del char[] del registro menos el '\0'
const int LARGO_NOMBRE = 29;          /// char[30]
const int LARGO_ENUNCIADO = 199;      /// char[200]
const int LARGO_OPCION = 59;          /// char[60]

/// rangos de los numeros
const int DIFICULTAD_MAXIMA = 10;
const int ID_SPRITE_MAXIMO = 99;      /// images/sprites/<id>.png (Juego carga del 0 al 99)
const int FILAS_MAXIMAS = 13;         /// mas grande no entra en la ventana del juego
const int COLUMNAS_MAXIMAS = 19;
const int LADO_MINIMO = 5;
const int SEMILLA_MAXIMA = 99999;

/// textos de los enum (para mostrar)

string textoTipoEnemigo(TipoEnemigo tipo)
{
    switch(tipo){
        case TipoEnemigo::COMUN: return "COMUN";
        case TipoEnemigo::ELITE: return "ELITE";
        case TipoEnemigo::JEFE:  return "JEFE";
    }
    return "?";
}

string textoTipoItem(TipoItem tipo)
{
    switch(tipo){
        case TipoItem::ARMA:     return "ARMA";
        case TipoItem::ARMADURA: return "ARMADURA";
        case TipoItem::RELIQUIA: return "RELIQUIA";
        case TipoItem::CURACION: return "CURACION";
    }
    return "?";
}

string textoTipoNivel(TipoNivel tipo)
{
    switch(tipo){
        case TipoNivel::COMBATE: return "COMBATE";
        case TipoNivel::DESAFIO: return "DESAFIO";
        case TipoNivel::JEFE:    return "JEFE";
    }
    return "?";
}

///  menu principal del ABM
void MenuAdministracion::ejecutar()
{
    int opcion = -1;
    while(opcion != 0){
        cout << endl << "========== ADMINISTRACION ==========" << endl;
        cout << "1. Enemigos" << endl;
        cout << "2. Items" << endl;
        cout << "3. Acertijos" << endl;
        cout << "4. Niveles" << endl;
        cout << "0. Volver al juego" << endl;
        opcion = _validador.pedirEntero("Opcion: ", 0, 4);

        switch(opcion){
            case 1: menuEnemigos(); break;
            case 2: menuItems(); break;
            case 3: menuAcertijos(); break;
            case 4: menuNiveles(); break;
            default: break;
        }
    }
}

int MenuAdministracion::pedirAccion(std::string catalogo)
{
    cout << endl << "---------- " << catalogo << " ----------" << endl;
    cout << "1. Listar" << endl;
    cout << "2. Alta" << endl;
    cout << "3. Modificar" << endl;
    cout << "4. Baja" << endl;
    cout << "0. Volver" << endl;
    return _validador.pedirEntero("Opcion: ", 0, 4);
}

/// ==========
/// ENEMIGOS
/// ==========

void MenuAdministracion::menuEnemigos()
{
    int opcion = -1;
    while(opcion != 0){
        opcion = pedirAccion("ENEMIGOS");
        switch(opcion){
            case 1: listarEnemigos(); break;
            case 2: altaEnemigo(); break;
            case 3: modificarEnemigo(); break;
            case 4: bajaEnemigo(); break;
            default: break;
        }
    }
}

void MenuAdministracion::mostrarEnemigo(RegistroEnemigo registro)
{
    cout << "ID " << registro.getIdEnemigo() << " | " << registro.getNombre()
         << " | " << textoTipoEnemigo(registro.getTipo())
         << " | vida " << registro.getVidaMaxima() << ", ataque " << registro.getAtaque()
         << ", defensa " << registro.getDefensa()
         << " | exp " << registro.getExperienciaOtorgada()
         << " | dificultad " << registro.getDificultadMinima() << "-" << registro.getDificultadMaxima()
         << " | sprite " << registro.getIdSprite() << endl;
}

void MenuAdministracion::listarEnemigos()
{
    int cantidad = _archivoEnemigos.getCantidadRegistros();
    cout << endl << "--- Listado de enemigos ---" << endl;
    if(cantidad == 0){
        cout << "No hay enemigos cargados." << endl;
        return;
    }

    RegistroEnemigo *registros = new RegistroEnemigo[cantidad];
    if(registros == nullptr){
        cout << "No se pudo pedir memoria..." << endl;
        return;
    }
    if(_archivoEnemigos.leerTodos(registros, cantidad) != ArchivoError::OK){
        cout << _archivoEnemigos.getLastError() << endl;
        delete [] registros;
        return;
    }

    for(int i = 0; i < cantidad; i++){
        if(!registros[i].getEliminado()){
            mostrarEnemigo(registros[i]);
        }
    }
    delete [] registros;
}

void MenuAdministracion::pedirDatosEnemigo(RegistroEnemigo &registro)
{
    registro.setNombre(_validador.pedirTexto("Nombre: ", LARGO_NOMBRE));

    cout << "Tipo: 0 = COMUN, 1 = ELITE, 2 = JEFE (el JEFE es el Minotauro)" << endl;
    registro.setTipo((TipoEnemigo)_validador.pedirEntero("Tipo: ", 0, 2));

    registro.setVidaMaxima(_validador.pedirEntero("Vida (1 a 999): ", 1, 999));
    registro.setAtaque(_validador.pedirEntero("Ataque (1 a 99): ", 1, 99));
    registro.setDefensa(_validador.pedirEntero("Defensa (0 a 99): ", 0, 99));
    registro.setExperienciaOtorgada(_validador.pedirEntero("Experiencia que da (0 a 9999): ", 0, 9999));

    /// la maxima se pide desde la minima: asi nunca queda minima > maxima
    int minima = _validador.pedirEntero("Dificultad minima (1 a 10): ", 1, DIFICULTAD_MAXIMA);
    registro.setDificultadMinima(minima);
    registro.setDificultadMaxima(_validador.pedirEntero("Dificultad maxima (" + to_string(minima) + " a 10): ",
                                                        minima, DIFICULTAD_MAXIMA));

    registro.setIdSprite(_validador.pedirEntero("Sprite (0 a 99, images/sprites/<id>.png): ", 0, ID_SPRITE_MAXIMO));
}

void MenuAdministracion::altaEnemigo()
{
    cout << endl << "--- Alta de enemigo ---" << endl;
    RegistroEnemigo registro;
    registro.setIdEnemigo(_archivoEnemigos.getNewID());
    registro.setEliminado(false);
    cout << "ID: " << registro.getIdEnemigo() << endl;

    pedirDatosEnemigo(registro);

    if(_archivoEnemigos.guardar(registro) == ArchivoError::OK){
        cout << "Enemigo guardado." << endl;
    }
    else{
        cout << "No se pudo guardar: " << _archivoEnemigos.getLastError() << endl;
    }
}

void MenuAdministracion::modificarEnemigo()
{
    int cantidad = _archivoEnemigos.getCantidadRegistros();
    if(cantidad == 0){
        cout << "No hay enemigos cargados." << endl;
        return;
    }

    /// los id van de 1 a la cantidad de registros (nunca se borra fisicamente)
    int id = _validador.pedirEntero("ID del enemigo a modificar: ", 1, cantidad);
    int posicion = _archivoEnemigos.buscarByID(id);
    if(posicion == -1){
        cout << "No existe un enemigo con ese ID." << endl;
        return;
    }
    RegistroEnemigo registro = _archivoEnemigos.leer(posicion);
    if(registro.getEliminado()){
        cout << "Ese enemigo esta dado de baja." << endl;
        return;
    }

    cout << "Datos actuales: ";
    mostrarEnemigo(registro);
    cout << "Ingresar los datos nuevos:" << endl;
    pedirDatosEnemigo(registro);   /// el id y el eliminado quedan como estaban

    if(_archivoEnemigos.actualizar(posicion, registro) == ArchivoError::OK){
        cout << "Enemigo modificado." << endl;
    }
    else{
        cout << "No se pudo modificar: " << _archivoEnemigos.getLastError() << endl;
    }
}

void MenuAdministracion::bajaEnemigo()
{
    int cantidad = _archivoEnemigos.getCantidadRegistros();
    if(cantidad == 0){
        cout << "No hay enemigos cargados." << endl;
        return;
    }

    int id = _validador.pedirEntero("ID del enemigo a dar de baja: ", 1, cantidad);
    int posicion = _archivoEnemigos.buscarByID(id);
    if(posicion == -1){
        cout << "No existe un enemigo con ese ID." << endl;
        return;
    }
    RegistroEnemigo registro = _archivoEnemigos.leer(posicion);
    if(registro.getEliminado()){
        cout << "Ese enemigo ya esta dado de baja." << endl;
        return;
    }

    mostrarEnemigo(registro);
    if(!_validador.pedirConfirmacion("Dar de baja este enemigo?")){
        cout << "No se dio de baja." << endl;
        return;
    }

    /// baja logica: el registro queda en el archivo con eliminado = true
    if(_archivoEnemigos.eliminar(posicion) == ArchivoError::OK){
        cout << "Enemigo dado de baja." << endl;
    }
    else{
        cout << "No se pudo dar de baja: " << _archivoEnemigos.getLastError() << endl;
    }
}

/// ======
/// ITEMS
/// ======

void MenuAdministracion::menuItems()
{
    int opcion = -1;
    while(opcion != 0){
        opcion = pedirAccion("ITEMS");
        switch(opcion){
            case 1: listarItems(); break;
            case 2: altaItem(); break;
            case 3: modificarItem(); break;
            case 4: bajaItem(); break;
            default: break;
        }
    }
}

void MenuAdministracion::mostrarItem(RegistroItem registro)
{
    cout << "ID " << registro.getIdItem() << " | " << registro.getNombre()
         << " | " << textoTipoItem(registro.getTipo())
         << " | poder " << registro.getPoder()
         << " | desde dificultad " << registro.getDificultadMinima()
         << " | sprite " << registro.getIdSprite() << endl;
}

void MenuAdministracion::listarItems()
{
    int cantidad = _archivoItems.getCantidadRegistros();
    cout << endl << "--- Listado de items ---" << endl;
    if(cantidad == 0){
        cout << "No hay items cargados." << endl;
        return;
    }

    RegistroItem *registros = new RegistroItem[cantidad];
    if(registros == nullptr){
        cout << "No se pudo pedir memoria..." << endl;
        return;
    }
    if(_archivoItems.leerTodos(registros, cantidad) != ArchivoError::OK){
        cout << _archivoItems.getLastError() << endl;
        delete [] registros;
        return;
    }

    for(int i = 0; i < cantidad; i++){
        if(!registros[i].getEliminado()){
            mostrarItem(registros[i]);
        }
    }
    delete [] registros;
}

void MenuAdministracion::pedirDatosItem(RegistroItem &registro)
{
    registro.setNombre(_validador.pedirTexto("Nombre: ", LARGO_NOMBRE));

    cout << "Tipo: 0 = ARMA (ataque), 1 = ARMADURA (defensa), 2 = RELIQUIA (vida maxima), 3 = CURACION (cura)" << endl;
    registro.setTipo((TipoItem)_validador.pedirEntero("Tipo: ", 0, 3));

    registro.setPoder(_validador.pedirEntero("Poder (1 a 999): ", 1, 999));
    registro.setDificultadMinima(_validador.pedirEntero("Aparece desde la dificultad (1 a 10): ", 1, DIFICULTAD_MAXIMA));
    registro.setIdSprite(_validador.pedirEntero("Sprite (0 a 99, images/sprites/<id>.png): ", 0, ID_SPRITE_MAXIMO));
}

void MenuAdministracion::altaItem()
{
    cout << endl << "--- Alta de item ---" << endl;
    RegistroItem registro;
    registro.setIdItem(_archivoItems.getNewID());
    registro.setEliminado(false);
    cout << "ID: " << registro.getIdItem() << endl;

    pedirDatosItem(registro);

    if(_archivoItems.guardar(registro) == ArchivoError::OK){
        cout << "Item guardado." << endl;
    }
    else{
        cout << "No se pudo guardar: " << _archivoItems.getLastError() << endl;
    }
}

void MenuAdministracion::modificarItem()
{
    int cantidad = _archivoItems.getCantidadRegistros();
    if(cantidad == 0){
        cout << "No hay items cargados." << endl;
        return;
    }

    int id = _validador.pedirEntero("ID del item a modificar: ", 1, cantidad);
    int posicion = _archivoItems.buscarByID(id);
    if(posicion == -1){
        cout << "No existe un item con ese ID." << endl;
        return;
    }
    RegistroItem registro = _archivoItems.leer(posicion);
    if(registro.getEliminado()){
        cout << "Ese item esta dado de baja." << endl;
        return;
    }

    cout << "Datos actuales: ";
    mostrarItem(registro);
    cout << "Ingresar los datos nuevos:" << endl;
    pedirDatosItem(registro);

    if(_archivoItems.actualizar(posicion, registro) == ArchivoError::OK){
        cout << "Item modificado." << endl;
    }
    else{
        cout << "No se pudo modificar: " << _archivoItems.getLastError() << endl;
    }
}

void MenuAdministracion::bajaItem()
{
    int cantidad = _archivoItems.getCantidadRegistros();
    if(cantidad == 0){
        cout << "No hay items cargados." << endl;
        return;
    }

    int id = _validador.pedirEntero("ID del item a dar de baja: ", 1, cantidad);
    int posicion = _archivoItems.buscarByID(id);
    if(posicion == -1){
        cout << "No existe un item con ese ID." << endl;
        return;
    }
    RegistroItem registro = _archivoItems.leer(posicion);
    if(registro.getEliminado()){
        cout << "Ese item ya esta dado de baja." << endl;
        return;
    }

    mostrarItem(registro);
    if(!_validador.pedirConfirmacion("Dar de baja este item?")){
        cout << "No se dio de baja." << endl;
        return;
    }

    if(_archivoItems.eliminar(posicion) == ArchivoError::OK){
        cout << "Item dado de baja." << endl;
    }
    else{
        cout << "No se pudo dar de baja: " << _archivoItems.getLastError() << endl;
    }
}

/// =========
/// ACERTIJOS
/// =========

void MenuAdministracion::menuAcertijos()
{
    int opcion = -1;
    while(opcion != 0){
        opcion = pedirAccion("ACERTIJOS");
        switch(opcion){
            case 1: listarAcertijos(); break;
            case 2: altaAcertijo(); break;
            case 3: modificarAcertijo(); break;
            case 4: bajaAcertijo(); break;
            default: break;
        }
    }
}

void MenuAdministracion::mostrarAcertijo(RegistroAcertijo registro)
{
    cout << "ID " << registro.getIdAcertijo() << " | dificultad " << registro.getDificultad()
         << " | premio: item " << registro.getIdItemPremio() << endl;
    cout << "   " << registro.getEnunciado() << endl;
    cout << "   1) " << registro.getOpcionA() << "   2) " << registro.getOpcionB()
         << "   3) " << registro.getOpcionC() << "   (correcta: " << registro.getCorrecta() << ")" << endl;
}

void MenuAdministracion::listarAcertijos()
{
    int cantidad = _archivoAcertijos.getCantidadRegistros();
    cout << endl << "--- Listado de acertijos ---" << endl;
    if(cantidad == 0){
        cout << "No hay acertijos cargados." << endl;
        return;
    }

    RegistroAcertijo *registros = new RegistroAcertijo[cantidad];
    if(registros == nullptr){
        cout << "No se pudo pedir memoria..." << endl;
        return;
    }
    if(_archivoAcertijos.leerTodos(registros, cantidad) != ArchivoError::OK){
        cout << _archivoAcertijos.getLastError() << endl;
        delete [] registros;
        return;
    }

    for(int i = 0; i < cantidad; i++){
        if(!registros[i].getEliminado()){
            mostrarAcertijo(registros[i]);
        }
    }
    delete [] registros;
}

bool MenuAdministracion::existeItem(int idItem)
{
    int posicion = _archivoItems.buscarByID(idItem);
    if(posicion == -1){
        return false;
    }
    return !_archivoItems.leer(posicion).getEliminado();
}

void MenuAdministracion::pedirDatosAcertijo(RegistroAcertijo &registro)
{
    registro.setEnunciado(_validador.pedirTexto("Enunciado: ", LARGO_ENUNCIADO));
    registro.setOpcionA(_validador.pedirTexto("Opcion 1: ", LARGO_OPCION));
    registro.setOpcionB(_validador.pedirTexto("Opcion 2: ", LARGO_OPCION));
    registro.setOpcionC(_validador.pedirTexto("Opcion 3: ", LARGO_OPCION));
    registro.setCorrecta(_validador.pedirEntero("Opcion correcta (1 a 3): ", 1, 3));
    registro.setDificultad(_validador.pedirEntero("Dificultad (1 a 10): ", 1, DIFICULTAD_MAXIMA));

    /// relacion entre archivos: el premio tiene que ser un item que exista y no este dado de baja
    listarItems();
    int cantidadItems = _archivoItems.getCantidadRegistros();
    int idPremio = _validador.pedirEntero("ID del item premio: ", 1, cantidadItems > 0 ? cantidadItems : 1);
    while(!existeItem(idPremio)){
        cout << "Ese item no existe o esta dado de baja." << endl;
        idPremio = _validador.pedirEntero("ID del item premio: ", 1, cantidadItems > 0 ? cantidadItems : 1);
    }
    registro.setIdItemPremio(idPremio);
}

void MenuAdministracion::altaAcertijo()
{
    cout << endl << "--- Alta de acertijo ---" << endl;

    /// sin items no hay premio posible
    if(_archivoItems.getCantidadRegistros() == 0){
        cout << "Primero hay que cargar items: el acertijo necesita un item de premio." << endl;
        return;
    }

    RegistroAcertijo registro;
    registro.setIdAcertijo(_archivoAcertijos.getNewID());
    registro.setEliminado(false);
    cout << "ID: " << registro.getIdAcertijo() << endl;

    pedirDatosAcertijo(registro);

    if(_archivoAcertijos.guardar(registro) == ArchivoError::OK){
        cout << "Acertijo guardado." << endl;
    }
    else{
        cout << "No se pudo guardar: " << _archivoAcertijos.getLastError() << endl;
    }
}

void MenuAdministracion::modificarAcertijo()
{
    int cantidad = _archivoAcertijos.getCantidadRegistros();
    if(cantidad == 0){
        cout << "No hay acertijos cargados." << endl;
        return;
    }

    int id = _validador.pedirEntero("ID del acertijo a modificar: ", 1, cantidad);
    int posicion = _archivoAcertijos.buscarByID(id);
    if(posicion == -1){
        cout << "No existe un acertijo con ese ID." << endl;
        return;
    }
    RegistroAcertijo registro = _archivoAcertijos.leer(posicion);
    if(registro.getEliminado()){
        cout << "Ese acertijo esta dado de baja." << endl;
        return;
    }

    cout << "Datos actuales:" << endl;
    mostrarAcertijo(registro);
    cout << "Ingresar los datos nuevos:" << endl;
    pedirDatosAcertijo(registro);

    if(_archivoAcertijos.actualizar(posicion, registro) == ArchivoError::OK){
        cout << "Acertijo modificado." << endl;
    }
    else{
        cout << "No se pudo modificar: " << _archivoAcertijos.getLastError() << endl;
    }
}

void MenuAdministracion::bajaAcertijo()
{
    int cantidad = _archivoAcertijos.getCantidadRegistros();
    if(cantidad == 0){
        cout << "No hay acertijos cargados." << endl;
        return;
    }

    int id = _validador.pedirEntero("ID del acertijo a dar de baja: ", 1, cantidad);
    int posicion = _archivoAcertijos.buscarByID(id);
    if(posicion == -1){
        cout << "No existe un acertijo con ese ID." << endl;
        return;
    }
    RegistroAcertijo registro = _archivoAcertijos.leer(posicion);
    if(registro.getEliminado()){
        cout << "Ese acertijo ya esta dado de baja." << endl;
        return;
    }

    mostrarAcertijo(registro);
    if(!_validador.pedirConfirmacion("Dar de baja este acertijo?")){
        cout << "No se dio de baja." << endl;
        return;
    }

    if(_archivoAcertijos.eliminar(posicion) == ArchivoError::OK){
        cout << "Acertijo dado de baja." << endl;
    }
    else{
        cout << "No se pudo dar de baja: " << _archivoAcertijos.getLastError() << endl;
    }
}

/// =========
/// NIVELES
/// =========

void MenuAdministracion::menuNiveles()
{
    int opcion = -1;
    while(opcion != 0){
        opcion = pedirAccion("NIVELES");
        switch(opcion){
            case 1: listarNiveles(); break;
            case 2: altaNivel(); break;
            case 3: modificarNivel(); break;
            case 4: bajaNivel(); break;
            default: break;
        }
    }
}

void MenuAdministracion::mostrarNivel(RegistroNivel registro)
{
    cout << "ID " << registro.getIdNivel() << " | piso " << registro.getNumeroPiso()
         << " | " << registro.getNombre() << " | " << textoTipoNivel(registro.getTipo())
         << " | " << registro.getFilas() << "x" << registro.getColumnas()
         << " | enemigos " << registro.getCantidadEnemigos() << ", cofres " << registro.getCantidadItems()
         << " | dificultad " << registro.getDificultad()
         << " | semilla " << registro.getSemilla() << endl;
}

void MenuAdministracion::listarNiveles()
{
    int cantidad = _archivoNiveles.getCantidadRegistros();
    cout << endl << "--- Listado de niveles ---" << endl;
    if(cantidad == 0){
        cout << "No hay niveles cargados." << endl;
        return;
    }

    RegistroNivel *registros = new RegistroNivel[cantidad];
    if(registros == nullptr){
        cout << "No se pudo pedir memoria..." << endl;
        return;
    }
    if(_archivoNiveles.leerTodos(registros, cantidad) != ArchivoError::OK){
        cout << _archivoNiveles.getLastError() << endl;
        delete [] registros;
        return;
    }

    for(int i = 0; i < cantidad; i++){
        if(!registros[i].getEliminado()){
            mostrarNivel(registros[i]);
        }
    }
    delete [] registros;
}

bool MenuAdministracion::pisoOcupado(int numeroPiso, int idPropio)
{
    /// recorre el archivo registro por registro: el que se modifica puede conservar su numero
    int cantidad = _archivoNiveles.getCantidadRegistros();
    for(int i = 0; i < cantidad; i++){
        RegistroNivel otro = _archivoNiveles.leer(i);
        if(!otro.getEliminado() && otro.getNumeroPiso() == numeroPiso && otro.getIdNivel() != idPropio){
            return true;
        }
    }
    return false;
}

int MenuAdministracion::pedirImpar(std::string mensaje, int minimo, int maximo)
{
    /// el generador de laberintos necesita filas y columnas impares
    int numero = _validador.pedirEntero(mensaje, minimo, maximo);
    while(numero % 2 == 0){
        cout << "Tiene que ser impar." << endl;
        numero = _validador.pedirEntero(mensaje, minimo, maximo);
    }
    return numero;
}

void MenuAdministracion::pedirDatosNivel(RegistroNivel &registro)
{
    registro.setNombre(_validador.pedirTexto("Nombre: ", LARGO_NOMBRE));

    /// dos pisos con el mismo numero harian que el juego saltee uno
    int numero = _validador.pedirEntero("Numero de piso (1 a 99): ", 1, 99);
    while(pisoOcupado(numero, registro.getIdNivel())){
        cout << "Ya hay otro nivel en ese piso." << endl;
        numero = _validador.pedirEntero("Numero de piso (1 a 99): ", 1, 99);
    }
    registro.setNumeroPiso(numero);

    cout << "Tipo: 0 = COMBATE, 1 = DESAFIO (puerta con acertijo), 2 = JEFE (arena del Minotauro)" << endl;
    registro.setTipo((TipoNivel)_validador.pedirEntero("Tipo: ", 0, 2));

    registro.setFilas(pedirImpar("Filas (impar, 5 a 13): ", LADO_MINIMO, FILAS_MAXIMAS));
    registro.setColumnas(pedirImpar("Columnas (impar, 5 a 19): ", LADO_MINIMO, COLUMNAS_MAXIMAS));
    registro.setCantidadEnemigos(_validador.pedirEntero("Cantidad de enemigos (0 a 20): ", 0, 20));
    registro.setCantidadItems(_validador.pedirEntero("Cantidad de cofres (0 a 10): ", 0, 10));
    registro.setDificultad(_validador.pedirEntero("Dificultad (1 a 10): ", 1, DIFICULTAD_MAXIMA));
    registro.setSemilla(_validador.pedirEntero("Semilla (0 = al azar, hasta 99999): ", 0, SEMILLA_MAXIMA));
}

void MenuAdministracion::altaNivel()
{
    cout << endl << "--- Alta de nivel ---" << endl;
    RegistroNivel registro;
    registro.setIdNivel(_archivoNiveles.getNewID());
    registro.setEliminado(false);
    cout << "ID: " << registro.getIdNivel() << endl;

    pedirDatosNivel(registro);

    if(_archivoNiveles.guardar(registro) == ArchivoError::OK){
        cout << "Nivel guardado." << endl;
    }
    else{
        cout << "No se pudo guardar: " << _archivoNiveles.getLastError() << endl;
    }
}

void MenuAdministracion::modificarNivel()
{
    int cantidad = _archivoNiveles.getCantidadRegistros();
    if(cantidad == 0){
        cout << "No hay niveles cargados." << endl;
        return;
    }

    int id = _validador.pedirEntero("ID del nivel a modificar: ", 1, cantidad);
    int posicion = _archivoNiveles.buscarByID(id);
    if(posicion == -1){
        cout << "No existe un nivel con ese ID." << endl;
        return;
    }
    RegistroNivel registro = _archivoNiveles.leer(posicion);
    if(registro.getEliminado()){
        cout << "Ese nivel esta dado de baja." << endl;
        return;
    }

    cout << "Datos actuales: ";
    mostrarNivel(registro);
    cout << "Ingresar los datos nuevos:" << endl;
    pedirDatosNivel(registro);

    if(_archivoNiveles.actualizar(posicion, registro) == ArchivoError::OK){
        cout << "Nivel modificado." << endl;
    }
    else{
        cout << "No se pudo modificar: " << _archivoNiveles.getLastError() << endl;
    }
}

void MenuAdministracion::bajaNivel()
{
    int cantidad = _archivoNiveles.getCantidadRegistros();
    if(cantidad == 0){
        cout << "No hay niveles cargados." << endl;
        return;
    }

    int id = _validador.pedirEntero("ID del nivel a dar de baja: ", 1, cantidad);
    int posicion = _archivoNiveles.buscarByID(id);
    if(posicion == -1){
        cout << "No existe un nivel con ese ID." << endl;
        return;
    }
    RegistroNivel registro = _archivoNiveles.leer(posicion);
    if(registro.getEliminado()){
        cout << "Ese nivel ya esta dado de baja." << endl;
        return;
    }

    mostrarNivel(registro);
    if(!_validador.pedirConfirmacion("Dar de baja este nivel?")){
        cout << "No se dio de baja." << endl;
        return;
    }

    if(_archivoNiveles.eliminar(posicion) == ArchivoError::OK){
        cout << "Nivel dado de baja." << endl;
    }
    else{
        cout << "No se pudo dar de baja: " << _archivoNiveles.getLastError() << endl;
    }
}
