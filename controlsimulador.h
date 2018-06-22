#ifndef CONTROLSIMULADOR_H
#define CONTROLSIMULADOR_H
#include <QVector>
#include <QMessageBox>
#include "proceso.h"
#include "cpu.h"
#include "consolaproceso.h"

class ControlSimulador
{
public:

    ControlSimulador();
    ~ControlSimulador();
    QVector<Proceso *> procesos;
    QVector<CPU *> procesadores;
    void RoundRobin();
    void FIFO();
    void SJF();
    void Prioridad();
    int Ordenar(int parametroOrdenamiento);
    bool apagar;
    bool pausa;
	// ventana del proceso
	ConsolaProceso *consola;


};

#endif // CONTROLSIMULADOR_H
