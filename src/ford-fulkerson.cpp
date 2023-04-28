#include<bits/stdc++.h>
using namespace std;

#define int long long

class Graph {

public:
    int n;
    bool directed = false;
    Graph();
    Graph(int n);
    map<int, set<int>> adj;
    map<pair<int,int>, int> cap;
    void readEdge(string filename, bool directed, bool weighted, bool oneBased);
    void addDirectedEdge(int x, int y, int c);
    void addUndirectedEdge(int x, int y, int c);
    void getPath(int start, int end, vector<int>& path, vector<int>& seen);
    void dfs(int node, vector<int>& seen);
    int getCapacity(int x, int y);
};

Graph::Graph()
{
    n=0;
}

Graph::Graph(int n)
{
    this->n = n;
}

int Graph::getCapacity(int x, int y)
{
    if (adj[x].find(y)==adj[x].end())
        return 0;

    return cap[{x,y}];
}

void Graph::addDirectedEdge(int x, int y, int c)
{
    if (adj[x].find(y)==adj[x].end())
    {
        adj[x].insert(y);
        cap[{x,y}]=c;
    }
    else
        cap[{x,y}]+=c;
}

void Graph::addUndirectedEdge(int x, int y, int c)
{
    addDirectedEdge(x,y,c);
    addDirectedEdge(y,x,c);
}

void Graph::readEdge(string filename, bool directed, bool weighted, bool oneBased)
{
    ifstream cin;
    cin.open(filename);
    cin>>n;

    int m;
    cin>>m;

    while (m--)
    {
        int x,y,c;
        cin>>x>>y;

        if (weighted)
            cin>>c;
        else
            c=1;

        if (oneBased)        
        {
            x--;
            y--;
        }

        if (directed)
            addDirectedEdge(x,y,c);
        else
            addUndirectedEdge(x,y,c);
    }
}

void Graph::getPath(int start, int end, vector<int>& path, vector<int>& seen)
{
    if (seen[start])
        return;

    seen[start]=1;

    if (start==end)
    {
        path.push_back(end);
        return;
    }

    for (auto p : adj[start])
    {
        getPath(p, end, path,seen);

        if (path.size())
        {
            path.push_back(start);
            return;
        }
    }
}

void Graph::dfs(int node, vector<int>& seen)
{
    if (seen[node])
        return;

    seen[node]=1;

    for (auto p : adj[node])
        dfs(p, seen);
}

class Flow
{

private:
    int s,t;
    Graph g;
    map<pair<int,int>, int> flow;
    map<pair<int,int>,int> fordFulkerson();
    Graph buildResidualGraph();
    bool isForward(int x, int y);
    int getFlow(int x, int y);
    void updateFlow(int x, int y, int f);
    void augment(vector<int> path, Graph res);

public:
    Flow(Graph g, int s, int t);
    map<pair<int,int>,int> maxFlow();
    vector<int> stCut();
};

Flow::Flow(Graph g, int s, int t)
{
    this->s = s;
    this->t = t;
    this->g = g;

    for (int i=0; i<g.n; i++)
    {
        for (auto p : g.adj[i])
            flow[{i,p}]=0;
    }
}

bool Flow::isForward(int x, int y)
{
    if (g.adj[x].find(y)==g.adj[x].end())
        return false;

    return true;
}

int Flow::getFlow(int x, int y)
{
    if (g.adj[x].find(y)==g.adj[x].end())
        return 0;
    
    return flow[{x,y}];
}

void Flow::updateFlow(int x, int y, int f)
{
    flow[{x,y}]=f;
}

Graph Flow::buildResidualGraph()
{
    Graph res(g.n);
    
    for (int i=0; i<g.n; i++)
    {
        for (auto j : g.adj[i])
        {
            int c = g.getCapacity(i,j);
            int f = getFlow(i,j);

            if (f<c)
                res.addDirectedEdge(i,j,c-f);

            if (f>0)
                res.addDirectedEdge(j,i,f);
        }
    }

    return res;
}

void Flow::augment(vector<int> path, Graph res)
{
    int b = LLONG_MAX;

    for (int i=0; i<path.size()-1; i++)
        b = min(b, res.getCapacity(path[i+1],path[i]));

    for (int i=0; i<path.size()-1; i++)
    {
        int x = path[i+1];
        int y = path[i];

        if (isForward(x,y))
        {
            int f = getFlow(x,y);
            updateFlow(x,y,f+b);
        }
        else
        {
            int f = getFlow(y,x);
            updateFlow(y,x,f-b);
        }
    }
}

map<pair<int,int>,int> Flow::fordFulkerson()
{
    Graph res = buildResidualGraph();
    vector<int> path;
    vector<int> seen(g.n,0);
    res.getPath(s,t,path,seen);

    if (path.size()==0)
        return flow;

    augment(path,res);
    return fordFulkerson();
}

map<pair<int,int>,int> Flow::maxFlow()
{
    return fordFulkerson();
}

vector<int> Flow::stCut()
{
    vector<int> seen(g.n,0);
    g.dfs(s,seen);
    return seen;
}

class Bipartite
{

private:
    Graph g;
    vector<int> partition;

public:
    bool twoColoring(vector<int>& col);
    vector<pair<int,int>> maximumMatching();
};

bool Bipartite::twoColoring(vector<int>& col)
{
    col.resize(g.n, -1);
    col[0]=0;
    
    vector<int> v;
    v.push_back(0);

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

    return true;
}

vector<pair<int,int>> Bipartite::maximumMatching()
{
    vector<int> col;
    vector<pair<int,int>> res;

    if (!twoColoring(col))
    {
        res.push_back({-1,-1});
        return res;
    }
        
    int s = g.n + 1;
    int t = s+1;

    Graph b(g);

    b.n+=2;
    vector<pair<int,int>> temp;

    for (int i=0; i<g.n; i++)
    {
        if (col[i]==0)
            b.addDirectedEdge(s,i,1);
        else
            b.addDirectedEdge(i,t,1);
    }

    Flow f(b,s,t);
    map<pair<int,int>,int> flow = f.maxFlow();

    for (auto x : flow)
    {
        if (x.second > 0)
        {
            pair<int,int> p = x.first;

            if (p.first<g.n && p.second<g.n)
                res.push_back(p);
        }
    }
    return res;
}

void runFordFulkerson(string filename)
{
    Graph g;
    g.readEdge(filename,true,true,true);

    Flow f(g,0,g.n-1);
    map<pair<int,int>,int> flow = f.maxFlow();

    int c=0;

    for (auto p : g.adj[0])
        c+=flow[{0,p}];

    cout<<"The max flow is : "<<c<<"\n";
}

int32_t main(int32_t argc, char** argv)
{
    runFordFulkerson(argv[1]);
}
