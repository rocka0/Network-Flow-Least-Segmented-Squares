/**
 * @file ford-fulkerson.cpp
 * @brief Implements Ford Fulkerson algorithm, and uses it to find the Bipartite Matching of a graph.
*/

#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Graph class
*/
class Graph {
public:

    /**
     * The number of vertices
    */
    int n;

    /**
     * Default constructor
    */
    Graph();
    
    /**
     * Parameterized constructor
     * @param n Number of vertices
    */
    Graph(int n);

    /**
     * Ordered map implementation of adjacency list
    */
    map<int, set<int>> adj;

    /**
     * Ordered map of pairs storing edge weights
    */
    map<pair<int, int>, int> cap;

    /**
     * @brief Function to read edges from a file
     * @param filename Name of the file
     * @param directed True if the input graph is directed. False otherwise. 
     * @param weighted True if the input graph is weighted. False otherwise. 
     * @param oneBased True if the nodes of the input graph follows one-based indexing. False if it follows zero-based indexing. 
    
    */
    void readEdge(string filename, bool directed, bool weighted, bool oneBased);
    
    /**
     * Function to add a directed edge to the graph
     * @param x source node of edge
     * @param y destination node of edge
     * @param c capacity of edge
    */
    void addDirectedEdge(int x, int y, int c);
    
     /**
     * Function to add a undirected edge to the graph
     * @param x first node of edge
     * @param y second node of edge
     * @param c capacity of edge
    */
    void addUndirectedEdge(int x, int y, int c);

    /**
     * @brief Find a simple path from source to destination
     * @param start source node
     * @param end destination node
     * @param path A reference to the currently building path
     * @param seen A reference to seen vector, storing which nodes have been visited so far.
    */
    void getPath(int start, int end, vector<int>& path, vector<int>& seen);
    
    /**
     * @brief Run depth first search from input node.
     * @param node Source node 
     * @param seen A reference to seen vector, storing which nodes have been visited so far.
    */
    void dfs(int node, vector<int>& seen);
    
    /**
     * @param x Source node
     * @param y Desitination node
     * @return Weight of edge connecting nodes x and y.
    */
    int getCapacity(int x, int y);
};

Graph::Graph() {
    n = 0;
}

Graph::Graph(int n) {
    this->n = n;
}

int Graph::getCapacity(int x, int y) {
    if (adj[x].find(y) == adj[x].end()) return 0;

    return cap[{x, y}];
}

void Graph::addDirectedEdge(int x, int y, int c) {
    if (adj[x].find(y) == adj[x].end()) {
        adj[x].insert(y);
        cap[{x, y}] = c;
    } else
        cap[{x, y}] += c;
}

void Graph::addUndirectedEdge(int x, int y, int c) {
    addDirectedEdge(x, y, c);
    addDirectedEdge(y, x, c);
}

void Graph::readEdge(string filename, bool directed, bool weighted, bool oneBased) {
    ifstream cin;
    cin.open(filename);
    cin >> n;

    int m;
    cin >> m;

    while (m--) {
        int x, y, c;
        cin >> x >> y;

        if (weighted)
            cin >> c;
        else
            c = 1;

        if (oneBased) {
            x--;
            y--;
        }

        if (directed)
            addDirectedEdge(x, y, c);
        else
            addUndirectedEdge(x, y, c);
    }
}

void Graph::getPath(int start, int end, vector<int>& path, vector<int>& seen)
{
    if (seen[start])
        return;

    seen[start] = 1;

    if (start == end) {
        path.push_back(end);
        return;
    }

    for (auto p : adj[start]) {
        getPath(p, end, path, seen);

        if (!path.empty()) {
            path.push_back(start);
            return;
        }
    }
}

void Graph::dfs(int node, vector<int>& seen) {
    assert(0 <= node && node < n);

    if (seen[node]) return;

    seen[node] = 1;

    for (auto p : adj[node]) dfs(p, seen);
}


/**
 * Flow class
*/
class Flow {
private:

    /**
     * The source node of graph
    */
    int s;

    /**
     * Sink node of graph
    */
    int t;
    
    /**
     * Graph on which flow is to be computed.
    */
    Graph g;
    
    /**
     * Ordered map of pairs representing flow at each edge
    */
    map<pair<int, int>, int> flow;
    
    /**
     * Implements the Ford Fulkerson algorithm to find max flow in the given graph.  
     * @return The maximum flow in the graph as an ordered map of pairs.
    */
    map<pair<int, int>, int> fordFulkerson();
    
    /**
     * Builds the residual graph from the original graph and current flows.
     * @return The residual graph
    */
    Graph buildResidualGraph();
    
    /**
     * Checks if the edge from x to y is a forward edge in the residual graph or not.
     * @param x The source node of edge
     * @param y The destination node of edge
     * @return True if the edge is a forward edge, False otherwise. 
    */
    bool isForward(int x, int y);
    
    /**
     * @param x The source node of edge
     * @param y The destination node of edge
     * @return The current flow along the edge from x to y  
    */
    int getFlow(int x, int y);
    
    /**
     * Updates the flow along the edge from x to y with the given value.
     * @param x The source node of edge
     * @param y The destination node of edge
     * @param f The new flow value along the edge from x to y
    */
    void updateFlow(int x, int y, int f);
    
    /**
     * Updates the flow for all the edges in the augmenting path 
     * @param path A vector containing the augmenting path (in reverse order of nodes)
     * @param res The residual graph
    */
    void augment(vector<int> path, Graph res);

public:

    /**
     * Parameterized constructor for Flow class
     * @param g The original graph
     * @param s The source node 
     * @param t The sink node
    */
    Flow(Graph g, int s, int t);

    /**
     * Function to compute max flow. Internally uses the Ford Fulkerson algorithm for the same.
     * @return An ordered map of pairs having the maximum flow in the graph.
    */
    map<pair<int, int>, int> maxFlow();
    
    /**
     * Computes the s-t cut for the given source and sink nodes.
     * @return A vector containing 1's for all the nodes in the same cut as source s, and 0's for all the nodes in the same cut as sink node t.
    */
    vector<int> stCut();
};

Flow::Flow(Graph g, int s, int t) {
    this->s = s;
    this->t = t;
    this->g = g;

    for (int i = 0; i < g.n; i++) {
        for (auto p : g.adj[i]) flow[{i, p}] = 0;
    }
}

bool Flow::isForward(int x, int y) {
    if (g.adj[x].find(y) == g.adj[x].end()) return false;

    return true;
}

int Flow::getFlow(int x, int y) {
    if (g.adj[x].find(y) == g.adj[x].end()) return 0;

    return flow[{x, y}];
}

void Flow::updateFlow(int x, int y, int f) {
    flow[{x, y}] = f;
}

Graph Flow::buildResidualGraph() {
    Graph res(g.n);

    for (int i = 0; i < g.n; i++) {
        for (auto j : g.adj[i]) {
            int c = g.getCapacity(i, j);
            int f = getFlow(i, j);

            if (f < c) res.addDirectedEdge(i, j, c - f);

            if (f > 0) res.addDirectedEdge(j, i, f);
        }
    }

    return res;
}

void Flow::augment(vector<int> path, Graph res) {
    int b = INT_MAX;

    for (int i = 0; i < path.size() - 1; i++) b = min(b, res.getCapacity(path[i + 1], path[i]));

    for (int i = 0; i < path.size() - 1; i++) {
        int x = path[i + 1];
        int y = path[i];

        if (isForward(x, y)) {
            int f = getFlow(x, y);
            updateFlow(x, y, f + b);
        } else {
            int f = getFlow(y, x);
            updateFlow(y, x, f - b);
        }
    }
}

map<pair<int, int>, int> Flow::fordFulkerson() {
    Graph res = buildResidualGraph();
    vector<int> path;
    vector<int> seen(g.n, 0);
    res.getPath(s, t, path, seen);

    if (path.size() == 0) return flow;

    int f = 0;

    for (auto x : g.adj[0]) f += flow[{0, x}];

    augment(path,res);
    return fordFulkerson();
}

map<pair<int, int>, int> Flow::maxFlow() {
    return fordFulkerson();
}

vector<int> Flow::stCut() {
    Graph res = buildResidualGraph();
    vector<int> seen(res.n, 0);
    res.dfs(s, seen);
    return seen;
}


/**
 * Bipartite Matching class
*/
class Bipartite {
private:
    
    /**
     * The bipartite graph 
    */
    Graph g;

public:
    
    /**
     * Parameterized constructor
     * @param g The bipartite graph for which maximum matching is to be computed.
    */
    Bipartite(Graph g);
    
    /**
     * Function to check if the given graph is a bipartite graph.
     * @param col A reference parameter to hold the 2-coloring of the given graph, if it is bipartite.
     * @return True if the graph is bipartite, False otherwise.
    */
    bool twoColoring(vector<int>& col);
    
    /**
     * Function to compute the maximum matching of the given bipartite graph.
     * @return A vector containing all the edges in the maximum matching.
    */
    vector<pair<int, int>> maximumMatching();
};

Bipartite::Bipartite(Graph g)
{
    this->g = g;
}

bool Bipartite::twoColoring(vector<int>& col) {
    col.resize(g.n, -1);

    for (int i=0; i<g.n; i++)
    {
        if (col[i]!=-1)
            continue;

        vector<int> v;
        col[i]=0;
        v.push_back(i);

        while (v.size())
        {
            int x = v[v.size()-1];
            v.pop_back();
            int c = col[x];

            for (auto p : g.adj[x])
            {
                int y = p;

                if (col[y]==-1)
                {
                    col[y] = 1-col[x];
                    v.push_back(y);
                }
                else
                {
                    if (col[y]==col[x])
                        return false;
                }
            }
        }
    }

    return true;
}

vector<pair<int, int>> Bipartite::maximumMatching() {
    vector<int> col;
    vector<pair<int, int>> res;

    if (!twoColoring(col)) {
        res.push_back({-1, -1});
        return res;
    }
        
    int s = g.n;
    int t = s+1;

    Graph b(g.n+2);

    for (int i=0; i<g.n; i++)
    {
        for (auto j : g.adj[i])
        {
            if (col[i]==0)
                b.addDirectedEdge(i,j,g.getCapacity(i,j));
        }
    }

    for (int i = 0; i < g.n; i++) {
        if (col[i] == 0)
            b.addDirectedEdge(s, i, 1);
        else
            b.addDirectedEdge(i, t, 1);
    }

    Flow f(b,s,t);
    map<pair<int,int>,int> flow = f.maxFlow();

    int tot = 0;

    for (auto x : b.adj[s])
        tot += flow[{s,x}];
    
    cout<<"Max flow : "<<tot<<"\n\n";

    for (auto x : flow) {
        if (x.second > 0) {
            pair<int, int> p = x.first;

            if (p.first < g.n && p.second < g.n) res.push_back(p);
        }
    }
    return res;
}

void runFordFulkerson(string filename, bool directed, bool weighted, bool oneBased)
{
    Graph g;
    g.readEdge(filename, directed, weighted, oneBased);

    Flow f(g, 0, g.n - 1);
    map<pair<int, int>, int> flow = f.maxFlow();

    int c = 0;

    for (auto p : g.adj[0]) c += flow[{0, p}];

    cout << "The max flow is : " << c << "\n";

    vector<int> cut = f.stCut();

    cout<<"\nST cut : \n";

    for (int i = 0; i < cut.size(); i++) {
        if (cut[i] == 1) cout << i << " ";
    }

    cout << "\n";

    for (int i = 0; i < cut.size(); i++) {
        if (cut[i] == 0) cout << i << " ";
    }

    cout << "\n";
}

void runBipartiteMatching(string filename, bool directed, bool weighted, bool oneBased)
{
    Graph g;
    g.readEdge(filename, directed, weighted, oneBased);
    Bipartite b(g);
    
    vector<pair<int,int>> edges = b.maximumMatching();

    pair<int,int> p = {-1,-1};
    if (edges[0]==p)
    {
        cout<<"Not a bipartite graph\n";
        return;
    }

    cout<<"The edges of the matching are : \n";

    for (auto p : edges)
        cout<<p.first<<" "<<p.second<<"\n";
}

int main(int argc, char** argv)
{
    if (argc<2)
    {
        cout<<"Enter mode and input file\n";
        exit(0);
    }
 

    int mode = atoi(argv[1]);

    bool oneBased = true;
    bool directed,weighted;

    if (mode==1)
    {
        directed = true;
        weighted = true;
        runFordFulkerson(argv[2],directed,weighted,oneBased);
    }
    else
    {
        directed = false;
        weighted = false;
        runBipartiteMatching(argv[2],directed,weighted,oneBased);
    }
}
