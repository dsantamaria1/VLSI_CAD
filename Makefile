

main.o: Parser.cpp Net.cpp Cell.cpp Site.cpp Placement.cpp main.cpp
	g++ -O2 -std=c++11 $^ -o $@

clean:
	rm -rf *.o
