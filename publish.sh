#!/bin/bash

# Ensure we're using bash
set -e  # Exit on any error

# Transpile C-Next source to C
cnext src/J1939Message.cnx

# Build and test
pio test

# If tests pass, publish
if [ $? -eq 0 ]; then
    echo "Tests passed, publishing..."
    pio pkg publish
else
    echo "Tests failed, not publishing"
    exit 1
fi