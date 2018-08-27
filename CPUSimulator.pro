#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T15:03:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPUSimulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controlsimulador.cpp \
    proceso.cpp \
    consolaproceso.cpp \
    editfile.cpp \
    helpfile.cpp \
    statusmemory.cpp \
    asim.cpp \
    cpu.cpp \
    showmem.cpp \
    hueco.cpp

HEADERS  += mainwindow.h \
    controlsimulador.h \
    proceso.h \
    consolaproceso.h \
    instrucciones.h \
    editfile.h \
    helpfile.h \
    statusmemory.h \
    asim.h \
    cpu.h \
    showmem.h \
    hueco.h

FORMS    += mainwindow.ui \
    consolaproceso.ui \
    editfile.ui \
    helpfile.ui \
    statusmemory.ui \
    showmem.ui

RESOURCES += \
    archivos.qrc

DISTFILES += \
    ayuda/ayuda1.pdf \
    ayuda/ayuda1.htm \
    ejemplos/cadena.asim \
    ejemplos/cadena.lm \
    ejemplos/ej_cadenas.asim \
    ejemplos/ej_cadenas.lm \
    ejemplos/ej_dec.asim \
    ejemplos/ej_dec.lm \
    ejemplos/ej_funcion_suma.asim \
    ejemplos/ej_funcion_suma.lm \
    ejemplos/ej_serie.asim \
    ejemplos/ej_serie.lm \
    ejemplos/ej_viejo.asim \
    ejemplos/ej_viejo.lm \
    ejemplos/Ejemplo1.asim \
    ejemplos/Ejemplo1.lm \
    ejemplos/Ejemplo2.asim \
    ejemplos/Ejemplo2.lm \
    ejemplos/Ejemplo3.asim \
    ejemplos/Ejemplo3.lm \
    ejemplos/Ejemplo4.asim \
    ejemplos/Ejemplo4.lm \
    ejemplos/ejer1.asim \
    ejemplos/ejer2.asim \
    ejemplos/ejer3.asim \
    ejemplos/ejExtra.asim \
    ejemplos/errores.asim \
    ejemplos/parimpar.asim \
    ejemplos/parimpar.lm
