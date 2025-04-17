QT += core
CONFIG += console
CONFIG -= app_bundle

TARGET = mi_proyecto
INCLUDEPATH += $$PWD
SOURCES += main.cpp \
           enmascaramiento.cpp \
           transformaciones.cpp \
           experimentos.cpp \
           utilidades.cpp
HEADERS += utilidades.h \
           enmascaramiento.h \
           transformaciones.h \
           experimentos.h
