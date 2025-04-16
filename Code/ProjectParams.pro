QT += core
CONFIG += console
CONFIG -= app_bundle

TARGET = mi_proyecto
INCLUDEPATH += $$PWD
SOURCES += main.cpp \
           transformaciones.cpp \
           experimentos.cpp
HEADERS += utilidades.h \
           transformaciones.h \
           experimentos.h
