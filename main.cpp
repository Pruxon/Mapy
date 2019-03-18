#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>

#include "TreeMap.h"
#include "HashMap.h"

using namespace aisdi;

template<typename Func>
long long measureTime(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto finish = std::chrono::high_resolution_clock::now();
    auto dur = finish - start;
    std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}


void fillTree(TreeMap<int, std::string> &Tree, int Number)
{
    int half = Number*3;
    Tree[half] = "ROOT";
    while (Tree.getSize() != (unsigned int)Number)
    {
        int nr = rand()%Number*6+1;
        Tree[nr] = "NEXT";
    }
}


void fillMap(HashMap<int, std::string> &HashMap, int Number)
{
    for (int i = 0; i < Number; i++)
    {
        HashMap[i] = "NONAME";
    }
}

void testInsert(int number, int new_elements)
{
    HashMap<int, std::string> HashMap;
    TreeMap<int, std::string> Tree;

    fillMap(HashMap, number);
    fillTree(Tree, number);


    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < new_elements; i++) {
        int where = i*new_elements%number;
        Tree[where] = "ELEMENT";
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "> Tree elapsed time: " << elapsed.count() << " s\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < new_elements; i++) {
        HashMap[i+number] = "ELEMENT";
    }
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << ">   HashMap elapsed time: " << elapsed.count() << " s\n\n";

}



int main()
{
//    testInsert(100, 1);
//    testInsert(1000, 10);
//    testInsert(10000, 100);
//    testInsert(100000, 1000);

    TreeMap<int, std::string> map;
    map[4] = "D";
    map[6] = "F";
    map[2] = "B";
    map[1] = "A";
    map[3] = "C";
    map[8] = "H";
    map[5] = "E";
    map[7] = "G";
    map[9] = "I";
    map[10] = "J";
    return 0;
}




/*
namespace
{

template <typename K, typename V>
using Map = aisdi::Map<K, V>;

void perfomTest()
{
  Map<int, std::string> map;
  map[1] = "TODO";
}

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
  return 0;
}
*/


