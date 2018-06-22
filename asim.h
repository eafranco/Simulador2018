#ifndef ASIM_H
#define ASIM_H

#include <QObject>

/**
 * @brief The asim class
 * Clase que implementa un programa ensamblador para traducir a lenguaje máquina
 *
 */

class asim : public QObject
{
public:
    asim(QString nombre);
   bool hayerrores;
};


#endif // ASIM_H
