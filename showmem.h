#ifndef SHOWMEM_H
#define SHOWMEM_H

#include <QDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QVector>
#include "cpu.h"
#include "proceso.h"

namespace Ui {
class showmem;
}

class showmem : public QDialog
{
    Q_OBJECT

public:
    explicit showmem(QWidget *parent = 0);
    ~showmem();
    void recibeProcesos(QVector<Proceso *> *listaProcs);
    QVector<Proceso *> *listaProcesos;


private:
    Ui::showmem *ui;
    QString estado(int edo);

protected:
    void paintEvent(QPaintEvent *e);
private slots:
    void on_pushButton_clicked();
};

#endif // SHOWMEM_H
