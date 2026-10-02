#include "batcher.h"
#include "Reader.h"

/**
 * @brief Clears stored batch data to prepare for the next batch.
 */
void Batcher::clearBatchData() {
    mode.clear();
    source = destination = -1;
    maxWalkTime = -1;
    avoidNodes.clear();
    avoidSegments.clear();
}

/**
 * @brief Reads the input file and extracts batch configuration.
 * @param filename The name of the input file.
 */
void Batcher::readInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return;
    }

    std::string line;
    clearBatchData();

    while (std::getline(file, line)) {
        parseLine(line);
    }

    file.close();
    processBatch();
}

/**
 * @brief Parses a line of input to extract the key-value pair.
 * @param line The line to parse.
 */
void Batcher::parseLine(const std::string& line) {
    std::stringstream ss(line);
    std::string key, value;
    std::getline(ss, key, ':');
    std::getline(ss, value);
    value = value.substr(0);

    if (key == "Mode") {
        mode = value;
    } else if (key == "Source") {
        source = std::stoi(value);
    } else if (key == "Destination") {
        destination = std::stoi(value);
    } else if (key == "MaxWalkTime") {
        maxWalkTime = std::stoi(value);
        no_alternative = true;
    } else if (key == "AvoidNodes") {
        no_alternative = true;
        std::stringstream nodes(value);
        std::string node;
        while (std::getline(nodes, node, ',')) {
            if (!node.empty()) avoidNodes.insert(std::stoi(node));
        }
    } else if (key == "AvoidSegments") {
        std::stringstream segments(value);
        std::string segment;
        while (std::getline(segments, segment, ',')) {
            if (segment.find('-') != std::string::npos) {
                int from = std::stoi(segment.substr(0, segment.find('-')));
                int to = std::stoi(segment.substr(segment.find('-') + 1));
                avoidSegments.emplace(from, to);
            }
        }
    }
}

/**
 * @brief Processes the batch and finds the shortest path.
 */
void Batcher::processBatch() {
    Reader reader;
    reader.readNodes("../test_data/small_data/Locations.csv");
    reader.readEdges("../test_data/small_data/Distances.csv");

    Graph<int>& graph = reader.graph;
    handleShortestPath(graph);
    generateOutput("../test_data/batch/output.txt");
}

/**
 * @brief Handles the calculation of the shortest path and alternative route.
 * @param graph The graph object to find paths in.
 */
void Batcher::handleShortestPath(Graph<int>& graph) {
    auto v1 = graph.findVertex(source);
    auto v2 = graph.findVertex(destination);

    if (!v1 || !v2) {
        std::cerr << "Invalid source or destination!" << std::endl;
        return;
    }


    // Dijkstra for the best route
    graph.Dijkstra(v1, v2, graph, mode, avoidNodes, avoidSegments);
    bestRoute = graph.getPath(&graph, v1->getInfo(), v2->getInfo());
    bestDistance = bestRoute.empty() ? -1 : graph.findVertex(bestRoute.back())->getDist();

    std::unordered_set<int> forbiddenNodes(bestRoute.begin() + 1, bestRoute.end() - 1);
    graph.Dijkstra(v1, v2, graph, mode, forbiddenNodes, avoidSegments);
    altRoute = graph.getPath(&graph, v1->getInfo(), v2->getInfo());
    altDistance = altRoute.empty() ? -1 : graph.findVertex(altRoute.back())->getDist();
}

/**
 * @brief Generates the output file containing the calculated routes.
 * @param outputFilename The name of the output file.
 */
void Batcher::generateOutput(const std::string& outputFilename) {
    std::ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open " << outputFilename << " for writing!" << std::endl;
        return;
    }

    outFile << "Source:" << source << std::endl;
    outFile << "Destination:" << destination << std::endl;

    if (bestRoute.empty()) {
        outFile << "BestDrivingRoute:none\n";
    } else {
        outFile << "BestDrivingRoute:";
        for (size_t i = 0; i < bestRoute.size(); i++) {
            outFile << bestRoute[i];
            if (i + 1 < bestRoute.size()) outFile << ",";
        }
        outFile << "(" << bestDistance << ")\n";
    }

    if (altRoute.empty() && !no_alternative) {
        outFile << "AlternativeDrivingRoute:none\n";
    } else if (!no_alternative) {
        outFile << "AlternativeDrivingRoute:";
        for (size_t i = 0; i < altRoute.size(); i++) {
            outFile << altRoute[i];
            if (i + 1 < altRoute.size()) outFile << ",";
        }
        outFile << "(" << altDistance << ")\n";
    }

    outFile.close();
    std::cout << "Output written to " << outputFilename << " successfully." << std::endl;
}

/**
 * @brief Default constructor for Batcher.
 */
Batcher::Batcher() {
}
