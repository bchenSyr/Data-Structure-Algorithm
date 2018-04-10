//Dijkstra's algorithm
//Implemented with min-Heap
//Time Complexity O((N + E) log N)

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
    int id;
    int cost;
    h_node() { id = -1; cost = 9999; }
    h_node(int i, int j) { id = i; cost = j; }
    bool operator < (h_node h) { return (cost < h.cost); }
};

//Insert the souce node to heap and update rt and heap accordingly
void set_up(vector<rt_node> &rt, vector<h_node> & heap, int &start);

void heap_adjust_up(vector<rt_node> &rt, vector<h_node> & heap, int &n);
void heap_adjust_down(vector<rt_node> & rt, vector<h_node> & heap);
void heap_op(vector<vector<e_node>> & graph, vector<rt_node> & rt, vector<h_node> &heap);

//rearrange the heap when changes happen
void heapify(vector<h_node> & heap, int i, int n);

int main() {
    ifstream in("text1.txt");
    int start;
    in >> start;//source node for the Dijkstra's algorithm
    int n, e; //n: num of nodes ; e: num of edges
    in >> n >> e;
    
    vector<e_node> ve;
    vector<vector<e_node> > graph(n, ve);
    e_node e1;
    int n1, n2, w;
    for (int i = 0; i < e; i++)
    {
        in >> n1 >> n2 >> w;
        e1.nb = n2;
        e1.weight = w;
        graph[n1].push_back(e1);
    }
    in.close();
    
    vector<rt_node> rt(n); //rt stands for routing table
    vector<h_node> heap(n);
    
    set_up(rt, heap, start);
    heap_op(graph, rt, heap);
    
    cout<< "\n\nFinal Result:\n======================\n";
    for (int i = 0; i < n; i++)
    {
        cout << "cost from " << start << " to " << i << " is: " << rt[i].cost << "  from node is " << rt[i].from << endl;
    }
    getchar();
    getchar();
    return 0;
}

void set_up(vector<rt_node> &rt, vector<h_node> & heap, int &start)
{
    rt[start].from = start;
    rt[start].cost = 0;
    
    for (int i = 0; i < heap.size(); i++) {
        heap[i].id = i;
        rt[i].h_pos = i;
    }
    
    heap[start].id = 0;
    heap[0].id = start;
    heap[0].cost = 0;
    rt[0].h_pos = start;
    rt[start].h_pos = 0;
}

//remove the root of the heap and then rearrange the heap
void heap_adjust_down(vector<rt_node> & rt, vector<h_node> & heap)
{
    //find out the last node in heap
    int i = 0;
    while(heap[i].cost < 100) { i++;}
    swap(heap[0], heap[i-1]);
    
    //remove the root from the heap
    heap[i-1].cost = 9999;
    
    //rearrange the heap
    int n = i-1;
    heap_adjust_up(rt, heap, n);
}


//after updating a neighbor, rearrange the heap
void heap_adjust_up(vector<rt_node> & rt, vector<h_node> & heap, int &n)
{
    for(int i = (n -2) / 2; i >= 0; i--)
    {
        heapify(heap, i, n);
    }
    //In order to find the last node of a heap easily in function heap_adjust_down(),
    //set the cost of a node after the heap array to 9999
    heap[n].cost = 9999;
}

void heapify(vector<h_node> & heap, int i, int n)
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
                heapify(heap, left, n);
            }
            else
            {
                swap(heap[i], heap[right]);
                heapify(heap, right, n);
            }
        }
    }
    else if(left < n && heap[i].cost > heap[left].cost)//the node has only left child node
    {
        swap(heap[i], heap[left]);
        heapify(heap, left, n);
    }
}

//while the heap is not empty, keep removing root and updating routing table
void heap_op(vector<vector<e_node>> & graph, vector<rt_node> & rt, vector<h_node> &heap)
{
    int num_un =  int (rt.size());//num of unchecked nodes
    
    for(int i = 0; i < rt.size(); i++)
    {
        int current = heap[0].id;//the node with optimal cost is the root of the heap
        int c_cost = rt[current].cost;
        for(int i = 0; i < graph[current].size(); i++)
        {
            int current_nb = graph[current][i].nb;
            int new_cost = graph[current][i].weight + c_cost;
            
            if(rt[current_nb].cost > new_cost)
            {
                rt[current_nb].cost = new_cost;
                rt[current_nb].from = current;
                
                heap[rt[current_nb].h_pos].id = current_nb;
                heap[rt[current_nb].h_pos].cost = rt[current_nb].cost;
            }
        }
        
        heap_adjust_up(rt, heap, num_un);
        rt[current].checked = 1;
        
        heap_adjust_down(rt, heap);
        num_un--;
        
        cout << "\n\nTemporary Routing Table :";
        for(int i = 0; i < rt.size(); i++)
        {
            cout <<"\n Node "<< i << ": cost = " << rt[i].cost<<" from node " << rt[i].from;
            cout <<" checked = "<< rt[i].checked;
        }
    }
}

