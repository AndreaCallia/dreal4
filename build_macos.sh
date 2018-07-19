#!/bin/bash

bazel build //... && bazel build //:archive
