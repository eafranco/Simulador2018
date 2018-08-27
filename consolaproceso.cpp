#include "consolaproceso.h"
#include "ui_consolaproceso.h"
//#include <QPalette>

ConsolaProceso::ConsolaProceso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConsolaProceso)
{
    ui->setupUi(this);
    pendiente=false;
    ui->display->setReadOnly(true);
    ui->input->setReadOnly(true);
}

ConsolaProceso::~ConsolaProceso()
{
    delete ui;
}

void ConsolaProceso::Display(QString msg){
    ui->display->append(msg);
}

QString ConsolaProceso::getInput()
{ QString rs;
    rs = ui->input->text();
    ui->input->setStyleSheet("* { background-color: rgb(156,156,156); }"); //gris
    ui->input->setEnabled(false);
    ui->input->setReadOnly(true);
    pendiente=false;
    ui->input->setText("");
    return rs;
}

void ConsolaProceso::setFocusLine()
{
    ui->input->setEnabled(true);
    ui->input->setReadOnly(false);
    ui->input->setStyleSheet("* { background-color: rgb(254, 254, 254); }"); //blanco
    ui->input->setFocus();
}

void ConsolaProceso::on_input_returnPressed()
{
    pendiente=true; //debe leer input desde ucp
    ui->input->setReadOnly(true);
    ui->input->setStyleSheet("* { background-color: rgb(100, 200, 150); }"); //verde
}