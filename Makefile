compile:./main.cpp
	g++ -c -g -Wall ./main.cpp ./Map.cpp
	g++ main.o Map.o -o app -lsfml-graphics -lsfml-window -lsfml-system

run:
	./app
