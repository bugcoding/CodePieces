#¿®∫≈∆•≈‰Makefile


CC = gcc
FLAGS = -Wall -std=c99
OBJECT = main.o braceStack.o handleInput.o

main: $(OBJECT)
	$(CC) $(FLAGS) -o main $(OBJECT)

main.o: main.c main.h
	$(CC) $(FLAGS) -c main.c
braceStack.o: braceStack.c braceStack.h
	$(CC) $(FLAGS) -c braceStack.c
handleInput.o: handleInput.c handleInput.h
	$(CC) $(FLAGS) -c handleInput.c

.PHONY: clean
clean:
	del $(OBJECT) main.exe

