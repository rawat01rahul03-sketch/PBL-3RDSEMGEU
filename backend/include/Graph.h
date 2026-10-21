#ifndef GRIDGUARD_GRAPH_H
#define GRIDGUARD_GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

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

// Maps substation ID to its vector index.
std::unordered_map<int, int> indexById;

// Stores neighboring substations and line IDs.
std::unordered_map<int, std::vector<std::pair<int, int>>> adjacency;

public:
// Module 1: Grid Data Management
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

// Module 2: Minimum Spanning Tree
std::vector<TransmissionLine> kruskalMST() const;
std::vector<TransmissionLine> primMST() const;

};

#endif