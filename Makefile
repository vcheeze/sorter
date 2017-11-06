all: sorterexec mysorter

sorterexec: sorter.o
	g++ sorter.o -o sorterexec

mysorter: main.o
	g++ main.o -o mysorter

sorter.o: sorter.cpp
	g++ -c sorter.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o mysorter sorterexec output/*
