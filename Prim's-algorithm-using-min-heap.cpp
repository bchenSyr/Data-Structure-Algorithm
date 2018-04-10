//  Author: Beier Chen, bchen22@syr.edu
//  Date: 2017-10-25
//  Applicaiton: 674 HW4 - Prim's algorithm, implemented with min-Heap

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

//stands for edge node
class e_node
{
public:
    int nb;//the neighbor of a currently considered node
    int weight; //weight of the edge to the above neighbor
    e_node() {}//constructor
};

//rt stands for routing table
class rt_node
{
public:
    int cost;  //cost to a node
    int from;  //the from node to this node
    int checked;
    int h_pos; //the postion in heap for this node
    rt_node() { from = -1;  cost = 9999;  checked = 0; }
};

//stands for heap_node
class h_node
{
public:
    int to;
    int from;
    int cost;
    h_node() { to = -1; from = -1; cost = 9999; }
    h_node(int i, int j, int k) {  to = i; from =j; cost = k; }
    bool operator < (h_node h) { return (cost < h.cost); }
};


void heap_adjust_up(vector<rt_node> &rt, vector<h_node> & heap, int &n);
void heap_adjust_down(vector<rt_node> & rt, vector<h_node> & heap);
void heap_op(vector<vector<e_node>> & graph, vector<rt_node> & rt, vector<h_node> &heap,h_node startNode);

//rearrange the heap when changes happen
void heapify(vector<rt_node> & rt, vector<h_node> & heap, int i, int n);

int main() {
    ifstream in("text1.txt");
    
    int n, e; //n: num of nodes ; e: num of edges
    in >> n >> e;
    
    vector<e_node> ve;
    vector<vector<e_node> > graph(n, ve);
    e_node e1, e2;
    h_node startNode;
    int n1, n2, w;
    
    for (int i = 0; i < e; i++)
    {
        in >> n1 >> n2 >> w;
        e1.nb = n2;
        e1.weight = w;
        graph[n1].push_back(e1);
        
        e2.nb = n1;
        e2.weight = w;
        graph[n2].push_back(e2);
        
        /*---<Step#0 Choose the node with smallest weight as the starting node">---*/
        if(w < startNode.cost)
        {
            startNode.cost = w;
            startNode.from = n1;
            startNode.to = n2;
        }
    }
    in.close();
    
    vector<rt_node> rt(n); //rt stands for routing table
    vector<h_node> heap(0);
    
    heap_op(graph, rt, heap, startNode);

    //print the final result of minimum spanning tree
    for(int i = 0; i<rt.size();i++)
    {
        if(rt[i].cost != 0)
        {
            cout << i <<" "<< rt[i].from<<" "<< rt[i].cost<<"\n";
        }
    }
    
    getchar();
    getchar();
    return 0;
}

//while not all the node being checked, keep selecting minimum cost edge
void heap_op(vector<vector<e_node>> & graph, vector<rt_node> & rt, vector<h_node> & heap, h_node startNode)
{
    //updating startNode's routing table
    rt[startNode.from].from = startNode.from;
    rt[startNode.from].cost = 0;
    
    h_node tail;
    
    bool finished = false; //When all the noded being selected, finished == true
    int current = startNode.from;
    
    while(finished == false)
    {
        int num_checked = 0;
        
        /*---<Step#1 Push current node'nbs which are not being selected to the heap">---*/
        for(int i = 0; i < graph[current].size(); i++)
        {
            int current_nb = graph[current][i].nb;
            int current_cost = graph[current][i].weight;
            
            tail.cost = current_cost;
            tail.from = current;
            tail.to = current_nb;
            
            if(rt[tail.to].checked != 1) heap.push_back(tail);
        }
        
        /*---<Step#2 After udating the heap, rearrange the heap">---*/
        int heapSize = int(heap.size());
        heap_adjust_up(rt, heap, heapSize);
        
        /*---<Step#3 Mark the selected nodes as checked">---*/
        rt[current].checked = 1;
        
        rt[heap[0].to].from = heap[0].from;
        rt[heap[0].to].cost= heap[0].cost;
        
        current = heap[0].to;
        rt[current].checked = 1;
        
        /*---<Step#4 If an edge conneced two selected nodes, remove it from the heap">---*/
        heapSize = int(heap.size());
        for(int i = 0; i<heapSize;i++)
        {
            if(rt[heap[i].from].checked ==  1 && rt[heap[i].to].checked == 1)
            {
                swap(heap[i], heap[heapSize-1]);
                heap.pop_back();
            }
            heapSize = int(heap.size());
        }
        
        //counting the num of checked nodes
        for(int i = 0; i < rt.size(); i++)
        {
            if(rt[i].checked == 1)
            {
                num_checked++;
            }
        }
        if(num_checked == rt.size())
        {
            finished = true;
        }
    }
}

//remove the root of the heap and then rearrange the heap
void heap_adjust_down(vector<rt_node> & rt, vector<h_node> & heap)
{
    int heapSize = int(heap.size());
    swap(heap[0], heap[heapSize -1]);
    
    heap.pop_back();
    
    //rearrange the heap
    heapSize = int(heap.size());
    heap_adjust_up(rt, heap, heapSize);
}

//after updating a neighbor, rearrange the heap
void heap_adjust_up(vector<rt_node> & rt, vector<h_node> & heap, int &n)
{
    for(int i = (n -2) / 2; i >= 0; i--)
    {
        heapify(rt, heap, i, n);
    }
}

void heapify(vector<rt_node> & rt, vector<h_node> & heap, int i, int n)
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && right <= n-1)//the node have both left and right child node
    {
        if(heap[i].cost > heap[left].cost || heap[i].cost > heap[right].cost)
        {
            if(heap[left].cost < heap[right].cost)
            {
                swap(heap[i], heap[left]);
                heapify(rt, heap, left, n);
            }
            else
            {
                swap(heap[i], heap[right]);
                heapify(rt, heap, right, n);
            }
        }
    }
    else if(left < n && heap[i].cost > heap[left].cost)//the node has only left child node
    {
        swap(heap[i], heap[left]);
        heapify(rt, heap, left, n);
    }
}
