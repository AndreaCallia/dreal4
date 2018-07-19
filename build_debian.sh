#!/bin/bash

bazel build //... && bazel build //:package_debian
