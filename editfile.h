#ifndef EDITFILE_H
#define EDITFILE_H

#include <QDialog>
#include <QString>

namespace Ui {
class editFile;
}

class editFile : public QDialog
{
    Q_OBJECT

public:
    explicit editFile(QWidget *parent = 0);
    ~editFile();
    void setFileName(QString filename);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_helpRequested();

private:
    Ui::editFile *ui;
    QString namefile;
};

#endif // EDITFILE_H
