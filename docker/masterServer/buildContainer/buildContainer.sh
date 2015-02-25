#!/bin/bash

rm -fr PFA
mkdir -p PFA
rsync -r ../../../ ./PFA/

docker build -t "fusion_build" .