#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

//DEFINICION  del conjunto de instrucciones de la maquina virtual
//memoria máxima
#define MEMORY_SIZE 10000
enum estados{NUEVO=0,LISTO,EJECUTANDO,BLOQUEADO,FINALIZADO};

// OPERACIONES DE ENTRADA Y SALIDA
#define ASP 8
#define ASN 9
#define RD 10
#define WD 11

// Lectura de cadenas
#define NL 12
#define RS 13
#define WS 14

// Mover a memoria-registros
#define LDA 20
#define STA 21

// OPERACIONES ARITMETICA
#define ADD 30
#define SUB 31
#define DIV 32
#define MUL 33
#define MOD 34
#define EXP 35

// BIFURCACIONES
#define J 40
#define JLT 41
#define JEQ 42
#define JGT 43
#define COMP 44

// LLAMADAS A SUBRUTINAS Y USO DE LA PILA
#define JSUB 50
#define RSUB 51
#define PUSH 52
#define POP  53
#define END 64


#endif // INSTRUCCIONES_H

