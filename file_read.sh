#!/usr/bin/bash

directory=$1

> results.csv
echo "Vertices, Edges, Average Degree, Number of Threads, Graph Name, Tarjan Vishkin Parallel with Union Find, Tarjan, CK, CK(TV)\n" > results.csv
for entry in "$directory"/*
do
  if [[ $entry != *.in ]]
  then
    continue
  fi

  ./main other $entry
done