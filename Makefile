CC=gcc
CFLAGS=-I.

client_tema: client_tema.c requests.c helpers.c buffer.c parson.c
	$(CC) -o client_tema client_tema.c requests.c helpers.c buffer.c parson.c -Wall

run: client_tema
	./client_tema

clean:
	rm -f *.o client_tema
