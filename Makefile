CC = g++
CFLAGS = -Wall
SRC = dictionary.cpp main.cpp corrector.cpp candidate.cpp
PRG = corrector

build: clean
	$(CC) $(CFLAGS) $(SRC) -o $(PRG)

clean:
	rm -rf *.o $(PRG)

.PHONY: clean
