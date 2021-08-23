VULKAN_SDK_PATH = /home/ubuntu/Documents/bibliotheque/1.1.106.0/x86_64



FLAGS =  -std=c++17 -pedantic
CFLAGS = -std=c++17 -Wfatal-errors -I$(VULKAN_SDK_PATH)/include -Wall
LINKS =  -lstdc++fs -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan
OBJ = objects

all :  main test
	g++ -o exec main.o $(OBJ)/vulkan.a  $(OBJ)/gnuRender.o $(OBJ)/global.a   $(LINKS)

vulk : 
	(cd vulkan; make all; make clean)
	g++ -o exec main.o $(OBJ)/vulkan.a  $(OBJ)/gnuRender.o $(OBJ)/global.a   $(LINKS)

main: main.cpp
	g++ -c $(FLAGS) $(CFLAGS) -o main.o main.cpp

clear :  
	rm $(OBJ)/*.o 

test: 
	(cd global; make all)
	(cd gnuPlot; make all)
	(cd vulkan; make all)
	
