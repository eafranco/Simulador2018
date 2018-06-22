#include "asim.h"
#define _ETIQ 1
#define _VAR 2
#define _CAD 3
#define _CONS 4
#define _INSTRUCCION 5
#define _IDENTIFICADOR 6
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_MEMORY 1000

#include<QMessageBox>
#include <QString>

FILE* entrada;
FILE* salida;

int numTokens;
int currMemPos = 0;
int memoriaTotal=0;
int contEnd=0;
QString merror;


/**
 * @struct token
 * @brief The token struct
 * Atributos de los token en la gramatica del lenguaje ensamblador
 */
struct token {
    //Para optimizacion futura, volver dinamico la siguiente instruccion
    char lexema[10];
    int tipo;
    int linea;
    char codigoMaquina[4];
    int indexOnTable;
};

typedef struct token Token;

Token* tablaSimbolos;

/**
 * @brief The error struct
 * Para almacenar errores en la traducción
 */
struct error {
    int tipo;
    int linea;
};

typedef struct error Error;

Error* tablaErrores;

int numErrores;

void inicializar() {
    tablaSimbolos = (Token*)malloc(sizeof(Token));
    numTokens = 0;
    tablaErrores = (Error*)malloc(sizeof(Error));
    numErrores = 0;
}


char mnemonicos [][2][8] ={
    { "ASP",    "8"},
    { "ASN",    "9"},
    { "RD",    "10"},
    { "WD",    "11"},
    { "NL",    "12"},
    {  "RS",   "13"},
    {  "WS",   "14"},
    {"LDA",    "20"},
    {"STA",    "21"},
    {"ADD",    "30"},
    {"SUB",    "31"},
    {"DIV",    "32"},
    {"MUL",    "33"},
    {"MOD",    "34"},
    {"EXP",    "35"},
    {"J",      "40"},
    {"JLT",    "41"},
    {"JEQ",    "42"},
    {"JGT",    "43"},
    {"COMP",   "44"},
    {"JSUB",   "50"},
    {"RSUB",   "51"},
    {"PUSH",   "52"},
    {"POP",    "53"},
    {"END",    "64"}
};


void agregarError(int tipo, int linea){
     (tablaErrores+numErrores)->tipo = tipo;
     (tablaErrores+numErrores)->linea = linea;
     numErrores++;
     tablaErrores = (Error*)realloc(tablaErrores,(numErrores+1)*sizeof(Error));
}

bool imprimeErrores(){
     int i=1,j=0;
     char buffer [150];
     int n;
     bool errores = false;
     while(i<=(tablaSimbolos+numTokens-1)->linea){
             while(j<numErrores){
                 if((tablaErrores+j)->linea == i){
                    if((tablaErrores+j)->tipo == 1)
                        n=sprintf(buffer,"ERROR linea: %d - Demasiados parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 2)
                        n=sprintf(buffer,"ERROR linea: %d - Instruccion necesita parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 3)
                        n=sprintf(buffer,"ERROR linea: %d - Primer argumento debe ser una instruccion\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 4)
                       n=sprintf(buffer,"ERROR linea: %d - Segundo parametro debe ser un identificador.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 5)
                        n=sprintf(buffer,"ERROR linea: %d - Segundo parametro no es constante.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 6)
                        n=sprintf(buffer,"ERROR linea: %d - Segundo parametro no es variable.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 7)
                        n=sprintf(buffer,"ERROR linea: %d - Segundo parametro no es cadena.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 8)
                       n=sprintf(buffer,"ERROR linea: %d - Solo se permite una instruccion END.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 9)
                       n=sprintf(buffer,"ERROR linea: %d - La instruccion END no recibe parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 10)
                        n=sprintf(buffer,"ERROR linea: %d - intruccion JSUB no recibe parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 11)
                        n=sprintf(buffer,"ERROR linea:%d - Memoria insuficiente\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 12)
                        n=sprintf(buffer,"ERROR linea:%d - Parametro No valido se esperaba Etiqueta.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 13)
                       n=sprintf(buffer,"ERROR linea:%d - Etiqueta Repetida.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 14)
                        n=sprintf(buffer,"WARNING linea:%d - Etiqueta nunca es llamada.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 15)
                        n=sprintf(buffer,"ERROR linea: %d - La instruccion NL no recibe parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 16)
                        n=sprintf(buffer,"ERROR linea: %d - La instruccion PUSH no recibe parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 17)
                        n=sprintf(buffer,"ERROR linea: %d - La instruccion POP no recibe parametros.\n",(tablaErrores+j)->linea);
                    errores=true;
                    merror.append(buffer);
                    n++;
                 }
                 j++;
              }
              j=0;
              i++;
     }
     if(errores)
      QMessageBox::information(NULL,"Informe de errores",merror);
     return errores;
}

bool imprimeErroresAnt(){
     int i=1,j=0;
     bool errores = false;
     while(i<=(tablaSimbolos+numTokens-1)->linea){
             while(j<numErrores){
                 if((tablaErrores+j)->linea == i){
                    if((tablaErrores+j)->tipo == 1)
                        printf("ERROR linea: %d - Demasiados parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 2)
                        printf("ERROR linea: %d - Instruccion necesita parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 3)
                        printf("ERROR linea: %d - Primer argumento debe ser una instruccion\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 4)
                        printf("ERROR linea: %d - Segundo parametro debe ser un identificador.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 5)
                        printf("ERROR linea: %d - Segundo parametro no es constante.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 6)
                        printf("ERROR linea: %d - Segundo parametro no es variable.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 7)
                        printf("ERROR linea: %d - Segundo parametro no es cadena.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 8)
                        printf("ERROR linea: %d - Solo se permite una instruccion END.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 9)
                        printf("ERROR linea: %d - La instruccion END no recibe parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 10)
                        printf("ERROR linea: %d - intruccion JSUB no recibe parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 11)
                        printf("ERROR linea:%d - Memoria insuficiente\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 12)
                        printf("ERROR linea:%d - Parametro No valido se esperaba Etiqueta.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 13)
                        printf("ERROR linea:%d - Etiqueta Repetida.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 14)
                        printf("WARNING linea:%d - Etiqueta nunca es llamada.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 15)
                        printf("ERROR linea: %d - La instruccion NL no recibe parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 16)
                        printf("ERROR linea: %d - La instruccion PUSH no recibe parametros.\n",(tablaErrores+j)->linea);
                    if((tablaErrores+j)->tipo == 17)
                        printf("ERROR linea: %d - La instruccion POP no recibe parametros.\n",(tablaErrores+j)->linea);
                    errores=true;
                 }
                 j++;
              }
              j=0;
              i++;
     }
     return errores;
}

bool esMnemonico(char* operador) {
    int cont;
    for(cont=0;cont<25;cont++) {  //25 instrucciones
        if(strcmp(operador,mnemonicos[cont][0])==0){
            tablaSimbolos[numTokens].indexOnTable=cont; //Se guarda la posicion de la instruccion
            return true;
        }
    }
    return false;
}

void truncarCadena(char* cadena){
     int i=0;
     for(i=0;i<10;i++){
         (tablaSimbolos+numTokens)->lexema[i]=cadena[i];
     }
}

void Tokenizar(char* elemento, int num_lin) {
     if(strlen(elemento)>10){
          truncarCadena(elemento);
     }else{
          strcpy((tablaSimbolos+numTokens)->lexema,elemento);
     }
    (tablaSimbolos+numTokens)->linea = num_lin;
    if(esMnemonico(elemento)) {
        (tablaSimbolos+numTokens)->tipo = _INSTRUCCION;
    }
    else {
        (tablaSimbolos+numTokens)->tipo = _IDENTIFICADOR;
        strcpy( (tablaSimbolos+numTokens)->codigoMaquina, "--");
    }
    //strcpy( (tablaSimbolos+numTokens)->codigoMaquina, "--");
    //(tablaSimbolos+numTokens)->indexOnTable = 0;
    numTokens++;
    tablaSimbolos = (Token*)realloc(tablaSimbolos,(numTokens+1)*sizeof(Token));
}

void actualizarEtiq(){
    int i=0,j=0;
    while(i<numTokens){
        if(tablaSimbolos[i].tipo == _ETIQ && (i-1)==-1){
            j=i+1;
            while(j<numTokens){
                if(strcmp(tablaSimbolos[i].lexema,tablaSimbolos[j].lexema)==0){
                       tablaSimbolos[j].tipo = _ETIQ;
                }
                j++;
            }
        }else{
           if(tablaSimbolos[i].tipo == _ETIQ && tablaSimbolos[i-1].linea != tablaSimbolos[i].linea){
                j=0;
                while(j<numTokens){
                    if(strcmp(tablaSimbolos[i].lexema,tablaSimbolos[j].lexema)==0){
                         tablaSimbolos[j].tipo = _ETIQ;
                    }
                    j++;
                }
           }
        }
        i++;
    }
}

/**
 * @brief revisarSemantica
 * Analiza la construcción de las instrucciones
 */

void revisarSemantica(){
    int i=0,j=0,lineaTemp=0,band=0;
    while(i<numTokens){

            if(strcmp(tablaSimbolos[i].lexema,"END")==0 &&
                tablaSimbolos[i-1].linea != tablaSimbolos[i].linea && i!=lineaTemp && ++contEnd ==0){
                 agregarError(8,tablaSimbolos[i].linea);
                 if(tablaSimbolos[i].linea == tablaSimbolos[i+1].linea){
                      agregarError(9,tablaSimbolos[i+1].linea);
                 }
            }

            if(strcmp(tablaSimbolos[i].lexema,"JSUB")==0){
                  j = i-1;
                  while(j>0){
                       if(tablaSimbolos[j].tipo == _ETIQ &&
                            tablaSimbolos[j-1].linea != tablaSimbolos[j].linea){
                                break;
                       }else{
                            if((strcmp(tablaSimbolos[j].lexema,"JSUB")==0 &&
                            tablaSimbolos[j-1].linea != tablaSimbolos[j].linea) || j==0){
                               agregarError(10,tablaSimbolos[j].linea);
                               break;
                            }
                      }
                   j--;
                  }
            }

            if ((strcmp(tablaSimbolos[i-1].lexema,"ASP")==0 ||
            strcmp(tablaSimbolos[i-1].lexema,"ASN")==0) && tablaSimbolos[i].tipo != _CONS){
                 agregarError(5,tablaSimbolos[i].linea);
            }

            if ( strcmp(tablaSimbolos[i].lexema,"NL")==0 ) {
                if(tablaSimbolos[i].linea == tablaSimbolos[i+1].linea){
                    agregarError(15,tablaSimbolos[i+1].linea);
                  }
            }
            if ( strcmp(tablaSimbolos[i].lexema,"PUSH")==0 ) {
                if(tablaSimbolos[i].linea == tablaSimbolos[i+1].linea){
                    agregarError(16,tablaSimbolos[i+1].linea);
                }
            }
            if ( strcmp(tablaSimbolos[i].lexema,"POP")==0 ) {
                if(tablaSimbolos[i].linea == tablaSimbolos[i+1].linea){
                    agregarError(17,tablaSimbolos[i+1].linea);
                }
            }
            if ( strcmp(tablaSimbolos[i].lexema,"RSUB")==0 ) {
                if(tablaSimbolos[i].linea == tablaSimbolos[i+1].linea){
                    agregarError(10,tablaSimbolos[i+1].linea);
                }
            }

            if((strcmp(tablaSimbolos[i-1].lexema,"RD")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"WD")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"LDA")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"STA")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"ADD")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"SUB")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"DIV")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"MUL")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"MOD")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"EXP")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"COMP")==0 ) && tablaSimbolos[i].tipo != _VAR) {
                            agregarError(6,tablaSimbolos[i].linea);
            }
            if((strcmp(tablaSimbolos[i-1].lexema,"RS")==0 ||
                strcmp(tablaSimbolos[i-1].lexema,"WS")==0) && tablaSimbolos[i].tipo!=_CAD) {
                            agregarError(7,tablaSimbolos[i].linea);
            }
            if((strcmp(tablaSimbolos[i-1].lexema,"J")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"JLT")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"JEQ")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"JGT")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"JSUB")==0) && tablaSimbolos[i].tipo != _ETIQ) {
                            agregarError(12,tablaSimbolos[i].linea);
            }
        i++;
    }
    band=0;
    for(i=0;i<numTokens;i++){
          if(tablaSimbolos[i].tipo == _ETIQ && (i-1) == -1){
              for(j=i+1;j<numTokens;j++){
                  if(strcmp(tablaSimbolos[i].lexema,tablaSimbolos[j].lexema)==0 && tablaSimbolos[j-1].linea != tablaSimbolos[j].linea){
                      agregarError(13,tablaSimbolos[j].linea);
                      band=1;
                  }else{
                      if(band!=1 && j+1==numTokens){
                           agregarError(14,tablaSimbolos[i].linea);
                      }
                  }
              }
          }else{
             if(tablaSimbolos[i].tipo == _ETIQ && tablaSimbolos[i-1].linea != tablaSimbolos[i].linea){
                  for(j=i+1;j<numTokens;j++){
                      if(strcmp(tablaSimbolos[i].lexema,tablaSimbolos[j].lexema)==0 && tablaSimbolos[j-1].linea != tablaSimbolos[j].linea){
                          agregarError(13,tablaSimbolos[j].linea);
                      }
                  }
             }
          }
    }
}

/**
 * @brief revisarSintactica
 * Verifica sintaxis del programa, revisa las instrucciones válidas y otros tokens
 */

void revisarSintactica() {
    int param=0,i=0;
    while(i<numTokens){
        param = 1;
        int _linea = tablaSimbolos[i].linea;
        while(tablaSimbolos[i+1].linea==_linea && i+1<numTokens) {
            param++;
            i++;       // Esta variable maneja el indice de la palabra actual.
        }
        if(param>2) {
            agregarError(1,_linea);
        }
        //else {
            if(param==1) { //Un solo parametro
                if(tablaSimbolos[i].tipo == _IDENTIFICADOR) {
                    tablaSimbolos[i].tipo = _ETIQ;
                }
                else {
                    if( strcmp(tablaSimbolos[i].lexema,"RSUB") !=0 &&
                        strcmp(tablaSimbolos[i].lexema,"END") !=0 &&
                        strcmp(tablaSimbolos[i].lexema,"NL") !=0  &&
                        strcmp(tablaSimbolos[i].lexema,"PUSH") !=0 &&
                        strcmp(tablaSimbolos[i].lexema,"POP") !=0 ){
                            agregarError(2,_linea);
                    }
                }
            }
            else { // Necesariamente son dos parametros
                if(tablaSimbolos[i-1].tipo == _IDENTIFICADOR) {
                    agregarError(3,_linea);
                }
                else {//El primer parametro necesariamente es una instruccion
                    if(tablaSimbolos[i].tipo == _INSTRUCCION) {
                        agregarError(4,_linea);
                    }
                    else {
                         int p = atoi(tablaSimbolos[i].lexema);
                        if(((strcmp(tablaSimbolos[i-1].lexema,"ASP")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"ASN")==0) && tablaSimbolos[i].tipo==_IDENTIFICADOR)
                            || p != 0) {
                                tablaSimbolos[i].tipo = _CONS;
                        }
                        else if((strcmp(tablaSimbolos[i-1].lexema,"RD")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"WD")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"LDA")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"STA")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"ADD")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"SUB")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"DIV")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"MUL")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"MOD")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"EXP")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"COMP")==0 ) && tablaSimbolos[i].tipo==_IDENTIFICADOR) {
                            tablaSimbolos[i].tipo = _VAR;
                        }
                        else if((strcmp(tablaSimbolos[i-1].lexema,"RS")==0 ||
                            strcmp(tablaSimbolos[i-1].lexema,"WS")==0) && tablaSimbolos[i].tipo==_IDENTIFICADOR) {
                            tablaSimbolos[i].tipo = _CAD;
                        }
//						else if((strcmp(tablaSimbolos[i-1].lexema,"JSUB")==0 ) &&
//							     tablaSimbolos[i].tipo==_IDENTIFICADOR) {
//								tablaSimbolos[i].tipo = _ETIQ;
//							}
                        else{  //El resto es parametro de: j, jlt, jgt, jeq, jsub
                          //  if(tablaSimbolos[i].tipo==_IDENTIFICADOR){
                                tablaSimbolos[i].tipo = _ETIQ;
                           // }
                        }
                    }
                }
            }
        //} // else
        i++; //Al aumentar esta variable se brinca a la siguiente linea
    }
    actualizarEtiq();
}

void imprimir() {
    int i=0;
    printf("\n** Tabla de simbolos **\n");
    while(i<numTokens) {
        printf("%s \t%d \t tipo:%d \t cm:%s \t index: %d\n",
            (tablaSimbolos+i)->lexema,
            (tablaSimbolos+i)->linea,
            (tablaSimbolos+i)->tipo,
            (tablaSimbolos+i)->codigoMaquina,
        (tablaSimbolos+i)->indexOnTable);
        i++;
    }
}



void asignarEtiqueta( int actual  ) {
    int i=0;

    for(i=0;i<numTokens;i++) {
        if( tablaSimbolos[i-1].linea != tablaSimbolos[i].linea &&
            tablaSimbolos[i+1].linea != tablaSimbolos[i].linea &&
            strcmp(tablaSimbolos[i].lexema,tablaSimbolos[actual].lexema )==0 )
            {
                strcpy( tablaSimbolos[actual].codigoMaquina, tablaSimbolos[i].codigoMaquina);
            }

        }
}


/**
 * @brief asignarMemoriaEtiquetas
 * Determina la dirección de la etiqueta, que puede ser variable o etiqueta de salto
 */
void asignarMemoriaEtiquetas() {
    int i=0;
    int numIns=0;

    for(i=0;i<numTokens;i++) {
        switch(tablaSimbolos[i].tipo) {
        case _INSTRUCCION:
            numIns++;
            break;

        case _ETIQ:
            if( tablaSimbolos[i-1].linea != tablaSimbolos[i].linea && tablaSimbolos[i+1].linea != tablaSimbolos[i].linea )
            {
                sprintf( tablaSimbolos[i].codigoMaquina,"%d",numIns);
            }
            break;

        }
    }

}

/**
 * @brief calculaMemoria
 * Cuenta la cantidad de instrucciones y regresa la dirección de inicio de variables
 * @return dv es la dirección de inicio de variables.
 */
int calculaMemoria( ){
int dv=0;
int i=0;
int ci=0;
int aux=-1;

for(i=0;i<numTokens;i++) {
    if( tablaSimbolos[i].linea != aux)
    {
        ci++;
        aux = tablaSimbolos[i].linea;
    }
  }
  dv = ci; //al final de las instrucciones inician las variables
  return dv;
}

/**
 * @brief asignarMemoria
 * Asigna direcciones de memoria a las variables de tipo entero y cadena
 * un entero es una posición y una cadena ocupa 40 posiciones, máximo 80 caracteres.
 * Máximo de memoria para un programa es 1000 enteros.
 */
void asignarMemoria() {
    int i=0, aux=0;
    int numIns=0;
    int dirVars=500; // pensando una memoria de 1000 enteros


    dirVars = calculaMemoria( );  //inicio de variables: cadena y entero combinadas


    for(i=0;i<numTokens;i++) {
        switch(tablaSimbolos[i].tipo) {
            case _INSTRUCCION:
                strcpy((tablaSimbolos+i)->codigoMaquina,
                    mnemonicos[tablaSimbolos[i].indexOnTable][1]);

                numIns++;  //
                break;
            case _VAR:
                aux=i-1;
                while(aux > 0) {
                    if(strcmp((tablaSimbolos+aux)->lexema,
                    (tablaSimbolos+i)->lexema) != 0) {
                        aux--;
                    }
                    else {
                        strcpy((tablaSimbolos+i)->codigoMaquina,(tablaSimbolos+aux)->codigoMaquina);
                        break;
                    }
                }
                if(aux == 0) { // Si no se definio antes
                    sprintf ((tablaSimbolos+i)->codigoMaquina, "%d", dirVars++ );
                    //printf ("var: %s    %d\n",(tablaSimbolos+i)->codigoMaquina, dirVars );

                }
                //numIns++;
                break;
            case _ETIQ:
                aux=i-1;
                while(aux > 0) {
                    if(strcmp((tablaSimbolos+aux)->lexema,
                        (tablaSimbolos+i)->lexema) != 0) {
                        aux--;
                    }
                    else {
                        strcpy((tablaSimbolos+i)->codigoMaquina,(tablaSimbolos+aux)->codigoMaquina);
                        break;
                    }
                }
                if(aux < 1) { // Si no se definio antes (aux <1)

                    //sprintf ((tablaSimbolos+i)->codigoMaquina, "%d", asignarEtiqueta(i) );
                    asignarEtiqueta(i);

                    //printf ("etiq: %s    \n",(tablaSimbolos+i)->codigoMaquina);
                }
                break;
            case _CONS:
                strcpy((tablaSimbolos+i)->codigoMaquina,(tablaSimbolos+i)->lexema);
                //numIns++;
                break;
            case _CAD:
                aux=i-1;
                while(aux > 0) {
                    if(strcmp((tablaSimbolos+aux)->lexema,
                        (tablaSimbolos+i)->lexema) != 0) {
                        aux--;
                    }
                    else {
                        strcpy((tablaSimbolos+i)->codigoMaquina,(tablaSimbolos+aux)->codigoMaquina);
                        break;
                    }
                }


                if(aux == 0) { // si no se definio antes
                    if(dirVars + 40 <= MAX_MEMORY) {
                        //itoa(dirCadenas,(tablaSimbolos+i)->codigoMaquina,10);
                        sprintf ((tablaSimbolos+i)->codigoMaquina, "%d", dirVars ); // yo
                        dirVars += 40; //variables cadena de 40 palabras
                    }
                    else
                        agregarError(11,(tablaSimbolos+i)->linea);
                }
                //numIns++;
                break;
            default: puts("ERROR en asignacion de memoria!\n");
        }
     }
    memoriaTotal = dirVars + 25; //25 para el stack
    //printf("\n Memoria para el proceso:  %d  \n", memoriaTotal);
    //printf("variables: cadena 40 palabras, entero 1 palabra, stack 25 palabras\n");
}

void liberarMemoria() {
     free(tablaSimbolos);
     free(tablaErrores);
}

/**
 * @brief generarSalida
 * Escribe el resultado de la traducción en un archivo .lm
 * @param salida nombre del archivo a generar
 */
void generarSalida(FILE* salida) {
    int i;
    fprintf(salida,"@ %d \n",memoriaTotal); //memoria necesaria

    for(i=0;i<numTokens;i++) {
        if(i+1<numTokens && (tablaSimbolos+i)->linea==(tablaSimbolos+i+1)->linea){
            switch(strlen((tablaSimbolos+i+1)->codigoMaquina)) {
                case 1:
                    fprintf(salida,"%s000%s\n",
                        (tablaSimbolos+i)->codigoMaquina,
                        (tablaSimbolos+i+1)->codigoMaquina);
                    break;
                case 2:
                    fprintf(salida,"%s00%s\n",
                        (tablaSimbolos+i)->codigoMaquina,
                        (tablaSimbolos+i+1)->codigoMaquina);
                    break;
                case 3:
                    fprintf(salida,"%s0%s\n",
                        (tablaSimbolos+i)->codigoMaquina,
                        (tablaSimbolos+i+1)->codigoMaquina);
                    break;
                case 4:
                    fprintf(salida,"%s%s\n",
                        (tablaSimbolos+i)->codigoMaquina,
                        (tablaSimbolos+i+1)->codigoMaquina);
                    break;
                default: puts("ERROR en impresion!");
            }
        }
        else {
            if((tablaSimbolos+i)->tipo != _INSTRUCCION) {
                if(i+1<numTokens)
                    continue;
                else {
                    printf("Warning: linea: %d - Etiqueta no encierra instrucciones",(tablaSimbolos+i)->linea);
                }
            }
            else {
                fprintf(salida,"%s0000\n",(tablaSimbolos+i)->codigoMaquina);
            }
        }
    }
}



void primeraPasada(FILE* file) {
    inicializar();
    char curr_line[120];                                                         //Optmizar Dinamico
    int num_lin=0;
    char* elemento;

    while(fgets(curr_line,120,file)!=NULL) {   //Lee línea por línea

        if(curr_line[0]!='#' && curr_line[0]!=' ' && curr_line[0]!='\r' && curr_line[0]!='\n' && curr_line[0]!='\t') {
        num_lin++;
        elemento = strtok(curr_line," \r\n\t");
        while(elemento != NULL) {
            Tokenizar(elemento,num_lin);
            elemento = strtok(NULL," \r\n\t");
        }
        }
    }


    revisarSintactica();
    revisarSemantica();
    //imprimir();
}

void segundaPasada(){
     asignarMemoriaEtiquetas();
     asignarMemoria();
    // imprimir();
}

void loadAsim(char* archivo){
    entrada = fopen(archivo,"r");
    if (entrada == NULL){
      printf("Error al leer el archivo.\n\n");
    }
}

/**
 * @brief asim::asim
 * Constructor que ejecuta al ensamblador para generar un ejecutable (.lm)
 * @param nombre nombre del archivo fuente en formato ensamblador
 */
asim::asim(QString nombre)
{
    hayerrores=true;
    merror.clear();
    QByteArray array = nombre.toLocal8Bit();
    char* archivo = array.data();
    QByteArray ASalida = (nombre.split(".asim")[0]+".lm").toLocal8Bit();
    char* nomSalida = ASalida.data();
    loadAsim(archivo);
    if(entrada!=NULL){
        primeraPasada(entrada);
        segundaPasada();
        if(!imprimeErrores()){
            remove(nomSalida);
            salida = fopen(nomSalida, "a");
            generarSalida(salida);
            fclose(salida);
            hayerrores=false;
           // printf("\n** Se ha generado el archivo ejecutable:  %s  \nContinuar...\n",nomSalida);
        }
        liberarMemoria();
        fclose(entrada);
    }
}

