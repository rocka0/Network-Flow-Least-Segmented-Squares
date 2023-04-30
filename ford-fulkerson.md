# Ford Fulkerson

## Algorithm
The Ford-Fulkerson algorithm is used to find the maximum flow possible in a graph. In each iteration, we find an augmenting path, and increase the flow on the edges of that path by the bottleneck residual capacity. We repeat this process till no more augmenting paths can be found, at which stage we conclude that maximum flow has been achieved on the graph.

## Time Complexity
In our implementation, we have used an ordered map of sets to store the adjacency list information, and a map of pairs to store the capacity information. This allows us to lookup if a vertex is adjacent to a node in $\mathcal{O}(\log{m})$. Lookups to find the capacity of an edge is also $\mathcal{O}(\log{m})$. 

In each iteration, building the residual graph and finding the augmenting path can take $\mathcal{O}(m \cdot \log{m})$ in the worst case. Since the edges in the residual graph all have weights greater than zero, we can say that the bottleneck residual capacity is greater than zero. Thus, the total flow increases by at-least $1$ unit in every iteration. Since the max-flow is upper-bounded by the total capacity $C$ out of the source, the algorithm will terminate in atmost $C$ iterations. Hence in the worst case, the time complexity is $\mathcal{O}(m \cdot C \cdot \log m)$. 
