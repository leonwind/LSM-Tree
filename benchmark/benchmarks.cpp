#include "lsm_tree_benchmarks.cpp"
#include "rocksdb_benchmarks.cpp"

int main() {
    size_t num_elements = 100000000; // 100E6
    lsm_tree_benchmarks::run(num_elements);
    rocksdb_benchmarks::run(num_elements);

	return 0;
}
