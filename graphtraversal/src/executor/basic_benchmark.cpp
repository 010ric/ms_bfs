//
// Created by 010ric on 17.12.19.
//

#include <iostream>
#include "graphtraversal/webgraph/webgraph.h"
#include "graphtraversal/bfs/bfs.h"
#include "graphtraversal/ms_bfs/ms_bfs.h"
#include "graphtraversal/ms_bfs/ms_bfs_anp.h"
#include "graphtraversal/data_writer/data_writer.h"

using namespace graphtraversal;

// Referencing : https://ldbc.github.io/ldbc_graphalytics_docs/graphalytics_spec.pdf

int main() {
    // Application is the "allvertices closeness centrality" problem
    typedef std::chrono::milliseconds ms;

    auto start1 = std::chrono::high_resolution_clock::now();
    graphtraversal::Webgraph webgraph2 = buildWebgraphFromFile("/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/multisourcegraphtraversal/graphtraversal/data/ldbc10k.csv",
                                         "/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/multisourcegraphtraversal/graphtraversal/data/ldbc10k.csv");
    // Get ending timepoint
    auto stop1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

    std::cout << "Time taken to build webgraph : "
              << duration1.count() / 1000.000 << " milliseconds" << std::endl;

    std::vector<graphtraversal::Vertex> start_vertices;
    int BFS_CNT = 128;
    // TODO: Fixing way of how AVCC is computed with ms_bfs
    for(int i = 0; i < BFS_CNT; i++){
        start_vertices.push_back(webgraph2.vertices[i]);
    }

    std::double_t endresult = 0;
    int k = 0;

    /* ######################################################################################################################################## */
    /* ####################################                    Running the Textbook_BFS                      ################################## */
    /* ######################################################################################################################################## */

    std::vector<graphtraversal::BFS> bfs_set1;
    uint64_t total_traversed_edges = 0;
    auto start2 = std::chrono::high_resolution_clock::now();
    auto stop2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2);

    for(auto& vert : start_vertices){
        graphtraversal::BFS curr_bfs = graphtraversal::BFS(webgraph2, vert);
        start2 = std::chrono::high_resolution_clock::now();
        endresult += curr_bfs.run_T_BFS();
        stop2 = std::chrono::high_resolution_clock::now();
        duration2 += std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2);
        total_traversed_edges += curr_bfs.getTraversedEdges();
        k++;
    }

    std::cout << " ###################################### " << std::endl;
    std::cout << "Time taken to run " << k << " T_BFS computation : "
              << duration2.count() / 1000.000 << " milliseconds" << std::endl;

    std::cout << " T_BFS endresult : " << (endresult / k ) << " for the average closeness centrality of a node " << std::endl;
    std::cout << " T_BFS - traversed edges in total : " << total_traversed_edges << std::endl;
    /* ######################################################################################################################################## */
    /* ####################################                     Running the MS_BFS_ANP                       ################################## */
    /* ######################################################################################################################################## */

    // std::vector<graphtraversal::Vertex> part_start_verts;
    std::vector<graphtraversal::BFS> bfs_set2;

    auto start3 = std::chrono::high_resolution_clock::now();
    auto stop3 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);
    uint64_t traversed_edges_by_ms = 0;
    endresult = 0;
    int y = 0;

    for(size_t i=0; i < start_vertices.size(); i+=64){
        for(size_t j=i; j < std::min(i+64, start_vertices.size()); j++){
            graphtraversal::BFS curr_bfs = graphtraversal::BFS(webgraph2, start_vertices[j]);
            bfs_set2.push_back(curr_bfs);
            // part_start_verts.push_back(start_vertices[j]);
            // std::cout << " ########### ---->>> std::min(i+64, start_vertices.size() : " << std::min(i+64, start_vertices.size()) << std::endl;
        }
        start3 = std::chrono::high_resolution_clock::now();
        graphtraversal::MS_BFS_ANP msBfs = graphtraversal::MS_BFS_ANP(webgraph2, webgraph2.vertices, bfs_set2);
        stop3 = std::chrono::high_resolution_clock::now();

        duration3 += std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);
        // Get ending timepoint
        endresult += (msBfs.getEndresult() / bfs_set2.size());
        // std::cout << " bfs_set size  : " << bfs_set2.size() << std::endl;
        traversed_edges_by_ms += msBfs.getTraversedEdges();
        // duration3 += std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);
        // part_start_verts.clear();
        bfs_set2.clear();
        y++;
    }

    std::cout << " ###################################### " << std::endl;
    std::cout << "Time taken to run " << y << " MS_BFS computation with T_BFS set containing " << 64 << " T_BFSs : "
              << duration3.count() / 1000.000 << " milliseconds" << std::endl;

    std::cout << " MS_BFS - endresult : " << (endresult / y) << std::endl;
    std::cout << " MS_BFS - traversed edges in total : " << traversed_edges_by_ms << std::endl;
    /* ######################################################################################################################################## */
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start1);
    std::cout << "Time taken to run the hole benchmark " << duration.count() / 1000.000 << " milliseconds" << std::endl;
    /* ######################################################################################################################################## */

    std::string write_path = "/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/multisourcegraphtraversal/bfs_measurements1.csv";
    graphtraversal::CSVWriter writer(write_path, ",");

    std::vector<std::double_t >number_vertices;
    std::vector<std::double_t >computation_times;
    writer.addHeaderForRow("NumberOfVertices","ComputationTime(seconds)");
    writer.addDataInRow(number_vertices.begin(), computation_times.begin(), number_vertices.end(), computation_times.end());

    return 0;
}