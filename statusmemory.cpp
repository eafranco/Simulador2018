#include "statusmemory.h"
#include "ui_statusmemory.h"


StatusMemory::StatusMemory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusMemory)
{
    ui->setupUi(this);
}

StatusMemory::~StatusMemory()
{
    delete ui;
}

 QString StatusMemory::qestado(int estado){
     switch(estado){
     case 0: return "NUEVO";
     case 1: return "LISTO";
     case 2: return "EJECUTANDO";
     case 3: return "BLOQUEADO";
     case 4: return "FINALIZADO";
     default: return "DESCONOCIDO";
     }
     return "ERROR";
 }

 void StatusMemory::recibeProceso(Proceso *procs){
     this->proceso = procs;
     ui->label_Pid->setText( QString::number(proceso->id) );
     ui->label_Es->setText( qestado(proceso->estado) );
     QString dato;
    for(int i=0; i<100; i++){
     if(i<proceso->memProcess){
      dato = QString::number( proceso->codigoMem[i]);
     }
     else
     { dato="0"; }
     ui->tabla->setItem(i, 0, new QTableWidgetItem(dato));
     ui->tabla->setVerticalHeaderItem(i,new QTableWidgetItem(QString::number(i)));
    }
 }

 void StatusMemory::actualiza(){
    this->setWindowTitle("Proceso: " + QString::number(proceso->id));
    ui->label_Pid->setText(QString::number(proceso->id));
    ui->label_Es->setText(qestado(proceso->estado));
    ui->label_prioridad->setText(QString::number(proceso->prioridad));
    ui->label_MemProc->setText(QString::number(proceso->memAsignada));
    ui->label_Pi->setText(QString::number(proceso->iniMem));
    ui->label_pf->setText(QString::number(proceso->finMem));
    ui->label_memPointer->setText(QString::number(proceso->memPointer));
    ui->label_sAc->setText(QString::number(proceso->stackPointer));
    ui->label_acc->setText(QString::number(proceso->accumulator));
    ui->label_RAc->setText(QString::number( proceso->codigoMem[proceso->memPointer-proceso->iniMem]) );
    ui->tabla->setCurrentCell(proceso->memPointer-proceso->iniMem,0);
}



