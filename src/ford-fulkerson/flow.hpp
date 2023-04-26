#ifndef _FLOW_H
#define _FLOW_H

#include "graph.hpp"

class Flow
{

private:
    int s,t;
    Graph g;
    vector<vector<pair<int,int>>> flow;
    vector<vector<pair<int,int>>> fordFulkerson();
    Graph buildResidualGraph();
    bool isForward(int x, int y);
    int getFlow(int x, int y);
    void updateFlow(int x, int y, int f);
    void augment(vector<int> path);

public:
    Flow(Graph g, int s, int t);
    vector<vector<pair<int,int>>> maxFlow();
    vector<int> stCut();
};

Flow::Flow(Graph g, int s, int t)
{
    this->s = s;
    this->t = t;
    this->g = g;
    flow.resize(g.n);

    for (int i=0; i<g.n; i++)
    {
        for (auto p : g.adjList[i])
            flow[i].push_back({p.first,0});
    }
}

bool Flow::isForward(int x, int y)
{
    for (int i=0; i<g.adjList[x].size(); i++)
    {
        if ((g.adjList[x][i].first)==y)
            return true;
    }

    return false;
}

int Flow::getFlow(int x, int y)
{
    for (int i=0; i<flow[x].size(); i++)
    {
        if ((flow[x][i].first)==y)
            return flow[x][i].second;
    }

    return -1;
}

void Flow::updateFlow(int x, int y, int f)
{
    for (int i=0; i<flow[x].size(); i++)
    {
        if ((flow[x][i].first)==y)
        {
            flow[x][i].second = f;
            return;
        }
    }
}

Graph Flow::buildResidualGraph()
{
    Graph res(g.n);
    
    for (int i=0; i<g.n; i++)
    {
        for (int j=0; j<g.adjList[i].size(); j++)
        {
            int c = g.adjList[i][j].second;
            int f = flow[i][j].second;
            int x = g.adjList[i][j].first;

            if (f<c)
                res.adjList[i].push_back({x,c-f});

            if (f>0)
                res.adjList[x].push_back({i,f});
        }
    }

    return res;
}

void Flow::augment(vector<int> path)
{
    int b = INT32_MAX; 

    for (int i=0; i<path.size()-1; i++)
        b = min(b, getFlow(path[i+1],path[i]));

    for (int i=0; i<path.size()-1; i++)
    {
        int x = path[i+1];
        int y = path[i];
        int f = getFlow(x,y);
        
        if (isForward(x,y))
            updateFlow(x,y,f+b);
        else
            updateFlow(y,x,f-b);
    }
}

vector<vector<pair<int,int>>> Flow::fordFulkerson()
{
    Graph res = buildResidualGraph();

    vector<int> path;
    res.getPath(s,t,path);

    if (path.size()==0)
        return flow;

    augment(path);
    return fordFulkerson();
}

vector<vector<pair<int,int>>> Flow::maxFlow()
{
    return fordFulkerson();
}

vector<int> Flow::stCut()
{
    vector<int> seen(g.n,0);
    g.dfs(s,seen);
    return seen;
}

#endif
