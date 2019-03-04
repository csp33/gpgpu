BIN = ./bin
OBJ = ./obj
SRC = ./src
INC = ./include

FLAGS = -I $(INC) -std=c++11

generate_graphs : $(BIN)/create_graph
	$(SRC)/generate_graphs.sh

$(BIN)/create_graph : $(OBJ)/create_graph.o
	g++ $(FLAGS) -o $@ $^

$(OBJ)/create_graph.o : $(SRC)/create_graph.cpp $(OBJ)/tools.o
	g++ $(FLAGS) -c -o $@ $^

$(BIN)/dijkstra_cpu : $(SRC)/dijkstra_cpu.cpp
	g++ $(FLAGS) -o $@ $^


$(OBJ)/tools.o : $(SRC)/tools.cpp
	g++ $(FLAGS) -c -o $@ $^

clean :
	rm obj/* times/* img/* bin/*

plot_cpu : measure_cpu
	$(SRC)/plot_cpu.sh;

measure_cpu : $(BIN)/dijkstra_cpu
	$(SRC)/measure_sequential_time.sh;

$(BIN)/dijkstra_gpu : $(SRC)/dijkstra_gpu.cpp
	g++ $(FLAGS) -l opencl -o $@ $^
