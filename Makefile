CC=g++
TARGET=compiler

all: build

build: main.cpp
	$(CC) main.cpp -o $(TARGET)

build_for_debug: main.cpp
	$(CC) -g main.cpp -o $(TARGET)
