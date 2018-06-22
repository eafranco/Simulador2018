#include "proceso.h"
#include "cpu.h"
<<<<<<< HEAD
#include "hueco.h"
=======
>>>>>>> 6613bffbc211c3331f3089d9e222c6dad30fa7ad
#include <stdio.h>

int Proceso::cuenta=0;

<<<<<<< HEAD
bool processLessThan(Proceso *&p1, Proceso *&p2) {
    return p1->iniMem < p2->iniMem;
}

bool fragmentLessThan(Hole *&s1, Hole *&s2)
{
    return s1->size() < s2->size();
}

bool fragmentMoreThan(Hole *&s1, Hole *&s2)
{
    return s1->size() > s2->size();
}
=======
>>>>>>> 6613bffbc211c3331f3089d9e222c6dad30fa7ad

/**
 * @brief Proceso::Proceso
 * Constructor que crera el procesos, asignando valores a sus variables
 * carga archivo y determina memoria a usar, posicion de inicio y fin de memoria
 * y acumula en memoriaOcupada (desde loadSML() )
 * @param nfile
 */
<<<<<<< HEAD
Proceso::Proceso(QString nfile, QVector<Proceso*> processList, int indiceUI)
=======
Proceso::Proceso(QString nfile)
>>>>>>> 6613bffbc211c3331f3089d9e222c6dad30fa7ad
{
  //strcpy(nameFile,nfile);
  nameFile = nfile; //archivo con código maquina
  cuenta++;  //cantidad de procesos vivos
  id = cuenta;
  DEBUG = 1;  //1 = instruccion a instruccion se muestra
  machineOn=1; //encendido
  estado=NUEVO; //poner LISTO cuando quede el programa en memoria general
  memPointer=0; //determinar
  memProcess=0; //cantidad de instrucciones
  stackPointer=0; //determinar
  accumulator=0;
  memAsignada=0; //cantidad de memoria asignada
  wait=0;
  prioridad=1;

  consola = new ConsolaProceso();
  //consola->setWindowTitle( QString("Proceso ")+QString::number(id)+"  "+nameFile);
  consola->setWindowTitle( QString("Proceso ")+QString::number(id)+"  "+nameFile.split('/')[nameFile.split('/').length()-1]);
  consola->show();

  //al cargar el archivo se establecen limites de memoria y copian instrucciones
<<<<<<< HEAD
  loadSML(processList, indiceUI); //carga en memoria de proceso el código ejecutable
=======
  loadSML(); //carga en memoria de proceso el código ejecutable
>>>>>>> 6613bffbc211c3331f3089d9e222c6dad30fa7ad

  ventanaEstado=new StatusMemory();
  ventanaEstado->recibeProceso( this ); //pasa referencia del proceso

}

//Libera memoria de objetos y arreglos dinamicos
Proceso::~Proceso()
{
  delete consola;
  delete ventanaEstado;
  delete [] codigoMem;
}


//carga archivo a memoria del proceso
<<<<<<< HEAD
void Proceso::loadSML(QVector<Proceso*> processList, int indiceUI) {
=======
void Proceso::loadSML() {
>>>>>>> 6613bffbc211c3331f3089d9e222c6dad30fa7ad
    //QString display;
    int curInst=0;
    char stemp[5];

    consola->Display("Cargando: " + nameFile);

    smlFile = fopen(nameFile.toStdString().c_str(),"r");
    if (smlFile == NULL){
        consola->Display("Error al leer el archivo.\n");
        throw -1; //error
    }

    fscanf(smlFile, "%s %d",stemp, &memProcess);  //leer cantidad de instrucciones para el proceso
    //la primer linea del archivo en código maquina contiene la cantidad aproximada de instrucciones

    //limites de memoria
    //cantidad de memoria asignada en BLOQUES de 50 palabras (enteros)
    memAsignada = (memProcess - memProcess % 50) + ( (memProcess%50>0)?50:0); //bloques de 50 localidades

    CPU::memoriaOcupada += memAsignada; //acumular memoria pedida

    codigoMem = new int[memAsignada]; //memoria total asignada al proceso

<<<<<<< HEAD
    //*********************** PONER CODIGO AQUI ****************************

    //int indice_del_metodo = 1;
    int selectedAlgUI = indiceUI;

    switch(selectedAlgUI)
    {
        case 1:
            PrimerAjuste(processList);
            break;
        case 2:
            MejorAjuste(processList);
            break;
        case 3:
            PeorAjuste(processList);
            break;
    }

    //*********************TU CODIGO TERMINA AQUI***************************
=======

    iniMem = (id-1) * 200 ; //se asigna inicio segun ID ahorita (luego por algun metodo)
>>>>>>> 6613bffbc211c3331f3089d9e222c6dad30fa7ad

    finMem = iniMem + memAsignada - 1; //ultima posicion valida de memoria
    stackPointer = finMem;

    if (finMem > CPU::mayorMemProceso)
        CPU::mayorMemProceso=finMem;   //establece el valor de memoria mas alto usado por algun proceso

    for(int k = 0; k<memAsignada; k++)
        codigoMem[k] = 0; //inicializa memoria del proceso


    do { //copia codigo de archivo a memoria del proceso
        curInst = 0;
        fscanf(smlFile, "%d", &curInst);
        codigoMem[memPointer] = curInst;
        memPointer++;
    }while (!feof(smlFile));

    memProcess = memPointer-1;

    fclose(smlFile);
    consola->Display("Programa cargado a la memoria del proceso\n");
    memPointer = iniMem; 
}
<<<<<<< HEAD

void Proceso::MejorAjuste(QVector<Proceso*> processList)
{
	int iniMem = 0;

	int processCounter = 1;

    QVector<Hole* > fragmentList;

	// Qt function used to sort processes by their initial position
	qSort(processList.begin(), processList.end(), processLessThan);

	if(processList.size() == 0) {
	        // If the memmory is empty, the process is assigned at the beggining
	        this->iniMem = 0;
	    } else {
	        int previousProcessEnd;
	        int nextProcessStart;

	        // When there's only one process in memmory, one big fragment is created
	        nextProcessStart = processList.at(0)->iniMem;
            fragmentList.push_back(new Hole(0, nextProcessStart));

	        while(processCounter < processList.size()) {
	            previousProcessEnd = processList.at(processCounter-1)->finMem;
	            nextProcessStart = processList.at(processCounter)->iniMem;

                Hole *fragm = new Hole(previousProcessEnd, nextProcessStart);
	            fragmentList.push_back(fragm);

	            processCounter++;
	        }

	        // Add the fragment at the end of the memory (from end process to end of memory)
	        previousProcessEnd = processList.at(processList.size()-1)->finMem;
            fragmentList.push_back(new Hole(previousProcessEnd, CPU::RAM));

	       // Next, the fragmentList is sorted by ascending size
	        qSort(fragmentList.begin(), fragmentList.end(), fragmentLessThan);

	        // Now that the list of processes is sorted, we can assign the requesting processes to the nearest in size fragment of memmory
	        for(int i = 0; i < fragmentList.size(); i++) {
                Hole *fragm = fragmentList.at(i);

                if(fragm->size() >= memAsignada) {
                    iniMem = fragm->iniMem + 1;
	                break;
	            }
	        }

	        this->iniMem = iniMem;
	    }
}

void Proceso::PeorAjuste(QVector<Proceso*> processList)
{
    //int iniMem = 0; // <--- DEBES CALCULAR ESTO EN TU CODIGO
    // * CALCULAR LA POSICION DE MEMORIA DONDE EMPIEZA EL PROCESO
    // * Y GUARDARLA EN iniMem
    //this->iniMem = iniMem;

    int iniMem = 0;

    int processCounter = 1;

    QVector<Hole* > fragmentList;

    // sort processes by their initial position
    qSort(processList.begin(), processList.end(), processLessThan);

        if(processList.size() == 0) {
            // Asigna la primera posicion
            this->iniMem = 0;
        } else {
            // Variables which will be used to determine where a fragment starts and ends
            int previousProcessEnd;
            int nextProcessStart;

            // Add first fragment (from start of memory to first process)
            nextProcessStart = processList.at(0)->iniMem;
            fragmentList.push_back(new Hole(0, nextProcessStart));

            // Add middle fragments (from process to process)
            while(processCounter < processList.size()) {
                previousProcessEnd = processList.at(processCounter-1)->finMem;
                nextProcessStart = processList.at(processCounter)->iniMem;

                Hole *hueco = new Hole(previousProcessEnd, nextProcessStart);
                fragmentList.push_back(hueco);

                processCounter++;
            }

            // Add final fragment (from end process to end of memory)
            previousProcessEnd = processList.at(processList.size()-1)->finMem;
            fragmentList.push_back(new Hole(previousProcessEnd, CPU::RAM));

            // Next, the fragmentList is sorted by descending size (biggest fragment is at the beginning of the list)
            qSort(fragmentList.begin(), fragmentList.end(), fragmentMoreThan);

            // Now that the list of processes is sorted, we can assign the requesting processes to the nearest in size fragment of memmory
            for(int i = 0; i < fragmentList.size(); i++) {
                Hole *fragm = fragmentList.at(i);

                if(fragm->size() >= memAsignada) { // Control statement
                    iniMem = fragm->iniMem + 1;
                    break;
                }
            }

            this->iniMem = iniMem;
        }
}

void Proceso::PrimerAjuste(QVector<Proceso*> processList)
{
    int iniMem = 0; // <--- DEBES CALCULAR ESTO EN TU CODIGO

    // *********** CODIGO EMPIEZA AQUI *************
    // * DEBES CALCULAR LA POSICION DE MEMORIA DONDE EMPIEZA EL PROCESO
    // * Y GUARDARLA EN iniMem
    // *********************************************

    // Hole list
    QVector<Hole* > huecosList;
    static int lastPosition;
    int processCounter = 1;

    // sort processes by their initial position
    qSort(processList.begin(), processList.end(), processLessThan);


        if(processList.size() == 0) {
            // Asigna la primera posicion
            this->iniMem = 0;
        } else {
            // Variables which will be used to determine where a hole starts and ends
            int previousProcessEnd;
            int nextProcessStart;

            // Add first hole (from start of memory to first process)
            nextProcessStart = processList.at(0)->iniMem;
            huecosList.push_back(new Hole(0, nextProcessStart));

            // Add middle holes (from process to process)
            while(processCounter < processList.size()) {
                previousProcessEnd = processList.at(processCounter-1)->finMem;
                nextProcessStart = processList.at(processCounter)->iniMem;

                Hole *hueco = new Hole(previousProcessEnd, nextProcessStart);
                huecosList.push_back(hueco);

                processCounter++;
            }

            // Add final hole (from end process to end of memory)
            previousProcessEnd = processList.at(processList.size()-1)->finMem;
            huecosList.push_back(new Hole(previousProcessEnd, CPU::RAM));

            // Now that you have a list of holes you can choose based on whichever criterion
            // First check for the holes which are after the lastPosition
            // Loop back
            bool loopBack = true;
            for(int i = 0; i < huecosList.size(); i++) {
                Hole *hueco = huecosList.at(i);
                if(hueco->size() >= memAsignada && hueco->iniMem >= lastPosition) {
                    iniMem = hueco->iniMem + 1;
                    lastPosition = iniMem + memAsignada - 1;
                    loopBack = false;
                    break;
                }
            }

            // Now check the holes before the lastPosition
            if(loopBack) {
                for(int i = 0; i < huecosList.size(); i++) {
                    Hole *hueco = huecosList.at(i);
                    if(hueco->size() >= memAsignada && hueco->iniMem <= lastPosition) {
                        iniMem = hueco->iniMem + 1;
                        lastPosition = iniMem + memAsignada - 1;
                        break;
                    }
                }
            }

    // *********** CODIGO TERMINA AQUI *************

    this->iniMem = iniMem;
    }
}
=======
>>>>>>> 6613bffbc211c3331f3089d9e222c6dad30fa7ad
