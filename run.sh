#!/bin/bash

# Folder where the 'walk' program is located
program_folder="."

# Total number of batches
total_batches=1000

# Maximum number of parallel jobs
max_parallel_jobs=16

# Batch execution loop
for ((batch_no=1; batch_no<=total_batches; batch_no++)); do
    # Run the program in the background
    "$program_folder/walk" "$batch_no" &

    # Limit to max_parallel_jobs
    if (( batch_no % max_parallel_jobs == 0 )); then
        wait  # Wait for all background jobs to finish
    fi
done

# Ensure all remaining jobs finish
wait

echo "All batches completed."
