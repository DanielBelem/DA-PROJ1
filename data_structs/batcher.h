#ifndef DAPROJ1_BATCHER_H
#define DAPROJ1_BATCHER_H

#include <unordered_set>
#include <unordered_map>
#include "Graph.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>


/**
 * @brief Class to handle batch processing of route calculations.
 */
class Batcher {
public:
    /**
     * @brief Constructs a new Batcher object.
     */
    Batcher();

    /**
     * @brief Reads input from a file.
     * @param filename The path to the input file.
     */
    void readInput(const std::string& filename);

    /**
     * @brief Generates the output file with calculated routes.
     * @param outputFilename The path to the output file.
     */
    void generateOutput(const std::string& outputFilename);

    /**
     * @brief Processes a batch of routes by calculating shortest paths.
     */
    void processBatch();

private:
    std::string mode; ///< Transportation mode (e.g., driving, walking)
    int source;       ///< Source node ID
    int destination;  ///< Destination node ID
    int maxWalkTime = -1;  ///< Maximum walking time (-1 for unlimited)
    bool no_alternative = false; ///< Flag to disable alternative routes
    std::unordered_set<int> avoidNodes; ///< Nodes to avoid during route calculation
    std::unordered_set<std::pair<int, int>, pair_hash> avoidSegments; ///< Segments to avoid

    std::vector<int> bestRoute; ///< The best calculated route
    int bestDistance = -1;      ///< Distance of the best route
    std::vector<int> altRoute;  ///< The alternative calculated route
    int altDistance = -1;       ///< Distance of the alternative route

    // Helper methods
    /**
     * @brief Parses a single line of input and extracts key-value pairs.
     * @param line The line to parse.
     */
    void parseLine(const std::string& line);

    /**
     * @brief Clears batch data between executions.
     */
    void clearBatchData();

    /**
     * @brief Handles the calculation of the shortest path.
     * @param graph The graph to operate on.
     */
    void handleShortestPath(Graph<int>& graph);
};

#endif // DAPROJ1_BATCHER_H
