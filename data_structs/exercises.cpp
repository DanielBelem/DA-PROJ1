
#include "exercises.h"
/**
 * @file exercises.cpp
 * @brief Implementation of exercise functions for pathfinding.
 */


/**
 * @brief Finds the best and alternative driving routes between two locations.
 * @param reader Reference to the Reader object containing the graph.
 */
void Exerciser::ex_2_1(Reader &reader) {
    std::cout << "\nSource (id): -";
    std::string loc1, loc2;
    std::cin >> loc1;
    std::cout << "Destination (id): -";
    std::cin >> loc2;


    auto v1 = reader.graph.findVertex(std::stoi(loc1));
    auto v2 = reader.graph.findVertex(std::stoi(loc2));

    std::string mode = "driving";

    reader.graph.Dijkstra(v1, v2, reader.graph,mode, {}, {});

    std::vector<int> path;
    path = reader.graph.getPath(&(reader.graph), v1->getInfo(), v2->getInfo());

    if (path.empty()) {
        std::cout << "BestDrivingRoute: none\n";
        std::cout << "AlternativeDrivingRoute: none\n";
        return;
    }

    int totalDist = reader.graph.findVertex(path.back())->getDist();

    std::cout << "BestDrivingRoute:";
    for (size_t i = 0; i < path.size(); i++) {
        std::cout << path[i];
        if (i + 1 < path.size()) std::cout << ",";
    }
    std::cout << "(" << totalDist << ")\n";


    //Alternative Route
    std::unordered_set<int> forbiddenNodes;
    for (auto id : path) {
        if (id != v1->getInfo() && id != v2->getInfo()) {
            forbiddenNodes.insert(id);
        }
    }

    reader.graph.Dijkstra(v1, v2, reader.graph, mode, forbiddenNodes, {});
    auto pathB = reader.graph.getPath(&(reader.graph), v1->getInfo(), v2->getInfo());

    if (!pathB.empty()) {
        std::cout << "AlternativeDrivingRoute:";
        for (size_t i = 0; i < pathB.size(); i++) {
            std::cout << pathB[i];
            if (i + 1 < pathB.size()) std::cout << ",";
        }
        std::cout << "(" << reader.graph.findVertex(pathB.back())->getDist() << ")\n";
    } else {
        std::cout << "AlternativeDrivingRoute: none\n";
    }
};

/**
 * @brief Finds a restricted driving route avoiding specific nodes and segments.
 * @param reader Reference to the Reader object containing the graph.
 */
void Exerciser::ex_2_2(Reader &reader) {
    std::string loc1, loc2, avoidNodes, avoidEdges, includeNode;
    std::cout << "\nSource (id): -";
    std::cin >> loc1;
    std::cout << "Destination (id): -";
    std::cin >> loc2;
    std::cin.ignore();
    std::cout << "Avoid Nodes (id,id) (blank if none): -";
    std::getline(std::cin, avoidNodes);
    std::cout << "Avoid Segments ((id,id),(id,id)) (blank if none): -";
    std::getline(std::cin, avoidEdges);
    std::cout << "Include Node (id) (blank if none): -";
    std::getline(std::cin, includeNode);

    auto v1 = reader.graph.findVertex(std::stoi(loc1));
    auto v2 = reader.graph.findVertex(std::stoi(loc2));

    std::string mode = "driving";

    std::unordered_set<int> forbiddenNodes;
    std::unordered_set<std::pair<int, int>, pair_hash> forbiddenEdges;
    int include = -1;

    //Clean if any avoidNodes
    if (!avoidNodes.empty()) {
        std::stringstream ss(avoidNodes);
        std::string token;
        while (getline(ss, token, ',')) {
            forbiddenNodes.insert(std::stoi(token));
        }
    }

    //Clean if any avoidEdges
    if (!avoidEdges.empty()) {
        std::string cleaned = avoidEdges;
        cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '('), cleaned.end());
        cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), ')'), cleaned.end());

        std::stringstream ss(cleaned);
        std::string a, b;
        while (getline(ss, a, ',')) {
            if (!getline(ss, b, ',')) break;
            forbiddenEdges.insert({std::stoi(a), std::stoi(b)});
        }
    }

    //Clean if any includeNode
    if (!includeNode.empty()) {
        include = std::stoi(includeNode);
    }

    int totalDist;
    auto path = reader.graph.getRestrictedPath(reader.graph, mode, v1, v2, include, forbiddenNodes, forbiddenEdges, totalDist);

    if (!path.empty()) {
        std::cout << "RestrictedDrivingRoute:";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i + 1 < path.size()) std::cout << ",";
        }
        std::cout << "(" << totalDist << ")\n";
    } else {
        std::cout << "RestrictedDrivingRoute: none\n";
    }
};

/**
 * @brief Finds the best parking location to minimize walking distance.
 * @param reader Reference to the Reader object containing the graph.
 * @param v1 Starting vertex.
 * @param v2 Destination vertex.
 * @param max_walk_time Maximum walking time allowed.
 * @param drivingPath Vector to store the driving path.
 * @param walkingPath Vector to store the walking path.
 * @param walkingDist Stores the walking distance.
 * @param drivingDist Stores the driving distance.
 * @param parkingNode Stores the chosen parking node.
 * @param totalTime Stores the total travel time.
 * @param altPathsMap Map of alternative paths.
 * @param parkingNodesDistMap Map of parking nodes and their distances.
 * @param parkingNodesPathMap Map of paths to parking nodes.
 * @param altNodesDistMap Map of alternative nodes and their distances.
 */
void Exerciser::ex_3(Reader &reader, Vertex<int> &v1, Vertex<int> &v2, int &max_walk_time,
          std::vector<int> &drivingPath, std::vector<int> &walkingPath,
          int& walkingDist, int& drivingDist, int &parkingNode, int &totalTime,
          std::unordered_map<int, std::vector<int>> &altPathsMap,
          std::unordered_map<int, int> &parkingNodesDistMap,
          std::unordered_map<int, std::vector<int>> &parkingNodesPathMap,
          std::unordered_map<int, int> &altNodesDistMap,
          std::unordered_set<int> &forbiddenNodes,
          std::unordered_set<std::pair<int, int>, pair_hash> &forbiddenEdges) {

    std::string loc1, loc2, max_walk_time_str, avoidNodes, avoidEdges;

    std::cout << "\nSource (code): ";
    std::cin >> loc1;
    std::cout << "Destination (code): ";
    std::cin >> loc2;
    std::cout << "Max Walk Time: ";
    std::cin >> max_walk_time_str;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpar buffer
    std::cout << "Avoid Nodes (id,id) (blank if none):";
    std::getline(std::cin, avoidNodes);
    std::cout << "Avoid Segments ((id,id),(id,id)) (blank if none):";
    std::getline(std::cin, avoidEdges);

    v1 = *reader.graph.findVertex(std::stoi(loc1));
    v2 = *reader.graph.findVertex(std::stoi(loc2));
    max_walk_time = std::stoi(max_walk_time_str);

    //Clean if any avoidNodes
    if (!avoidNodes.empty()) {
        std::stringstream ss(avoidNodes);
        std::string token;
        while (getline(ss, token, ',')) {
            forbiddenNodes.insert(std::stoi(token));
        }
    }

    //Clean if any avoidEdges
    if (!avoidEdges.empty()) {
        std::string cleaned = avoidEdges;
        cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '('), cleaned.end());
        cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), ')'), cleaned.end());

        std::stringstream ss(cleaned);
        std::string a, b;
        while (getline(ss, a, ',')) {
            if (!getline(ss, b, ',')) break;
            forbiddenEdges.insert({std::stoi(a), std::stoi(b)});
        }
    }

    std::unordered_set<Vertex<int> *> parkingNodes;
    for (auto v : reader.graph.getVertexSet()) {
        if (v->getParking() == 1) {
            parkingNodes.insert(v);
        }
    }

    int bestTotal = INF;
    parkingNode = -1;
    int minDist = INF;


    for (auto p : parkingNodes) {
        reader.graph.Dijkstra(&v1, p, reader.graph, "driving", forbiddenNodes,forbiddenEdges);
        std::vector<int> curDrivingPath = reader.graph.getPath(&(reader.graph), v1.getInfo(), p->getInfo());
        if (curDrivingPath.empty()) {
            continue;
        }

        parkingNodesPathMap[p->getInfo()] = curDrivingPath;

        /*drivingPath = reader.graph.getPath(&(reader.graph), v1.getInfo(), p->getInfo());
         parkingNodesDistMap[p->getInfo()] = reader.graph.findVertex(drivingPath.back())->getDist();

         if (drivingPath.empty()) {
             continue;
         } */

        int curDist = reader.graph.findVertex(curDrivingPath.back())->getDist();
        parkingNodesDistMap[p->getInfo()] = curDist;

    }


    for (auto &p : parkingNodesPathMap) {
        auto v = reader.graph.findVertex(p.first);
        reader.graph.Dijkstra(v, &v2, reader.graph, "walking", forbiddenNodes, forbiddenEdges);
        auto curWalkPath = reader.graph.getPath(&(reader.graph), v->getInfo(), v2.getInfo());

        if (curWalkPath.empty()) continue;

        //std::cout << "Current parking node: " << p.first << "\n";
        //std::cout << "Current walkingPath: ";
        /*for (size_t i = 0; i < curWalkPath.size(); i++) {
            std::cout << curWalkPath[i];
            if (i + 1 < curWalkPath.size()) std::cout << ",";
        }
        std::cout << "\n";*/

        int curWalkingDist = reader.graph.findVertex(curWalkPath.back())->getDist();

        if (curWalkingDist > max_walk_time) {
            altPathsMap[p.first] = curWalkPath;
            altNodesDistMap[p.first] = curWalkingDist;
            continue;
        }


        int curDrivingDist = parkingNodesDistMap[p.first];

        int curTotalTime = curDrivingDist + curWalkingDist;

        if (curTotalTime < minDist) {
            minDist = curTotalTime;
            totalTime = curTotalTime;
            drivingDist = curDrivingDist;
            walkingDist = curWalkingDist;
            parkingNode = p.first;
            walkingPath = curWalkPath;
            drivingPath = std::vector<int>(p.second.begin(), p.second.end());;
        }
        else {
            altPathsMap[p.first] = curWalkPath;
        }
    }

}

/**
 * @brief Executes ex_3 with user input and displays the best route with parking.
 * @param reader Reference to the Reader object containing the graph.
 */
void Exerciser::ex_3_1(Reader &reader) {

    Vertex<int> v1 = Vertex<int>(0);
    Vertex<int> v2 = Vertex<int>(0);
    int max_walk_time;
    std::vector<int> drivingPath;
    std::vector<int> walkingPath;
    int walkingDist = 0;
    int drivingDist = 0;
    int parkingNode = 0;
    int totalTime = 0;
    std::unordered_map <int, std::vector<int>> altPathsMap;
    std::unordered_map <int, int> parkingNodesDistMap;
    std::unordered_map <int, std::vector<int>> parkingNodesPathMap;
    std::unordered_map <int, int> altNodesDistMap;
    std::unordered_set<int> forbiddenNodes;
    std::unordered_set<std::pair<int, int>, pair_hash> forbiddenEdges;

    ex_3(reader, v1, v2, max_walk_time, drivingPath, walkingPath, walkingDist, drivingDist, parkingNode, totalTime, altPathsMap, parkingNodesDistMap, parkingNodesPathMap, altNodesDistMap, forbiddenNodes, forbiddenEdges);

    // Output results
    std::cout << "Source: " << v1.getInfo() << "\n";
    std::cout << "Destination: " << v2.getInfo() << "\n";

    std::cout << "DrivingRoute: ";
    if (!drivingPath.empty()) {
        for (size_t i = 0; i < drivingPath.size(); i++) {
            std::cout << drivingPath[i] << (i + 1 < drivingPath.size() ? "," : "");
        }
        std::cout << " (" << drivingDist << ")\n";
    } else {
        std::cout << "none\n";
    }

    std::cout << "Parking node: " << (parkingNode != -1 ? std::to_string(parkingNode) : "none") << "\n";

    std::cout << "WalkingRoute: ";
    if (!walkingPath.empty()) {
        for (size_t i = 0; i < walkingPath.size(); i++) {
            std::cout << walkingPath[i] << (i + 1 < walkingPath.size() ? "," : "");
        }
        std::cout << " (" << walkingDist << ")\n";
    } else {
        std::cout << "none\n";
    }

    std::cout << "Total time: " << (walkingDist != max_walk_time + 1 ? std::to_string(totalTime) : "none") << "\n";

    if (drivingPath.empty() && walkingPath.empty()) {
        std::cout << "Message: TNo possible route with max. walking time of ", max_walk_time," minutes\n";
    }
}

/**
 * @brief Executes ex_3 with user input and displays alternative routes if necessary.
 * @param reader Reference to the Reader object containing the graph.
 */
void Exerciser::ex_3_2(Reader &reader) {

    Vertex<int> v1 = Vertex<int>(0), v2 = Vertex<int>(0);
    int max_walk_time;
    std::vector<int> drivingPath;
    std::vector<int> walkingPath;
    int walkingDist = 0;
    int drivingDist = 0;
    int parkingNode = 0;
    int totalTime = 0;
    std::unordered_map <int, std::vector<int>> altPathsMap;
    std::unordered_map <int, int> parkingNodesDistMap;
    std::unordered_map <int, std::vector<int>> parkingNodesPathMap;
    std::unordered_map <int, int> altNodesDistMap;
    std::unordered_set<int> forbiddenNodes;
    std::unordered_set<std::pair<int, int>, pair_hash> forbiddenEdges;

    ex_3(reader, v1, v2, max_walk_time, drivingPath, walkingPath, walkingDist, drivingDist, parkingNode, totalTime, altPathsMap, parkingNodesDistMap, parkingNodesPathMap, altNodesDistMap, forbiddenNodes, forbiddenEdges);

    if (!drivingPath.empty() && !walkingPath.empty()) {
        std::cout << "Source: " << v1.getInfo() << "\n";
        std::cout << "Destination: " << v2.getInfo() << "\n";

        std::cout << "DrivingRoute: ";
        if (!drivingPath.empty()) {
            for (size_t i = 0; i < drivingPath.size(); i++) {
                std::cout << drivingPath[i] << (i + 1 < drivingPath.size() ? "," : "");
            }
            std::cout << " (" << drivingDist << ")\n";  // This should be the total driving distance
        } else {
            std::cout << "none\n";
        }

        std::cout << "Parking node: " << (parkingNode != -1 ? std::to_string(parkingNode) : "none") << "\n";

        std::cout << "WalkingRoute: ";
        if (!walkingPath.empty()) {
            for (size_t i = 0; i < walkingPath.size(); i++) {
                std::cout << walkingPath[i] << (i + 1 < walkingPath.size() ? "," : "");
            }
            std::cout << " (" << walkingDist << ")\n";
        } else {
            std::cout << "none\n";
        }

        std::cout << "Total time: " << (walkingDist != max_walk_time + 1 ? std::to_string(totalTime) : "none") << "\n";

    }
    else {
        std::cout << "Source: " << v1.getInfo() << "\n";
        std::cout << "Destination: " << v2.getInfo() << "\n";

        if (altPathsMap.size() == 0) {
            std::cout << "DrivingRoute: \n";
            std::cout << "Parking node: \n";
            std::cout << "WalkingRoute: \n";
            std::cout << "Total time: \n";
            std::cout << "Message: :No possible route\n";
        }

        std::cout << "we're here \n";


        int minDist1 = INF;
        int minDist2 = INF;
        std::vector<int> altWalkPath1;
        std::vector<int>  altDrivePath1;
        std::vector<int>  altWalkPath2;
        std::vector<int>  altDrivePath2;
        int walkDist1;
        int driveDist1;
        int walkDist2;
        int driveDist2;
        int parkingNode1 = -1;
        int parkingNode2 = -1;

        for (auto alt : altPathsMap) {
            int curWalkDist = altNodesDistMap[alt.first];

            int curDriveDist = parkingNodesDistMap[alt.first];
            int curTotalDist = curWalkDist + curDriveDist;

            if (curTotalDist < minDist1 && curTotalDist < minDist2) {

                if (minDist2 < minDist1){
                    minDist2 = minDist1;
                    altWalkPath2 = altDrivePath1;
                    altDrivePath2 = altDrivePath1;
                    walkDist2 = walkDist1;
                    driveDist2 = driveDist1;
                    parkingNode2 = parkingNode1;
                }

                minDist1 = curTotalDist;
                altWalkPath1 = alt.second;
                altDrivePath1 = parkingNodesPathMap[alt.first];
                walkDist1 = curWalkDist;
                driveDist1 = curDriveDist;
                parkingNode1 = alt.first;
            }
            else if (curTotalDist > minDist1 && curTotalDist < minDist2) {
                minDist2 = curTotalDist;
                altWalkPath2 = alt.second;
                altDrivePath2 = parkingNodesPathMap[alt.first];
                walkDist2 = curWalkDist;
                driveDist2 = curDriveDist;
                parkingNode2 = alt.first;
            }

        }


        std::cout << "Source: " << v1.getInfo() << "\n";
        std::cout << "Destination: " << v2.getInfo() << "\n";

        std::cout << "DrivingRoute1: ";
        if (!altDrivePath1.empty()) {
            for (size_t i = 0; i < altDrivePath1.size(); i++) {
                std::cout << altDrivePath1[i] << (i + 1 < altDrivePath1.size() ? "," : "");
            }
            std::cout << " (" << driveDist1 << ")\n";  // This should be the total driving distance
        } else {
            std::cout << "none\n";
        }

        std::cout << "Parking node1: " << (parkingNode1 != -1 ? std::to_string(parkingNode1) : "none") << "\n";

        std::cout << "WalkingRoute1: ";
        if (!altWalkPath1.empty()) {
            for (size_t i = 0; i < altWalkPath1.size(); i++) {
                std::cout << altWalkPath1[i] << (i + 1 < altWalkPath1.size() ? "," : "");
            }
            std::cout << " (" << walkDist1 << ")\n";
        } else {
            std::cout << "none\n";
        }

        std::cout << "TotalTime1: " << (walkDist1 != max_walk_time + 1 ? std::to_string(minDist1) : "none") << "\n";


        std::cout << "DrivingRoute2: ";
        if (!altDrivePath2.empty()) {
            for (size_t i = 0; i < altDrivePath2.size(); i++) {
                std::cout << altDrivePath2[i] << (i + 1 < altDrivePath2.size() ? "," : "");
            }
            std::cout << " (" << driveDist2 << ")\n";  // This should be the total driving distance
        } else {
            std::cout << "none\n";
        }

        std::cout << "Parking node2: " << (parkingNode2 != -1 ? std::to_string(parkingNode2) : "none") << "\n";

        std::cout << "WalkingRoute2: ";
        if (!altWalkPath2.empty()) {
            for (size_t i = 0; i < altWalkPath2.size(); i++) {
                std::cout << altWalkPath2[i] << (i + 1 < altWalkPath2.size() ? "," : "");
            }
            std::cout << " (" << walkDist2 << ")\n";
        } else {
            std::cout << "none\n";
        }

        std::cout << "TotalTime2: " << (walkDist2 != max_walk_time + 1 ? std::to_string(minDist2) : "none") << "\n";



    }



}
