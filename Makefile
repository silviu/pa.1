CC = g++
CFLAGS = -Wall -O1 -O3
SRC = dictionary.cpp main.cpp corrector.cpp candidate.cpp
PRG = corrector

build: clean
	$(CC) $(CFLAGS) $(SRC) -o $(PRG)

run:
	./$(PRG)

clean:
	rm -rf *.o $(PRG)

.PHONY: clean
