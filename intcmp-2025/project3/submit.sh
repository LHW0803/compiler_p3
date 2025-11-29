#!/bin/bash

# Project number
project="project3"

source ../submit_helper.sh

# Check if exactly one argument is provided
if [ "$#" -ne 1 ]; then
  show_help
  exit 1
fi

compress "$1"