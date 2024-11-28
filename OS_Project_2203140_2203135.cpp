#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm> // For std::remove

using namespace std;

// Graph structure for Resource Allocation Graph
class ResourceAllocationGraph {
private:
    unordered_map<string, vector<string>> adjacencyList;

    // Helper function to detect cycles using DFS
    bool detectCycleUtil(const string& node, unordered_set<string>& visited, unordered_set<string>& recStack) {
        if (!visited.count(node)) {
            visited.insert(node);
            recStack.insert(node);

            // Recur for all neighbors
            for (const auto& neighbor : adjacencyList[node]) {
                if (!visited.count(neighbor) && detectCycleUtil(neighbor, visited, recStack))
                    return true;
                else if (recStack.count(neighbor))
                    return true;
            }
        }
        recStack.erase(node);
        return false;
    }

public:
    // Add a process or resource to the graph
    void addNode(const string& node) {
        if (!adjacencyList.count(node)) {
            adjacencyList[node] = {};
        }
    }

    // Add an edge: Process requests Resource or Resource is allocated to Process
    void addEdge(const string& from, const string& to) {
        if (adjacencyList.count(from) && adjacencyList.count(to)) {
            adjacencyList[from].push_back(to);
        } else {
            cout << "Error: Node(s) " << from << " or " << to << " not found in the graph." << endl;
        }
    }

    // Remove an edge: Process releases Resource
    void removeEdge(const string& from, const string& to) {
        if (adjacencyList.count(from)) {
            auto& neighbors = adjacencyList[from];
            auto it = remove(neighbors.begin(), neighbors.end(), to);
            if (it != neighbors.end()) {
                neighbors.erase(it, neighbors.end());
            }
        } else {
            cout << "Error: Node " << from << " not found in the graph." << endl;
        }
    }

    // Detect deadlock by checking for cycles
    bool detectDeadlock() {
        unordered_set<string> visited;
        unordered_set<string> recStack;

        for (const auto& node : adjacencyList) {
            if (detectCycleUtil(node.first, visited, recStack))
                return true;
        }
        return false;
    }

    // Print the graph
    void printGraph() {
        cout << "Resource Allocation Graph:" << endl;
        for (const auto& pair : adjacencyList) {
            cout << pair.first << " -> ";
            for (const auto& neighbor : pair.second) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    ResourceAllocationGraph rag;

    // Example usage
    rag.addNode("P1"); // Process 1
    rag.addNode("P2"); // Process 2
    rag.addNode("R1"); // Resource 1
    rag.addNode("R2"); // Resource 2

    // Simulating resource requests and allocations
    rag.addEdge("P1", "R1"); // P1 requests R1
    rag.addEdge("R1", "P2"); // R1 allocated to P2
    rag.addEdge("P2", "R2"); // P2 requests R2
    rag.addEdge("R2", "P1"); // R2 allocated to P1 (Cycle - Deadlock)

    rag.printGraph();

    // Check for deadlocks
    if (rag.detectDeadlock()) {
        cout << "Deadlock detected in the system!" << endl;
    } else {
        cout << "No deadlock detected in the system." << endl;
    }

    // Simulate releasing resources
    rag.removeEdge("R2", "P1"); // R2 released by P1

    cout << "\nAfter releasing resource R2 from P1:" << endl;
    rag.printGraph();

    // Recheck for deadlocks
    if (rag.detectDeadlock()) {
        cout << "Deadlock detected in the system!" << endl;
    } else {
        cout << "No deadlock detected in the system." << endl;
    }

    return 0;
}
