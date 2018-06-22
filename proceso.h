#ifndef PROCESO_H
#define PROCESO_H
#include "consolaproceso.h"
#include "instrucciones.h"
#include "statusmemory.h"


class StatusMemory;

class Proceso
{
public:
    //atributos del BCP Bloque de Control del Proceso
    int id;
    static int cuenta; //cantidad de proceso vivos
    int DEBUG;
    int machineOn;
    int estado; //del proceso
    int memPointer; //posicion a instruccion actual
    int memProcess; //cantidad de instrucciones
    int stackPointer; //posicion a stack en memoria
    int accumulator;
    int wait;
    int prioridad;
    //--- al cargar archivo
    FILE * smlFile;
    QString nameFile;
    int *codigoMem; //memoria interna para almacenar codigo
    int iniMem;  //inicio de memoria asignada
    int finMem;  //fin de memoria asignada
    int memAsignada; //cantidad de memoria asignada


    // ventana del proceso
    ConsolaProceso *consola;
    //Ventana de estado del proceso
    StatusMemory *ventanaEstado;

    //metodos
    Proceso(QString nfile, QVector<Proceso*> processList, int indiceUI);
    ~Proceso();
    void loadSML(QVector<Proceso*> processList, int indiceUI);

    void MejorAjuste(QVector<Proceso*> processList);
    void PeorAjuste(QVector<Proceso*> processList);
    void PrimerAjuste(QVector<Proceso*> processList);
    Proceso(QString nfile);
    void loadSML();


};

#endif // PROCESO_H
