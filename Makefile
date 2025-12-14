SRC := ./src
BIN := ./bin
OBJ := ./obj

CXX = g++
CXXFLAGS =

all: $(BIN)/main.exe

$(BIN)/main.exe: $(OBJ)/main.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)/main.o: $(SRC)/main.cpp $(SRC)/ray.h $(SRC)/camera.h $(SRC)/material.h $(SRC)/thread_pool.h $(SRC)/kd_tree.h $(SRC)/perlin.h $(SRC)/obj_parser.h $(SRC)/triangle.h $(SRC)/cube_map.h | $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(SRC) $< -o $@ -c

$(BIN):
	mkdir "$@"
	
$(OBJ):
	mkdir "$@"

.PHONY: clean
clean:
	rm -f $(BIN)/*.exe $(OBJ)/*.o *.ppm