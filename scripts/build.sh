#!/usr/bin/env bash
set -e

DIR=$(dirname $(realpath "$BASH_SOURCE"))
# The directory contains .git directory.
REPO_DIR=${REPO_DIR:-$(git rev-parse --show-toplevel)}

echo "Repo name: "$REPO_DIR

echo "======================================="
echo "Usage: $0 <ProjectName>"
echo "example: $0 HelloWorld"
echo "======================================="

if [ -z "$1" ]; then
  echo "Must set project name, such as HelloWorld"
  exit 1
fi

PROJECT_NAME=$1
PROJECT_PATH=$REPO_DIR/$PROJECT_NAME
BUILD=$PROJECT_PATH/build

if [ ! -d "$PROJECT_PATH" ]; then
  echo "Project name[$PROJECT_NAME] set error."
  exit 1
fi

pushd $PROJECT_PATH
if [ -d "$BUILD" ]; then
  echo "build dir exist, rm $BUILD"
  rm -rf $BUILD
fi
mkdir -p $BUILD

cd $BUILD && cmake .. && make

echo "Build target: " $BUILD/$PROJECT_NAME

# run
$BUILD/$PROJECT_NAME

popd


