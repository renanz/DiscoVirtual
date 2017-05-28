#ifndef DISCOVIRTUAL_H
#define DISCOVIRTUAL_H
#include "archivo.h"
#include "masterblock.h"
#include "BloqueArchivo.h"
#include <list>
#include "fileentry.h"
#include "BloqueFolder.h"

class DiscoVirtual
{
    public:
        DiscoVirtual(char * nombre, int tamDV);

        DiscoVirtual * crearDiscoVirtual(char * nombreArchivo);
        void asignarSigBloque(char * nombre, char * cont, int tam, int actual, bool esFolder);

        Archivo * getArchivo();
        int getTamanoDiscoVirtual();
        MasterBlock * getMasterBlock();

        void setArchivo(Archivo * ar);
        void setTamanoDiscoVirtual(int tamDV);
        void setMasterBlock(MasterBlock * mb);

        void formatear();
        void cargar();
        list<FileEntry *> listarArchivos(int actual);

    private:
        Archivo * archivo;
        MasterBlock * masterBlock;
        int tamanoDiscoVirtual;

        void copyEntries(list<FileEntry*> * origen, list<FileEntry*> * destino);
};

#endif // DISCOVIRTUAL_H
