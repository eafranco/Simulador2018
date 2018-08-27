#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStringListModel>
#include <QtCore>
#include <string.h>
#include "asim.h"
#include "showmem.h"
#include "helpfile.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    simulador = new ControlSimulador();
    enSimulacion=false;
    graficador = new showmem(this);
}

MainWindow::~MainWindow()
{
    if(simulador!=NULL)
        delete simulador;
    delete graficador;
    delete ui;

}

//Crea procesos y queda en espera (NUEVO)(ventanas de consola y estado)
void MainWindow::on_pushButton_clicked()
{
   if(ui->listWidget_Process->selectedItems().length()>0){
     QString filenameX = ui->listWidget_Process->currentItem()->toolTip();

    try{

     simulador->procesos.push_back(new Proceso(filenameX, simulador->procesos, ui->comboBox_2->currentIndex()));

    // simulador->procesos.push_back(new Proceso(filenameX));

    }
    catch(...){
       QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado");
    }
   }
   ui->pushButton_4->setEnabled(true);
   ui->pushButton_2->setEnabled(true);
   ui->pushButton_5->setEnabled(true);
   ui->pushButton_8->setEnabled(true);
}

//Muestra ventanas de consolas de procesos
void MainWindow::on_pushButton_2_clicked()
{
    for(int i=0; i<simulador->procesos.size(); i++)
     simulador->procesos[i]->consola->showNormal();
}

//Muestra ventanas de estados de procesos
void MainWindow::on_pushButton_5_clicked()
{
    for(int i=0; i<simulador->procesos.size(); i++)
    {
     simulador->procesos[i]->ventanaEstado->show();
    }
}

//Cambia a modo depuracion en la ejecucion de instrucciones
//1: muestra cada instruccion ejecutada
void MainWindow::on_radioButton_clicked()
{
    if( ui->radioButton->isChecked() )
       CPU::setDebug(1);
    else
       CPU::setDebug(0);
}


//Actualiza tiempo de espera en simulacion
void MainWindow::on_hsTime_valueChanged(int value)
{
    CPU::tiempo_espera = value;
    ui->time->setText( QString::number( value ) );
}

//Modifica cantidad de RAM disponible
//si esta en ejecucion, se debe reiniciar o verificar limites
void MainWindow::on_hsRAM_valueChanged(int value)
{
  // if (value> CPU::memoriaOcupada) //NO permitir RAM menor a la cantidad dememoria en uso
       //requerida ubicar el proceso con memoria mas alta (Pendiente  --> ya)
   if (value > CPU::mayorMemProceso )
   {
    CPU::RAM = value;
    ui->line_RAM->setText( QString::number(value) );
    graficador->update();

   }
}

void MainWindow::salir()
{
    if(simulador!=NULL)
    {
        delete simulador;
        simulador=NULL;
    }
    this->close();
}

void MainWindow::ayuda()
{
 helpFile *ayuda1=new helpFile();
 ayuda1->show();
 ayuda1->showFile(":/ayuda/ayuda1.htm"); // : ruta absoluta desde la raiz del proyecto
  /*crear recursos en QT:
 Right clicking on your project
 Add new
 Step on the left option QT
 Qt Resource file
 Follow the wizard to add one*/

}

void MainWindow::acercade()
{
    QString msg;
    msg.append("CPU & OS simulator 3.1\n\n");
    msg.append("Es un software simulador de una computadora virtual, de su sistema operativo y de herramientas propias de la programación de sistemas.\n");
    msg.append("El objetivo original es utilizarlo en cursos de programación de sistemas, "
               "compiladores, sistemas operativos y cursos iniciales de fundamentos"
               " de programación, como recurso didáctico para analizar el funcionamiento de la computadora y de los lenguajes de programación.\n\n");
    msg.append("Contacto: Enrique Ayala Franco\neafranco@gmail.com\n\n");
    msg.append("noviembre, 2017\n");
    QMessageBox::about(this,"Acerca de",msg);
    msg.clear();
}


//Detener simulacion y reiniciar
void MainWindow::on_pushButton_3_clicked()
{
   if(enSimulacion==true){
    simulador->apagar=true;  //en simulacion actual
    ui->label_Simula->setText(" ");
   }
}


//Pausar simulacion
void MainWindow::on_pushButton_7_clicked()
{
     if(enSimulacion==true){
        if(simulador->pausa==false){
          simulador->pausa=true;
          ui->pushButton_7->setText("CONTINUAR");
          ui->label_Simula->setText("SIMULACIÓN EN PAUSA");
        }
       else{
           simulador->pausa=false;
           ui->pushButton_7->setText("PAUSAR");
           ui->label_Simula->setText("SIMULACIÓN EN PROCESO");
        }
     }
}

//Iniciar la simulacion
void MainWindow::on_pushButton_4_clicked()
{
    bool seleccionAlgoritmo = true;

    //ejecutar procesos existentes
    if(enSimulacion==false && simulador->procesos.size()>=1)
     {
        switch(ui->comboBox->currentIndex()){
        case 1:

            iniciar();
            simulador->FIFO();
            break;

        case 2:
            iniciar();
            simulador->SJF();
            break;

        case 3:
            iniciar();
            simulador->RoundRobin();
            break;

        case 4:
            iniciar();
            simulador->Prioridad();
            break;

        default:
            QMessageBox::information(this,"Simulador","Es necesario seleccionar un algoritmo");
            seleccionAlgoritmo = false;
            enSimulacion = false;
        }

        if(seleccionAlgoritmo){

            ui->pushButton->setEnabled(true);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_7->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_5->setEnabled(false);
            ui->pushButton_8->setEnabled(false);

            ui->label_Simula->setText(" ");
            QMessageBox::information(this,"Simulador","La simulación ha concluido");

            //prepara la siguiente simulacion
            delete simulador;
            simulador=NULL;

            simulador = new ControlSimulador();
            graficador->recibeProcesos(&simulador->procesos);
            enSimulacion=false;
            simulador->apagar=false;

        }
     }

}



//Abre abre un navegador y  carga archivo ASIM ern lista
void MainWindow::on_pushButton_20_clicked()
{
   //QStringList filenames = QFileDialog::getOpenFileNames(this,tr("Archivos .ASIM"),"/",tr("archivo ASIM (*.asim)") );
    QStringList filenames = QFileDialog::getOpenFileNames(this,"","..",tr("archivo ASIM (*.asim)") );

    if(!filenames.isEmpty())
   {
       int exist = 0;
       for (int i =0;i<filenames.count();i++){
           if((filenames[i]).endsWith(".asim")){

             for(int j = 0; j < ui->listWidget_ASM->count(); j++){
                 if(ui->listWidget_ASM->item(j)->text() == filenames[i].split('/')[filenames[i].split('/').length()-1]){
                     exist = 1;
                     break;
                   }
             }

             if(exist != 1){
                 QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_ASM);
                 newitem->setWhatsThis(filenames[i]);
                 newitem->setText(filenames[i].split('/')[filenames[i].split('/').length()-1]);
                 newitem->setToolTip(filenames[i]);
               }else{
                   //mbox.setText("El archivo "+filename.split('/')[filename.split('/').length()-1]+" ya fue agregado");
                   //mbox.exec();
               }
             exist=0;
             }
         }

       if (ui->listWidget_ASM->count()> 0){
           ui->pushButton_19->setEnabled(true);
           ui->pushButton_17->setEnabled(true);
           ui->pushButton_18->setEnabled(true);
           ui->pushButton_11->setEnabled(true);
       }
     }
}





//Coloca nombre de archivo en la lista LM
void MainWindow::on_pushButton_22_clicked()
{

   //QStringList filenames = QFileDialog::getOpenFileNames(this,tr("Archivos .LM"),"/",tr("archivo LM (*.lm)") );
   QStringList filenames = QFileDialog::getOpenFileNames(this,"","..",tr("archivo LM (*.lm)") );

   if(!filenames.isEmpty())
   {
       int exist = 0;
       for (int i =0;i<filenames.count();i++){
           if((filenames[i]).endsWith(".lm")){

             for(int j = 0; j < ui->listWidget_LM->count(); j++){
                 if(ui->listWidget_LM->item(j)->text() == filenames[i].split('/')[filenames[i].split('/').length()-1]){
                     break;
                   }
             }

             if(exist != 1){
                 QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_LM);
                 newitem->setWhatsThis(filenames[i]);
                 newitem->setText(filenames[i].split('/')[filenames[i].split('/').length()-1]);
                 newitem->setToolTip(filenames[i]);
                 exist=0;
               }else{
                   //mbox.setText("El archivo "+filename.split('/')[filename.split('/').length()-1]+" ya fue agregado");
                   //mbox.exec();
               }
             exist=0;
             }
         }
       if (ui->listWidget_LM->count()> 0){
               ui->pushButton_15->setEnabled(true);
               ui->pushButton_21->setEnabled(true);
               ui->pushButton_6->setEnabled(true);

               ui->pushButton_10->setEnabled(true);
			   ui->pushButton_16->setEnabled(true);
     }
 }
}

//Crea nuevo archivo ASIM
void MainWindow::on_pushButton_14_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save File"),"..",tr("ASIM (*.asim)"));
    if(filename.isEmpty())
        return;
    if(filename.split(" ").length()!= 0){

        int exist = 0;
        for(int i = 0; i < ui->listWidget_ASM->count(); i++){
            if(ui->listWidget_ASM->item(i)->text() == filename.split('/')[filename.split('/').length()-1]){
                exist = 1;
                break;
            }

        }
        if(exist != 1){
            QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_ASM);
            newitem->setWhatsThis(filename);
            newitem->setText(filename.split('/')[filename.split('/').length()-1]);
            newitem->setToolTip(filename);

            QFile file(filename);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.close();

            mFile = new editFile(this);
            mFile->setFileName(filename);
            mFile->show();
        }else{
            mbox.setText("El archivo "+filename.split('/')[filename.split('/').length()-1]+" ya existe");
            mbox.exec();
        }
    }
    if (ui->listWidget_ASM->count()> 0){
        ui->pushButton_19->setEnabled(true);
        ui->pushButton_17->setEnabled(true);
        ui->pushButton_18->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
    }
}

//Edita archivo .ASIM
void MainWindow::on_pushButton_17_clicked()
{
    if(ui->listWidget_ASM->selectedItems().length()>0){
            QString filename = ui->listWidget_ASM->selectedItems()[0]->toolTip();
            mFile = new editFile(this);
            mFile->setFileName(filename);
            mFile->show();
        }
}

//Elimina de la lista el archivo ASIM
void MainWindow::on_pushButton_18_clicked()
{
  ui->listWidget_ASM->takeItem(ui->listWidget_ASM->currentIndex().row());

    if (ui->listWidget_ASM->count()== 0){
      ui->pushButton_19->setEnabled(false);
      ui->pushButton_17->setEnabled(false);
      ui->pushButton_18->setEnabled(false);
      ui->pushButton_11->setEnabled(false);
    }
}

//Ver archivo LM sin modificar
void MainWindow::on_pushButton_21_clicked()
{
    if(ui->listWidget_LM->selectedItems().length()>0){
        QString filename = ui->listWidget_LM->selectedItems()[0]->toolTip();
        mFile = new editFile(this);
        mFile->setFileName(filename);
        mFile->show();
    }
}

//Elimina de lista archivo LM
void MainWindow::on_pushButton_6_clicked()
{
   ui->listWidget_LM->takeItem(ui->listWidget_LM->currentIndex().row());
   if (ui->listWidget_LM->count()== 0){
       ui->pushButton_15->setEnabled(false);
       ui->pushButton_21->setEnabled(false);
       ui->pushButton_6->setEnabled(false);

       ui->pushButton_10->setEnabled(false);
	   ui->pushButton_16->setEnabled(true);

       ui->pushButton_10->setEnabled(false);

      }
}


//Carga archivo en lista de espera de procesos para la ejecución
void MainWindow::on_pushButton_15_clicked()
{
    QMessageBox mbox;
    bool bandera = true;
    std::string archivos;
    int aux = ui->listWidget_LM->count();
    for(int j=0; j<ui->listWidget_LM->count(); j++){

        int exist = 0;
        for(int i = 0; i < ui->listWidget_Process->count(); i++){
          if(ui->listWidget_Process->item(i)->text()==
                   ui->listWidget_LM->item(j)->text().split(".lm")[0]){
           exist = 1;
           bandera = false;
           break;
         }
        }
        if(exist != 1){

         QString filename = ui->listWidget_LM->item(j)->toolTip();
         QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_Process);
         newitem->setWhatsThis(filename);
         QString nombre = (filename.split('/')[filename.split('/').length()-1]).split(".lm")[0];
         newitem->setText(nombre);

         newitem->setToolTip(filename);

         QString str = ui->listWidget_LM->item(j)->text();
         if (aux > 1){
             if (aux >=2){
                 bandera = false;
                 archivos = archivos+ str.toStdString()+ ", " ;
             } else {
                 archivos = archivos+" "+ str.toStdString()+ " ";
             }
             aux--;
         } else {
             archivos = archivos + str.toStdString();
         }
        }
        else{
            aux--;
        }
      }
    if( ui->listWidget_LM->count()>0){
        if(bandera==true){
            mbox.setText("El archivo " + QString::fromStdString(archivos) + " se ha agregado a simulación");
            mbox.exec();
        }else{
            mbox.setText("Los archivos " + QString::fromStdString(archivos) + " se han agregado a simulación");
            mbox.exec();
        }
    }

    if (ui->listWidget_Process->count()> 0){
        ui->pushButton->setEnabled(true);
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_12->setEnabled(true);
    }
 }


// Ejecuta el Programa ensamblador para generar código ejecutable
void MainWindow::on_pushButton_19_clicked()
{
    QMessageBox mbox;
    bool bandera = true, bandera2=true;
    std::string archivos;
    int aux = ui->listWidget_ASM->count();
    for(int j=0; j<ui->listWidget_ASM->count(); j++){
        QString filename = ui->listWidget_ASM->item(j)->toolTip();
        int exist = 0;
        asim archivo(filename); //ejecuta y deja en directorio el resultado
        if(archivo.hayerrores){
          bandera2=false;
          exist = 1;
        }

        filename = filename.split(".asim")[0]+".lm";

        for(int i = 0; i < ui->listWidget_LM->count(); i++){
            if(ui->listWidget_LM->item(i)->text() == filename.split('/')[filename.split('/').length()-1]){
                exist = 1;
                bandera = false;
                break;
            }

        }
        if(exist != 1){
            QListWidgetItem *newitem = new QListWidgetItem(ui->listWidget_LM);
            newitem->setWhatsThis(filename);
            newitem->setText(filename.split('/')[filename.split('/').length()-1]);
            newitem->setToolTip(filename);
            QString str = ui->listWidget_ASM->item(j)->text();
                if (aux > 1){
                    if (aux >=2){
                        bandera = false;
                        archivos = archivos+ str.toStdString()+ ", " ;
                    } else {
                        archivos = archivos+" "+ str.toStdString()+ " ";
                    }
                    aux--;
                } else {
                    archivos = archivos + str.toStdString();
                }
        }else{
            aux--;
        }

    }
    if( ui->listWidget_ASM->count()>0 && bandera2==true){
            if(bandera==true && aux>0){
                mbox.setText("El archivo " + QString::fromStdString(archivos) + " se ha generado con éxito");
                mbox.exec();
            }else{
                if(aux>0){
                    mbox.setText("Los archivos " + QString::fromStdString(archivos) + " se han generado con éxito");
                    mbox.exec();
                }

            }
     }if(bandera2==false){
        mbox.setText("No todos los archivos fueron generados con éxito");
        mbox.exec();
    }

    if (ui->listWidget_LM->count()> 0){
      ui->pushButton_15->setEnabled(true);
      ui->pushButton_21->setEnabled(true);
      ui->pushButton_6->setEnabled(true);
      ui->pushButton_10->setEnabled(true);
    }
}

//carga ventana para visualizar memoria
void MainWindow::on_pushButton_8_clicked()
{
   // graficador = new showmem(this);
    if(simulador!=NULL) {
     graficador->recibeProcesos(&simulador->procesos);
     graficador->show();
  //  graficador->update();
     QTimer::singleShot(1000, this, SLOT(timerUpdate()));
    }
}

//Para actualizar la pantalla del monitor de memoria
void MainWindow::timerUpdate()
{
   if(simulador!=NULL) {
    graficador->update();
    QTimer::singleShot(1000, this, SLOT(timerUpdate()));
    //volver a llamar cada segundo
   }
}

//Quita imagen de lista de procesos para simulación
//si esta en ejecucion no termina el proceso
void MainWindow::on_pushButton_9_clicked()
{
     ui->listWidget_Process->takeItem(ui->listWidget_Process->currentIndex().row());
     if (ui->listWidget_Process->count()== 0){
             ui->pushButton->setEnabled(false);
             ui->pushButton_2->setEnabled(false);
             ui->pushButton_5->setEnabled(false);
             ui->pushButton_8->setEnabled(false);
             ui->pushButton_9->setEnabled(false);
             ui->pushButton_12->setEnabled(false);
      }

}

//BOTON LIMPIAR TODO ASIM
void MainWindow::on_pushButton_11_clicked()
{
    ui->listWidget_ASM->clear();
    if (ui->listWidget_ASM->count()== 0){
    ui->pushButton_19->setEnabled(false);
    ui->pushButton_17->setEnabled(false);
    ui->pushButton_18->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    }

}

//BOTON LIMPIAR TODO LM
void MainWindow::on_pushButton_10_clicked()
{
    ui->listWidget_LM->clear();
if (ui->listWidget_LM->count()== 0){
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_21->setEnabled(false);
    ui->pushButton_6->setEnabled(false);

    ui->pushButton_10->setEnabled(false);
	ui->pushButton_16->setEnabled(false);

    ui->pushButton_10->setEnabled(false);

    }
}

void MainWindow::on_pushButton_12_clicked()
{
    ui->listWidget_Process->clear();
    if(ui->listWidget_Process->count()== 0){
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
        ui->pushButton_9->setEnabled(false);

        ui->pushButton_12->setEnabled(false);
		ui->pushButton_16->setEnabled(false);

        ui->pushButton_12->setEnabled(false);

    }
}


void MainWindow::on_pushButton_13_clicked()
{

	enSimulacion = true;
	ui->pushButton_5->setEnabled(true);
	ui->pushButton_8->setEnabled(true);
	ui->pushButton_3->setEnabled(true);
	ui->pushButton_16->setEnabled(false);
	ui->pushButton_4->setEnabled(false);
	
	QString input =("Prueba1.lm");
	QString filename = QCoreApplication::applicationDirPath() + "/" + input;			
	QString input1 =("Prueba2.lm");
	QString filename1 = QCoreApplication::applicationDirPath() + "/" + input1;	
	QString input2 =("Prueba3.lm");
	QString filename2 = QCoreApplication::applicationDirPath() + "/" + input2;	
	QString input3 =("Prueba4.lm");
	QString filename3 = QCoreApplication::applicationDirPath() + "/" + input3;	
	
	try{
		simulador->procesos.push_back(new Proceso(filename, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename1, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename2, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename3, simulador->procesos, ui->comboBox_2->currentIndex()));
	}
	catch(...){
		QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado"); 
	}
	simulador->RoundRobin();
	
	try{
		simulador->procesos.push_back(new Proceso(filename, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename1, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename2, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename3, simulador->procesos, ui->comboBox_2->currentIndex()));
	}
	catch(...){
		QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado"); 
	}
	simulador->FIFO();
	
	try{
		simulador->procesos.push_back(new Proceso(filename, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename1, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename2, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename3, simulador->procesos, ui->comboBox_2->currentIndex()));
	}
	catch(...){
		QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado"); 
	}
	simulador->SJF();
	
	try{
		simulador->procesos.push_back(new Proceso(filename, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename1, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename2, simulador->procesos, ui->comboBox_2->currentIndex()));
		simulador->procesos.push_back(new Proceso(filename3, simulador->procesos, ui->comboBox_2->currentIndex()));
	}
	catch(...){
		QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado"); 
	}
	simulador->Prioridad();
	ui->pushButton_5->setEnabled(false);
	ui->pushButton_8->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	delete simulador;
	simulador=NULL;
	
	simulador = new ControlSimulador();
	graficador->recibeProcesos(&simulador->procesos);
	enSimulacion=false;
	simulador->apagar=false;
	ui->label_Simula->setText(" ");
	QMessageBox::information(this,"Simulador","La simulación ha concluido");
	
}

void MainWindow::on_pushButton_16_clicked()
{
	enSimulacion = true;
	ui->pushButton_5->setEnabled(true);
	ui->pushButton_8->setEnabled(true);
	ui->pushButton_3->setEnabled(true);
	ui->pushButton_13->setEnabled(false);
	ui->pushButton_4->setEnabled(false);
	
	{
		if(simulador->procesos.size()>=0) //This ensures that we have at least one thing to test
		{
			for(int i = 0; i < ui->listWidget_Process->count(); ++i)
			{
				QString filenameX = ui->listWidget_Process->item(i)->toolTip();				
				
				try{
					simulador->procesos.push_back(new Proceso(filenameX, simulador->procesos, ui->comboBox_2->currentIndex())); //filenameX
				}
				catch(...){
					QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado");
				}				
			}
			
			//benchmark->recibeProcesos(&simulador->procesos); //I'm meant to pass the actual processes.			
			simulador->RoundRobin(); //Hopefully will load our algorithms and run them.
			
			for(int i = 0; i < ui->listWidget_Process->count(); ++i)
			{
				QString filenameX = ui->listWidget_Process->item(i)->toolTip();				
				
				try{
					simulador->procesos.push_back(new Proceso(filenameX, simulador->procesos, ui->comboBox_2->currentIndex())); //filenameX
				}
				catch(...){
					QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado");
				}				
			}
			
			//benchmark->recibeProcesos(&simulador->procesos); //I'm meant to pass the actual processes.			
			simulador->FIFO(); //Hopefully will load our algorithms and run them.
			
			for(int i = 0; i < ui->listWidget_Process->count(); ++i)
			{				
				QString filenameX = ui->listWidget_Process->item(i)->toolTip();				
				
				try{
					simulador->procesos.push_back(new Proceso(filenameX, simulador->procesos, ui->comboBox_2->currentIndex())); //filenameX
				}
				catch(...){
					QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado");
				}				
			}
			
			//benchmark->recibeProcesos(&simulador->procesos); //I'm meant to pass the actual processes.			
			simulador->SJF(); //Hopefully will load our algorithms and run them.
			for(int i = 0; i < ui->listWidget_Process->count(); ++i)
			{
				QString filenameX = ui->listWidget_Process->item(i)->toolTip();				
				
				try{
					simulador->procesos.push_back(new Proceso(filenameX, simulador->procesos, ui->comboBox_2->currentIndex())); //filenameX
				}
				catch(...){
					QMessageBox::information(this,"Error","Archivo no localizado, Proceso no creado");
				}				
			}
			
			//benchmark->recibeProcesos(&simulador->procesos); //I'm meant to pass the actual processes.			
			simulador->Prioridad(); //Hopefully will load our algorithms and run them.
			
		}//end if
	}//end case 1		
	ui->label_Simula->setText(" ");
	QMessageBox::information(this,"Simulador","La simulación ha concluido");
	ui->pushButton_5->setEnabled(false);
	ui->pushButton_8->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_13->setEnabled(true);
	ui->pushButton_4->setEnabled(true);
	delete simulador;
	simulador=NULL;
	
	simulador = new ControlSimulador();
	graficador->recibeProcesos(&simulador->procesos);
	enSimulacion=false;
	simulador->apagar=false;
}




void MainWindow::iniciar(){
    enSimulacion=true;
    ui->label_Simula->setText("SIMULACIÓN EN PROCESO");
    ui->pushButton->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
}

void MainWindow::validar(){
    for(int x=0;x<graficador->listaProcesos->size();x++){
        if(graficador->listaProcesos->at(x)->estado != FINALIZADO){
            ui->pushButton->setEnabled(false);
            break;
        }
        else{
            ui->pushButton->setEnabled(true);
        }
    }


}



