#!/bin/bash -e

bpath="_build-form-script_"
mkdir -p $bpath

cmake -S . -B $bpath in $bpath

cmake --build $bpath --parallel 2 --config Release

./$bpath/first-qt-app
