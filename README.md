Dedekind Number Calculation using Random Walks

Overview

This program calculates Dedekind numbers up to 12.

In this implementation, we utilize random walks to approach the problem, relying on the Monte Carlo method to estimate the value.

Dedekind Numbers
The Dedekind number M(n) is defined as the number of antichains in the power set of a set of size n. The Dedekind numbers grow extremely fast as n increases, and they are known for their large computational complexity when calculating for larger values of n.

Execution: 
1. change DIMENSION constant in ShortList.h in line 4.
2. run make command
3. change number of jobs to be run in parallel accoring to the number of vCPU on your machine in run.sh
4. execute script run.sh, it should run for several hours

Post-processing:
1. merge all result files into one
cat *.*.csv > res.csv

2. use sqlite to aggregate the result and find execution time
sqlite3 res.db < process.sql

3. take a look at result.csv - file with statistics of 40,000 loops
4. take a look at runtime.csv - it contains time in seconds, to do all tasks

In result.sql, identify 1-layer records, and compute Dedekind number from it.
