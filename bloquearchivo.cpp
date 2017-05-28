#include "BloqueArchivo.h"

BloqueArchivo::BloqueArchivo(char * nom, char * cont, int pb, int ub, int tam, Archivo * ar){
    nombre = nom;
    contenido = cont;
    primerBloque = pb;
    ultimoBloque = ub;
    tamanoBloque = tam;
    this->ar = ar;
}

char * BloqueArchivo::getNombre(){
    return nombre;
}

char * BloqueArchivo::getContenido(){
    return contenido;
}

int BloqueArchivo::getPrimerBloque(){
    return primerBloque;
}

int BloqueArchivo::getUltimoBloque(){
    return ultimoBloque;
}

int BloqueArchivo::getTamanoBloque(){
    return tamanoBloque;
}

void BloqueArchivo::setContenido(char * contenido){
    this->contenido = contenido;
}

char * BloqueArchivo::bloqueArchivoToChar(){
    int tam = 20, pos = 0;
    tam += (strlen(nombre) + strlen(contenido));

    char * data = new char[tam];

    int tamNombre = strlen(nombre), tamContenido = strlen(contenido);

    memcpy(&(data[pos]), &tamNombre, 4);
    pos += 4;
    memcpy(&(data[pos]), nombre, tamNombre);
    pos += tamNombre;
    memcpy(&(data[pos]), &primerBloque, 4);
    pos += 4;
    memcpy(&(data[pos]), &ultimoBloque, 4);
    pos += 4;
    memcpy(&(data[pos]), &tamanoBloque, 4);
    pos += 4;
    memcpy(&(data[pos]), &tamContenido, 4);
    pos += 4;
    memcpy(&(data[pos]), contenido, tamContenido);

    return data;
}

void BloqueArchivo::guardar(){
    char * data = bloqueArchivoToChar();
    int longitud = (ultimoBloque-primerBloque+1) * tamanoBloque;
    ar->escribir(primerBloque*tamanoBloque, data, longitud);
}

void BloqueArchivo::initFromChar(char * data){
    int pos = 0, tamNombre = 0, tamContenido = 0;

    memcpy(&tamNombre, &(data[pos]), 4);
    pos += 4;
    nombre = new char[tamNombre];
    memcpy(nombre, &(data[pos]), tamNombre);
    pos += tamNombre;
    memcpy(&primerBloque, &(data[pos]), 4);
    pos += 4;
    memcpy(&ultimoBloque, &(data[pos]), 4);
    pos += 4;
    memcpy(&tamanoBloque, &(data[pos]), 4);
    pos += 4;
    memcpy(&tamContenido, &(data[pos]), 4);
    pos += 4;
    contenido = new char[tamContenido];
    memcpy(contenido, &(data[pos]), tamContenido);
}
