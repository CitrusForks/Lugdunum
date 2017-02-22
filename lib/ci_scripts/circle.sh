#!/bin/bash

case $CIRCLE_NODE_INDEX in
  0)  export CXX=clang++
      mkdir build && cd build
      cmake ../ -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DENABLE_CI=true
      make all test
      ;;
  1)  export CXX=g++
      mkdir build && cd build
      cmake ../ -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DENABLE_CI=true
      make all test
      ;;
esac

