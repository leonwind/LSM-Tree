file(GLOB BENCHMARKS lsm_tree_benchmarks.cpp rocksdb_benchmarks.cpp)
add_executable(run_benchmarks benchmarks.cpp)
target_link_libraries(run_benchmarks lsm-tree)
target_link_libraries(run_benchmarks rocksdb)