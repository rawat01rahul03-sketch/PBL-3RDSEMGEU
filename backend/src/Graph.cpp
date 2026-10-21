#include "Graph.h"

#include <algorithm>

// ==========================================
// MODULE 1: GRID DATA MANAGEMENT
// ==========================================

// Add a new substation
bool Graph::addSubstation(const Substation& s) {
if (s.id < 0 || s.name.empty() || s.loadDemand < 0) {
return false;
}

if (indexById.count(s.id) > 0) {
    return false;
}

indexById[s.id] = static_cast<int>(substations.size());

substations.push_back(s);

adjacency[s.id] = {};

return true;

}

// Calculate construction cost
double Graph::calculateConstructionCost(
double distance,
double terrainFactor
) const {
const double BASE_COST_PER_KM = 10.0;

return distance * terrainFactor * BASE_COST_PER_KM;

}

// Add a new transmission line
bool Graph::addLine(const TransmissionLine& inputLine) {
if (inputLine.id < 0 ||
inputLine.from == inputLine.to ||
indexById.count(inputLine.from) == 0 ||
indexById.count(inputLine.to) == 0 ||
inputLine.distance <= 0 ||
inputLine.terrainFactor <= 0 ||
inputLine.capacity <= 0) {
return false;
}

for (const auto& existing : lines) {
    if (existing.id == inputLine.id) {
        return false;
    }

    // Reject duplicate connections in either direction.
    if ((existing.from == inputLine.from &&
         existing.to == inputLine.to) ||
        (existing.from == inputLine.to &&
         existing.to == inputLine.from)) {
        return false;
    }
}

TransmissionLine line = inputLine;

line.constructionCost = calculateConstructionCost(
    line.distance,
    line.terrainFactor
);

lines.push_back(line);

// Store the connection in both directions.
adjacency[line.from].push_back({line.to, line.id});
adjacency[line.to].push_back({line.from, line.id});

return true;

}

// Return the number of substations
int Graph::vertexCount() const {
return static_cast<int>(substations.size());
}

// Return the number of transmission lines
int Graph::edgeCount() const {
return static_cast<int>(lines.size());
}

// Return all substations
const std::vector<Substation>& Graph::getSubstations() const {
return substations;
}

// Return all transmission lines
const std::vector<TransmissionLine>& Graph::getLines() const {
return lines;
}

// Return the adjacency list
const std::unordered_map<int, std::vector<std::pair<int, int>>>&
Graph::getAdjacencyList() const {
return adjacency;
}

// ==========================================
// MODULE 2: KRUSKAL'S ALGORITHM
// ==========================================

std::vector<TransmissionLine> Graph::kruskalMST() const {

// Copy the transmission lines.
std::vector<TransmissionLine> sortedLines = lines;

// Sort lines by construction cost using simple loops.
for (int i = 0; i < static_cast<int>(sortedLines.size()); i++) {

    for (int j = i + 1;
         j < static_cast<int>(sortedLines.size()); j++) {

        if (sortedLines[i].constructionCost >
            sortedLines[j].constructionCost) {

            TransmissionLine temp = sortedLines[i];

            sortedLines[i] = sortedLines[j];

            sortedLines[j] = temp;
        }
    }
}

// Store the selected transmission lines.
std::vector<TransmissionLine> result;

// Initially, every substation belongs to its own group.
std::unordered_map<int, int> parent;

for (const auto& s : substations) {
    parent[s.id] = s.id;
}

// Find the group representative without recursion.
auto findParent = [&](int x) {

    while (parent[x] != x) {
        x = parent[x];
    }

    return x;
};

// Examine lines from lowest cost to highest cost.
for (const auto& line : sortedLines) {

    int parent1 = findParent(line.from);

    int parent2 = findParent(line.to);

    // Different groups mean this line will not create a cycle.
    if (parent1 != parent2) {

        result.push_back(line);

        // Join the two groups.
        parent[parent1] = parent2;
    }

    // A spanning tree requires V - 1 lines.
    if (result.size() ==
        static_cast<int>(substations.size()) - 1) {

        break;
    }
}

return result;

}

// ==========================================
// MODULE 2: PRIM'S ALGORITHM
// ==========================================

std::vector<TransmissionLine> Graph::primMST() const {

std::vector<TransmissionLine> result;

// Handle an empty graph.
if (substations.empty()) {
    return result;
}

// Track which substations have been visited.
std::unordered_map<int, bool> visited;

for (const auto& s : substations) {
    visited[s.id] = false;
}

// Start from the first substation.
int start = substations[0].id;

visited[start] = true;

// Continue until V - 1 lines have been selected.
while (result.size() <
       static_cast<int>(substations.size()) - 1) {

    double minimumCost = 1e18;

    int selectedIndex = -1;

    // Find the cheapest line connecting the visited
    // group to an unvisited substation.
    for (int i = 0; i < static_cast<int>(lines.size()); i++) {

        int u = lines[i].from;

        int v = lines[i].to;

        // First direction: visited to unvisited.
        if (visited[u] && !visited[v]) {

            if (lines[i].constructionCost < minimumCost) {

                minimumCost = lines[i].constructionCost;

                selectedIndex = i;
            }
        }

        // Second direction: unvisited to visited.
        else if (visited[v] && !visited[u]) {

            if (lines[i].constructionCost < minimumCost) {

                minimumCost = lines[i].constructionCost;

                selectedIndex = i;
            }
        }
    }

    // No available line means the graph is disconnected.
    if (selectedIndex == -1) {
        break;
    }

    // Add the cheapest line.
    result.push_back(lines[selectedIndex]);

    // Mark both endpoints as visited.
    visited[lines[selectedIndex].from] = true;

    visited[lines[selectedIndex].to] = true;
}

return result;

}