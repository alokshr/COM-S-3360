SRC := ./src
BIN := ./bin
OBJ := ./obj

CXX = g++
CXXFLAGS = -Wall -g

all: $(BIN)/vec3test.exe $(BIN)/imagewriting.exe $(BIN)/barycentric.exe $(BIN)/triangle_texturing.exe $(BIN)/image.exe $(BIN)/sphere.exe $(BIN)/main.exe 

$(BIN)/vec3test.exe: $(OBJ)/vec3test.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/imagewriting.exe: $(OBJ)/imagewriting.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/barycentric.exe: $(OBJ)/barycentric.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/triangle_texturing.exe: $(OBJ)/triangle_texturing.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/image.exe: $(OBJ)/image.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/sphere.exe: $(OBJ)/sphere.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/main.exe: $(OBJ)/main.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)/vec3test.o: $(SRC)/tests/vec3test.cpp $(SRC)/vec3.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(OBJ)/imagewriting.o: $(SRC)/learning/imagewriting.cpp $(SRC)/vec3.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(OBJ)/barycentric.o: $(SRC)/learning/barycentric.cpp $(SRC)/vec3.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(OBJ)/triangle_texturing.o: $(SRC)/learning/triangle_texturing.cpp $(SRC)/vec3.h $(SRC)/quaternion.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(OBJ)/image.o: $(SRC)/learning/image.cpp $(SRC)/image.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(OBJ)/sphere.o: $(SRC)/learning/sphere.cpp $(SRC)/ray.h $(SRC)/camera.h $(SRC)/material.h $(SRC)/thread_pool.h $(SRC)/kd_tree.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(OBJ)/main.o: $(SRC)/main.cpp $(SRC)/ray.h $(SRC)/camera.h $(SRC)/material.h $(SRC)/thread_pool.h $(SRC)/kd_tree.h $(SRC)/perlin.h $(SRC)/obj_parser.h $(SRC)/triangle.h  | $(OBJ)
	$(CXX) $< -o $@ -c

$(BIN):
	mkdir "$@"
	
$(OBJ):
	mkdir "$@"

.PHONY: clean
clean:
	rm -f $(BIN)/*.exe $(OBJ)/*.o *.ppm