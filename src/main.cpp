#include <iostream>
#include "minisql/index/bplus_tree.hpp"

int main() {
    minisql::BPlusTree tree;

    tree.Insert(10, 100);
    tree.Insert(20, 200);
    tree.Insert(30, 300);
    tree.Insert(40, 400);
    tree.Insert(50, 500);

    int64_t result;

    for(int64_t key : {10, 20, 30, 40, 50, 99}){
        bool found = tree.Search(key, &result);
        std::cout<< "Search "<<key<<" : ";
        if(found){
            std::cout<<"FOUND, value = "<<result<<"\n";
        }else{
            std::cout<<"NOT FOUND \n";
        }
    }

    // bool found10 = tree.Search(10, &result);
    // std::cout << "Search 10: " << (found10 ? "FOUND, value=" : "NOT FOUND") ;
    // if (found10) std::cout << result;
    // std::cout << "\n";

    // bool found20 = tree.Search(20, &result);
    // std::cout << "Search 20: " << (found20 ? "FOUND, value=" : "NOT FOUND");
    // if (found20) std::cout << result;
    // std::cout << "\n";

    // bool found99 = tree.Search(99, &result);
    // std::cout << "Search 99: " << (found99 ? "FOUND (unexpected!)" : "NOT FOUND (correct)") << "\n";



    return 0;
}