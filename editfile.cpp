#include "editfile.h"
#include "ui_editfile.h"
#include<QTextStream>
#include<QString>
#include<QFile>
#include<QMessageBox>
#include "helpfile.h"

editFile::editFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editFile)
{
    ui->setupUi(this);
}

editFile::~editFile()
{
    delete ui;
}


void editFile::setFileName(QString filename){
    namefile = filename;
    ui->label->setText(filename.split('/')[filename.split('/').length()-1]);
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString data;
    data = in.readAll();
    ui->editFile_2->setText(data);
    file.close();
}

void editFile::on_buttonBox_accepted()
{
    QFile file(namefile);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out<<ui->editFile_2->toPlainText();
    file.close();
}

void editFile::on_buttonBox_helpRequested()
{
    helpFile *ayuda=new helpFile();
    ayuda->show();
    ayuda->showFile(":/ayuda/ayuda1.htm");
}
