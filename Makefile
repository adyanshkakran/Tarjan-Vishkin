make: main.cpp utils.cpp tarjan-vishkin.cpp tarjan-vishkin-parallel.cpp tarjan-vishkin-uf.cpp tv-parallel-union-find.cpp tarjan.cpp
	g++ -std=c++17 -O3 -o main -fopenmp main.cpp utils.cpp tarjan-vishkin.cpp tarjan-vishkin-parallel.cpp tarjan-vishkin-uf.cpp tv-parallel-union-find.cpp tarjan.cpp
apple:
	g++-12 -O3 -o main -fopenmp *.cpp ./TestingAlgos/*.cpp
debug:
	g++-12 -O3 -DDEBUG -o main -fopenmp main.cpp utils.cpp tarjan-vishkin.cpp tarjan-vishkin-parallel.cpp tarjan-vishkin-uf.cpp tv-parallel-union-find.cpp tarjan.cpp