CC = clang 
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: banhammer.o bf.o speck.o bv.o hash.o ll.o node.o parser.o
	$(CC) -o banhammer banhammer.o bf.o speck.o bv.o hash.o ll.o node.o parser.o

banhammer: banhammer.o bf.o speck.o bv.o hash.o ll.o node.o
	$(CC) -o banhammer banhammer.o bf.o speck.o bv.o hash.o ll.o node.o parser.o

banhammer.o: banhammer.c 
	$(CC) $(CFLAGS) -c banhammer.c 

bf.o: bf.c bv.c speck.c 
	$(CC) $(CFLAGS) -c bf.c bv.c speck.c 

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c 

hash.o: hash.c ll.c node.c speck.c
	$(CC) $(CFLAGS) -c hash.c ll.c node.c speck.c

ll.o: ll.c node.c
	$(CC) $(CFLAGS) -c ll.c node.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c 

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c 

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c 


clean: 
	rm -f *.o banhammer

format:
	clang-format -i -style=file *.c *.h 
