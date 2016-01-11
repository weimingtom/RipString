@echo off
@rmdir /S /Q output
@mkdir output
@for /l %%i in (1,1,500) do (
	echo %%i
	copy test.pdf output\test%%i.pdf
)
@echo on
@pause
