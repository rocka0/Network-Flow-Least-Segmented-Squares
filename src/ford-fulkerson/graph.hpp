#ifndef _GRAPH_H
#define _GRAPH_H

#include<vector>
#include<fstream>

using namespace std;

class Graph {

private:
    void readEdge(ifstream file, bool directed, bool weighted, bool oneBased);

public:
    int n;
    Graph();
    Graph(int n);
    vector<vector<pair<int,int>>> adjList;
    void getPath(int start, int end, vector<int>& path);
    void dfs(int node, vector<int>& seen);
};

Graph::Graph(int n)
{
    this->n = n;
    adjList.resize(n);
}

void Graph::readEdge(ifstream file, bool directed, bool weighted, bool oneBased)
{
    file.open("input.txt");
    file>>n;

    adjList = vector<vector<pair<int,int>>>(n);

    int m;
    file>>m;

    while (m--)
    {
        int x,y,c;
        file>>x>>y;

        if (weighted)
            file>>c;
        else
            c=1;

        if (oneBased)        
        {
            x--;
            y--;
        }

        adjList[x].push_back({y,c});

        if (!directed)
            adjList[y].push_back({x,c});
    }

    file.close();
}

void Graph::getPath(int start, int end, vector<int>& path)
{

    if (start==end)
    {
        path.push_back(end);
        return;
    }

    for (auto p : adjList[start])
    {
        getPath(p.first, end, path);

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

    for (auto p : adjList[node])
        dfs(p.first, seen);
}

#endif


