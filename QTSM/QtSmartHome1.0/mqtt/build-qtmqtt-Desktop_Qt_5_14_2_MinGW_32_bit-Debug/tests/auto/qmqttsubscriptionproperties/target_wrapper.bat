@echo off
SetLocal EnableDelayedExpansion
(set PATH=C:\Users\JTL\Desktop\QtSmartHome\mqtt\build-qtmqtt-Desktop_Qt_5_14_2_MinGW_32_bit-Debug\bin;D:\QT\creator\5.14.2\mingw73_32\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=D:\QT\creator\5.14.2\mingw73_32\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=D:\QT\creator\5.14.2\mingw73_32\plugins
)
%*
EndLocal
