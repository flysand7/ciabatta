@echo off
pushd %~pd0
if not exist bin mkdir bin
pushd bin
rem cl ..\src\*.c -c -I ..\inc /EHa-
clang ..\src\*.c -c -I ..\inc
lib /out:..\unicope.lib *.o
popd
popd
