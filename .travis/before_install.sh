#!/bin/sh

set -e

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo apt-get update -qq
sudo apt-get install libgtest-dev g++-6
git clone --depth=1 --branch=ci-packages https://github.com/sjinks/pcre2pp.git packages
sudo apt-get purge libpcre3-dev
sudo dpkg -i packages/*.deb
