//
// Created by 010ric on 28.11.19.
//

#ifndef GRAPHTRAVERSAL_BFS_H
#define GRAPHTRAVERSAL_BFS_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <graphtraversal/webgraph/webgraph.h>

namespace graphtraversal {

    class BFS {
        private:
            graphtraversal::Vertex startVertex;
            // graph containing all vertices and edges
            graphtraversal::Webgraph& webgraph;
            // closeness centrality for given startVertex
            std::double_t closeness_centrality;
            uint64_t traversed_edges;
            // seen bitfield
            std::vector<bool> seenVertices;
            // visit set
            std::vector<bool> visitedVertices;
            // visitNext set
            std::vector<bool> visitNextVertices;

        public:
            struct find_vertex {
                graphtraversal::Vertex v1;
                find_vertex(graphtraversal::Vertex v2) : v1(v2) {}
                bool operator () ( const graphtraversal::Vertex& check_vert) const {
                    return v1.vert_id == check_vert.vert_id;
                }
            };

            bool operator==(const BFS& bfs){
                if ((this->startVertex.vert_id==bfs.startVertex.vert_id))
                    return true;
                return false;
            }

            explicit BFS(graphtraversal::Webgraph& webgraph, graphtraversal::Vertex& start) : webgraph(webgraph), startVertex(start) {
                size_t number_of_verts = webgraph.vertices.size();
                this->visitedVertices = std::vector<bool>(number_of_verts, false);
                this->seenVertices = std::vector<bool>(number_of_verts, false);
                this->visitNextVertices = std::vector<bool>(number_of_verts, false);
            }

            bool check_bool_array(const std::vector<bool>& bool_array){
                for(auto bool_field : bool_array){
                    if(bool_field == 1){
                        return true;
                    }
                }
                return false;
            }

            ~BFS(){}

            std::double_t run_T_BFS(){
                // Assumption vertices id start from 1
                size_t number_of_vertices = webgraph.getVertices().size();
                uint64_t upper_id_limit = webgraph.getVertices().back().vert_id;
                std::vector<uint64_t > vert_vect;
                for(int i = 1; i <= upper_id_limit; i++){
                    vert_vect.push_back(i);
                }
                this->seenVertices[this->getStartVertex().vert_id - 1] = 1;
                this->visitedVertices[this->getStartVertex().vert_id - 1] = 1;
                uint64_t bfs_level = 1, number_of_edges_traversed = 0;
                double sum_dist_s_to_n = 0;

                while(check_bool_array(visitedVertices)){
                    for(int i = 1; i <= number_of_vertices; i++){
                        if(visitedVertices[i - 1] == 1){
                            for(auto neighbour : this->webgraph.getNeighbours(vert_vect[i - 1])){
                                if(this->seenVertices[neighbour - 1] == 0){
                                    this->seenVertices[neighbour - 1] = 1;
                                    this->visitNextVertices[neighbour - 1] = 1;
                                    // distance from start to vertex n equals the bfs_level
                                    sum_dist_s_to_n += bfs_level;
                                }
                                number_of_edges_traversed++;
                            }
                        }
                    }
                    this->visitedVertices = this->visitNextVertices;
                    this->visitNextVertices = std::vector<bool>(number_of_vertices, false);
                    bfs_level++;
                }
                this->traversed_edges = number_of_edges_traversed;
                // std::cout << "number of edges traversed : " << number_of_edges_traversed << std::endl;
                if(sum_dist_s_to_n < 1){
                    // std::cout << " error : node sum = 0" << std::endl;
                    return this->closeness_centrality = 0;
                }
                return this->closeness_centrality = (number_of_vertices - 1) / sum_dist_s_to_n;
            }

            std::double_t get_closeness_centrality(){
                return closeness_centrality;
            };

            graphtraversal::Vertex getStartVertex(){
                return startVertex;
            };

            uint64_t getTraversedEdges(){
                return this->traversed_edges;
            };
    };
}

#endif GRAPHTRAVERSAL_BFS_H