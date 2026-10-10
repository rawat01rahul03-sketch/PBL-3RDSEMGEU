
#include "Graph.h"

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

double Graph::calculateConstructionCost(
    double distance,
    double terrainFactor
) const {
    const double BASE_COST_PER_KM = 10.0;
    return distance * terrainFactor * BASE_COST_PER_KM;
}

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

        // Reject the same undirected connection in either direction.
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

    adjacency[line.from].push_back({line.to, line.id});
    adjacency[line.to].push_back({line.from, line.id});

    return true;
}

int Graph::vertexCount() const {
    return static_cast<int>(substations.size());
}

int Graph::edgeCount() const {
    return static_cast<int>(lines.size());
}

const std::vector<Substation>& Graph::getSubstations() const {
    return substations;
}

const std::vector<TransmissionLine>& Graph::getLines() const {
    return lines;
}

const std::unordered_map<int, std::vector<std::pair<int, int>>>&
Graph::getAdjacencyList() const {
    return adjacency;
}
