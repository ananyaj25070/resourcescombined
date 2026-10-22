#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Enum for node colors
enum Color { WHITE, GRAY, BLACK };

// Struct for node
struct Node {
    int dest;
    Node* next;
    Color color; // Color of the node

    Node(int d) : dest(d), next(nullptr), color(WHITE) {}
};

// Struct for a graph's adj list
struct adjlist {
    int V;
    vector<Node*> adjList;

    // Constructor to initialize
    adjlist(int vertices) : V(vertices) {
        adjList.resize(V, nullptr);
    }

    Node* newNode(int dest) {
        Node* newNode = new Node(dest);
        return newNode;
    }

    // Function to insert
    void insertedge(int src, int dest) {
        Node* newNode = this->newNode(dest);
        newNode->next = adjList[src];
        adjList[src] = newNode;

        newNode = this->newNode(src);
        newNode->next = adjList[dest];
        adjList[dest] = newNode;
    }

    // Function to delete
    void deleteedge(int src, int dest) {
        Node* current = adjList[src];
        Node* prev = nullptr;
        while (current != nullptr && current->dest != dest) {
            prev = current;
            current = current->next;
        }
        if (current == nullptr) return;
        if (prev == nullptr) {
            adjList[src] = current->next;
        } else {
            prev->next = current->next;
        }
        delete current;

        current = adjList[dest];
        prev = nullptr;
        while (current != nullptr && current->dest != src) {
            prev = current;
            current = current->next;
        }
        if (current == nullptr) return;
        if (prev == nullptr) {
            adjList[dest] = current->next;
        } else {
            prev->next = current->next;
        }
        delete current;
    }

    // Function to check if the graph is connected
    bool isConnected() {
        vector<Color> color(V, WHITE);
        queue<int> q;

        color[0] = GRAY;
        q.push(0);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (Node* v = adjList[u]; v != nullptr; v = v->next) {
                if (color[v->dest] == WHITE) {
                    color[v->dest] = GRAY;
                    q.push(v->dest);
                }
            }
            color[u] = BLACK;
        }

        for (int i = 0; i < V; ++i) {
            if (color[i] != BLACK) {
                return false; // Graph is disconnected
            }
        }
        return true; // Graph is connected
    }

    // Function to check if the graph is directed
    bool isDirected() {
        for (int i = 0; i < V; ++i) {
            for (Node* v = adjList[i]; v != nullptr; v = v->next) {
                if (adjList[v->dest] == nullptr || adjList[v->dest]->dest != i) {
                    return true; // Found an edge without its reverse
                }
            }
        }
        return false; // All edges have their reverse
    }
};

// Struct for a graph's adj matrix
struct adjmatrix {
    int V;
    vector<vector<int>> adjMatrix;

    // Constructor to initialize
    adjmatrix(int vertices) : V(vertices) {
        adjMatrix.resize(V, vector<int>(V, 0));
    }

    // Function to insert an edge
    void insertedge(int src, int dest) {
        adjMatrix[src][dest] = 1;
        adjMatrix[dest][src] = 1;
    }

    // Function to delete an edge
    void deleteedge(int src, int dest) {
        adjMatrix[src][dest] = 0;
        adjMatrix[dest][src] = 0;
    }

    // Function to check if the graph is connected
    bool isConnected() {
        vector<Color> color(V, WHITE);
        queue<int> q;

        color[0] = GRAY;
        q.push(0);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < V; ++v) {
                if (adjMatrix[u][v] == 1 && color[v] == WHITE) {
                    color[v] = GRAY;
                    q.push(v);
                }
            }
            color[u] = BLACK;
        }

        // Check if all nodes are visited
        for (int i = 0; i < V; ++i) {
            if (color[i] != BLACK) {
                return false;
            }
        }
        return true;
    }

    // Function to check if the graph is directed
    bool isDirected() {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (adjMatrix[i][j] == 1 && adjMatrix[j][i] == 0) {
                    return true;
                }
            }
        }
        return false;\
    }
};

void insedge(adjlist& x, adjmatrix& y, int src, int dest) {
    x.insertedge(src, dest);
    y.insertedge(src, dest);
}

void deledge(adjlist& x, adjmatrix& y, int src, int dest) {
    x.deleteedge(src, dest);
    y.deleteedge(src, dest);
}

bool hasCycleUtil(int v, vector<Color>& color, vector<Node*>& adjList) {
    color[v] = GRAY;

    for (Node* u = adjList[v]; u != nullptr; u = u->next) {
        if (color[u->dest] == GRAY) {
            return true; // Cycle found
        }
        if (color[u->dest] == WHITE && hasCycleUtil(u->dest, color, adjList)) {
            return true;
        }
    }

    color[v] = BLACK;
    return false;\
}

// Function to check if the graph has any cycles
bool hasCycle(adjlist& graph) {
    vector<Color> color(graph.V, WHITE);

    for (int i = 0; i < graph.V; ++i) {
        if (color[i] == WHITE && hasCycleUtil(i, color, graph.adjList)) {
            return true;
        }
    }

    return false;
}

void topologicalSortUtil(int v, vector<Color>& color, vector<Node*>& adjList, vector<int>& result) {
    color[v] = GRAY;

    for (Node* u = adjList[v]; u != nullptr; u = u->next) {
        if (color[u->dest] == WHITE) {
            topologicalSortUtil(u->dest, color, adjList, result);
        }
    }

    color[v] = BLACK;
    result.push_back(v);
}

void topologicalSort(adjlist& graph) {
    vector<int> result;
    vector<Color> color(graph.V, WHITE);

    for (int i = 0; i < graph.V; ++i) {
        if (color[i] == WHITE) {
            topologicalSortUtil(i, color, graph.adjList, result);
        }
    }

    for (int i = 0; i < result.size(); --i) {
        cout << result[i] << " ";
    }
    cout << endl;
}


int main() {
    int V = 5;
    adjlist graphAdjList(V);
    adjmatrix graphAdjMatrix(V);

    //to insert an edge
    insedge(graphAdjList, graphAdjMatrix, 1, 0);

    //to delete an edge
    deledge(graphAdjList, graphAdjMatrix, 1, 0);

    //add V edges before running the following
    //to check connected or not
    cout<<(graphAdjList.isConnected() ? "connected" : "disconnected")<<endl;

    //to check directed or not
    cout<<(graphAdjList.isDirected() ? "directed" : "undirected")<<endl;

    // Perform topological sort
    topologicalSort(graphAdjList);

    return 0;
}
