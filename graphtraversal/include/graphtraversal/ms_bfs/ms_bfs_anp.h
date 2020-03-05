//
// Created by 010ric on 28.11.19.
//

#ifndef GRAPHTRAVERSAL_MS_BFS_ANP_H
#define GRAPHTRAVERSAL_MS_BFS_ANP_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <bitset>
#include <graphtraversal/webgraph/webgraph.h>
#include <graphtraversal/bfs/bfs.h>
#include <boost/dynamic_bitset.hpp>

namespace graphtraversal {

    class MS_BFS_ANP {
        typedef uint64_t bitset;

    private:
        // graph containing all vertices and edges
        graphtraversal::Webgraph webgraph;
        std::double_t endresult;
        uint64_t traversed_edges;

    public:
        /* bool isDuplicate(const std::vector<graphtraversal::BFS>& seen_bfs){
            return (std::find(seen_bfs.begin(), seen_bfs.end(), bfs) != seen_bfs.end());
        } */
        struct find_vertex_by_value {
            uint64_t v1;
            find_vertex_by_value(uint64_t v2) : v1(v2) {}
            bool operator () ( const graphtraversal::Vertex& check_vert) const {
                return v1 == check_vert.vert_id;
            }
        };


        bool check_bitset(std::vector<uint64_t > set_of_bitfields){
            for(auto& bitfield : set_of_bitfields){
                if(bitfield != 0){
                    return true;
                }
            }
            return false;
        }

        bool check_full_bitset_vect(std::vector<uint64_t > set_of_bitfields){
            bitset margin = 0;
            bool all_filed = 1;
            for(int i = 0; i < set_of_bitfields.size(); i++){
                margin |= 1 << i;
            }
            for(auto& bitfield : set_of_bitfields){
                if(bitfield != margin){
                    all_filed = 0;
                }
            }
            return all_filed;
        }

        explicit MS_BFS_ANP(graphtraversal::Webgraph &webgraph, std::vector<graphtraversal::Vertex> &vert_set,
                        std::vector<graphtraversal::BFS > &bfs_set) : webgraph(webgraph) {
            const int register_width = 64;
            // Assumption vertices id start from 1
            const uint64_t number_of_vertices = webgraph.vertices.size();
            std::vector<bitset > seen(number_of_vertices, 0), visit(number_of_vertices, 0), visitNext(number_of_vertices, 0);
            graphtraversal::Vertex vend = webgraph.getVertices().back();

            for(auto& bfs : bfs_set) {
                seen[bfs.getStartVertex().vert_id - 1] = -1;
                visit[bfs.getStartVertex().vert_id - 1] = -1;
                uint8_t dist = std::distance(bfs_set.begin(), std::find(bfs_set.begin(), bfs_set.end(), bfs));
                seen[bfs.getStartVertex().vert_id - 1] &= 1ULL << dist;
                visit[bfs.getStartVertex().vert_id - 1] &= 1ULL << dist;
            }

            double global_depth = 1;
            double sum_arr[register_width] = {};

            int16_t upper_margin = vend.vert_id, number_of_edges_traversed = 0;

            while(check_bitset(visit) && !check_full_bitset_vect(seen)){
                if(global_depth > 9){break;}
                double closeness_addition = global_depth;
                for(auto vect : vert_set){
                    int i = vect.vert_id;
                    if(visit[vect.vert_id - 1] == 0) {
                        ;
                    }
                    else {
                        const graphtraversal::Vertex vert_found = webgraph.vertices[i - 1];
                        for(const auto& neighbour : webgraph.getNeighbours(vert_found.vert_id)){
                            visitNext[neighbour - 1] |= visit[i - 1];
                            number_of_edges_traversed++;
                        }
                    }
                }
                for(auto vect : vert_set){
                    int i = vect.vert_id;
                    if(visit[vect.vert_id - 1] == 0) {
                        ;
                    }
                    else{
                        visitNext[i - 1] &= ~seen[i - 1];
                        seen[i - 1] |= visitNext[i - 1];
                        if(visitNext[i - 1] != 0){
                            bitset difference = visitNext[i - 1];
                            while(difference != 0){
                                if(difference & 1<<0){
                                    sum_arr[(i % (register_width + 1)) - 1] += closeness_addition;
                                }
                                difference >>= 1;
                            }
                        }
                    }
                }
                visit = visitNext;
                for(auto& bitset : visitNext){
                    bitset = 0;
                }
                global_depth++;
            }
            double sum_val = 0;
            for(auto val : sum_arr){
                if(val >= (number_of_vertices - 1)){
                    this->endresult +=  (number_of_vertices - 1) / val;
                }
                else {
                    // std::cout << " error: val calculated to low : " << val << std::endl;
                }

            }
            // std::cout << "number of edges traversed : " << number_of_edges_traversed << std::endl;
            this->traversed_edges = number_of_edges_traversed;
            // this->endresult /= bfs_set.size();
            // std::cout << " bfs_Set size  : " << bfs_set.size() << std::endl;
        }

        ~MS_BFS_ANP(){}

        std::double_t getEndresult(){
            return this->endresult;
        }

        std::uint64_t getTraversedEdges(){
            return this->traversed_edges;
        }
    };

}

#endif GRAPHTRAVERSAL_MS_BFS_ANP_H