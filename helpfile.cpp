#include "helpfile.h"
#include "ui_helpfile.h"
#include<QTextStream>
#include<QString>
#include<QFile>
#include<QMessageBox>

helpFile::helpFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpFile)
{
    ui->setupUi(this);
}

helpFile::~helpFile()
{
    delete ui;
}

void helpFile::showFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString data;
    data = in.readAll();
    ui->textBrowser->setText(data);
    file.close();
}

