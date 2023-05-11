# Tarjan Vishkin Implementation

This is a simple implementation of the Tarjan-Vishkin using Union Find  algorithm for finding the biconnected components of a graph. It is based on the algorithm in the [research paper](https://epubs.siam.org/doi/epdf/10.1137/0214061).

## Usage
Compile the code using the following command:
```
make
```
Run the code using the following command:
```
./main <command> <operator>
```
The following commands are supported:
```
./main random 1
```
This generates random graphs for N vertices and various edge densities and runs the algorithm on them. The output is stored in the file `results.csv`.

<hr>

```
./main random 2
```
This runs a user provided graph on the algorithms while varying the number of threads. The output is stored in the file `results.csv`.

<hr>


```
./main random 3
```
This runs various dense, complete graphs on the algorithms. The output is stored in the file `results.csv`.

<hr>

```
./main random 4
```
This runs an N vertex graph with varying edge densities on the algorithms. The output is the number of edges in the auxiliary graph. The output is stored in the file `results.csv`.

<hr>

```
./main random 5
```
This runs an N vertex graph with average degree of 2 on the Tarjan-Vishkin (Parallel with UF) algorithm and the Tarjan Algorithm. The output is the time taken by the algorithms. The output is stored in the file `results.csv`.

<hr>

```
./main file <path>
```
This runs the algorithms on the graph provided in the file at the path `<path>`. The output is stored in the file `results.csv`.

<hr>

```
./main file_aux <path>
```
This runs the algorithms on the graph provided in the file at the path `<path>` and outputs the number of edges in the auxiliary graph. The output is stored in the file `results.csv`.

<hr>

```
./main other <path>
```
This runs the Tarjan-Vishkin (Parallel with UF) algorithm and Tarjan's Algorithm on the graph provided in the file at the path `<path>` and outputs the time taken by the algorithms. The output is stored in the file `results.csv`.