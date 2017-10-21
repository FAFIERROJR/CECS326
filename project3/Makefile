SRC = receiver1.cpp receiver2.cpp sender251.cpp sender257.cpp sender997.cpp
EXE = $(SRC:.cpp=.exe)
CC = g++

make: $(SRC)
	$(CC) $(SRC) -o $(EXE)

run: make
	$./

clean:
	rm -f $(EXE)

.PHONY:
	make clean
