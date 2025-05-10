CXX = g++
CXXFLAGS = -std=c++17 -g -march=native
INCLUDES = -I. -Icommon -Igraph -Igraph/traversal -Igraph/tree -Igraph/coloring -Imonte_carlo -Ilogger -Iprufer_graph -Irandomizer
SRC_COMMON = logger/logger.cpp \
             graph/traversal/traversal.cpp \
             graph/tree/rand_tree.cpp \
             graph/tree/spanning_tree.cpp \
             graph/coloring/colorer.cpp

all: main.exe main_prufer.exe mainspan.exe

main.exe: main.cpp monte_carlo/monte_carlo.cpp $(SRC_COMMON)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) -o $@

main_prufer.exe: main_prufer.cpp $(SRC_COMMON)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) -o $@

mainspan.exe: mainspan.cpp $(SRC_COMMON)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) -o $@

clean:
	del *.exe 2>nul || true