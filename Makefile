all: pong

pong: main.cpp util.hpp pong.hpp setupFunctions.hpp
	g++ -O1 --std=c++11 main.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lm -o pong
