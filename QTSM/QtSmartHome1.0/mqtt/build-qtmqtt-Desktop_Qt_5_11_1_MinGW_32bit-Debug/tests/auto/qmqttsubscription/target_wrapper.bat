@echo off
SetLocal EnableDelayedExpansion
(set PATH=D:\QT\QT_Project\mqtt\build-qtmqtt-Desktop_Qt_5_11_1_MinGW_32bit-Debug\bin;D:\QT\QT5.11.1\5.11.1\mingw53_32\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=D:\QT\QT5.11.1\5.11.1\mingw53_32\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=D:\QT\QT5.11.1\5.11.1\mingw53_32\plugins
)
%*
EndLocal
