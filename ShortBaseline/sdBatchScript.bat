@echo off
REM exe\uxsd.exe -f C:\Users\Erik\Documents\sfppp\cpp\main\shortbaseline\exampledata\m8t119470035.ubx
REM for /r %%i in (exampledata\*) do exe\uxsd.exe -f %%i

REM for /r %%i in (.\..\..\..\thesis\fieldtest_2\data\rpi2\mon1_5_thue4_5\*) do exe\uxsd.exe -f %%i


for %%f in (C:\Users\Erik\Documents\thesis\fieldtest_2\data\rpi2\mon1_5_thue4_5\*) do ( exe\uxsd.exe -f %%f )