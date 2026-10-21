
#ifndef GRIDGUARD_GRAPH_H
#define GRIDGUARD_GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

// STORES THE DETAILS OF A POWER GRID SUBSTATIONS

struct Substation {
    int id;
    std::string name;
    double loadDemand;
};

struct TransmissionLine {
    int id;
    int from;
    int to;
    double distance;
    double terrainFactor;
    double capacity;
    double constructionCost;
};

class Graph {
private:
    std::vector<Substation> substations;
    std::vector<TransmissionLine> lines;

    // Maps a substation ID to its internal vector index.
    std::unordered_map<int, int> indexById;

    // Adjacency list: each entry stores (neighbor ID, line ID).
    std::unordered_map<int, std::vector<std::pair<int, int>>> adjacency;

public:
    bool addSubstation(const Substation& substation);
    bool addLine(const TransmissionLine& line);

    double calculateConstructionCost(
        double distance,
        double terrainFactor
    ) const;

    int vertexCount() const;
    int edgeCount() const;

    const std::vector<Substation>& getSubstations() const;
    const std::vector<TransmissionLine>& getLines() const;

    const std::unordered_map<int, std::vector<std::pair<int, int>>>&
    getAdjacencyList() const;
};

#endif
