#include "DiscoVirtual.h"

DiscoVirtual::DiscoVirtual(char * nombre, int tamDV)
{
    archivo = new Archivo(nombre, true);
    archivo->abrir();
    this->tamanoDiscoVirtual = tamDV;
    this->masterBlock = new MasterBlock(this->archivo, tamDV);
    cantidadEntradas = (this->masterBlock->getCantBloques-2)/28;
}

DiscoVirtual * DiscoVirtual::crearDiscoVirtual(char* nombreArchivo)
{
    DiscoVirtual * disc = new DiscoVirtual(nombreArchivo,this->getTamanoDiscoVirtual());
    disc->formatear();
    return disc;
}

void DiscoVirtual::asignarSigBloque(char * nombre, char * cont, int tam, int actual, bool esFolder)
{
    int pb = masterBlock->getSigBloque();
    int ub = tam/masterBlock->getTamBloque();

    char * data = archivo->leer(actual * masterBlock->getTamBloque(), masterBlock->getTamBloque());
    BloqueFolder * foldActual = new BloqueFolder("", -1, -1, archivo);
    BloqueFolder * bf;
    foldActual->initFromChar(data);

    FileEntry * fe = new FileEntry("", 0, 0, false);

    if(esFolder){
         bf = new BloqueFolder(nombre, pb, masterBlock->getTamBloque(), archivo);
        bf->guardar();
        masterBlock->setSigBloque(pb+1);

        fe = new FileEntry(bf->getNombre(), bf->getNumeroBloque(), bf->getNumeroBloque(), true);
    }
    else{
        BloqueArchivo * ba = new BloqueArchivo(nombre, cont, pb, ub, tam, archivo);
        ba->guardar();
        masterBlock->setSigBloque(ub+1);

        fe = new FileEntry(ba->getNombre(), ba->getPrimerBloque(), ba->getUltimoBloque(), false);
    }

    if(fe->getTamArchivo()<foldActual->getEspacioLibre()){
        foldActual->agregarEntry(fe);
        foldActual->guardar();
        masterBlock->setSigBloque(pb+1);
    }
    else{
        while(foldActual->getSig() != -1){
            data = archivo->leer(foldActual->getSig()*masterBlock->getTamBloque(), masterBlock->getTamBloque());
            foldActual->initFromChar(data);
        }
        foldActual->setSig(masterBlock->getSigBloque());
        foldActual->guardar();

        BloqueFolder * sigBF = new BloqueFolder(bf->getNombre(), foldActual->getSig(), masterBlock->getTamBloque(), archivo);
        sigBF->agregarEntry(fe);
        sigBF->guardar();
        masterBlock->setSigBloque(masterBlock->getSigBloque()+1);
    }
}

void DiscoVirtual::copyEntries(list<FileEntry*> * origen, list<FileEntry*> * destino){
    for(list<FileEntry*>::iterator it = origen->begin(); it!=origen->end(); it++){
        FileEntry * fe = *it;
        destino->push_back(fe);
    }
}

list<FileEntry *> DiscoVirtual::listarArchivos(int actual)
{
    char * data = archivo->leer((actual * masterBlock->getTamBloque()), masterBlock->getTamBloque());
    BloqueFolder * bf = new BloqueFolder("", 0, 0, archivo);
    bf->initFromChar(data);

    list<FileEntry*> entries;

    do{
        list<FileEntry*> origen = bf->getArchivoEntries();
        copyEntries(&origen, &entries);

        data = archivo->leer((bf->getSig()*masterBlock->getTamBloque()), masterBlock->getTamBloque());
        bf->initFromChar(data);
    }while(bf->getSig() != -1);

    return entries;
}

void DiscoVirtual::cargar()
{
    this->archivo->abrir();
    this->masterBlock = new MasterBlock(this->archivo, tamanoDiscoVirtual);
    this->masterBlock->cargar();
}

void DiscoVirtual::formatear()
{
    MasterBlock * mb = new MasterBlock(this->archivo, tamanoDiscoVirtual);
    mb->setSigBloque(2);
    mb->guardar();

    setMasterBlock(mb);
    archivo->setTamano(tamanoDiscoVirtual);

    BloqueFolder * raiz = new BloqueFolder("Raiz", 1, mb->getTamBloque(), archivo);
    raiz->guardar();
}

Archivo * DiscoVirtual::getArchivo()
{
    return this->archivo;
}

MasterBlock* DiscoVirtual::getMasterBlock()
{
    return this->masterBlock;
}

void DiscoVirtual::setMasterBlock(MasterBlock* mb)
{
    this->masterBlock = mb;
}

void DiscoVirtual::setArchivo(Archivo* ar)
{
    this->archivo = ar;
}
int DiscoVirtual::getTamanoDiscoVirtual()
{
    return this->tamanoDiscoVirtual;
}

void DiscoVirtual::setTamanoDiscoVirtual(int tamDV)
{
    this->tamanoDiscoVirtual = tamDV;
}

int DiscoVirtual::hash(char *key)
{
    int sum = 0;
        for (int k = 0; k < strlen(key); k++)
            sum = sum + int(key[k]);
    return  sum % cantidadEntradas;
}
