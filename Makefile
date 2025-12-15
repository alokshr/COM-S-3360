SRC := ./src
BIN := ./bin
OBJ := ./obj

CXX = g++
CXXFLAGS =

all: $(BIN)/main.exe

$(BIN)/main.exe: $(OBJ)/main.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)/main.o: $(SRC)/main.cpp $(SRC)/camera.h $(SRC)/collidable_list.h $(SRC)/kd_tree.h $(SRC)/texture.h $(SRC)/sphere.h $(SRC)/quad.h $(SRC)/triangle.h $(SRC)/obj_parser.h $(SRC)/constant_medium.h | $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(SRC) $< -o $@ -c

$(BIN):
	mkdir "$@"
	
$(OBJ):
	mkdir "$@"

.PHONY: clean run
clean:
	rm -f $(BIN)/*.exe $(OBJ)/*.o *.ppm

run: $(BIN)/main.exe
	$(BIN)/main.exe $(ARGS)