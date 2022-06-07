# LSM-Tree 
Implementation of a simplistic Key-Value Store using a [Log-structured Merge-tree (LSM-Tree)](https://en.wikipedia.org/wiki/Log-structured_merge-tree) as its underlying datastructure with a Red-Black Tree for the Memtable and the Sparse Index of the SSTs.

## Usage

```
cd build/
cmake ..
make
./src/kv_store
```
