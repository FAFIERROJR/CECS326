SRC = receiver1.cpp receiver2.cpp sender251.cpp sender257.cpp sender997.cpp
EXE = $(SRC:.cpp=.exe)

build: $(SRC)
	g++ receiver1.cpp -o receiver1.exe
	g++ receiver2.cpp -o receiver2.exe
	g++ sender997.cpp -o sender997.exe
	g++ sender257.cpp -o sender257.exe
	g++ sender251.cpp -o sender251.exe

clean:
	rm -f $(EXE)

.PHONY:
	make clean run
