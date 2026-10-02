// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include "MutablePriorityQueue.h"

struct pair_hash {
    size_t operator()(const std::pair<int, int> &p) const {
        auto h1 = std::hash<int>{}(p.first);
        auto h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 1); // combine hashes
    }
};


template <class T>
class Edge;

#define INF std::numeric_limits<int>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
public:
    Vertex(T in);
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue

    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge<T> *getPath() const;
    std::vector<Edge<T> *> getIncoming() const;

    void setInfo(T info);
    void setVisited(bool visited);
    void setProcessing(bool processing);

    //Changes to the graph
    void setLocation(std::string location);
    void setCode(std::string code);
    void setParking(int parking);
    std::string getLocation();
    std::string getCode();
    int getParking();
    //Endof changes

    int getLow() const;
    void setLow(int value);
    int getNum() const;
    void setNum(int value);

    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge<T> *path);
    Edge<T> * addEdge(Vertex<T> *dest, double ww, double dw);
    bool removeEdge(T in);
    void removeOutgoingEdges();

    friend class MutablePriorityQueue<Vertex>;
protected:
    T info;                // info node
    std::string location;
    std::string code;
    int parking;
    std::vector<Edge<T> *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    int low = -1, num = -1; // used by SCC Tarjan
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming; // incoming edges

    int queueIndex = 0; 		// required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, double ww, double dw);

    Vertex<T> * getDest() const;
    bool isSelected() const;
    Vertex<T> * getOrig() const;
    Edge<T> *getReverse() const;
    double getFlow() const;
    //Additions to the graph
    double getWweight() const;
    double getDweight() const;
    void setWweight(double ww);
    void setDweight(double dw);
    //Endof additions

    void setSelected(bool selected);
    void setReverse(Edge<T> *reverse);
    void setFlow(double flow);
protected:
    Vertex<T> * dest; // destination vertex
    double wweight; // edge weight, can also be used for capacity
    double dweight;

    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

    double flow; // for flow-related problems
};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:
    ~Graph();
    /*
    * Auxiliary function to find a vertex with a given the content.
    */
    Vertex<T> *findVertex(const T &in) const;
    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const T &in);
    bool removeVertex(const T &in);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const T &sourc, const T &dest, double dw, double ww);
    bool removeEdge(const T &source, const T &dest);
    bool addBidirectionalEdge(const T &sourc, const T &dest, double dw, double ww);


    //ADDED

    static std::vector<T> getPath(Graph<T> * g, const int &origin, const int &dest);

    //From TPs

    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;


    int Dijkstra(Vertex<T> *src, Vertex<T> *dest, Graph<T> &g, const std::string mode,
                 const std::unordered_set<int> &forbiddenNodes,
                 const std::unordered_set<std::pair<int, int>, pair_hash> &forbiddenEdges);

    std::vector<T>
    getRestrictedPath(Graph<int> &graph, const std::string mode, Vertex<int> *src, Vertex<int> *dest, int includeNode,
                      const std::unordered_set<int> &forbiddenNodes,
                      const std::unordered_set<std::pair<int, int>, pair_hash> &forbiddenEdges, int &totalDist);

protected:
    std::vector<Vertex<T> *> vertexSet;    // vertex set

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

    /*
     * Finds the index of the vertex with a given content.
     */
    int findVertexIdx(const T &in) const;

};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);


/************************* Vertex  **************************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}
/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *dest, double dw, double ww) {
    auto newEdge = new Edge<T>(this, dest, dw, ww);
    adj.push_back(newEdge);
    dest->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
int Vertex<T>::getLow() const {
    return this->low;
}

template <class T>
void Vertex<T>::setLow(int value) {
    this->low = value;
}

template <class T>
int Vertex<T>::getNum() const {
    return this->num;
}

template <class T>
void Vertex<T>::setNum(int value) {
    this->num = value;
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

template <class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

//Additions to the graph -----------------------------------------------------------------
template <class T>
int Vertex<T>::getParking() {
    return this->parking;
}
template <class T>
std::string Vertex<T>::getLocation() {
    return this->location;
}
template <class T>
std::string Vertex<T>::getCode() {
    return this->code;
}

template <class T>
void Vertex<T>::setParking(int parking) {
    this->parking = parking;
}

template <class T>
void Vertex<T>::setLocation(std::string location) {
    this->location = location;
}

template <class T>
void Vertex<T>::setCode(std::string code) {
    this->code = code;
}

//Endof Additions to the graph
template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template <class T>
void Vertex<T>::setProcessing(bool processing) {
    this->processing = processing;
}

template <class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

template <class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double ww, double dw): orig(orig), dest(dest), wweight(ww), dweight(dw) {}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}
//Additions to edge class
template <class T>
double Edge<T>::getDweight() const {
    return this->dweight;
}

template <class T>
double Edge<T>::getWweight() const {
    return this->wweight;
}

template <class T>
void Edge<T>::setWweight(double ww){
    this->wweight = ww;
}

template <class T>
void Edge<T>::setDweight(double dw){
    this->dweight = dw;
}
//Endof additions to edge class

template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template <class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template <class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

template <class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}

/********************** Graph  ****************************/

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getInfo());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double dw, double ww) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, dw, ww);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double dw, double ww) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, dw, ww);
    auto e2 = v2->addEdge(v1, dw, ww);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}


template <class T>
bool relax(Edge<T> *edge, const std::string mode,  const std::unordered_set<int> &forbiddenNodes, const std::unordered_set<std::pair<int, int>, pair_hash> &forbiddenEdges) {
    if (forbiddenNodes.count(edge->getDest()->getInfo())) return false;

    int from = edge->getOrig()->getInfo();
    int to = edge->getDest()->getInfo();
    if (forbiddenEdges.count({from, to})) return false;
    
    if (mode == "driving"){
        if (edge->getDest()->getDist() > edge->getOrig()->getDist() + edge->getDweight()) {
            edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getDweight());
            edge->getDest()->setPath(edge);
            return true;
        }
    }
    else if (mode == "walking"){
        if (edge->getDest()->getDist() > edge->getOrig()->getDist() + edge->getWweight()) {
            edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getWweight());
            edge->getDest()->setPath(edge);
            return true;
        }
    }
    else {
        std::cerr << "Invalid mode: " << mode << std::endl;
        return false;
    }

    return false;
}

template <class T>


int Graph<T>::Dijkstra(Vertex<T> *src, Vertex<T> *dest, Graph<T>& g, const std::string mode, const std::unordered_set<int> &forbiddenNodes, const std::unordered_set<std::pair<int, int>, pair_hash> &forbiddenEdges) {

    for (auto v : g.getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);
        v->setVisited(false);
    }

    auto s = g.findVertex(src->getInfo());

    s->setDist(0);
    MutablePriorityQueue<Vertex<T>> q;

    q.insert(s);
    while (!q.empty()) {
        auto v = q.extractMin();
        v->setVisited(true);
        for(auto e : v->getAdj()) {
            auto oldDist = e->getDest()->getDist();

            if (relax(e,mode, forbiddenNodes, forbiddenEdges)) {
                if (oldDist == INF) {
                    q.insert(e->getDest());
                }
                else {
                    q.decreaseKey(e->getDest());
                }
            }
        }
    }

    return 0;
}



template <class T>
std::vector<T> Graph<T>::getPath(Graph<T> * g, const int &origin, const int &dest) {
    std::vector<T> res;
    auto v = g->findVertex(dest);
    if (v == nullptr || v->getDist() == INF) {
        return res;
    }
    res.push_back(v->getInfo());

    while (v->getPath() != nullptr) {
        auto edge = v->getPath();
        auto orig = edge->getOrig();
        v = orig;
        res.push_back(v->getInfo());
    }

    std::reverse(res.begin(), res.end());

    if (res.empty() || res[0] != origin) {
        std::cout << "No path was found";
    }
    return res;
}
template <class T>
std::vector<T> Graph<T>::getRestrictedPath(Graph<int>& graph, std::string const mode, Vertex<int>* src, Vertex<int>* dest, int includeNode, const std::unordered_set<int>& forbiddenNodes, const std::unordered_set<std::pair<int, int>, pair_hash>& forbiddenEdges, int& totalDist) {
    totalDist = -1;
    if (!src || !dest) return {};

    if (includeNode == -1) {
        graph.Dijkstra(src, dest, graph, mode, forbiddenNodes, forbiddenEdges);
        return graph.getPath(&graph, src->getInfo(), dest->getInfo());
    }

    // src -> includeNode -> dest
    auto mid = graph.findVertex(includeNode);
    if (!mid) return {};

    //src → include
    graph.Dijkstra(src, mid, graph, mode, forbiddenNodes, forbiddenEdges);
    auto path1 = graph.getPath(&graph, src->getInfo(), mid->getInfo());

    if (path1.empty()) return {};
    int d1 = graph.findVertex(mid->getInfo())->getDist();

    //include → dest
    graph.Dijkstra(mid, dest, graph, mode, forbiddenNodes, forbiddenEdges);
    auto path2 = graph.getPath(&graph, mid->getInfo(), dest->getInfo());

    if (path2.empty()) return {};
    int d2 = graph.findVertex(dest->getInfo())->getDist();

    totalDist = d1 + d2;

    //Path 1 + path 2;
    path1.insert(path1.end(), path2.begin() + 1, path2.end());

    return path1;
}
// End of added functions

inline void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

inline void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

template <class T>
Graph<T>::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}

#endif /* DA_TP_CLASSES_GRAPH */