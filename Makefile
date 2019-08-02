cppsrcfiles = $(wildcard *.cpp)
cppobjfiles = $(patsubst %.cpp,%.o,$(cppsrcfiles))
csrcfiles = $(wildcard *.c)
cobjfiles = $(patsubst %.c,%.o,$(csrcfiles))
allhead = $(wildcard *.h)


all:
	g++ -I ./include/ $(cppsrcfiles) $(csrcfiles) -o app -lwiringPi

up:
	cp $(allhead) include/

clear:
	#rm $(cobjfiles) $(cppobjfiles)
	rm $(allhead)

allcpp:
	g++ $(cppsrcfiles) -c

allc:
	gcc $(csrcfiles) -c
