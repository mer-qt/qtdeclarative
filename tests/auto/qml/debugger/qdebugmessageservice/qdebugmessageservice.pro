CONFIG += testcase
TARGET = tst_qdebugmessageservice
QT += qml network testlib
macx:CONFIG -= app_bundle

SOURCES +=     tst_qdebugmessageservice.cpp

INCLUDEPATH += ../shared
include(../../../shared/util.pri)
include(../shared/debugutil.pri)

TESTDATA = data/*

CONFIG += parallel_test

OTHER_FILES += data/test.qml
