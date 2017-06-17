#Makefile para pathping :v
BIN=./OUT/
SOURCE=./C/
LIBS=./H/
LIST= $(BIN)traceroute.o $(BIN)main.o $(BIN)lista.o

traceroute.o: $(SOURCE)pathping.c
	gcc -c $(SOURCE)pathping.c -o $(BIN)traceroute.o -I $(LIBS)
main.o: $(SOURCE)main.c
	gcc -c $(SOURCE)main.c -o $(BIN)main.o -I $(LIBS)
lista.o: $(SOURCE)lista.c
	gcc -c $(SOURCE)lista.c -o $(BIN)lista.o -I $(LIBS)
pathping: traceroute.o main.o lista.o
	gcc -o $(BIN)pathping $(LIST)
clean:
	rm -f $(BIN)*
#		@todmephis