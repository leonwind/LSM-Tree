#include <iostream>
#include <chrono>
#include <rocksdb/db.h>
#include <filesystem>

namespace rocksdb_benchmarks {

    namespace {
        rocksdb::DB *create_new_db() {
            std::string db_files{"/tmp/testdb"};
            std::filesystem::remove_all(db_files);

            rocksdb::DB *db;
            rocksdb::Options options;
            options.create_if_missing = true;
            options.env->SetBackgroundThreads(0, rocksdb::Env::Priority::HIGH);
            options.env->SetBackgroundThreads(0, rocksdb::Env::Priority::LOW);
            rocksdb::Status status =
                    rocksdb::DB::Open(options, db_files, &db);
            assert(status.ok());

            return db;
        }

        rocksdb::DB *sequential_writes(size_t num_elements) {
            auto* db = create_new_db();

            std::cout << "ROCKSDB SEQUENTIAL WRITES: ";

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            for (size_t i = 0; i < num_elements; ++i) {
                std::string s = std::to_string(i);
                db->Put(rocksdb::WriteOptions(), s, s);
            }

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                      << "[ms]" << std::endl;

            return db;
        }

        void sequential_reads(rocksdb::DB* db, size_t num_elements) {
            std::cout << "ROCKSDB SEQUENTIAL READS: ";
            std::string value;

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            for (size_t i = 0; i < num_elements; ++i) {
                db->Get(rocksdb::ReadOptions(), std::to_string(i), &value);
            }

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                      << "[ms]" << std::endl;
        }

        rocksdb::DB *random_writes(size_t num_elements) {
            auto* db = create_new_db();

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, (int) num_elements);

            std::cout << "ROCKSDB RANDOM WRITES: ";

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            for (size_t i = 0; i < num_elements; ++i) {
                std::string s = std::to_string(distr(gen));
                db->Put(rocksdb::WriteOptions(), s, s);
            }

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                      << "[ms]" << std::endl;

            return db;
        }

        void random_reads(rocksdb::DB* db, size_t num_elements) {
            std::ofstream log;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, (int) num_elements);

            std::string value;
            std::cout << "ROCKSDB RANDOM READS: ";

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            for (size_t i = 0; i < num_elements; ++i) {
                db->Get(rocksdb::ReadOptions(), std::to_string(distr(gen)), &value);
            }

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                      << "[ms]" << std::endl;
        }
    } // namespace

    void run(size_t num_elements) {
        std::cout << "Start RocksDB Benchmark..." << std::endl;

        rocksdb::DB *db = sequential_writes(num_elements);
        sequential_reads(db, num_elements);
        random_reads(db, num_elements);
        delete(db);

        db = random_writes(num_elements);
        delete(db);
    }

} // namespace rocksdb_benchmarks