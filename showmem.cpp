#include "showmem.h"
#include "ui_showmem.h"
#include <QGridLayout>
#include <QLabel>
#include <QString>

showmem::showmem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showmem)
{
    ui->setupUi(this);
}

showmem::~showmem()
{
    delete ui;
}


void showmem::recibeProcesos(QVector<Proceso *> *listaProcs){
    this->listaProcesos = listaProcs;
}


//Escalamos el proceso para que encaje a la memoria grafica
double scaleProcesss(double actualSize){
    double newSize=actualSize/20;
    return newSize/2.5;

}
//Regresmoa el proceso a su tamaño normal
double renderIn(double actual){
    double newSize=actual*2.5;
    return newSize*20;
}


//Calculamos cuantas veces se contiene 50 en el proceso
double numberJum(double data){
    return data/50;
}

//Determinamos cual es la longitud de cada proceso segun el tamaño asignado
double sizeSection(double size){
    double newLong ;
    double value;
    double newSize;
    if(size==10000){
        return 2.5;
    }else{
        newLong=size/20;
        newSize=newLong/2.5;
        //newSize=newLong/2.5;
        value=500/newSize;
        if(value*11>500){//Bug!!!
            return 45;
        }else{
            return value;
        }


    }
}


QString showmem::estado(int edo){
    switch(edo){
    case 0: return "NUEVO";
    case 1: return "LISTO";
    case 2: return "EJECUTANDO";
    case 3: return "BLOQUEADO";
    case 4: return "FINALIZADO";
    default: return "DESCONOCIDO";
    }
    return "ERROR";
}

void showmem::paintEvent(QPaintEvent *e)
{

   /** sizeSection(int size) toma el valor que recibe de la vista */
    //Entrada el tamaño maximo de la memoria real con la que se trabajara
    //double entrada=800;


    double entrada = CPU::RAM;
    double salto=sizeSection(entrada);

    Qt::GlobalColor color[10]={Qt::red, Qt::yellow, Qt::blue,Qt::darkMagenta,Qt::darkCyan,Qt::darkGray, Qt::darkRed, Qt::darkYellow, Qt::darkGreen, Qt::cyan};

    //Variables de las etiquetas
    int xInicio = 50, yInicio = 50;

    //Establecemos una longitud de memoria
    int limite = 500 + xInicio; // Longitud de la memoria escalada

    int xSiguiente=salto+xInicio;
    //variable para Establecer la longitud de las lineas a pintar
    int alto=50;

    //Crea la memoria base (Se pinta de blanco)
    QPainter painter(this);
    QPen pen(Qt::white);
    pen.setWidth(alto);
    QPoint st(xInicio,yInicio);
    QPoint end(limite,yInicio);
    pen.setCapStyle(Qt::FlatCap);
    painter.setPen(pen);
    painter.drawLine(st,end);


    //Contador que indica el proceso actual
    int cp=0;
    //cantidad de bloques a pintar
    double jum, inicio, fin;

   //Ciclo que recorre el arreglo de procesos
   while (cp < listaProcesos->size()) {
    jum = listaProcesos->at(cp)->memAsignada / 50; //bloques asignados y que se pintaran
    inicio = xInicio+(listaProcesos->at(cp)->iniMem/50)* salto;    ;  //scaleProcesss( listaProcesos->at(cp)->iniMem );
    fin = inicio + (jum*salto);

    QPen Proc(color[cp%10]);
    //Establecemos el alto de la linea
    Proc.setWidth(alto);
    //Determinamos donde inicia a pintar x,y
    QPoint st1(inicio,yInicio);
    //Determinamos donde termina de pintar x,y
    QPoint end1(fin,yInicio);
    //Fijamos un estilo de la linea
    Proc.setCapStyle(Qt::FlatCap);
    //Establecemos que se va pintar
    painter.setPen(Proc);
    //Cargamos lo que pintamos
    painter.drawLine(st1,end1);
    cp++;
   }



    //Pintamos las lineas
    double longMem=0;
    longMem=xSiguiente;

    while(longMem<=limite){
        QPen Line(Qt::black);
        Line.setWidth(0.5);
        QPoint beginLine(longMem,25);
        QPoint endLine(longMem,75);
        Line.setCapStyle(Qt::FlatCap);
        painter.setPen(Line);
        painter.drawLine(beginLine,endLine);
        longMem+=salto;
    }//Fin ciclo While pinta linea divisoras



    //Totales de memoria
    ui->locupada->setText(QString::number(CPU::memoriaOcupada));
    ui->llibre->setText(QString::number(CPU::RAM-CPU::memoriaOcupada));
    ui->ltotal->setText(QString::number(CPU::RAM));

    // Insertar Datos en la tabla de procesos
   ui->tabla->setHorizontalHeaderItem(0,new QTableWidgetItem("Id") );
   ui->tabla->setHorizontalHeaderItem(1,new QTableWidgetItem("Nombre") );
   ui->tabla->setHorizontalHeaderItem(2,new QTableWidgetItem("Estado") );
   ui->tabla->setHorizontalHeaderItem(3,new QTableWidgetItem("Memoria") );
   ui->tabla->setHorizontalHeaderItem(4,new QTableWidgetItem("Inicio") );
   ui->tabla->setHorizontalHeaderItem(5,new QTableWidgetItem("Fin") );
   ui->tabla->setHorizontalHeaderItem(6,new QTableWidgetItem("Bloques") );

   ui->tabla->setColumnWidth(0,40);
   ui->tabla->setColumnWidth(1,140);
   ui->tabla->setColumnWidth(6,50);

   QString dato;
   //limpiar tabla en cada actualizacion
   for(int i=0; i<ui->tabla->rowCount(); i++){
       dato = "     ";
       for(int j=0; j<ui->tabla->columnCount(); j++){
        ui->tabla->setItem(i,j, new QTableWidgetItem(dato));
       }
   }


   for(int i=0; i<listaProcesos->size(); i++){
    dato =  " "+QString::number(listaProcesos->at(i)->id);
    ui->tabla->setItem(i, 0, new QTableWidgetItem(dato));

    dato =  listaProcesos->at(i)->nameFile;
    dato=dato.split('/')[dato.split('/').length()-1];
    ui->tabla->setItem(i, 1, new QTableWidgetItem(dato));

    dato =  estado(listaProcesos->at(i)->estado);
    ui->tabla->setItem(i, 2, new QTableWidgetItem(dato));

    dato =   QString::number(listaProcesos->at(i)->memAsignada);
    ui->tabla->setItem(i, 3, new QTableWidgetItem(dato));

    dato =   QString::number(listaProcesos->at(i)->iniMem);
    ui->tabla->setItem(i, 4, new QTableWidgetItem(dato));

    dato =  QString::number(listaProcesos->at(i)->finMem);
    ui->tabla->setItem(i, 5, new QTableWidgetItem(dato));

    dato =  QString::number( listaProcesos->at(i)->memAsignada/50 );
    ui->tabla->setItem(i, 6, new QTableWidgetItem(dato));
   }

   for(int n=0;n<listaProcesos->size();n++){
       if(listaProcesos->at(n)->estado != FINALIZADO){
           ui->pushButton->setEnabled(true);
       }
   }
}

//buscar el id del proceso a Finalizar
void showmem::on_pushButton_clicked()
{
    int tid = ui->tabla->item(ui->tabla->currentRow(),0)->text().toInt();
    if( ui->tabla->currentItem()->isSelected() )
        for(int i=0; i<listaProcesos->size(); i++){
            if ( listaProcesos->at(i)->id == tid ){ 
                listaProcesos->at(i)->estado = FINALIZADO;
                break;
            }
        }
        for(int x=0;x<listaProcesos->size();x++){
            if(listaProcesos->at(x)->estado != FINALIZADO){
            ui->pushButton->setEnabled(true);
            break;
        }
        else{
            ui->pushButton->setEnabled(false);
        }
    }
}