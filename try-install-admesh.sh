#! /usr/bin/env bash -e

# admesh is used to validate output STL files during unit testing. If you
# ensure it is installed and on the path, this script won't be run by
# `make check`.

if [ -n "`which brew`" ]; then INSTALL="brew install"; fi
if [ -n "`which apt-get`" ]; then INSTALL="sudo apt-get install"; fi
if [ -n "`which yum`" ]; then INSTALL="sudo yum install"; fi
if [ -n "`which dnf`" ]; then INSTALL="sudo dnf install"; fi
if [ -n "`which zypper`" ]; then INSTALL="sudo zypper install"; fi

$INSTALL admesh

