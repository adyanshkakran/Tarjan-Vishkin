make:
	g++ -std=c++17 -O3 -o main -fopenmp *.cpp
apple:
	g++-12 -O3 -o main -fopenmp *.cpp