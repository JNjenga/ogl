@echo off
set LIB_INCLUDES=/I..\lib\glew\include /I..\lib\glfw3\include /I..\lib\glm /I..\src
set CL_OPTIONS=/ZI /nologo /W3 /WX- /diagnostics:classic /Od /JMC /RTC1 /c /Od /Gd /TP /FC /errorReport:queue /Zc:wchar_t /Zc:forScope /Zc:inline /EHsc
set DEFINES=/D _UNICODE /D UNICODE
set SOURCE_FILES=..\tests\breakout.cpp

set LINK_OPTIONS=/INCREMENTAL /NOLOGO /manifest:embed /DEBUG:FASTLINK /MACHINE:X86 /SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT
set LIB_PATH=/LIBPATH:"..\lib\glew\lib\Win32" /LIBPATH:"..\lib\glfw3\lib-vc2017"
set LIBS=Opengl32.lib glfw3.lib glew32s.lib kernel32.lib user32.lib 
set OBJS=breakout.obj

cd .\Build
cl.exe %LIB_INCLUDES% %CL_OPTIONS% %DEFINES% %SOURCE_FILES%
link.exe %LINK_OPTIONS% %LIB_PATH% %LIBS% %OBJS%
cd ..
