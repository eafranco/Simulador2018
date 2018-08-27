#include "cpu.h"
#include <QTime>
#include <QEventLoop>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

//inicialización de variables estáticas
int CPU::tiempo_espera = 10;
int CPU::RAM = MEMORY_SIZE;
int CPU::DEBUG = 1;
int CPU::memoriaOcupada = 0;
int CPU::mayorMemProceso = 0;
int CPU::memory[MEMORY_SIZE]={0};

// ////////////////////////////

void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

/**
 * @brief CPU::CPU
 * Constructor de CPU, inicializacion de variables
 * @return
*/


CPU::CPU()
{ 
 // registros
 accumulator=0;
 stackPointer = (MEMORY_SIZE - 1);
 memPointer = 0;
 memProcess = 0;
 estado = NUEVO;
 machineOn = 1; // proceso encendido
 wait = 0 ;

 // para control
 DEBUG = 1; // 1 activa paso a paso en instrucciones
 tiempo_espera = 10; //pausa en cada instrucción
 //RAM = MEMORY_SIZE; //memoria total del sistema (dinamica en simulacion)
 //para que no modifique el valor actual de RAM y quede con  valor del widget
 CPU::memoriaOcupada = 0; //reiniciar contador de memoria ocupada
 CPU::mayorMemProceso = 0; //inicializar limite de memoria ocupada en RAM
 //conta=0;
 pProceso=NULL; //apuntador a proceso actual en ejecución

 for (int i=0; i<MEMORY_SIZE; i++)
     memory[i]= 0; //inicializa memoria de CPU

}//fin constructor


void CPU::setDebug(int v){
    if(v==1 || v==0)
       DEBUG = v;
    else
       DEBUG = 0;
}

//salva estado de la CPU en el BCP del proceso
void CPU::saveState(){
    pProceso->estado = estado;
    pProceso->accumulator = accumulator;
    //pProceso->DEBUG = DEBUG;
    pProceso->machineOn = machineOn;
    pProceso->memPointer = memPointer;
    pProceso->stackPointer = stackPointer;
    pProceso->wait = wait;
    pProceso->memProcess = memProcess;
    pProceso->iniMem=iniMem;
    pProceso->finMem = finMem;
    pProceso->memAsignada = memAsignada;
    pProceso->ventanaEstado->actualiza(); //valores en ventana de estado
}

//Carga el estado del proceso i en la CPU
void CPU::loadState(){
    if( pProceso->estado==NUEVO){
        //Copia el codigo maquina del proceso en la memoria de la maquina virtual (CPU)
       pProceso->consola->Display("Cargando código del proceso en memoria de la CPU!!");
       pProceso->consola->Display("Posición: inicio "+QString::number(pProceso->iniMem)+" fin "+QString::number(pProceso->finMem));
       for(int k = 0; k<pProceso->memAsignada; k++){
           memory[k+pProceso->iniMem] = pProceso->codigoMem[k];

       }
       //--muestra codigo del programa en consola
//       int v;
//       for(int i=pProceso->iniMem; i<pProceso->iniMem+pProceso->memProcess; i++)
//       {
//        v=memory[i];
//        pProceso->consola->Display(QString::number(v));
//       }
       pProceso->estado = LISTO;
       pProceso->machineOn = 1; //encendida
    }

    // Inicializa variables y registros de la CPU con variables del proceso
    estado = pProceso->estado;
    accumulator =pProceso->accumulator;
    //DEBUG = pProceso->DEBUG;
    machineOn = pProceso->machineOn;
    memPointer = pProceso->memPointer;
    stackPointer = pProceso->stackPointer;
    wait = pProceso->wait;
    memProcess = pProceso->memProcess;
    iniMem=pProceso->iniMem;
    finMem =  pProceso->finMem;
    memAsignada = pProceso->memAsignada;
}

//Verifica el acumulador
void CPU::checkAccumulator(){
  if(accumulator < -99999 || accumulator > 99999){
    pProceso->consola->Display("ERROR: Desbordamiento del acumulador");
    machineOn = 0;
    estado=FINALIZADO;
    saveState();
  }
}

//Recibe proceso y lo ejecuta
void CPU::ejecuta(Proceso *p){

    pProceso = p; //apuntar al proceso en ejecución

    loadState(); //recupera registros del proceso a la CPU

    if(machineOn && estado!=FINALIZADO) {
      if(wait == 0){ //no hay lecturas pendientes
         runInstruction(memory[memPointer]);
         memPointer++;
       }
      else{
          //si esta en espera de lectura de datos, estado Bloqueado
          delay(LAPSO);  //da tiempo a leer, si no satura memoria
          getInputVal();
       }
    }

    //if (pProceso->estado==FINALIZADO)
    //    estado = pProceso->estado;

    saveState();  //salva estado del proceso
 }



//Metodo de lectura de variables
void CPU::getInputVal(){
    QString st=".";
    int inputVal = 0;


    if(pProceso->consola->pendiente==true && estado==BLOQUEADO){ //hay valor pendiente

        st = pProceso->consola->getInput(); //lee el input y reestablece

    memPointer++; //pasar a la instruccion pendiente

    int arg = memory[memPointer-1];
    arg = (arg%10000) - iniMem;
    if(wait == 1){
        inputVal = st.toInt();
        pProceso->consola->Display("Entero leido: "+QString::number(inputVal));
        memory[arg] = inputVal;
        wait = 0;
    }else{
        if(wait == 2){
            pProceso->consola->Display("Cadena leida: "+st);
            QChar t = '0';
            for(int i = 0; i < st.length(); i++){
                    t = st.at(i);
                    memory[arg + 1+i] = (int)t.toLatin1();
            }
            memory[arg] = st.length()+1;
            wait = 0;
        }
    }
    estado = LISTO;
    saveState();
   }
 }


//Ejecuta las instrucciones de codigo maquina
void CPU::runInstruction(int instr) {
    int opCode = instr / 10000;
    int arg = (instr % 10000) + iniMem; // desplazamiento segun memoria del proceso
    int stringLen=0;
    int cnt=0, inputVal=0;

    QString res = "";

    if(arg>=RAM){
      pProceso->consola->Display("ERROR: direccion de memoria "+QString::number(arg)+ " no válida\n");
      machineOn = 0;
      estado=FINALIZADO;
      saveState();
      return;
    }


    if (estado==LISTO) {
         estado=EJECUTANDO;
         saveState();
    }


    delay(tiempo_espera*LAPSO);

    if( pProceso->estado==FINALIZADO  ){
      pProceso->consola->Display("Proceso terminado desde el monitor de memoria!");
      machineOn = 0;
      estado=FINALIZADO;
      saveState();
      return;
    }

   switch (opCode) {
    // OPERACIONES DE ENTRADA Y SALIDA
        case ASP: // asignacion de numero positivo
            if (DEBUG) {
                pProceso->consola->Display("ASP +"+QString::number(arg-iniMem));
            }
            accumulator = arg - iniMem; //valor real de arg
            break;
        case ASN: // asignacion de numero positivo
            if (DEBUG) {
                pProceso->consola->Display("ASN - "+QString::number(arg-iniMem));
            }
            accumulator = arg * (-1) - iniMem; //valor real de arg
            break;

        case RD: // READ - leer desde el teclado entero
            if (DEBUG) {
                pProceso->consola->Display("RD "+QString::number(arg));
            }
            pProceso->consola->Display("Escribir variable entera: ");
            wait = 1;
            pProceso->consola->setFocusLine();
            break;

        case WD: // WRITE - escribir en la pantalla entero
            if (DEBUG) {
                pProceso->consola->Display("WD "+QString::number(arg));
            }
            pProceso->consola->Display("Valor de variable entera: ");
            pProceso->consola->Display(QString::number(memory[arg]));
            break;

        case NL: // NEWLINE  - nueva línea
            if (DEBUG) {
                pProceso->consola->Display("NL ");
            }
            pProceso->consola->Display("");
            break;

        case RS: // READSTRING  -  leer una cadena desde el teclado
            if (DEBUG) {
                pProceso->consola->Display("RS "+QString::number(arg));
            }
            pProceso->consola->Display("Escribir variable cadena: ");
            wait = 2;
            pProceso->consola->setFocusLine();
            break;

        case WS: // WRITESTRING  - escribir una cadena en memoria
            if (DEBUG) {
                pProceso->consola->Display("WS "+QString::number(arg));
            }
            stringLen = memory[arg];
            //printf(": ");
            for (int i = 0; i < stringLen; i++) {
                res.push_back( QChar::fromLatin1(memory[arg + 1 +i]));

            }
            pProceso->consola->Display("Valor de variable cadena: ");
            pProceso->consola->Display(res);
            break;

        case LDA: // LOAD  -  cargar el acumulador con valor en memoria
            if (DEBUG) {
                pProceso->consola->Display("LDA "+QString::number(arg));
            }
            accumulator = memory[arg];
            break;

        case STA: // STORE  - enviar a memoria valor que esta en acumulador
            if (DEBUG) {
                pProceso->consola->Display("STA "+QString::number(arg));
            }
            memory[arg] = accumulator;
            break;
    //  OPERACIONES ARITMETICA
        case ADD: // ADD
            if (DEBUG) {
                pProceso->consola->Display("ADD "+QString::number(arg));
            }
            accumulator += memory[arg];
            checkAccumulator();
                break;
        case SUB: // SUBSTRACT
            if (DEBUG) {
                pProceso->consola->Display("SUB "+QString::number(arg));
            }

            accumulator -= memory[arg];
            checkAccumulator();
            break;

        case DIV: // DIVIDE
            if (DEBUG) {
                pProceso->consola->Display("DIV "+QString::number(arg));
            }
            if(memory[arg] == 0){
              pProceso->consola->Display("ERROR: Division entre 0.");
              machineOn = 0;
            }else{
              accumulator /= memory[arg];
            }
            break;

        case MUL: // MULTIPLY
            if (DEBUG) {
                pProceso->consola->Display("MUL "+QString::number(arg));
            }
            accumulator *= memory[arg];
            checkAccumulator();
            break;

        case MOD: // MODULUS
            if (DEBUG) {
                pProceso->consola->Display("MOD "+QString::number(arg));
            }
            accumulator %= memory[arg];
            break;

        case EXP: // EXPONENT
            if (DEBUG) {
                pProceso->consola->Display("EXP "+QString::number(arg));
                //printf("EXPONENT %d\n",arg);
            }
            if(memory[arg] <= 0){
              accumulator = 1;
              break;
            }
            inputVal = accumulator;
            for(cnt = 1; cnt < memory[arg]; cnt++){
              accumulator *= inputVal;
            }
            checkAccumulator();
            break;
    //  BIFURCACIONES
        case COMP: // COMPARE  - Compara A con valor en memoria
            if (DEBUG) {
                pProceso->consola->Display("COMP "+QString::number(arg));
            }
            accumulator -= memory[arg];
            break;

        case JLT: // BRANCHIFNEG - Salto si acumulador en negativo
            if (DEBUG) {
                pProceso->consola->Display("JLT "+QString::number(arg));
            }
            if (accumulator < 0) {
                memPointer = arg-1;
            }
            break;
        case JEQ: // BRANCHZERO - Salto si acumulador es cero
            if (DEBUG) {
                pProceso->consola->Display("JEQ "+QString::number(arg));
            }
            if (accumulator == 0) {
                memPointer = arg-1;
            }
            break;
        case JGT: // BRANCHIFPOS - Salto si acumulador en positivo
            if (DEBUG) {
                pProceso->consola->Display("JGT "+QString::number(arg));
            }
            if (accumulator > 0) {
                memPointer = arg-1;
            }
            break;
        case J: // BRANCH  - Salto Incondicional
            if (DEBUG) {
                pProceso->consola->Display("J "+QString::number(arg));
            }
            memPointer = arg-1;
            break;
    // LLAMADAS A SUBRUTINAS Y USO DE LA PILA
        case JSUB: // CALL - salta a subrutina
            if (DEBUG) {
                pProceso->consola->Display("JSUB "+QString::number(arg));
            }
            memory[stackPointer] = memPointer;
            stackPointer -= 1;
            memPointer = arg - 1;
            break;
        case RSUB: // RETIFNEG  - regreso de subrutina
            stackPointer += 1;
            //accumulator = memory[stackPointer];
            //memPointer = accumulator;
            memPointer = memory[stackPointer];
            if (DEBUG) {
                pProceso->consola->Display("RSUB "+QString::number(arg));
            }
            break;
        case PUSH: // PUSH  - colocar en la pila
            if (DEBUG) {
                pProceso->consola->Display("PUSH "+QString::number(arg));
            }

            memory[stackPointer] = accumulator;
            stackPointer -= 1;
            break;
        case POP: // POP - retirar de la pila

            stackPointer += 1;
            accumulator=memory[stackPointer];

            if (DEBUG) {
                pProceso->consola->Display("POP "+QString::number(arg));
            }

            break;
        case END: // HALT - Terminar programa y apagar la maquina
           // if (DEBUG) {
                pProceso->consola->Display("END "+QString::number(arg));
                pProceso->consola->Display("** Ejecucion finalizada **");
           // }
            pProceso->consola->Display("Proceso terminado de forma normal!");
            machineOn = 0;
            estado = FINALIZADO;
            saveState();

            return;
        default:
            pProceso->consola->Display("Código de operación "+QString::number(arg)+ " incorrecto");
            machineOn = 0;
    }

    if (wait==1 || wait==2){
        estado = BLOQUEADO;
        memPointer--; //no avanzar hasta leer datos de la consola
    }
    else
        estado = LISTO;  //continua con la siguiente instruccion


     saveState();
     delay(tiempo_espera*LAPSO);

     if( pProceso->estado==FINALIZADO  ){
       pProceso->consola->Display("Proceso terminado desde el monitor de memoria!");
       machineOn = 0;
       estado=FINALIZADO;
       saveState();
       return;
     }

    return;
}








