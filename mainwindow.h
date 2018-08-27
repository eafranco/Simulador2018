#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "controlsimulador.h"
#include "editfile.h"
#include "statusmemory.h"
#include "showmem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();                   // Crear procesos (GP)

    void on_pushButton_2_clicked();                 // Ver Consolas (GP)

    void on_pushButton_3_clicked();                 // Detener (CS)

    void on_pushButton_4_clicked();                 // Iniciar (CS)

    void on_pushButton_20_clicked();                // Cargar (ASIM)

    void on_pushButton_14_clicked();                // Nuevo (ASIM)

    void on_pushButton_17_clicked();                // Editar (ASIM)

    void on_pushButton_18_clicked();                // Borrar (ASIM)

    void on_pushButton_21_clicked();                // Ver (LM)

    void on_pushButton_6_clicked();                 // Borrar (LM)

    void on_pushButton_15_clicked();                // Agregar a Simulacion (LM)

    void on_pushButton_5_clicked();                 // Ver Estados (GP)

    void on_pushButton_22_clicked();                // Cargar (LM)

    void on_pushButton_7_clicked();                 // Pausar (CS)

    void on_radioButton_clicked();

    void on_hsTime_valueChanged(int value);

    void on_hsRAM_valueChanged(int value);

    void salir();

    void ayuda();

    void acercade();

    void on_pushButton_19_clicked();                // Generar ejecutable (ASIM)

    void on_pushButton_8_clicked();                 // Ver memoria (GP)

    void timerUpdate();

    void on_pushButton_9_clicked();                 // Quitar (GP)

    void on_pushButton_11_clicked();                // Limpiar (ASIM)

    void on_pushButton_10_clicked();                // Limpiar (LM)

    void on_pushButton_13_clicked();                // Prueba de desempeño (CS)

    void on_pushButton_12_clicked();                // Limpiar (GP)
	
	void on_pushButton_16_clicked();


    void iniciar();

    void validar();

private:
    Ui::MainWindow *ui;
    ControlSimulador *simulador;
    bool enSimulacion;
    QMessageBox mbox;
    editFile *mFile;
    showmem* graficador;
};

#endif // MAINWINDOW_H
