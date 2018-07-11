#!/bin/bash

bazel build //... && bazel build //:package_debian
rm -rf bazel-dreal4/external/picosat
ln -s `pwd`/picosat bazel-dreal4/external/picosat
