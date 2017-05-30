@echo off

cl /nologo /EHsc /c C:\Users\Erik\Documents\sfppp\cpp\main\common\UbxStream.cpp /Fo.\obj\UbxStream.obj
cl /nologo /EHsc /c C:\Users\Erik\Documents\sfppp\cpp\main\common\PolyFit.cpp /Fo.\obj\PolyFit.obj
cl /nologo /EHsc /c  Segments.cpp /Fo.\obj\Segments.obj /IC:\Users\Erik\Documents\sfppp\cpp\main\common
cl /nologo /EHsc obj/UbxStream.obj obj/Segments.obj obj/PolyFit.obj uxsd.cpp /Fe.\exe\uxsd.exe /Fo.\obj\uxsd.obj /IC:\Users\Erik\Documents\sfppp\cpp\main\common