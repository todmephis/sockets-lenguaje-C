#Makefile para traceroute :v
BIN=./OUT/
SOURCE=./C/
LIBS=./H/
LIST= $(BIN)traceroute.o $(BIN)main.o

traceroute.o: $(SOURCE)traceroute.c
	gcc -c $(SOURCE)traceroute.c -o $(BIN)traceroute.o -I $(LIBS)
main.o: $(SOURCE)main.c
	gcc -c $(SOURCE)main.c -o $(BIN)main.o -I $(LIBS)
traceroute: traceroute.o main.o
	gcc -o $(BIN)traceroute $(LIST)
clean:
	rm -f $(BIN)*
#		@todmephis