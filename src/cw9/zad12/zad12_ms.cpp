#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct nd *Node;
struct nd
{
    int id;
    int degree;
    vector<Node> edges;
};

Node make_node(int n)
{
    Node newNode = (Node)malloc(sizeof(nd));
    newNode->id = n;
    newNode->degree = 0;
    vector<Node> ed;
    newNode->edges = ed;
    return newNode;
}

void postpone_visit(Node node, queue<Node> &toBeVisited)
{
    toBeVisited.push(node);
}

bool compare_degrees(Node a, Node b)
{
    return a->degree < b->degree;
}

// T(n) = O(n^2)
// M(n) = O(n)
// create graph from edges' matrix and sort by degree
vector<Node> sort_nodes(const vector<vector<bool>> &edges)
{
    int n = (int)edges.size();
    vector<Node> nodes(n);
    for(int i = 0;i<n; i++){
        nodes[i] = make_node(i);
    }

    // calculate degree for all nodes
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            if (edges[j][i])
            {
                nodes[i]->degree++;
                nodes[j]->edges.push_back(nodes[i]);
            }
        }
    }
    // sort them by degree
    sort(nodes.begin(), nodes.end(), compare_degrees);
    return nodes;
}
class Compare
{
public:
    bool operator()(Node a, Node b)
    {
        return a->degree < b->degree;
    }
};

// T(n) = O(n^2)
// M(n) = O(n^2)
int count_dominating_nodes(const vector<vector<bool>> &edges)
{
    int n = (int)edges.size();
    vector<Node> sortedNodes = sort_nodes(edges);
    priority_queue<Node, vector<Node>, Compare> toBeVisited;
    vector<bool> wasVisited(n, false);
    vector<bool> canBeDominated(n, true);

    int i = 0;
    // push all 0-degree nodes onto the stack
    while (i < n && sortedNodes[i]->degree == 0)
    {
        toBeVisited.push(sortedNodes[i]);
        canBeDominated[sortedNodes[i]->id]=false;
        i++;
    }

    // no visit all of nodes bfa, but in degree order
    while (!toBeVisited.empty())
    {
        int currentDegree = toBeVisited.top()->degree;
        // visit all nodes of current degree \/
        while (!toBeVisited.empty() && toBeVisited.top()->degree == currentDegree)
        {
            Node current = toBeVisited.top();
            toBeVisited.pop();
            if (wasVisited[current->id])
            {
                // other walker was here, so 1. it cannnot be dominated, 2. we can trash current walker, because someone already was here
                canBeDominated[current->id] = false;
                continue;
            }
            wasVisited[current->id] = true;
            for (Node connected : current->edges)
            {
                toBeVisited.push(connected);
            }
        }
    }

    // finished, so count results
    int results = 0;
    for (int j = 0; j < n; j++)
    {
        if (canBeDominated[j])
            results++;
        free(sortedNodes[j]);
    }
    sortedNodes.clear();
    wasVisited.clear();
    return results;
}
int main()
{
    int n;
    cin >> n;

    vector<vector<bool>> edges(n, vector<bool>(n, false));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int b;
            cin >> b;
            edges[i][j] = (bool)b;
        }
    }

    int dominating = count_dominating_nodes(edges);
    cout << dominating << endl;

    for (vector<bool> row : edges)
    {
        row.clear();
    }
    edges.clear();
    return 0;
}