#!/bin/bash

git clone https://github.com/tobyck/cyxal.git
cd cyxal
make install
sudo make build FLAGS="-O3" OUT_DIR=/usr/local/bin --always-make
cd ..
rm -rf cyxal