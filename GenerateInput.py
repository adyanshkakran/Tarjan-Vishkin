'''
Usage: python GenerateInput.py file_path.mtx
Output goes to: file_path.in

Converts matrix market files to directed graphs of the following format

N M
u1 v1
u2 v2
...
uM vM
'''

import os
import sys
from colorama import Fore, Back, Style
from typing import List
#from icecream import ic


def GetFilePath() -> str:
    if len(sys.argv) != 2:
        sys.exit(f'Usage: python GenerateInput.py file_path.mtx')
    elif not sys.argv[1].endswith('.mtx'):
        sys.exit(f'The input file must end in .mtx')
    return sys.argv[1]


# Loads the input file into an adjacency list
def SetGraph(inputFilePath: str) -> List[set]:
    firstLine: bool = True
    with open(inputFilePath, 'r') as f:
        print(f'Reading {inputFilePath}')
        for line in f:
            if line[0] == '%' or line.isspace() or line in [None, '']:
                continue
            line = line.split()
            if firstLine:
                nrows, ncols, nnz = int(line[0]), int(line[1]), float(line[2])
                firstLine = False
                graph = [set() for j in range(nrows)]
            else:
                i, j = int(line[0]), int(line[1])
                graph[i - 1].add(j - 1)

    return graph


# Writes to file_path.in
def WriteOutput(inputFilePath: str, graph: List[str]):
    outputFilePath = inputFilePath[:len(inputFilePath) - 4] + '.in'
    print(f'Writing to {outputFilePath}')
    with open(outputFilePath, 'w') as f:
        # print(len(graph), file=f)
        numVertices = len(graph)
        numEdges = 0
        edges = []
        for i, node in enumerate(graph):
            # print(i,*node,file=f)
            for j in node:
                edges.append((i, j))
                print(i, j)
                numEdges += 1
                
        # go to the beginning of the file, write the number of edges
        f.seek(0, 0)
        print(len(graph), numEdges, file=f)
        for edge in edges:
            print(edge[0], edge[1], file=f)
    print('Done')


def main() -> None:
    inputFilePath = GetFilePath()
    graph = SetGraph(inputFilePath)
    WriteOutput(inputFilePath, graph)


if __name__ == '__main__':
    main()
