#!/bin/bash

# Define paths
INPUT_DIR="./input_files"
BRUTE_EXE="./BruteForce_Solution"
OPTIMIZED_EXE="./Optimized_Solution"

# 1. Select the Algorithm
echo "------------------------------------------------"
echo "Select the algorithm to run:"
echo "1) Brute Force"
echo "2) Optimized"
echo "------------------------------------------------"
read -p "Selection [1-2]: " algo_choice

if [ "$algo_choice" == "1" ]; then
    EXE=$BRUTE_EXE
    LABEL="BRUTE FORCE"
elif [ "$algo_choice" == "2" ]; then
    EXE=$OPTIMIZED_EXE
    LABEL="OPTIMIZED"
else
    echo "Invalid algorithm selection."
    exit 1
fi

# Check if executable exists
if [ ! -f "$EXE" ]; then
    echo "Error: $EXE not found. Please compile your code first."
    exit 1
fi

# 2. Select the Input File
echo -e "\n------------------------------------------------"
echo "Available input files:"
echo "------------------------------------------------"

# Map files to an array
files=($INPUT_DIR/*.txt)

for i in "${!files[@]}"; do
    printf "%s) %s\n" "$((i+1))" "$(basename "${files[$i]}")"
done

echo "------------------------------------------------"
read -p "Select file number: " file_choice

# Validate file selection
if [[ ! "$file_choice" =~ ^[0-9]+$ ]] || [ "$file_choice" -lt 1 ] || [ "$file_choice" -gt "${#files[@]}" ]; then
    echo "Error: Invalid file selection."
    exit 1
fi

SELECTED_FILE="${files[$((file_choice-1))]}"

# 3. Run it
echo -e "\n[RUNNING $LABEL] using $(basename "$SELECTED_FILE")..."
echo "------------------------------------------------"
time "$EXE" < "$SELECTED_FILE"
echo "------------------------------------------------"