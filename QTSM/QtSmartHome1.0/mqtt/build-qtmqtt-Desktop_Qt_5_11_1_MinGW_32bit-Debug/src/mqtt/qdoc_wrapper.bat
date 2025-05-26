@echo off
SetLocal EnableDelayedExpansion
(set QT_VERSION=5.15.3)
(set QT_VER=5.15)
(set QT_VERSION_TAG=5153)
(set QT_INSTALL_DOCS=D:/QT/QT5.11.1/Docs/Qt-5.11.1)
(set BUILDDIR=D:/QT/QT_Project/mqtt/build-qtmqtt-Desktop_Qt_5_11_1_MinGW_32bit-Debug/src/mqtt)
D:\QT\QT5.11.1\5.11.1\mingw53_32\bin\qdoc.exe %*
EndLocal
