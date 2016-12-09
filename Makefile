

main.o: Parser.cpp Net.cpp Cell.cpp Site.cpp Placement.cpp main.cpp
	g++ -O2 -std=c++11 $^ -o $@

stampede: Parser.cpp Net.cpp Cell.cpp Site.cpp Placement.cpp main.cpp
	g++ -O2 -I/opt/apps/intel15/boost/1.61.0/include -std=c++11 $^ -o main.o
clean:
	rm -rf *.o
