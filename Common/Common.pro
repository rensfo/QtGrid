#-------------------------------------------------
#
# Project created by QtCreator 2016-02-05T12:27:30
#
#-------------------------------------------------

FRAMEWORK = $$(FRAMEWORK)

isEmpty($$FRAMEWORK){
    message('Enviroment variable FRAMEWORK is null')
}

include(CommonBuild.pri)

QT       -= gui

TARGET = $$COMMON_LIB_NAME
TEMPLATE = lib

DEFINES += COMMON_LIBRARY
CONFIG += c++11

SOURCES += NullableBase.cpp \
    Group.cpp \
    Filter.cpp

HEADERS += Nullable.h \
    NullableBase.h \
    Group.h \
    Filter.h \
    ActiveListBase.h \
    Operation.h

OTHER_FILES += CommonInclude.pri \
              Framework.pri

frameworkpri.files = Framework.pri
frameworkpri.path =  $$FRAMEWORK
INSTALLS += frameworkpri \

commonpri.files = CommonInclude.pri
commonpri.path =  $$FRAMEWORK
INSTALLS += commonpri \

headers.files = $$PWD\*.h
headers.path = $$COMMON_INSTALL_PATH
INSTALLS += headers

target.path = $$FRAMEWORK\libs
INSTALLS += target
