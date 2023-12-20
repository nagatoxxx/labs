CFLAGS=-Wall -pedantic -g -c
NAME=db
CC=gcc

all: $(NAME)

$(NAME): main.o db.o row.o serialization.o
	$(CC) main.o db.o row.o serialization.o -o $(NAME)

main.o:
	$(CC) $(CFLAGS) main.c

db.o:
	$(CC) $(CFLAGS) db.c

row.o:
	$(CC) $(CFLAGS) row.c

clean:
	rm *.o
