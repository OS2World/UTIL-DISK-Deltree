# MS VC++
#CC=	cl
#CFLAGS=-AL -Ox -W4 /F 4000
#BINDIR=dosbin

# OS/2 and VisAge C++
CC=	icc
CFLAGS=-W3 -O+ -DOS2 /B"/STACK:102400"
BINDIR=os2bin

all: 	deltree.exe

file.obj:       file.c config.h file.h
	$(CC) $(CFLAGS) -c file.c

deltree.exe:	deltree.c file.obj
	$(CC) $(CFLAGS) deltree.c file.obj


