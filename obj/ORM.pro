QT += core sql
QT -= gui

OBJECTS_DIR = ./
DESTDIR = ../bin

TARGET = ORM
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += \
    ../src/main.cpp \
    ../src/tableschema.cpp \
    ../src/model.cpp \
    ../src/query.cpp \
    ../src/globaldata.cpp

HEADERS += \
    ../src/tableschema.h \
    ../src/globaldata.h \
    ../src/model.h \
    ../src/query.h \
    ../src/table.h


