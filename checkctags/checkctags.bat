@ctags -u --fields=n -o lcode.txt lcode.c
@lua checkctags.lua > checkctags.dot
@dot -Tgif -ocheckctags.gif checkctags.dot
@pause
