#!/usr/bin/bash

directory=$1

> results.csv
echo "Vertices, Edges, Average Degree, Number of Threads, Tarjan Vishkin, Tarjan Vishkin with Union Find, Tarjan Vishkin Parallel, Tarjan Vishkin Parallel with Union Find, Graph Name\n" > results.csv
for entry in "$directory"/*
do
  if [[ $entry != *.in ]]
  then
    continue
  fi

  ./main file $entry 8
done