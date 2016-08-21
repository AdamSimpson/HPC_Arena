#!/bin/bash
ROOT=`pwd`
mkdir -p $ROOT/vendor; cd $ROOT/vendor
git clone https://github.com/SFML/SFML.git; cd $ROOT/vendor/SFML
mkdir -p $ROOT/vendor/SFML/build; cd $ROOT/vendor/SFML/build

mkdir -p $ROOT/vendor/install
cmake -DCMAKE_INSTALL_PREFIX:PATH=$ROOT/vendor/install $ROOT/vendor/SFML
make
ls -RAhlF $ROOT/vendor/install

cd $ROOT

mkdir -p $ROOT/build; cd $ROOT/build
SFML_INCLUDE_DIR=$ROOT/vendor/install/SFML CXX=g++-6 cmake $ROOT
make VERBOSE=1
