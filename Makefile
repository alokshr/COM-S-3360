SRC := ./src
BIN := ./bin
OBJ := ./obj
CPP_VERSION := c++20

all: clayui barycentric

clayui: $(OBJ)/clayui.o $(OBJ)/clay_renderer_raylib.o
	g++ $^ -o $@

barycentric: $(SRC)/barycentric.cpp $(SRC)/vec3.h
	g++ $(SRC)/barycentric.cpp -o $(BIN)/$@

$(OBJ)/clayui.o: $(SRC)/clayui.cpp $(SRC)/clay.h
	g++ $(SRC)/clayui.cpp -o $@ -c -std=$(CPP_VERSION)

$(OBJ)/clay_renderer_raylib.o: $(SRC)/renderers/raylib/clay_renderer_raylib.c
	g++ $(SRC)/renderers/raylib/clay_renderer_raylib.c -o $@ -c -std=$(CPP_VERSION)

$(BIN):
	mkdir "$@"

.PHONY: clean
clean:
	rm -f $(BIN)/*.exe $(OBJ)/*.o