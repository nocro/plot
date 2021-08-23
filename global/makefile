FLAGS =  -Wall -std=c++17 -pedantic -Wfatal-errors
LINKS =  -lstdc++fs
OBJ= ../objects

all: line settings hist	plotCaller render
	ar rvs $(OBJ)/global.a line.o settings.o hist.o	plotCaller.o render.o

line: line.cpp 
	g++ -c $(FLAGS) -o line.o line.cpp

settings: settings.cpp
	g++ -c $(FLAGS) -o settings.o settings.cpp

hist: hist.cpp
	g++ -c $(FLAGS) -o hist.o hist.cpp

plotCaller: plotCaller.cpp
	g++ -c $(FLAGS) -o plotCaller.o plotCaller.cpp 

render: render.cpp
	g++ -c $(FLAGS) -o render.o render.cpp
