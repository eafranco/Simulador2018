#ifndef CONSOLAPROCESO_H
#define CONSOLAPROCESO_H

#include <QDialog>

namespace Ui {
class ConsolaProceso;
}

class ConsolaProceso : public QDialog
{
    Q_OBJECT

public:
    explicit ConsolaProceso(QWidget *parent = 0);
    ~ConsolaProceso();
    void Display(QString msg);
    QString getInput();
    bool pendiente; //dato en input pendiente de leer
    void setFocusLine(); //se pone en input

private slots:

    void on_input_returnPressed();

private:
    Ui::ConsolaProceso *ui;
};

#endif // CONSOLAPROCESO_H
