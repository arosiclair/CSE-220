# Name of the C file to be compiled
CFILE=hw8.c
# Name of the main file to be linked
MAIN=main.o
# Flags to be compiled with
FLAGS=-Wall -Werror -g
# Desired name of the executable 
EXENAME=hw8

all: hw8

hw8: hw8.o $(MAIN)
	gcc hw8.o $(MAIN) -o $(EXENAME)

hw8.o: $(CFILE)
	gcc $(FLAGS) -c $(CFILE)

clean:
	rm hw8.o $(EXENAME)
