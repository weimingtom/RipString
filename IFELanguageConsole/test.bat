@echo off

mkdir output >nul 2>&1
del /S /Q output\*.txt >nul 2>&1

::	echo [process] input\%%~ni.png output\%%~ni.png
for %%i in (%cd%\input\*.txt) do (
	echo [process] input\%%~ni.txt output\%%~ni.txt
	ife.exe 196608 0 2 2 1 ".ACP" "MSIME.Japan" "input\%%~ni.txt" "output\%%~ni.txt"
)
pause
@echo on
