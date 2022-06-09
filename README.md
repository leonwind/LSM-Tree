# LSM-Tree 
Implementation of a simplistic Key-Value Store using a [Log-structured Merge-tree (LSM-Tree)](https://en.wikipedia.org/wiki/Log-structured_merge-tree) as its underlying datastructure with a Red-Black Tree for the Memtable and the Sparse Index of the SSTs.

## Usage

```
cd build/
cmake ..
make
./kv_store
```

## Benchmarks
The LSM-Tree got benchmarked against [RocksDB](https://github.com/facebook/rocksdb) by Facebook. You can run the benchmarks your self after compiling under `./build/benchmark/run_benchmarks`.
The results are also visualized in [benchmark.ipynb](https://github.com/leonwind/LSM-Tree/blob/main/benchmark/benchmarks.ipynb).
