'''
Usage: python GenerateInput.py directory_path <starting_string:optional>
Output goes to: directory_path/*.in (same name as input file, but with .in extension)

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
    if len(sys.argv) < 2:
        sys.exit(f'Usage: python GenerateInput.py directory_path <starting_string:optional>')

    # Get the directory path
    directoryPath = sys.argv[1]
    
    # Get the starting string, if none is provided, match all files
    startingString = ''
    if len(sys.argv) == 3:
        startingString = sys.argv[2] # find all files that start with this string
        
    inputFilePaths = [] # list of .mtx files in the directory
    
    print("Starting string: " + startingString)
    
    # Get all the files in the directory
    for filename in os.listdir(directoryPath):
        if filename.endswith('.mtx') and filename.startswith(startingString):
            inputFilePaths.append(os.path.join(directoryPath, filename))
            
    if len(inputFilePaths) == 0:
        sys.exit(f'No .mtx files found in {directoryPath}')
    
    return inputFilePaths

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
                vertices, ncols, edges = int(line[0]), int(line[1]), int(line[2])
                firstLine = False
                graph = [set() for j in range(vertices)]
            else:
                i, j = int(line[0]), int(line[1])
                graph[i - 1].add(j - 1)

    return graph, vertices, edges


# Writes to file_path.in
def WriteOutput(inputFilePath: str, graph: List[str], vertices: int, edges: int):
    outputFilePath = inputFilePath[:len(inputFilePath) - 4] + '.in'
    print(f'Writing to {outputFilePath}')
    with open(outputFilePath, 'w') as f:
        print(vertices, edges, file=f)
        for i, node in enumerate(graph):
            for j in node:
                print(i, j, file=f)
    print('Done')


def main() -> None:
    inputPaths = GetFilePath()

    for inputFilePath in inputPaths:
        graph, vertices, edges = SetGraph(inputFilePath)
        WriteOutput(inputFilePath, graph, vertices, edges)


if __name__ == '__main__':
    main()
