output: main.o
	g++ main.o -o mysorter

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o mysorter
