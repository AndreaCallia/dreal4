#!/bin/bash

bazel build //...
[[ "$?" != 0 ]] && { >&2 echo "Failed build." ; exit 1 ; }
bazel build //:archive
[[ "$?" != 0 ]] && { >&2 echo "Failed generation of macos archive." ; exit 2 ; }
echo "Macos archive generation completed."
