#!/bin/bash

rm dreal/solver/sat_solver.cc
[[ "$?" != 0 ]] && { >&2 echo "Failed removing link to sat_solver.cc" ; exit 8 ; }
ln -s `pwd`/smts/sat_solver.cc dreal/solver/sat_solver.cc
[[ "$?" != 0 ]] && { >&2 echo "Failed linking patched sat_solver.cc" ; exit 9 ; }
bazel build //...
[[ "$?" != 0 ]] && { >&2 echo "Failed build." ; exit 1 ; }
bazel build //:package_debian
[[ "$?" != 0 ]] && { >&2 echo "Failed generation of debian package." ; exit 2 ; }
echo "Debian package generation completed."
