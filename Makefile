cppsrcfiles = $(wildcard *.cpp)
cppobjfiles = $(patsubst %.cpp,%.o,$(cppsrcfiles))
csrcfiles = $(wildcard *.c)
cobjfiles = $(patsubst %.c,%.o,$(csrcfiles))
allhead = $(wildcard *.h)
abckfile = $(wildcard *~)

all:
	g++ -I ./include/ $(cppsrcfiles) $(csrcfiles)-o app -lwiringPi

up:
	#cp $(allhead) include/
	#rm $(allhead)
	mv $(cppsrcfiles) $(csrcfiles)

clear:
	rm $(cobjfiles) $(cppobjfiles)
	rm $(allhead)
	rm $(abckfile)

allcpp:
	g++ $(cppsrcfiles) -c

allc:
	gcc $(csrcfiles) -c
