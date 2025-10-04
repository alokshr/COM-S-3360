SRC := ./src
BIN := ./bin
OBJ := ./obj

CXX = g++
CXXFLAGS = -Wall

all: $(BIN)/imagewriting.exe $(BIN)/barycentric.exe $(BIN)/triangle_texturing.exe $(BIN)/image.exe

$(BIN)/imagewriting.exe: $(OBJ)/imagewriting.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/barycentric.exe: $(OBJ)/barycentric.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/triangle_texturing.exe: $(OBJ)/barycentric.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/image.exe: $(OBJ)/image.o | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)/imagewriting.o: $(SRC)/learning/imagewriting.cpp $(SRC)/vec3.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(OBJ)/barycentric.o: $(SRC)/learning/barycentric.cpp $(SRC)/vec3.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(OBJ)/triangle_texturing.o: $(SRC)/learning/triangle_texturing.cpp $(SRC)/vec3.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(OBJ)/image.o: $(SRC)/learning/image.cpp $(SRC)/image.h | $(OBJ)
	$(CXX) $< -o $@ -c

$(BIN):
	mkdir "$@"
	
$(OBJ):
	mkdir "$@"

.PHONY: clean
clean:
	rm -f $(BIN)/*.exe $(OBJ)/*.o *.ppm