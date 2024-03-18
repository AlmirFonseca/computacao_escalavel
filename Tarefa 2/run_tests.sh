#!/bin/bash

# Compile the program
g++ -std=c++11 main.cpp -o main

# Define parameter ranges
Ns=(10 100 1000 10000 100000 1000000) # Number of elements
Ms=(1 2 3 4 5 6 7 8 9 10) # Number of threads
methods=('r' 'i') # Recursive and iterative
approaches=(1 2 3 4 5) # Different approaches
oversampling=10 # Number of times to run each configuration for oversampling

# If there is a "results.csv" file, remove it
if [ -f results.csv ]; then
    rm results.csv
fi

# If there is a "results.txt" file, remove it
if [ -f results.txt ]; then
    rm results.txt
fi

# Write the header of the CSV file
echo "N,M,approach,method,numPrime,totalTime,timePerThread" > results.csv

# Iterate over each parameter set
for N in "${Ns[@]}"; do
    for M in "${Ms[@]}"; do
        for method in "${methods[@]}"; do
            for approach in "${approaches[@]}"; do
                for (( o=1; o<=oversampling; o++ )); do
                    echo "Running: N=$N, M=$M, method=$method, approach=$approach, iteration=$o"
                    ./main $N $M $method $approach >> results.txt
                    # Sleep for a short time to avoid overwhelming the system
                    sleep 0.01
                done
            done
        done
    done
done

echo "All runs completed."

# The execution line (on the bash terminal) is:
# ./run_tests.sh