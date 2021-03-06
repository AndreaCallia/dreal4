#!/bin/bash
set -euo pipefail

if [[ "${EUID}" -ne 0 ]]; then
  echo 'This script must be run as root' >&2
  exit 1
fi

apt update
apt install -y software-properties-common
apt-cache search --names-only '^g\+\+-5$' | grep "g++-5" || add-apt-repository ppa:ubuntu-toolchain-r/test -y 
apt-cache search --names-only '^openjdk-8-jdk$' | grep "openjdk-8-jdk" || add-apt-repository ppa:openjdk-r/ppa -y 
add-apt-repository ppa:dreal/dreal -y  # For libibex-dev
apt update
apt install -y --no-install-recommends $(tr '\n' ' ' <<EOF
bash-completion
bison
coinor-libclp-dev
flex
g++
g++-5
libibex-dev
libnlopt-dev
libpython2.7-dev
openjdk-8-jdk
pkg-config
python-dev
zlib1g-dev
EOF
)
      
# Install bazel
BAZEL_VERSION=0.13.0
BAZEL_DEBNAME=bazel_${BAZEL_VERSION}-linux-x86_64.deb
BAZEL_URL=https://github.com/bazelbuild/bazel/releases/download/${BAZEL_VERSION}/${BAZEL_DEBNAME}
BAZEL_SHA256=f7bffaec47387805b0fce7ade1e237f71a84ce8d8d93102b86f34d5dda912680
apt install -y --no-install-recommends wget
wget ${BAZEL_URL}
if echo "${BAZEL_SHA256}  ${BAZEL_DEBNAME}" | sha256sum -c; then
    dpkg -i ./${BAZEL_DEBNAME}
    rm ${BAZEL_DEBNAME}
else
    echo "SHA256 does not match ${BAZEL_DEBNAME}:"
    echo "    expected: ${BAZEL_SHA256}"
    echo "    actual  : `sha256sum ${BAZEL_DEBNAME}`"
    exit 1
fi
