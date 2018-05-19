@echo off
setlocal

set COMP_OPS=/nologo /Zi /O2
set LINK_OPS=/subsystem:console
set SCU_FILE=..\source\main.cpp

pushd ..\build
cl %COMP_OPS% %SCU_FILE% /link %LINK_OPS%
popd

endlocal