#!/bin/bash

> results_mpi.doc
> results_seq.doc

for size in 400 800 1200 1600; do
    for n in 2 4 6 8; do
        for rep in {1..10}; do
            mpirun -f ../hosts.txt -n $n ./mpi_version $size >> "results_mpi.doc"
        done
    done

    for rep in {1..10}; do
        ../sequential $size >> "results_seq.doc"
    done

done
