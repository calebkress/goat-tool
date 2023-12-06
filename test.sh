#!/bin/bash

# Define the Python test script and the output file
PYTHON_TEST_SCRIPT="test.py"  # Replace with your actual test script's name
OUTPUT_FILE="test_results.txt"

# Check if the Python test script exists
if [[ ! -f "$PYTHON_TEST_SCRIPT" ]]; then
    echo "Error: Python test script not found."
    exit 1
fi

# Run the Python test script and capture its output
echo "Running Python tests..."
python3 "$PYTHON_TEST_SCRIPT" > "$OUTPUT_FILE" 2>&1

echo "Test results written to $OUTPUT_FILE"
