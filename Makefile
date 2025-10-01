SRC := ./src
BIN := ./bin
OBJ := ./obj
CPP_VERSION := c++20

all: clayui learning

learning: imagewriting barycentric triangle_texturing

clayui: $(OBJ)/clayui.o $(OBJ)/clay_renderer_raylib.o
	g++ $^ -o $@

imagewriting: $(SRC)/learning/imagewriting.cpp $(SRC)/vec3.h
	g++ $(SRC)/learning/imagewriting.cpp -o $(BIN)/$@

barycentric: $(SRC)/learning/barycentric.cpp $(SRC)/vec3.h
	g++ $(SRC)/learning/barycentric.cpp -o $(BIN)/$@

triangle_texturing: $(SRC)/learning/triangle_texturing.cpp $(SRC)/vec3.h
	g++ $(SRC)/learning/triangle_texturing.cpp -o $(BIN)/$@

$(OBJ)/clayui.o: $(SRC)/clayui.cpp $(SRC)/clay.h
	g++ $(SRC)/clayui.cpp -o $@ -c -std=$(CPP_VERSION)

$(OBJ)/clay_renderer_raylib.o: $(SRC)/renderers/raylib/clay_renderer_raylib.cpp
	g++ $(SRC)/renderers/raylib/clay_renderer_raylib.cpp -o $@ -c -std=$(CPP_VERSION)

$(BIN):
	mkdir "$@"

.PHONY: clean
clean:
	rm -f $(BIN)/*.exe $(OBJ)/*.o *.ppm