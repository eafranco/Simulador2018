#ifndef CONTROLSIMULADOR_H
#define CONTROLSIMULADOR_H
#include <QVector>
#include <QMessageBox>
#include "proceso.h"
#include "cpu.h"
<<<<<<< HEAD
#include "consolaproceso.h"
=======
>>>>>>> 6613bffbc211c3331f3089d9e222c6dad30fa7ad

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
<<<<<<< HEAD
	// ventana del proceso
	ConsolaProceso *consola;
=======
>>>>>>> 6613bffbc211c3331f3089d9e222c6dad30fa7ad

};

#endif // CONTROLSIMULADOR_H
