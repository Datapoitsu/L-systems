ifeq ($(OS),Windows_NT)
	.DEFAULT_GOAL := windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		.DEFAULT_GOAL := linux
	endif
	ifeq ($(UNAME_S),Darwin)
		.DEFAULT_GOAL := mac
	endif
endif

windows: main.exe

linux: main

main.exe: src/include main.cpp
	g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lsdl2_image

main: src/include main.cpp
	g++ -I src/include -o main main.cpp  `sdl2-config --cflags --libs` -lSDL2_image