CXX = g++ --std=c++17

SRC = src/WeightManagerMat.cpp src/Curve.cpp
OBJ = .o/WeightManagerMat.o .o/Curve.o
LIB = lib/splinekit.a

init:
	-mkdir -p .bin
	-mkdir -p .o
	-mkdir -p lib

.o/%.o: src/%.cpp
	$(CXX) -c $< -o $@ -I ./include

build: $(OBJ)
	ar rcs $(LIB) $(OBJ)

build_demo: build
	$(CXX) ./example/demo.cpp ./lib/splinekit.a -o ./.bin/demo -I ./include

run_demo: build_demo
	./.bin/demo