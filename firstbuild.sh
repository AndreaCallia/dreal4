#!/bin/bash

bazel build //...
[[ "$?" != 0 ]] && { echo "Failed initial build" ; exit 1 ; }
# && bazel build //:package_debian
PICOSATDIR=$(dirname $(readlink bazel-dreal4/external/picosat/picosat.c))
[[ "$?" != 0 ]] && { >&2 echo "Failed getting picosat path" ; exit 2 ; }
[[ $PICOSATDIR == "" ]] && { >&2 echo "Failed getting picosat path" ; exit 3 ; }
rm $PICOSATDIR/picosat.c
[[ "$?" != 0 ]] && { >&2 echo "Failed removing original picosat.c" ; exit 4 ; }
ln -s `pwd`/smts/picosat.c $PICOSATDIR/picosat.c
[[ "$?" != 0 ]] && { >&2 echo "Failed linking patched picosat.c" ; exit 5 ; }
rm $PICOSATDIR/picosat.h
[[ "$?" != 0 ]] && { >&2 echo "Failed removing original picosat.h" ; exit 6 ; }
ln -s `pwd`/smts/picosat.h $PICOSATDIR/picosat.h
[[ "$?" != 0 ]] && { >&2 echo "Failed linking patched picosat.h" ; exit 7 ; }
rm dreal/solver/sat_solver.cc
[[ "$?" != 0 ]] && { >&2 echo "Failed removing unpatched sat_solver.cc" ; exit 8 ; }
ln -s `pwd`/smts/sat_solver.cc dreal/solver/sat_solver.cc
[[ "$?" != 0 ]] && { >&2 echo "Failed linking patched sat_solver.cc" ; exit 9 ; }
bazel build //...
[[ "$?" != 0 ]] && { >&2 echo "Failed build after patching" ; exit 10 ; }
./bazel2cmakelists --target //...
[[ "$?" != 0 ]] && { >&2 echo "Failed creatng file CMakeLists.txt" ; exit 11 ; }
echo "First build completed."
