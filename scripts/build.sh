#!/usr/bin/env bash
set -e

DIR=$(dirname $(realpath "$BASH_SOURCE"))
# The directory contains .git directory.
REPO_DIR=${REPO_DIR:-$(git rev-parse --show-toplevel)}

echo "Repo name: "$REPO_DIR

echo "======================================="
echo "Usage: $0 <Target>"
echo "example: $0 HelloWorld"
echo "======================================="

BUILD_DIR=$REPO_DIR/build
BUILD_TARGET=$1

pushd $BUILD_DIR
if [ ! -d "$BUILD_DIR" ]; then
  mkdir -p $BUILD_DIR
  echo "make build dir: $BUILD_DIR"
fi

cd $BUILD_DIR && cmake .. && make


# run
if [ ! -z "$BUILD_TARGET" ]; then
  echo "Run build target=== $BUILD_DIR/$BUILD_TARGET" 
  $BUILD_DIR/$BUILD_TARGET
fi

popd


