sorterexec: sorter.o
	g++ sorter.o -o sorterexec

sorter.o: sorter.cpp
	g++ -c sorter.cpp

mysorter: main.o
	g++ main.o -o mysorter

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o mysorter
