#! /bin/bash -e

# admesh is used to validate output STL files during unit testing. If you
# ensure it is installed and on the path, this script won't be run by
# `make check`.

if [ -n "`which brew`" ]; then INSTALL="brew install"; fi
if [ -n "`which apt-get`" ]; then INSTALL="sudo apt-get -y install"; fi
if [ -n "`which yum`" ]; then INSTALL="sudo yum -y install"; fi
if [ -n "`which dnf`" ]; then INSTALL="sudo dnf -y install"; fi
if [ -n "`which zypper`" ]; then INSTALL="sudo zypper -y install"; fi

$INSTALL admesh

