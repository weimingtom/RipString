@set path=D:\software\portable-pn2342350;%path%
@set check_path=.
@ctags.exe --recurse -o check_ctags_output.txt --fields=aSi "%check_path%\js"
@call check_ctags_redefined.bat
@pause
