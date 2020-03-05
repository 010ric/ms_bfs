//
// Created by 010ric on 28.11.19.
//

#ifndef GRAPHTRAVERSAL_WEBGRAPH_H
#define GRAPHTRAVERSAL_WEBGRAPH_H

#include <stdlib.h>
#include <stdio.h>
// #include <vector>
#include <unordered_map>
// #include <boost/graph/compressed_sparse_row_graph.hpp>
#include <graphtraversal/data_reader/data_reader.h>
#include <span>
// #include <gsl/span>

// namespace boost {
namespace graphtraversal {

    class Vertex {
        public:
            std::uint64_t vert_id;
            // std::uint16_t vert_value;

            explicit Vertex(uint64_t id): vert_id(id) {}
    };

    struct Edge {

    };

    struct parsedData {

    };

    class Webgraph {
        // Graph with small-world properties
        /*
            In fact, besides social networks,
            many other real-world graphs that are of critical interest
            for both academia and industry—including gene and neural
            networks, the world-wide web, wikis, movie-actor and
            scientific collaboration networks, as well as electrical power
            grids—exhibit small-world properties
        */


        /*

         I. It matters how many connected Components we inherit from the graph data.
         II. We take the assumption that in the provided data sets we have a strongly connected component
         --> Maybe more precise we have weakly connected components in an undirected graph schema
         look up:
         1. http://ldbcouncil.org/ldbc-graphalytics
         2. https://graphalytics.org/datasets -> Each dataset consists of the input data (vertex and edge files),
         the validation data for each algorithm (BFS, WCC, PR, CDLP, LCC, SSSP), and the metadata (properties file).

         */

    private:

    public:
        // Set of vertices
        std::vector<graphtraversal::Vertex> vertices;
        std::vector<std::uint64_t> matrix;
        std::vector<std::uint64_t> columnIndex;
        std::vector<std::uint64_t> rowPtr;

        Webgraph(std::vector<graphtraversal::Vertex> verts, std::vector<std::uint64_t> mtrx,
                std::vector<std::uint64_t> clmnInd, std::vector<std::uint64_t> rwPtr):
                 vertices(verts), matrix(mtrx), columnIndex(clmnInd), rowPtr(rwPtr) {}

        ~Webgraph() {}

        std::vector<graphtraversal::Vertex>& getVertices();
        std::vector<uint64_t > getMatrix();
        void addVertex(Vertex vert);
        void addEdge(graphtraversal::Vertex vert1, graphtraversal::Vertex vert2);
        std::span<uint64_t >  getNeighbours(const uint64_t);

        struct find_vertex_by_value {
            uint64_t v1;
            find_vertex_by_value(uint64_t v2) : v1(v2) {}
            bool operator () ( const graphtraversal::Vertex& check_vert) const {
                return v1 == check_vert.vert_id;
            }
        };

        struct less_than_first
        {
            inline bool operator() (const std::pair<uint64_t ,uint64_t> & pair1, const std::pair<uint64_t ,uint64_t> & pair2)
            {
                return (pair1.first < pair2.first);
            }
        };
    };

    std::vector<graphtraversal::Vertex>& graphtraversal::Webgraph::getVertices() {
        return this->vertices;
    }

    std::vector<std::uint64_t> graphtraversal::Webgraph::getMatrix() {
        return this->matrix;
    }

    void graphtraversal::Webgraph::addVertex(Vertex vert) {}

    void graphtraversal::Webgraph::addEdge(graphtraversal::Vertex vert1, graphtraversal::Vertex vert2) {}

    std::span<std::uint64_t > graphtraversal::Webgraph::getNeighbours(const uint64_t vertex_id){
        // assumption: row_id starts at 0
        // startIndex tells us how many values in matrix to skip, before we can read the neighbour vertices.
        uint64_t startIndex = 0, found_vertex_id = 0, number_of_neighbours = 0;

        startIndex = this->rowPtr.at(vertex_id);

        if(vertex_id + 1 < rowPtr.size()){
            number_of_neighbours = (this->rowPtr.at(vertex_id + 1) - this->rowPtr.at(vertex_id));
        }
        else if(rowPtr.size() == vertex_id + 1){
            number_of_neighbours = (this->rowPtr.at(vertex_id) - this->rowPtr.at(vertex_id - 1));
        }
        // std::cout << " startIndex : " << startIndex << " numberOfNeighbours : " << number_of_neighbours << std::endl;
        // size_t matrix_size = this->matrix.size();

        // std::span<std::uint64_t, 24 > span_matrix = this->matrix;
        std::span<uint64_t > span_matrix(this->matrix);
        // std::copy(this->matrix.begin() + startIndex, this->matrix.begin() + startIndex + number_of_neighbours, subvect.begin());
        auto subspan_matrix = span_matrix.subspan(startIndex, number_of_neighbours);

        return subspan_matrix;
    }

    struct find_vertex_by_value {
        uint64_t v1;
        find_vertex_by_value(uint64_t v2) : v1(v2) {}
        bool operator () ( const graphtraversal::Vertex& check_vert) const {
            return v1 == check_vert.vert_id;
        }
    };

    std::vector<graphtraversal::Vertex> remove_nonexisting_vertices(std::vector<graphtraversal::Vertex>& vertices, std::vector<uint64_t >& rowPtr){
        std::vector<graphtraversal::Vertex> notexisting;
        for(auto &vert : vertices){
            if((rowPtr.at(vert.vert_id + 1) - rowPtr.at(vert.vert_id)) == 0){
                notexisting.push_back(vertices[vert.vert_id - 1]);
            }
        }

        return notexisting;
    }

    Webgraph buildWebgraphFromFile(const std::string vertices_file, std::string edges_file) {
        // 1. access data
        graphtraversal::Data_Reader data_reader(edges_file,"|");
        // std::vector<uint64_t > vertices_ids = data_reader.read_vertices_from_file(vertices_file);
        std::vector<std::pair<uint64_t,uint64_t> > vertices_pairs = data_reader.read_pairs_from_file();
        std::set<uint64_t > vertices_ids = data_reader.read_vertices_from_file(vertices_file);
        std::vector<graphtraversal::Vertex> vertices;
        /*
        for(int vert_id = 1; vert_id <= 64; vert_id++){
            vertices.push_back(graphtraversal::Vertex(vert_id));
        }
         */
        for(auto &vert : vertices_ids){
            vertices.push_back(graphtraversal::Vertex(vert + 1));
        }

        // 2. construct the three one dimensional data structures for the CSR format
        std::vector<std::pair<uint64_t ,uint64_t> > adjacency1;
        std::vector<std::uint64_t> matrix;
        std::vector<std::uint64_t> columnIndex;
        std::vector<std::uint64_t> rowPtr;

        std::sort(vertices_pairs.begin(), vertices_pairs.end());

        for(auto& vert : vertices_pairs){
            adjacency1.push_back(std::make_pair(vert.first + 1, vert.second + 1));
            adjacency1.push_back(std::make_pair(vert.second + 1,vert.first + 1));
        }
        // std::merge(adjacency1.begin(), adjacency1.end(), adjacency2.begin(), adjacency2.end(), adjacency3.begin());
        sort(adjacency1.begin(), adjacency1.end());
        int nnz = 0, old_nnz = 0, old_row_index = adjacency1.at(0).first;

        for(auto& pair : adjacency1){
            columnIndex.push_back(pair.second);
            matrix.push_back(pair.second);
            if(pair.first == old_row_index){
                nnz++;
                old_row_index = pair.first;
            }
            if(pair.first != old_row_index){
                while(rowPtr.size() <= old_row_index){
                    rowPtr.push_back(old_nnz);
                }
                rowPtr.push_back(nnz);
                old_nnz = nnz;
                old_row_index = pair.first;
                nnz++;
            }
        }
        rowPtr.push_back(nnz);
        // std::vector<graphtraversal::Vertex> skip_vertices = remove_nonexisting_vertices(vertices, rowPtr);
        graphtraversal::Webgraph webgraph(vertices, matrix, columnIndex, rowPtr);
        return webgraph;
    }
}

#endif GRAPHTRAVERSAL_WEBGRAPH_H