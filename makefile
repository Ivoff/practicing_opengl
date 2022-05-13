OBJS = 	src/*.cpp \
		src/glad/glad.c \
		src/camera/camera.cpp \
		src/application/application.cpp \
		src/application/mouse/mouse.cpp \
		src/application/keyboard/keyboard.cpp \
		src/application/window/window.cpp \
		src/application/imgui/*.cpp \
		src/shader/shader.cpp \
		src/scene/scene.cpp \
		src/utils/*.cpp \
		src/texture/*.cpp \
		src/material/material.cpp \
		src/light/light.cpp \

LIBS_DIR = -Llib
INCLUDE_DIR = -Iinclude -Isrc -Iinclude/imgui
CC = g++
COMPILER_FLAGS = -w -g -std=c++11
LINKER_FLAGS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
OBJ_NAME = a.out

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(INCLUDE_DIR) $(LIBS_DIR) $(LINKER_FLAGS) -o ./build/$(OBJ_NAME)
clear:
	rm -rf ./build/*
run:
	./build/$(OBJ_NAME)
reset:
	make clear && make && make run	
setup:
	./setup.sh