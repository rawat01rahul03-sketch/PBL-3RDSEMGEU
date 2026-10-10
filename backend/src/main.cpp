
#include <iostream>
#include "Graph.h"

int main() {
    Graph grid;

    std::cout << "=== GridGuard: Module 1 ===\n\n";

    std::cout << "--- Adding Substations ---\n";
    std::cout << "North: "
              << (grid.addSubstation({1, "North", 100})
                      ? "Added" : "Rejected") << '\n';

    std::cout << "Central: "
              << (grid.addSubstation({2, "Central", 150})
                      ? "Added" : "Rejected") << '\n';

    std::cout << "South: "
              << (grid.addSubstation({3, "South", 120})
                      ? "Added" : "Rejected") << '\n';

    std::cout << "Duplicate ID: "
              << (grid.addSubstation({1, "Duplicate", 50})
                      ? "Unexpectedly added" : "Correctly rejected")
              << "\n\n";

    std::cout << "--- Adding Transmission Lines ---\n";

    grid.addLine({1, 1, 2, 10.0, 1.2, 100.0, 0.0});
    grid.addLine({2, 2, 3, 15.0, 1.0, 120.0, 0.0});
    grid.addLine({3, 1, 3, 20.0, 1.5, 80.0, 0.0});

    std::cout << "Substations: " << grid.vertexCount() << '\n';
    std::cout << "Transmission lines: " << grid.edgeCount() << "\n\n";

    std::cout << "--- Construction Cost Report ---\n";

    for (const auto& line : grid.getLines()) {
        std::cout << line.from << " - " << line.to
                  << " | Cost: " << line.constructionCost << '\n';
    }

    std::cout << "\n--- Adjacency List ---\n";

    for (const auto& entry : grid.getAdjacencyList()) {
        std::cout << "Substation " << entry.first << ": ";

        for (const auto& neighbor : entry.second) {
            std::cout << neighbor.first
                      << " (line " << neighbor.second << ") ";
        }

        std::cout << '\n';
    }

    std::cout << "\n--- Duplicate Line Test ---\n";

    bool duplicateLine = grid.addLine(
        {4, 2, 1, 12.0, 1.0, 90.0, 0.0}
    );

    std::cout << (duplicateLine
                      ? "FAIL: Duplicate line accepted"
                      : "PASS: Duplicate line rejected")
              << '\n';

    return 0;
}
