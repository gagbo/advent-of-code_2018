#!/usr/bin/sh

# This shell script uses ShellCheck to try to stay POSIX-compliant.
# It is developed on a Linux machine though, so compatibility with MacOS is
# no guaranteed

set -eo pipefail

day="UNDEFINED"

while getopts ":d:" opt; do
  case $opt in
    d)
      day=$OPTARG
      shift
      shift
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

if [ "$day" = "UNDEFINED" ]; then
    echo "Need to define a day using -d ; check README.md"
    exit 1
fi

padded_day=$(printf "%02d" "$day")
if [ -d "$padded_day" ] && [ -e "./$padded_day/day$padded_day" ]; then
    echo "Starting day $padded_day with arguments :" "$@"
    exec "./$padded_day/day$padded_day" "$@"
else
    echo "Error : the requested day $padded_day does not exist, or $padded_day/day$padded_day is not an executable"
    exit 1
fi
