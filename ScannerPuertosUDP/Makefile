#Makefile para scannerudp :v
BIN=./OUT/
SOURCE=./C/
LIBS=./H/
LIST= $(BIN)protocolos.o $(BIN)main.o

protocolos.o: $(SOURCE)protocolos.c
	gcc -c $(SOURCE)protocolos.c -o $(BIN)protocolos.o -I $(LIBS)
main.o: $(SOURCE)main.c
	gcc -c $(SOURCE)main.c -o $(BIN)main.o -I $(LIBS)
scannerudp: protocolos.o main.o
	gcc -o $(BIN)scannerudp $(LIST)
clean:
	rm -f $(BIN)*
#		@todmephis