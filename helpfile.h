#ifndef HELPFILE_H
#define HELPFILE_H

#include <QDialog>

namespace Ui {
class helpFile;
}

class helpFile : public QDialog
{
    Q_OBJECT

public:
    explicit helpFile(QWidget *parent = 0);
    ~helpFile();
    void showFile(QString filename);

private:
    Ui::helpFile *ui;
};

#endif // HELPFILE_H
