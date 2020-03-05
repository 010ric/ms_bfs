//
// Created by 010ric on 28.11.19.
//

#ifndef GRAPHTRAVERSAL_MS_BFS_H
#define GRAPHTRAVERSAL_MS_BFS_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <bitset>
#include <graphtraversal/webgraph/webgraph.h>
#include <graphtraversal/bfs/bfs.h>
#include <boost/dynamic_bitset.hpp>

namespace graphtraversal {

    class MS_BFS {
        typedef uint64_t bitset;

    private:
        // graph containing all vertices and edges
        graphtraversal::Webgraph webgraph;
        std::double_t endresult;

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

        explicit MS_BFS(graphtraversal::Webgraph &webgraph, std::vector<graphtraversal::Vertex> &vert_set,
                        std::vector<graphtraversal::BFS > &bfs_set) : webgraph(webgraph) {
            const uint64_t number_of_vertices = webgraph.getVertices().size();
            std::vector<bitset > seen(number_of_vertices, 0), visit(number_of_vertices, 0), visitNext(number_of_vertices, 0);
            graphtraversal::Vertex vend = webgraph.getVertices().back();

            for(auto& bfs : bfs_set) {
                seen[bfs.getStartVertex().vert_id - 1] = -1;
                visit[bfs.getStartVertex().vert_id - 1] = -1;
                uint8_t dist = std::distance(bfs_set.begin(), std::find(bfs_set.begin(), bfs_set.end(), bfs));
                seen[bfs.getStartVertex().vert_id - 1] &= 1 << dist;
                visit[bfs.getStartVertex().vert_id - 1] &= (1 << dist);
            }

            double global_depth = 1;
            double sum_arr[64];
            int16_t upper_margin = vend.vert_id;

            while(check_bitset(visit)){
                double closeness_addition = global_depth;
                for(int i = vert_set.at(0).vert_id; i <= upper_margin; i++){
                    if(visit[i - 1] == 0) {
                        break;
                    }
                    const graphtraversal::Vertex vert_found = webgraph.getVertices()[i - 1];
                    for(const auto& neighbour : webgraph.getNeighbours(vert_found.vert_id)){
                        bitset difference = visit[vert_found.vert_id] & ~seen[neighbour];
                        if(difference != 0){
                            visitNext[neighbour] |= difference;
                            seen[neighbour] |= difference;
                            std::uint8_t counter = 0;
                            while(difference != 0){
                                difference >>= 1;
                                if(difference & 1<<0){
                                    sum_arr[counter] += closeness_addition;
                                }
                                counter++;
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
            double sum_val;
            for(auto val : sum_arr){
                sum_val +=  val;
            }
            this->endresult = (number_of_vertices - 1) / sum_val;
        }

        ~MS_BFS(){}

        std::double_t getEndresult(){
            return this->endresult;
        }
    };

}

#endif GRAPHTRAVERSAL_MS_BFS_H