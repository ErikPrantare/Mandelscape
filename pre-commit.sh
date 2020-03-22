#!/usr/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

if [ "$(clang-format-9 --version 2> /dev/null | grep -o '9.0')" == "9.0" ];
then
  clang-format-9 -i ${DIR}/src/*
elif [ "$(clang-format --version | grep -o '9.0')" == "9.0" ];
then
  clang-format -i ${DIR}/src/*
fi
