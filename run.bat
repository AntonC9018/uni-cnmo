@echo off

@rem Ninja needs to be in path.
@rem Maybe It is possible to give the path to cmake, but I couldn't find how.
where ninja
IF %ERRORLEVEL% EQU 1   SET PATH=%PATH%;D:\Qt\Tools\Ninja

where qwt.dll
IF %ERRORLEVEL% EQU 1   SET PATH=%PATH%;D:\Coding\C++\libs\qwt

@rem Copied this command from Qt Creator.
@rem -S means Source, -B means Build (directories)
@rem Ninja is used to generate dependency graphs. -G means Generator.
@rem The rest specifies compilers and stuff like that. I'm willing to use gcc, but any compiler should work.
@rem Maybe using windows is even better, since I'm comfortable the just the VS debugger.
cmake -S D:/Coding/C++/cnmo -B D:/Coding/C++/build/cnmo -GNinja "-DCMAKE_BUILD_TYPE:String=Debug" "-DQT_QMAKE_EXECUTABLE:STRING=D:/Qt/5.15.2/mingw81_64/bin/qmake.exe" "-DCMAKE_PREFIX_PATH:STRING=D:/Qt/5.15.2/mingw81_64" "-DCMAKE_C_COMPILER:STRING=D:/msys64/mingw64/bin/gcc.exe" "-DCMAKE_CXX_COMPILER:STRING=D:/msys64/mingw64/bin/g++.exe"

IF %ERRORLEVEL% NEQ 0 GOTO build_failed

@rem Build the project to get an exe.
cmake --build D:/Coding/C++/build/cnmo

IF %ERRORLEVEL% NEQ 0 GOTO build_failed

@rem Call the exe.
D:/Coding/C++/build/cnmo/cnmo.exe

GOTO end_script

:build_failed
echo ======================= Build failed ===========================
popd
exit /b 1

:end_script