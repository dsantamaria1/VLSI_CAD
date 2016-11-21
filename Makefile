

main.o: Parser.cpp Net.cpp Cell.cpp Site.cpp Placement.cpp main.cpp
	g++ -std=c++11 -O2 $^ -o $@

clean:
	rm -rf *.o
