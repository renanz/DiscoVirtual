#ifndef BLOQUEARCHIVO_H
#define BLOQUEARCHIVO_H
#include <string.h>
#include "archivo.h"

class BloqueArchivo
{
    public:
        BloqueArchivo(char * nom, char * cont, int pb, int ub, int tam, Archivo * ar);

        char * getNombre();
        char * getContenido();
        int getPrimerBloque();
        int getUltimoBloque();
        int getTamanoBloque();

        void setContenido(char * contenido);
        void guardar();
        void initFromChar(char * data);

    private:
        char * nombre;
        char * contenido;
        int primerBloque, ultimoBloque, tamanoBloque;
        Archivo * ar;

        char * bloqueArchivoToChar();
};

#endif // BLOQUEARCHIVO_H
