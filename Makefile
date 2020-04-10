CC = gcc
CFLAGS = -Wall -g 

build: tema3

tema3: tema3.c ttrie.c functii.c
		$(CC) $(CFLAGS) $^ -o tema3

clean:
	rm -f *.o tema3

run: build
	./tema3 ${ARGS}