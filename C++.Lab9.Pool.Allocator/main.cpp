#include <chrono>
#include <iostream>
#include <vector>
 #include "PoolRealization.h"
#include <list>
#include<fstream>


template <typename Container>
void test(Container& container, std::list<int>& container2, int s) {
    std::ofstream file("C:\\Users\\maxfi\\Desktop\\C\\C++.Lab9.Pool.Allocator\\qwe.txt", std::ios::app);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < s; ++i) {
        container.push_back(i);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        if (i%100==0){
            file <<duration <<", ";
        }
    }

    //std::cout<<duration<<'\n';

    file.close();

    std::ofstream file2("C:\\Users\\maxfi\\Desktop\\C\\C++.Lab9.Pool.Allocator\\qwe2.txt", std::ios::app);
    auto start2 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < s; ++i) {
        container2.push_back(i);

        auto end2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();
        if (i%100==0){
            file2 <<duration2 <<", ";
        }
        //
    }



    file2.close();
    /*auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << allocator << ": " << duration << " ms for alloc" << std::endl;*/


    /*auto start_2 = std::chrono::high_resolution_clock::now();


    auto end_2 = std::chrono::high_resolution_clock::now();
    auto duration_2 = std::chrono::duration_cast<std::chrono::microseconds>(end_2 - start_2).count();

    std::cout << allocator << ": " << duration_2 << " ms for dealoc" << std::endl;*/
}

void test(std::vector<int>& container, std::string allocator) {

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 500000; ++i) {
        container.push_back(i);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << allocator << ": " << duration << " ms for alloc" << std::endl;


    //auto start_2 = std::chrono::high_resolution_clock::now();
/*
    for (int i = 0; i < 10000; ++i) {
        container.pop_back();
    }

    auto end_2 = std::chrono::high_resolution_clock::now();
    auto duration_2 = std::chrono::duration_cast<std::chrono::microseconds>(end_2 - start_2).count();

    std::cout << allocator << ": " << duration_2 << " ms for dealoc" << std::endl;*/
}





    int main() {


    std::list<int> v2;
    std::list<int, PoolAllocator<int, 1000000>> v;
    test(v, v2, 1000000);

    /*PoolAllocator<int, 500000> allocator;
    std::vector<int, PoolAllocator<int, 500000>> v(allocator);
    std::vector<int> v2;

    test(reinterpret_cast<std::vector<int> &>(v), "MyAllocator");
    test(v2, "std::allocator");
*/



    return 0;
}
