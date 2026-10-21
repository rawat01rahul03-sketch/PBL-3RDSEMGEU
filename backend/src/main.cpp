#include <iostream>
#include <iomanip>
#include "Graph.h"

int main() {

Graph grid;

std::cout << std::fixed << std::setprecision(2);

std::cout << "====================================\n";
std::cout << "          GRIDGUARD PROJECT\n";
std::cout << "====================================\n";

// ==========================================
// MODULE 1: GRID DATA MANAGEMENT
// ==========================================

std::cout << "\n--- Adding Substations ---\n";

std::cout << "North: "
          << (grid.addSubstation({1, "North", 100})
                  ? "Added" : "Rejected") << '\n';

std::cout << "Central: "
          << (grid.addSubstation({2, "Central", 150})
                  ? "Added" : "Rejected") << '\n';

std::cout << "South: "
          << (grid.addSubstation({3, "South", 120})
                  ? "Added" : "Rejected") << '\n';

std::cout << "West: "
          << (grid.addSubstation({4, "West", 90})
                  ? "Added" : "Rejected") << '\n';

std::cout << "Duplicate ID: "
          << (grid.addSubstation({1, "Duplicate", 50})
                  ? "Unexpectedly added" : "Correctly rejected")
          << '\n';


std::cout << "\n--- Adding Transmission Lines ---\n";

grid.addLine({1, 1, 2, 10.0, 1.2, 100.0, 0.0});

grid.addLine({2, 2, 3, 15.0, 1.0, 120.0, 0.0});

grid.addLine({3, 1, 3, 20.0, 1.5, 80.0, 0.0});

grid.addLine({4, 2, 4, 8.0, 1.0, 90.0, 0.0});

grid.addLine({5, 3, 4, 12.0, 1.0, 100.0, 0.0});


std::cout << "Substations: "
          << grid.vertexCount() << '\n';

std::cout << "Transmission lines: "
          << grid.edgeCount() << '\n';


std::cout << "\n--- Construction Cost Report ---\n";

for (const auto& line : grid.getLines()) {

    std::cout << line.from << " - " << line.to
              << " | Cost: "
              << line.constructionCost << '\n';
}


std::cout << "\n--- Adjacency List ---\n";

for (const auto& entry : grid.getAdjacencyList()) {

    std::cout << "Substation "
              << entry.first << ": ";

    for (const auto& neighbor : entry.second) {

        std::cout << neighbor.first
                  << " (line "
                  << neighbor.second << ") ";
    }

    std::cout << '\n';
}


// ==========================================
// MODULE 2: MINIMUM-COST OPTIMIZATION
// ==========================================

std::cout << "\n====================================\n";
std::cout << "     MODULE 2: MST OPTIMIZATION\n";
std::cout << "====================================\n";


// Run Kruskal's algorithm.
std::vector<TransmissionLine> kruskalResult =
    grid.kruskalMST();

double kruskalCost = 0;

std::cout << "\n--- Kruskal's Algorithm ---\n";

for (const auto& line : kruskalResult) {

    std::cout << line.from << " - " << line.to
              << " | Cost: "
              << line.constructionCost << '\n';

    kruskalCost += line.constructionCost;
}

std::cout << "Selected lines: "
          << kruskalResult.size() << '\n';

std::cout << "Total MST cost: "
          << kruskalCost << '\n';


// Run Prim's algorithm.
std::vector<TransmissionLine> primResult =
    grid.primMST();

double primCost = 0;

std::cout << "\n--- Prim's Algorithm ---\n";

for (const auto& line : primResult) {

    std::cout << line.from << " - " << line.to
              << " | Cost: "
              << line.constructionCost << '\n';

    primCost += line.constructionCost;
}

std::cout << "Selected lines: "
          << primResult.size() << '\n';

std::cout << "Total MST cost: "
          << primCost << '\n';


// Compare the results.
if (kruskalResult.size() == grid.vertexCount() - 1 &&
    primResult.size() == grid.vertexCount() - 1) {

    if (kruskalCost == primCost) {

        std::cout << "\nBoth algorithms found the same minimum cost.\n";
    }
    else {

        std::cout << "\nThe costs are different. Check the implementation.\n";
    }
}
else {

    std::cout << "\nThe graph is disconnected; a complete MST was not found.\n";
}

return 0;

}