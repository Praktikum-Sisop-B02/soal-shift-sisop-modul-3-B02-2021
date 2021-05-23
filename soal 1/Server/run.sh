#!/bin/bash

nf=$1
nc=$1".c"

gcc -pthread -o $nf $nc
./$nf
