#!/bin/bash

# Function to display help
show_help() {
  echo "Usage: $0 [-h] <student_number>"
  echo "Compress 'src' directory and 'report.pdf' for submission."
  echo "Example: $0 1234-56789"
  echo ""
  echo "Options:"
  echo "  -h    Show this help message"
}

compress() {
  # Check if -h or --help option is provided
  if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    show_help
    exit 0
  fi

  # Abort if any other options are provided (arguments starting with '-')
  if [[ "$1" == -* ]]; then
    echo "$0: Invalid option '$1'"
    exit 1
  fi

  # Check if exactly one argument is provided
  if [ "$#" -ne 1 ]; then
    show_help
    exit 1
  fi

  # Check the current directory
  if [[ $(basename "$PWD") != "$project" ]]; then
    echo "$0: Please run this script from the ${project} directory"
    exit 1
  fi

  # Check if src directory exists
  if [ ! -d "src" ]; then
    echo "$0: Cannot find src directory. Please make sure it exists in the ${project} directory."
    exit 1
  fi

  ################################################################
  # Report file existance checking
  ################################################################

  reportfile=$(find . -name "report.pdf") # Report

  # Check existance of the report file (not in project1)
  if [[ "$project" != "project1" ]]; then
    if ! echo ${reportfile} | grep -q .; then # report.pdf does not exist
      echo "$0: Cannot find report.pdf. Please make sure it is placed in the ${project} or its subdirectories."
      echo "Are you sure want to submit the project without the report? [y/N]"
      read -r response
      if [[ ! "$response" =~ ^[Yy]$ ]]; then
        echo "Submission aborted."
        exit 1
      fi
      reportfile="" # Proceed without the report
    fi
  else
    reportfile="" # No report in project1
  fi


  ################################################################
  # Compilation checking
  ################################################################

  pushd src > /dev/null && \
  make clean

  if ! make; then
    echo "$0: Cannot compile. Please make sure your code compiles successfully."
    echo "Are you sure you want to submit the project without successful compilation? [y/N]"
    read -r response
    if [[ ! "$response" =~ ^[Yy]$ ]]; then
      echo "Submission aborted."
      exit 1
    fi
  fi

  ################################################################
  # Compression
  ################################################################

  zipfile=${project}_${1}.zip # Compressed file name

  make clean && \
  popd > /dev/null && \
  rm -f ${zipfile} && \
  zip -r ${zipfile} src

  if [[ -n "${reportfile}" ]]; then
    zip -j ${zipfile} ${reportfile}
  fi

  if [[ "$?" = "0" ]] ; then
    echo "Prepared for submission. Please submit ${zipfile} on eTL."
  fi
}