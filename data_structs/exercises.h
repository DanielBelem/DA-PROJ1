/// @file exercises.h
/// @brief Declaration of Exerciser class and its methods.
/// @details This file contains the declaration of the Exerciser class, which provides multiple exercises related to route planning using graph algorithms.

#ifndef DAPROJ1_EXERCISES_H
#define DAPROJ1_EXERCISES_H

#include "Reader.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include "menu.h"
#include "Reader.h"
#include "Graph.h"
#include "MutablePriorityQueue.h"

/**
 * @class Exerciser
 * @brief A class that contains various exercises related to route calculation.
 */
class Exerciser{
public:
    /**
     * @brief Finds the best and alternative driving routes between two locations.
     * @param reader A reference to the Reader object containing graph data.
     */
    void ex_2_1(Reader &reader);

    /**
     * @brief Finds a restricted driving route between two locations while avoiding specified nodes and edges.
     * @param reader A reference to the Reader object containing graph data.
     */
    void ex_2_2(Reader &reader);

    /**
     * @brief Finds the optimal parking and walking route given the driving and walking distances.
     * @param reader A reference to the Reader object containing graph data.
     * @param v1 Source vertex.
     * @param v2 Destination vertex.
     * @param max_walk_time Maximum walking time allowed.
     * @param drivingPath A reference to a vector storing the driving path.
     * @param walkingPath A reference to a vector storing the walking path.
     * @param walkingDist A reference to store the walking distance.
     * @param drivingDist A reference to store the driving distance.
     * @param parkingNode A reference to store the parking node identifier.
     * @param totalTime A reference to store the total time of the route.
     * @param altPathsMap A reference to a map containing alternative paths.
     * @param parkingNodesDistMap A reference to a map containing distances to parking nodes.
     * @param parkingNodesPathMap A reference to a map containing paths to parking nodes.
     * @param altNodesDistMap A reference to a map containing distances to alternative nodes.
     */
    void ex_3(Reader &reader, Vertex<int> &v1, Vertex<int> &v2, int &max_walk_time,
              std::vector<int> &drivingPath, std::vector<int> &walkingPath,
              int& walkingDist, int& drivingDist, int &parkingNode, int &totalTime,
              std::unordered_map<int, std::vector<int>> &altPathsMap,
              std::unordered_map<int, int> &parkingNodesDistMap,
              std::unordered_map<int, std::vector<int>> &parkingNodesPathMap,
              std::unordered_map<int, int> &altNodesDistMap,
              std::unordered_set<int> &forbiddenNodes,
              std::unordered_set<std::pair<int, int>, pair_hash> &forbiddenEdges);

    /**
     * @brief Computes the best parking and walking route and displays the results.
     * @param reader A reference to the Reader object containing graph data.
     */
    void ex_3_1(Reader &reader);

    /**
     * @brief Computes alternative parking and walking routes and displays the results.
     * @param reader A reference to the Reader object containing graph data.
     */
    void ex_3_2(Reader &reader);
};

#endif //DAPROJ1_EXERCISES_H
