@echo off

echo./*
echo.* Check VC++ environment...
echo.*/
echo.

if defined VS110COMNTOOLS (
    set VSVARS="%VS110COMNTOOLS%vsvars32.bat"
    set VC_VER=110
) else if defined VS100COMNTOOLS (
    set VSVARS="%VS100COMNTOOLS%vsvars32.bat"
    set VC_VER=100
)

if not defined VSVARS (
    echo Can't find VC2010 or VC2012 installed!
    goto ERROR
)

echo./*
echo.* Building GemMatrix binary, please wait a while...
echo.*/
echo.

call %VSVARS%
if %VC_VER% == 110 (
    msbuild ..\proj.win32\GemMatrix.sln /t:Clean
    msbuild ..\proj.win32\GemMatrix.sln /p:Configuration="Release" /m   
) else (
    echo Script error.
    goto ERROR
)



:ERROR
echo its done.
pause