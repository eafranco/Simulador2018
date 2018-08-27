#ifndef STATUSMEMORY_H
#define STATUSMEMORY_H

#include <QDialog>
#include "proceso.h"


class Proceso;

namespace Ui {
class StatusMemory;
}



class StatusMemory : public QDialog
{
    Q_OBJECT

public:
    explicit StatusMemory(QWidget *parent = 0);
    void recibeProceso(Proceso *procs);
    void actualiza();
    ~StatusMemory();
     Proceso *proceso;

private:
    QString qestado(int estado);
    Ui::StatusMemory *ui;
};

#endif // STATUSMEMORY_H
