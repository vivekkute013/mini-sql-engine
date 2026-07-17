#include <iostream>
#include "minisql/index/bplus_tree.hpp"

int main() {
    minisql::BPlusTree tree;

    for (int64_t i = 1; i <= 20; i++) {
        tree.Insert(i * 10, i * 100);
    }

    int64_t result;
    bool all_correct = true;

    for (int64_t i = 1; i <= 20; i++) {
        int64_t key = i * 10;
        int64_t expected = i * 100;
        bool found = tree.Search(key, &result);

        if (!found || result != expected) {
            std::cout << "MISMATCH at key " << key << "\n";
            all_correct = false;
        }
    }

    bool found_missing = tree.Search(999, &result);

    std::cout << (all_correct ? "SUCCESS: all 20 keys correct.\n"
                               : "FAILURE: see mismatches above.\n");
    std::cout << "Search for non-existent key 999: "
              << (found_missing ? "FOUND (unexpected!)" : "NOT FOUND (correct)") << "\n";

    return 0;
}