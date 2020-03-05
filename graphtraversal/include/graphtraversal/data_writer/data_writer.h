//
// Created by 010ric on 18.12.19.
//

#ifndef GRAPHTRAVERSAL_DATA_WRITER_H
#define GRAPHTRAVERSAL_DATA_WRITER_H

/*
* A class to create and write data in a csv file.
*/

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>

namespace graphtraversal {

    class CSVWriter {
        std::string fileName;
        std::string delimeter;
        int linesCount;

    public:
        CSVWriter(std::string filename, std::string delm) :
                fileName(filename), delimeter(delm), linesCount(0) {}

        /*
        * Member function to store a range as comma seperated value
        */
        void addHeaderForRow(std::string h1, std::string h2);
        template<typename T>
        void addDataInRow(T first, T second, T last, T last2);
    };

    void CSVWriter::addHeaderForRow(std::string h1, std::string h2){
        std::fstream file;
        // Open the file in truncate mode if first line else in Append Mode
        file.open(fileName, std::ios::out | std::ios::trunc);
        file << h1 << " ";
        file << h2 << "\n";
        // Close the file
        linesCount++;
        file.close();
    }

    template<typename T>
    void CSVWriter::addDataInRow(T first, T second, T last, T last2) {
        std::fstream file;
        // Open the file in truncate mode if first line else in Append Mode
        file.open(fileName, std::ios::out | (linesCount ? std::ios::app : std::ios::trunc));

        // Iterate over the range and add each element to file seperated by delimeter.
        for (;(first != last && second != last2);first++, second++) {
            file << *first;
            file << delimeter;
            file << *second;
            file << "\n";
            linesCount++;
        }
        // Close the file
        file.close();
    }
}


#endif GRAPHTRAVERSAL_DATA_WRITER_H
