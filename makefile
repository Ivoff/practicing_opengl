OBJS = src/*.cpp src/glad/glad.c
LIBS_DIR = -Llib
INCLUDE_DIR = -Iinclude -Isrc
CC = g++
COMPILER_FLAGS = -w
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