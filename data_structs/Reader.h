#ifndef DA_ROUTE_PLANNING_READER_H
#define DA_ROUTE_PLANNING_READER_H

#include <unordered_set>
#include <unordered_map>
#include "Graph.h"
#include <chrono>

/**
 * @brief Class responsible for reading and parsing nodes and edges from input files.
 */
class Reader {
public:
    /**
     * @brief Constructs a new Reader object and initializes the driving graph.
     */
    Reader();

    /**
     * @brief Reads nodes from a CSV file and adds them to the graph.
     *
     * The CSV file should follow the format:
     * LocationName,NodeID,Code,Parking (0 or 1)
     *
     * @param filename The path to the file containing node data.
     */
    void readNodes(const std::string& filename);

    /**
     * @brief Reads edges from a CSV file and adds bidirectional edges to the graph.
     *
     * The CSV file should follow the format:
     * Location1,Location2,DrivingDistance,WalkingDistance
     *
     * @param filename The path to the file containing edge data.
     */
    void readEdges(const std::string& filename);



    Graph<int> graph; //< Graph representing the network of nodes.
    std::unordered_map<std::string, Vertex<int>*> allVertexes; //< Mapping of vertex codes to vertex pointers.


};

#endif // DA_ROUTE_PLANNING_READER_H
