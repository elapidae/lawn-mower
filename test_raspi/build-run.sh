#!/bin/bash -e

bpath="_build-form-script_"
mkdir -p $bpath

cmake -S . -B $bpath

cmake --build $bpath --parallel 2 --config Release

sudo ./$bpath/test_raspi
