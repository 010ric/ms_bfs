//
// Created by 010ric on 28.11.19.
//
#include "graphtraversal/bfs/bfs.h"
using namespace graphtraversal;

int main() {
    // Build Webgraph using Data_Reader
    graphtraversal::Webgraph webgraph = buildWebgraphFromFile("/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/multisourcegraphtraversal/graphtraversal/data/example-undirected 2/example-undirected.v",
                                                              "/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/multisourcegraphtraversal/graphtraversal/data/example-undirected 2/example-undirected.e");
    graphtraversal::BFS bfs_example = graphtraversal::BFS(webgraph, webgraph.getVertices().at(0));

    std::cout << " closeness centrality for vertex " << bfs_example.getStartVertex().vert_id << " is : " << bfs_example.get_closeness_centrality() << std::endl;

    return 0;
}
