#!/bin/bash

rm dreal/solver/sat_solver.cc
[[ "$?" != 0 ]] && { >&2 echo "Failed removing link to sat_solver.cc" ; exit 8 ; }
ln -s `pwd`/smts/sat_solver.cc dreal/solver/sat_solver.cc
[[ "$?" != 0 ]] && { >&2 echo "Failed linking patched sat_solver.cc" ; exit 9 ; }
bazel build //...
[[ "$?" != 0 ]] && { >&2 echo "Failed build." ; exit 1 ; }
bazel build //:archive
[[ "$?" != 0 ]] && { >&2 echo "Failed generation of macos archive." ; exit 2 ; }
echo "Macos archive generation completed."
