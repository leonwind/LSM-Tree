#!/bin/sh
make -C ../build/ | cli2cloud
./../build/benchmark/run_benchmarks | cli2cloud
