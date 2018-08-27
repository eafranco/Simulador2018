#ifndef CPU_H
#define CPU_H

#include "instrucciones.h"
#include "proceso.h"


class CPU
{
public:
    // Arquitectura de la máquina virtual ----------
    static int memory[MEMORY_SIZE];  //debe ser static para que sea compartida x varios procesadores
    int accumulator;  /**<Registro A*/
    int stackPointer; /**<Apuntador a memoria de la pila */
    int memPointer;  /**< Registro PC */
    int memProcess;  /**< cantidad de memoria para el proceso (instrucciones + variables)*/
    int estado; /**< nuevo,listo, ejecutando, bloqueado, finalizado */
    int machineOn;
    int iniMem;  /**<direccion de inicio de memoria asignada*/
    int finMem;  /**<direccion de fin de memoria asignada*/
    int memAsignada; /**<cantidad total de memoria asignada*/
    // ------------------------------------
    static int DEBUG;  /**< 1 activa paso a paso*/
    static int tiempo_espera;
    static int RAM; /**total de memoria al momento de la simulacion*/
    static int memoriaOcupada; /**<total de memoria ocupada por los procesos*/
    static int mayorMemProceso; /**<limite mayor de memoria usada por algun proceso*/
    static const int LAPSO=100;
    //int conta;
    int wait; /**<espera la lectura de datos en instruccion que Bloque*/
    Proceso *pProceso; /**< apuntador a proceso en ejecución*/


    /**
     * @brief CPU
     * Constructor
     */
    CPU();
    static void setDebug(int v);
    void saveState();
    void loadState();
    void checkAccumulator();
    void getInputVal();
    void runInstruction(int instr);
    void ejecuta(Proceso *p);

};

#endif // CPU_H
