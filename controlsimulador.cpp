#include "controlsimulador.h"
#include <QTime>
#include <QEventLoop>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <math.h>
#include <ctime>
#include "proceso.h"
#include "cpu.h"
#include "timers.h"


void delay2( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

//Crea un procesador y agrega a lista
ControlSimulador::ControlSimulador()
{
    procesadores.push_back(new CPU());
    procesadores.push_back(new CPU());
    apagar=false;
    pausa=false;
}

 //Libera procesos y procesadores pendientes alterminar simulacion
ControlSimulador::~ControlSimulador()
{
 apagar=true;
 for(int i=0; i<procesos.size(); i++){
     delete procesos[i];
 }
 procesos.clear();

 Proceso::cuenta = 0; //inicializa contador de procesos vivos

 for(int i=0; i<procesadores.size(); i++){
     delete procesadores[i];
 }
 procesadores.clear();
}


// Inicia ciclo de simulación con Round-Robin
void ControlSimulador::RoundRobin()
{
	double z;
	double suma[10000];
	QString num;
	//Initializing our beautiful console (With debug purposes).
	consola = new ConsolaProceso();	
	consola->setWindowTitle( QString("Round Robin Processing Times"));
	consola->show();
	clock_t tStart = clock();
	while( apagar==false && procesos.size()>=1){
		if(pausa==false) {
			for(int i=0; i<procesos.size(); i++) {
				clock_t tStart = clock();			
				//esta con dos procesadores alternando ejecucion
				procesadores[i%2]->ejecuta( procesos[i] );
				int j = i;
				if (apagar==true){
					return;
				}
				
				if ( procesos[i]->estado == FINALIZADO ) {
					delay2(4000);
					delete procesos[i]; //liberar de memoria el proceso
					procesos.remove(i); //quitar de la lista
					num.setNum(suma[j]);
					consola->Display("For process ROUND ROBIN");
					consola->Display(num);
					i--;
				}
				z = (double)(clock() - tStart)/CLOCKS_PER_SEC; //It stops timer
				suma[j] = suma[j] + z;
				delay2(50);
				//delay2(5000);
				
			}
			
		}
		//delay2(5000);
	}
	QString b;
	b.setNum((double)(clock() - tStart)/CLOCKS_PER_SEC); //It stops timer
	consola->Display("Total running time with RoundRobin:");
	consola->Display(b);
	
}

//Inicia ciclo de simulación con FIFO
void ControlSimulador::FIFO()
{
	consola = new ConsolaProceso();	
	consola->setWindowTitle( QString("FIFO Processing Times"));
	consola->show();
	clock_t tStart = clock();
	while( apagar==false && procesos.size()>=1){
		
		if (pausa==false){
			for(int i=0; i<procesos.size(); i++){
				clock_t tStart = clock();
				//esta con dos procesadores alternando ejecucion
				procesadores[i]->ejecuta( procesos[i] );
				
				//Espera a que termine el proceso
				while( procesos[i]->estado != FINALIZADO ){
					procesadores[i]->ejecuta( procesos[i] );
					if (apagar==true){
						return;
					}
				}
				
				//delay2(4000);
				QString a;
				a.setNum((double)(clock() - tStart)/CLOCKS_PER_SEC); //It stops timer
				QString b;
				b.setNum((double)(clock())/CLOCKS_PER_SEC); //It stops timer
				QString c;
				c.setNum((double)(tStart)/CLOCKS_PER_SEC); //It stops timer
				consola->Display("For process FIFO");	
				consola->Display("Time start");
				consola->Display(c);
				consola->Display("Time finish");
				consola->Display(b);
				consola->Display("Duration");
				consola->Display(a);
				consola->Display("----------------");
				delete procesos[i]; //liberar de memoria el proceso
				procesos.remove(i); //quitar de la lista
				i--;
			}
		}
		//delay2(5000);
	}
	QString b;
	b.setNum((double)(clock() - tStart)/CLOCKS_PER_SEC); //It stops timer
	consola->Display("Total running time with FIFO:");				
	consola->Display(b);
}

//Inicia ciclo de simulación con SJF (Trabajo más corto primero)
void ControlSimulador::SJF()
{
	consola = new ConsolaProceso();	
	consola->setWindowTitle( QString("SJF Processing Times"));
	consola->show();
	int estado;
	estado = Ordenar(0);
	clock_t tStart = clock();
	while( apagar == false && procesos.size()>=1){
		if (pausa == false){
			for(int i=0; i<procesos.size(); i++){
				clock_t tStart = clock();
				//esta con dos procesadores alternando ejecucion
				procesadores[i]->ejecuta( procesos[i] );
				
				//Espera a que termine el proceso
				while( procesos[i]->estado != FINALIZADO){
					procesadores[i]->ejecuta( procesos[i] );
					if (apagar==true){
						return;
					}
				}
				QString a;
				a.setNum((double)(clock() - tStart)/CLOCKS_PER_SEC); //It stops timer
				QString b;
				b.setNum((double)(clock())/CLOCKS_PER_SEC); //It stops timer
				QString c;
				c.setNum((double)(tStart)/CLOCKS_PER_SEC); //It stops timer
				consola->Display("For process SJF");	
				consola->Display("Time start");
				consola->Display(c);
				consola->Display("Time finish");
				consola->Display(b);
				consola->Display("Duration");
				consola->Display(a);
				consola->Display("----------------");
				//delay2(4000);
				delete procesos[i]; //liberar de memoria el proceso
				procesos.remove(i); //quitar de la lista
				i--;
			}
		}
		//delay2(5000);
	}
	QString b;
	b.setNum((double)(clock() - tStart)/CLOCKS_PER_SEC); //It stops timer
	consola->Display("Total running time with SJF:");				
	consola->Display(b);	

}

int ControlSimulador::Ordenar(int parametroOrdenamiento){
    Proceso *aux;
    printf("Se ordenaran los procesos");

    if(parametroOrdenamiento){

        for(int i=1;i<procesos.size();i++){
            for(int j=0;j<procesos.size()-i;j++){

                if(procesos[j]->prioridad>procesos[j+1]->prioridad){
                    aux = procesos[j];
                    procesos[j] = procesos[j+1];
                    procesos[j+1] = aux;
                }
            }
    }
    }
    else{
        for(int i=1;i<procesos.size();i++){
            for(int j=0;j<procesos.size()-i;j++){

                if(procesos[j]->memProcess>procesos[j+1]->memProcess){
                    aux = procesos[j];
                    procesos[j] = procesos[j+1];
                    procesos[j+1] = aux;
                }
            }
        }

        for(int i=0;i<procesos.size();i++){
            printf("\n El numero de instrucciones es de: %d \n",procesos[i]->memProcess);
        }
    }

    return 1;
}

void ControlSimulador::Prioridad()
{
    int estado;

        for(int i=0;i<procesos.size();i++){
            if(procesos[i]->memProcess < 50){
                procesos[i]->prioridad = ceil(procesos[i]->memProcess / 5) - 20;
            }else if(procesos[i]->memProcess < 550){
                procesos[i]->prioridad = ceil((procesos[i]->memProcess - 49) / 50) - 10;
            }else if(procesos[i]->memProcess < 5550){
                procesos[i]->prioridad = ceil((procesos[i]->memProcess - 549) / 500);
            }else if(procesos[i]->memProcess < 55550){
                procesos[i]->prioridad = ceil((procesos[i]->memProcess - 5549) / 5000) + 10;
            }else procesos[i]->prioridad = 20;
        }



		estado = Ordenar(1);
		consola = new ConsolaProceso();	
		consola->setWindowTitle( QString("Priority Processing Times"));
		consola->show();
		clock_t tStart = clock();
		while( apagar == false && procesos.size()>=1){
			if (pausa == false){
				for(int i=0; i<procesos.size(); i++){
					clock_t tStart = clock();
					//esta con dos procesadores alternando ejecucion
					procesadores[i]->ejecuta( procesos[i] );
					
					//Espera a que termine el proceso
					while( procesos[i]->estado != FINALIZADO){
						procesadores[i]->ejecuta( procesos[i] );
						if (apagar==true){
							return;
						}
					}
					QString a;
					a.setNum((double)(clock() - tStart)/CLOCKS_PER_SEC); //It stops timer
					QString b;
					b.setNum((double)(clock())/CLOCKS_PER_SEC); //It stops timer
					QString c;
					c.setNum((double)(tStart)/CLOCKS_PER_SEC); //It stops timer
					consola->Display("For process Priority");	
					consola->Display("Time start");
					consola->Display(c);
					consola->Display("Time finish");
					consola->Display(b);
					consola->Display("Duration");
					consola->Display(a);
					consola->Display("----------------");
					//delay2(4000);
					delete procesos[i]; //liberar de memoria el proceso
					procesos.remove(i); //quitar de la lista
					i--;
				}
			}
			//delay2(5000);
		}
		QString b;
		b.setNum((double)(clock() - tStart)/CLOCKS_PER_SEC); //It stops timer
		consola->Display("Total running time with Priority:");				
		consola->Display(b);

}
