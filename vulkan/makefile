VULKAN_SDK_PATH = /home/ubuntu/Documents/bibliotheque/1.1.106.0/x86_64

CFLAGS = -std=c++17 -Wfatal-errors -I$(VULKAN_SDK_PATH)/include -Wall
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan
OBJ = ../objects

all: window buffer uniformBuffer device context semaphores vulkanRender
	ar rvs $(OBJ)/vulkan.a window.o buffer.o uniformBuffer.o device.o context.o semaphores.o vulkanRender.o


window: Window.cpp
	g++ -c $(CFLAGS) -o window.o Window.cpp 

buffer: Buffer.cpp
	g++ -c $(CFLAGS) -o buffer.o Buffer.cpp

uniformBuffer: UniformBuffer.cpp
	g++ -c $(CFLAGS) -o uniformBuffer.o UniformBuffer.cpp

device: Device.cpp
	g++ -c $(CFLAGS) -o device.o Device.cpp

context: Context.cpp
	g++ -c $(CFLAGS) -o context.o Context.cpp

semaphores: Semaphores.cpp
	g++ -c $(CFLAGS) -o semaphores.o Semaphores.cpp

vulkanRender: VulkanRender.cpp
	g++ -c $(CFLAGS) -o vulkanRender.o VulkanRender.cpp

clean: 
	rm *.o


shader:
	./compile.sh
