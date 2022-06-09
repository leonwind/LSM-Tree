#include "../src/lsm_tree/lsm_tree.hpp"
#include <iostream>
#include <random>
#include <chrono>

namespace lsm_tree_benchmarks {

    namespace {
        lsm_tree *sequential_writes(size_t num_elements) {
            auto *db = new lsm_tree();
            db->drop_table();

            std::cout << "START SEQUENTIAL WRITES BENCHMARK" << std::endl;

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            for (size_t i = 0; i < num_elements; ++i) {
                std::string s = std::to_string(i);
                db->put(s, s);
            }

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << "Time difference = "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                      << "[ms]" << std::endl;

            return db;
        }

        void sequential_reads(lsm_tree *db, size_t num_elements) {
            std::cout << "START SEQUENTIAL READS BENCHMARK" << std::endl;

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            for (size_t i = 0; i < num_elements; ++i) {
                db->get(std::to_string(i));
            }

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << "Time difference = "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                      << "[ms]" << std::endl;

            delete(db);
        }

        lsm_tree *random_writes(size_t num_elements) {
            auto *db = new lsm_tree();
            db->drop_table();

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, (int) num_elements);

            std::cout << "START RANDOM WRITES BENCHMARK" << std::endl;

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            for (size_t i = 0; i < num_elements; ++i) {
                std::string s = std::to_string(distr(gen));
                db->put(s, s);
            }

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << "Time difference = "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                      << "[ms]" << std::endl;

            return db;
        }


        void random_reads(lsm_tree *db, size_t num_elements) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, (int) num_elements);

            std::cout << "START RANDOM READS BENCHMARK" << std::endl;

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            for (size_t i = 0; i < num_elements; ++i) {
                db->get(std::to_string(distr(gen)));
            }

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << "Time difference = "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                      << "[ms]" << std::endl;

            delete(db);
        }
    }

    void run(size_t num_elements) {
        std::cout << "Start LSM Tree Benchmarks..." << std::endl;

        lsm_tree *db = sequential_writes(num_elements);
        sequential_reads(db, num_elements);

        db = random_writes(num_elements);
        random_reads(db, num_elements);
    }
}