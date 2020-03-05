//
// Created by 010ric on 28.11.19.
//
#include "graphtraversal/ms_bfs/ms_bfs.h"
#include "graphtraversal/ms_bfs/ms_bfs_anp.h"
using namespace graphtraversal;

int main() {
    graphtraversal::Webgraph webgraph = buildWebgraphFromFile("/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/multisourcegraphtraversal/graphtraversal/data/example-undirected 2/example-undirected.v",
                                                              "/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/multisourcegraphtraversal/graphtraversal/data/ldbc10k.csv");
    std::vector<graphtraversal::Vertex> start_vertices;
    for(auto& vertex : webgraph.getVertices()){
        // TODO : random pick of vertices ?
        // considered: std::uniform_real_distribution<double> dist(0.0, 10000.0);
        double val = (double)rand() / RAND_MAX;
        if(val < 0.001) {
            start_vertices.push_back(vertex);
        }
    }
    std::vector<graphtraversal::BFS> bfs_set;
    for(auto& vert : start_vertices){
        bfs_set.push_back(graphtraversal::BFS(webgraph, vert));
    }

    graphtraversal::MS_BFS ms_bfs(webgraph, start_vertices, bfs_set);

    return 0;
}
