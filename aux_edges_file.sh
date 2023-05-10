#!/usr/bin/bash

directory=$1

> results.csv
echo "Vertices, Edges, Average Degree, Graph Name, Tarjan Vishkin, Tarjan Vishkin with Union Find\n" > results.csv
for entry in "$directory"/*
do
  if [[ $entry != *.in ]]
  then
    continue
  fi

  ./main file_aux $entry 8
done
