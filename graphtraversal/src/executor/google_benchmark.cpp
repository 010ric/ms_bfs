//
// Created by 010ric on 12.12.19.
//

#include <iostream>
#include "graphtraversal/webgraph/webgraph.h"
#include "graphtraversal/bfs/bfs.h"
#include "graphtraversal/ms_bfs/ms_bfs.h"
// #include "../../vendor/google/benchmark/src/statistics.h"
// #include "benchmark/PerfEvent.hpp"
#include <benchmark/benchmark.h>

using namespace graphtraversal;

static void execute_BFS(benchmark::State& state) {
    // Perform setup here
    graphtraversal::Webgraph webgraph = buildWebgraphFromFile("/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/multisourcegraphtraversal/graphtraversal/data/ldbc10k.csv");
    for (auto _ : state) {
        graphtraversal::BFS bfs_test(webgraph, webgraph.getVertices().at(0));
    }
}

static void execute_MS_BFS(benchmark::State& state) {
    graphtraversal::Webgraph webgraph = buildWebgraphFromFile("/Users/mturic/global/privat/studium/Informatik/Semester5/cppPraktikum/multisourcegraphtraversal/graphtraversal/data/ldbc10k.csv");
    std::vector<graphtraversal::Vertex> start_vertices;
    for(const auto& vertex : webgraph.getVertices()){
        // TODO : random pick of vertices ?
        // considered: std::uniform_real_distribution<double> dist(0.0, 10000.0);
        double val = (double)rand() / RAND_MAX;
        if(val < 0.001) {
            start_vertices.push_back(vertex);
        }
    }
    std::vector<graphtraversal::BFS*> bfs_set;
    for(const auto& vert : start_vertices){
        graphtraversal::BFS* curr_bfs = new graphtraversal::BFS(webgraph, *vert);
        bfs_set.push_back(curr_bfs);
    }
    for (auto _ : state) {
        // graphtraversal::MS_BFS ms_bfs_test(webgraph, start_vertices, bfs_set);
    }
    bfs_set.clear();
}

// Benchmark set up to evaluate -- Closeness Centrality --
// A) Ploting the results using ggplot2
    // 1. Data size scalability results : x - axis : vertices (in millions) ; y - axis : Runtime (in minutes)
    // 2. BFS count scalability results : x - axis : Number of BFSs ; y - axis : GTEPS ( GTEPS == billion TEPS, TEPS == traversed edges per second)
// B) Still missing calculation of closeness centrality

BENCHMARK(execute_BFS)->Unit(benchmark::kMillisecond);
// BENCHMARK(execute_MS_BFS)->Unit(benchmark::kMillisecond);
// BENCHMARK_OUT_FORMAT();

BENCHMARK_MAIN();

// current execution times for BFS and 10k vertices vary from 200 to 780 milliseconds and 1 to 13 iterations of the bfs while loop: