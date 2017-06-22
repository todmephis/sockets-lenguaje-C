#Makefile para analizadorllc :v
BIN=./OUT/
SOURCE=./C/
LIBS=./H/
LIST= $(BIN)analizadorllc.o $(BIN)main.o

analizadorllc.o: $(SOURCE)analizadorllc.c
	gcc -c $(SOURCE)analizadorllc.c -o $(BIN)analizadorllc.o -I $(LIBS)
main.o: $(SOURCE)main.c
	gcc -c $(SOURCE)main.c -o $(BIN)main.o -I $(LIBS)
analizadorllc: analizadorllc.o main.o
	gcc -o $(BIN)analizadorllc $(LIST)
clean:
	rm -f $(BIN)*
#		@todmephis