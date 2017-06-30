all:
	g++ tracer.cpp -o tracer -std=c++11 -LC:\MinGW\lib -IC:\MinGW\include\SFML -lsfml-graphics -lsfml-window -lsfml-system