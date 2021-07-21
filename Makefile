ALL = clean build run
LUAHOME = ./lib/lua

all: $(ALL)

build:
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*.cpp \
	-o game \
	-I$(LUAHOME) \
	-L$(LUAHOME) \
	-llua5.3 \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer;

clean:
	rm ./game;

run:
	./game;
