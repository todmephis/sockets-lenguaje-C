#Makefile para ping :v
BIN=./OUT/
SOURCE=./C/
LIBS=./H/
LIST= $(BIN)ping.o $(BIN)main.o

ping.o: $(SOURCE)ping.c
	gcc -c $(SOURCE)ping.c -o $(BIN)ping.o -I $(LIBS)
main.o: $(SOURCE)main.c
	gcc -c $(SOURCE)main.c -o $(BIN)main.o -I $(LIBS)
ping: ping.o main.o
	gcc -o $(BIN)ping $(LIST)
clean:
	rm -f $(BIN)*
#		@todmephis