#!/bin/bash

rm -f dreal/solver/sat_solver.cc
ln -s `pwd`/smts/sat_solver.cc dreal/solver/sat_solver.cc
[[ "$?" != 0 ]] && { >&2 echo "Failed linking patched sat_solver.cc" ; exit 9 ; }
bazel build //...
[[ "$?" != 0 ]] && { echo "Failed build" ; exit 1 ; }
echo "Build completed."
