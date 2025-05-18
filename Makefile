CXX = g++
CXXFLAGS = -std=c++17 -g -march=native
INCLUDES = -I.
SRC_COMMON = logger/logger.cpp logger/coloring_logger.cpp logger/spanning_logger.cpp logger/traversal_logger.cpp \
             monte_carlo/monte_carlo.cpp monte_carlo/monte_carlo_colorer.cpp monte_carlo/monte_carlo_traversal.cpp monte_carlo/monte_carlo_trees.cpp \
			 common/service.cpp \
			 graph/rand_graph.cpp graph/edge.cpp \
             graph/traversal/traversal.cpp \
             graph/tree/rand_tree.cpp graph/tree/spanning_tree.cpp \
             graph/coloring/colorer.cpp \
			 prufer_graph/random_graph.cpp prufer_graph/prufer.cpp

all: main.exe main_prufer.exe mainspan.exe

main.exe: main.cpp monte_carlo/monte_carlo.cpp $(SRC_COMMON)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) -o $@

main_prufer.exe: main_prufer.cpp $(SRC_COMMON)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) -o $@

mainspan.exe: mainspan.cpp $(SRC_COMMON)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) -o $@

clean:
	del *.exe 2>nul || true