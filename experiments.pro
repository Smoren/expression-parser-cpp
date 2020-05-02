TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        node.cpp \
        parser.cpp

HEADERS += \
    mysharedpointer.h \
    node.h \
    parser.h
