#!/bin/bash

# Ensure we're using bash
set -e  # Exit on any error

# Build and test first
pio test

# If tests pass, publish
if [ $? -eq 0 ]; then
    echo "Tests passed, publishing..."
    pio pkg publish
else
    echo "Tests failed, not publishing"
    exit 1
fi