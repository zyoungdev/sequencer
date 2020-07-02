#!/bin/bash

src_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

build_dir="sequencer_build"

cd /tmp
mkdir -p $build_dir
cd $build_dir

cmake $src_dir

