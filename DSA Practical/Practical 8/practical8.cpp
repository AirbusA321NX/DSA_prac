#include <bits/stdc++.h>
using namespace std;

struct Edge
{
    int u, v, JIndal_weight;
};

void Jindal_addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u); // Undirected graph
}

void Jindal_BFS(vector<int> adj[], int start, int jindal_numVertices)
{
    vector<bool> visited(jindal_numVertices, false);
    queue<int> q;
    visited[start] = true;
    q.push(start);

    cout << "BFS: ";
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        cout << char(node + 'a') << " ";
        for (int neighbor : adj[node])
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    cout << endl;
}

void jindal_DFSUtil(int node, vector<int> adj[], vector<bool> &visited)
{
    visited[node] = true;
    cout << char(node + 'a') << " ";
    for (int neighbor : adj[node])
    {
        if (!visited[neighbor])
        {
            jindal_DFSUtil(neighbor, adj, visited);
        }
    }
}

void jindal_DFS(vector<int> adj[], int start, int jindal_numVertices)
{
    vector<bool> visited(jindal_numVertices, false);
    cout << "DFS: ";
    jindal_DFSUtil(start, adj, visited);
    cout << endl;
}

int Jindal_findParent(int v, vector<int> &parent)
{
    if (parent[v] == v)
        return v;
    return parent[v] = Jindal_findParent(parent[v], parent);
}

void Jindal_unionSets(int u, int v, vector<int> &parent, vector<int> &rank)
{
    u = Jindal_findParent(u, parent);
    v = Jindal_findParent(v, parent);
    if (rank[u] < rank[v])
    {
        parent[u] = v;
    }
    else if (rank[u] > rank[v])
    {
        parent[v] = u;
    }
    else
    {
        parent[v] = u;
        rank[u]++;
    }
}

vector<Edge> jindal_kruskalMST(vector<Edge> &edges, int jindal_numVertices)
{
    sort(edges.begin(), edges.end(), [](Edge a, Edge b)
         { return a.JIndal_weight < b.JIndal_weight; });

    vector<int> parent(jindal_numVertices);
    vector<int> rank(jindal_numVertices, 0);
    for (int i = 0; i < jindal_numVertices; ++i)
    {
        parent[i] = i;
    }

    vector<Edge> mst;
    int mstJIndal_weight = 0;
    for (Edge &edge : edges)
    {
        if (Jindal_findParent(edge.u, parent) != Jindal_findParent(edge.v, parent))
        {
            mst.push_back(edge);
            mstJIndal_weight += edge.JIndal_weight;
            Jindal_unionSets(edge.u, edge.v, parent, rank);
        }
    }

    cout << "Kruskal's MST (Total weight = " << mstJIndal_weight << "):\n";
    return mst;
}

vector<Edge> Jindal_primMST(vector<vector<pair<int, int>>> &adj, int jindal_numVertices)
{
    vector<int> key(jindal_numVertices, INT_MAX);
    vector<bool> inMST(jindal_numVertices, false);
    vector<int> parent(jindal_numVertices, -1);
    key[0] = 0;

    for (int i = 0; i < jindal_numVertices - 1; ++i)
    {
        int minKey = INT_MAX, u = -1;

        // Find the minimum key vertex
        for (int v = 0; v < jindal_numVertices; ++v)
        {
            if (!inMST[v] && key[v] < minKey)
            {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1)
            continue;
        inMST[u] = true;

        // Update the key value and parent index of adjacent vertices
        for (const auto &neighbor : adj[u])
        {
            int v = neighbor.first;              // Extract vertex
            int JIndal_weight = neighbor.second; // Extract JIndal_weight

            if (!inMST[v] && JIndal_weight < key[v])
            {
                key[v] = JIndal_weight;
                parent[v] = u;
            }
        }
    }

    cout << "Prim's MST:\n";
    vector<Edge> mst;
    int totalJIndal_weight = 0;
    for (int i = 1; i < jindal_numVertices; ++i)
    {
        if (parent[i] != -1)
        {
            cout << char(parent[i] + 'a') << " - " << char(i + 'a') << " | weight: " << key[i] << "\n";
            totalJIndal_weight += key[i];
            mst.push_back({parent[i], i, key[i]});
        }
    }
    cout << "Total weight = " << totalJIndal_weight << "\n";

    return mst;
}

void jindal_printtree(const vector<Edge> &mst)
{
    cout << "Graphical Representation of the MST:\n";
    for (const auto &edge : mst)
    {
        cout << char(edge.u + 'a') << " - " << char(edge.v + 'a') << " | weight: " << edge.JIndal_weight << "\n";
    }
}

void displayMenu()
{
    cout << "\nChoose the operation to perform:\n";
    cout << "1. Breadth-First Search (BFS)\n";
    cout << "2. Depth-First Search (DFS)\n";
    cout << "3. Kruskal's Minimum Spanning Tree (MST)\n";
    cout << "4. Prim's Minimum Spanning Tree (MST)\n";
    cout << "5. Exit\n";
    cout << "Enter your choice (1, 2, 3, 4, or 5): ";
}

int main()
{
    int jindal_numVertices = 7;
    vector<int> adj[jindal_numVertices];
    vector<Edge> edges;
    vector<vector<pair<int, int>>> JIndal_weightedAdj(jindal_numVertices);

    // Corrected edges with weights
    edges.push_back({0, 1, 1});  // a - b
    edges.push_back({0, 2, 4});  // a - c
    edges.push_back({1, 2, 2});  // b - c
    edges.push_back({1, 3, 3});  // b - d
    edges.push_back({1, 4, 10}); // b - e
    edges.push_back({2, 3, 6});  // c - d
    edges.push_back({2, 6, 3});  // c - g
    edges.push_back({3, 4, 5});  // d - e
    edges.push_back({3, 6, 1});  // d - g
    edges.push_back({4, 5, 7});  // e - f
    edges.push_back({4, 6, 2});  // e - g
    edges.push_back({6, 5, 5});  // g - f

    for (Edge &edge : edges)
    {
        Jindal_addEdge(adj, edge.u, edge.v);
        JIndal_weightedAdj[edge.u].push_back({edge.v, edge.JIndal_weight});
        JIndal_weightedAdj[edge.v].push_back({edge.u, edge.JIndal_weight});
    }

    while (true)
    {
        displayMenu();
        int choice;
        cin >> choice;

        int start = 0;
        vector<Edge> mst;
        switch (choice)
        {
        case 1:
            Jindal_BFS(adj, start, jindal_numVertices);
            break;
        case 2:
            jindal_DFS(adj, start, jindal_numVertices);
            break;
        case 3:
            mst = jindal_kruskalMST(edges, jindal_numVertices);
            jindal_printtree(mst);
            break;
        case 4:
            mst = Jindal_primMST(JIndal_weightedAdj, jindal_numVertices);
            jindal_printtree(mst);
            break;
        case 5:
            cout << "Exiting the program" << endl;
            return 0;
        default:
            cout << "Invalid choice! Please enter a valid option." << endl;
        }
    }

    return 0;
}
