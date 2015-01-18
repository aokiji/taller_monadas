CC=g++
CXX_FLAGS=-Wall -std=c++1y

ej%: ejercicios/ej%.cpp
	$(CC) $(CXX_FLAGS) ejercicios/$@.cpp -o $@

sol%: soluciones/sol%.cpp
	$(CC) $(CXX_FLAGS) soluciones/$@.cpp -o $@

