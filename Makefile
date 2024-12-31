compile:./main.cpp
	g++ -c -g -Wall ./main.cpp ./class_implementations/Map.cpp ./class_implementations/Game.cpp
	g++ main.o Map.o Game.o -o app -lsfml-graphics -lsfml-window -lsfml-system

run:
	./app
