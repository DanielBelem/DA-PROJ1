#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "Reader.h"
#include "Graph.h"

/**
 * @brief Constructs a new Reader object and initializes the driving graph.
 */
Reader::Reader() {
    this->graph = Graph<int>();
}

void Reader::readNodes(const std::string& filename) {
    std::string location, idStr, code, parkingStr;

    std::string line;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return;
    }
    // Skip header line
    std::getline(file, line, '\n');
    while (std::getline(file, line, '\n')) {
        std::stringstream ss(line);

        std::getline(ss, location, ',');
        std::getline(ss, idStr, ',');
        std::getline(ss, code, ',');
        std::getline(ss, parkingStr, ',');


        int id = std::stoi(idStr);
        bool parking = (parkingStr == "1");


        // Add the vertex to the graph
        this->graph.addVertex(id);

        // Retrieve the added vertex and set its properties
        auto a = this->graph.findVertex(id);

        a->setLocation(location);
        a->setParking(stoi(parkingStr));
        a->setCode(code);

        // TODO check if we need this
        Vertex<int>* v = graph.findVertex(id);
        if (v != nullptr) {
            allVertexes[code] = v;
        }

    }

    file.close();


}

/**
 * @brief Reads edges from the given CSV file and populates the graph with bidirectional edges.
 *
 * The file should contain data in the format:
 * Location1,Location2,DrivingDistance,WalkingDistance
 *
 * @param filename The path to the file containing edge data.
 */
void Reader::readEdges(const std::string& filename) {
    std::string location1, location2, drivingStr, walkingStr;

    std::string line;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return;
    }

    // Skip header line
    std::getline(file, line, '\n');
    while (std::getline(file, line, '\n')) {
        std::stringstream ss(line);

        std::getline(ss, location1, ',');
        std::getline(ss, location2, ',');
        std::getline(ss, drivingStr, ',');
        std::getline(ss, walkingStr, ',');

        //std::cout << location1 << " " << location2 << " " << drivingStr << " " << walkingStr << std::endl;


        if (drivingStr != "X"){
            int driving = std::stoi(drivingStr);
            int walking = std::stoi(walkingStr);
            graph.addBidirectionalEdge(allVertexes[location1]->getInfo(), allVertexes[location2]->getInfo(), walking, driving);
        }
        else{
            int walking = std::stoi(walkingStr);
            graph.addBidirectionalEdge(allVertexes[location1]->getInfo(), allVertexes[location2]->getInfo(), walking, INF);

        }
    }

    file.close();


}
