#!/bin/bash

bazel build //...
[[ "$?" != 0 ]] && { >&2 echo "Failed build." ; exit 1 ; }
bazel build //:package_debian
[[ "$?" != 0 ]] && { >&2 echo "Failed generation of debian package." ; exit 2 ; }
echo "Debian package generation completed."
