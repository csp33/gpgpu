BIN = ./bin
OBJ = ./obj
SRC = ./src
INC = ./include

FLAGS = -I $(INC) -std=c++11 -w

# Object files
$(OBJ)/create_graph.o : $(SRC)/create_graph.cpp
	g++ $(FLAGS) -c -o $@ $^

$(OBJ)/dijkstra_cpu.o : $(SRC)/dijkstra_cpu.cpp
	g++ $(FLAGS) -c -o $@ $^

$(OBJ)/tools.o : $(SRC)/tools.cpp
	g++ $(FLAGS) -c -o $@ $^


# Binary files

$(BIN)/dijkstra_gpu : $(SRC)/dijkstra_gpu.cpp $(SRC)/tools.cpp
	g++ $(FLAGS) -o $@ $^ -lOpenCL

$(BIN)/create_graph : $(OBJ)/create_graph.o $(OBJ)/tools.o
	g++ $(FLAGS) -o $@ $^


$(BIN)/dijkstra_cpu : $(OBJ)/dijkstra_cpu.o $(OBJ)/tools.o
	g++ $(FLAGS) -o $@ $^


# Scripts

generate_graphs : $(BIN)/create_graph
	$(SRC)/generate_graphs.sh

plot_cpu : measure_cpu
	$(SRC)/plot_cpu.sh;

measure_cpu : $(BIN)/dijkstra_cpu
	$(SRC)/measure_sequential_time.sh;


clean :
	rm obj/* times/* img/* bin/*
