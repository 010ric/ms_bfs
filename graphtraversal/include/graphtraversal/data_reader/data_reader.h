//
// Created by 010ric on 05.12.19.
//

#ifndef GRAPHTRAVERSAL_DATA_READER_H
#define GRAPHTRAVERSAL_DATA_READER_H

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
#include <set>

namespace graphtraversal {

    class Data_Reader {
        private:
            //number of vertices
            uint64_t num_vertices;
            std::string absolute_file_path;
            std::string delimiter;
        public:
            explicit Data_Reader(std::string file_path, std::string delim): absolute_file_path(file_path),
                delimiter(delim) {};

            ~Data_Reader() {

            };

            uint64_t getNumVert();

            void setNumVert(uint64_t number);

            std::set<uint64_t > read_vertices_from_file(std::string relative_file_path);
            std::vector<std::pair<uint64_t,uint64_t> > read_pairs_from_file();

    };

    uint64_t graphtraversal::Data_Reader::getNumVert(){
        return this->num_vertices;
    }

    void graphtraversal::Data_Reader::setNumVert(uint64_t number){
        this->num_vertices = number;
    }

    std::set<uint64_t> graphtraversal::Data_Reader::read_vertices_from_file(std::string relative_file_path){
        std::ifstream ifstream(relative_file_path);
        if(!ifstream){
            std::cerr << "Couldn't open the file!" << std::endl;
        }
        std::set<uint64_t > data_buffer;
        std::string line = "";
        // spliting the content using delimeter
        while (getline(ifstream, line)) {
            uint64_t vert_id1, vid2;
            std::sscanf(line.c_str(), "%ld %ld", &vert_id1, &vid2);

            data_buffer.insert(vert_id1);
            data_buffer.insert(vid2);
        }
        // Close the File
        ifstream.close();

        return data_buffer;
    }

    std::vector<std::pair<uint64_t,uint64_t> > graphtraversal::Data_Reader::read_pairs_from_file(){
        std::ifstream ifstream(absolute_file_path);
        if(!ifstream){
            std::cerr << "Couldn't open the file!" << std::endl;
        }

        std::vector<std::pair<uint64_t,uint64_t> > data_buffer;

        std::string line = "";
        // spliting the content using delimeter
        while (getline(ifstream, line)) {
            uint64_t id1, id2;
            std::sscanf( line.c_str(), "%ld %ld", &id1, &id2);

            std::pair<uint64_t,uint64_t> vect_pair(id1, id2);
            data_buffer.push_back(vect_pair);
        }
        // Close the File
        ifstream.close();

        return data_buffer;
    }
}

#endif //GRAPHTRAVERSAL_DATA_READER_H