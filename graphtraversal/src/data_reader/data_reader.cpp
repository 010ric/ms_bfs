//
// Created by 010ric on 05.12.19.
//
#include "graphtraversal/data_reader/data_reader.h"
using namespace graphtraversal;

int main() {
    graphtraversal::Data_Reader dataReader("/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/"
                                           "multisourcegraphtraversal/graphtraversal/data/example-undirected 2/example-undirected.e"," ");

    std::vector<uint64_t > vertices_ids = dataReader.read_vertices_from_file("/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/"
                                                                             "multisourcegraphtraversal/graphtraversal/data/example-undirected 2/example-undirected.v");
    std::vector<std::pair<uint64_t,uint64_t> > test_vect = dataReader.read_pairs_from_file();
    int count = 0;
    for(auto vert : vertices_ids){
        std::cout << "vertex with id : " << vert << std::endl;
    }
    for(auto pair : test_vect){
        count += 1;
        std::cout << "Pair number " << count << " contains edge between  " << pair.first << " and " << pair.second << std::endl;
    }
    return 0;
}