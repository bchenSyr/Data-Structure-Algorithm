//Athor: Beier Chen
//Source: Roger Chen
//CSE674 - HW5: Merge Sort
//Due: Sunday (Nov. 5) at 11:59PM

#include <iostream>
#include <list>
#include <vector>

using namespace std;

class node {
public:
    int value;
    node * next;
    node() { next = nullptr; }
    node(int i) { value = i; next = nullptr; }
};

class linked_list {
public:
    int num_nodes;
    node * head;
    linked_list() { num_nodes = 0; head = nullptr; }
    void make_list(int k, int m);
    void print();
    
    //************************
    void merge_sort(node * p, int i);
    
    //************************
    void merge(node* p1, int i1, node* p2, int i2);
    
    node * mergeHead = nullptr;
    bool mergeFlag = false;
};

void linked_list::merge_sort(node * p, int i)
{
    node * p2 = p;
    node * temp = nullptr;
    node * subLeft = p;
    node * subRight = p;
    mergeFlag = false;
    
    int mid = i / 2;
    
    //---< p2 works as a traversal pointer of the right half
    for (int k = 0; k < mid; k++)
    {
        p2 = p2->next;
    }
    
    //---< sort the left half
    if (mid <= 2)
    {
        if (mid == 2) // if size of partition equals two, sort the two nodes
        {
            if (p->value > p->next->value) //if needs to swap
            {
                temp = p->next;
                p->next = temp->next;
                temp->next = p;
                p = temp;
            }
        }
        subLeft = p;
    }
    else // if size of partition larger than two, continue to devide partitions
        merge_sort(p, mid);
    
    if (mergeFlag)
        subLeft = mergeHead;
    
    //---< sort the right half
    if (i - mid <= 2)
    {
        if (i - mid == 2)
        {
            if (p2->value > p2->next->value)
            {
                temp = p2->next;
                p2->next = temp->next;
                temp->next = p2;
                p2 = temp;
            }
        }
        subRight = p2;
    }
    else merge_sort(p2, i - mid);
    
    if (mergeFlag)
        subRight = mergeHead;
    
    //---< merge the two halves
    merge(subLeft, mid, subRight, i - mid);
}

void linked_list::merge(node * p1, int i1, node * p2, int i2)
{
    mergeFlag = true;
    node* rightTail = p2;
    for (int k = 1; k < i2; k++)
    {
        rightTail = rightTail->next;
    }
    rightTail->next = nullptr;
    
    node* leftPtr = p1;//works as a traversal pointer of the left half
    node* temp;
    int lengthLeft = i1;//the length of left half
    
    for (int k = 0; (k < i1 + i2 - 1) && (lengthLeft != 0) && p2 != nullptr; k++)
    {
        if (leftPtr->value >= p2->value) //needs to swap node
        {
            temp = p2->next; //temp records the next node of p2
            p2->next = leftPtr;
            if (k == 0) p1 = p2;
            else
            {
                //find the node before leftPtr
                node* before = p1;
                while (before->next != leftPtr)
                {
                    before = before->next;
                }
                before->next = p2;
            }
            
            p2 = temp; //p2 moves to next node
        }
        else//no need to swap node
        {
            lengthLeft--;
            if (lengthLeft == 0)
            {
                leftPtr->next = p2;
            }
            else leftPtr = leftPtr->next;
        }
    }
    
    if ((i1 + i2) == num_nodes) // the last merge of the original list
    {
        mergeHead = p1;
        head = mergeHead;
        node* tail = head;
        
        for (int k = 1; k < num_nodes; k++)
        {
            tail = tail->next;
        }
        tail->next = nullptr;
    }
    else mergeHead = p1;
}

void linked_list::make_list(int k, int m) {
    for (int i = 0; i < k; i++) {
        node * p = new node(rand() % m);
        p->next = head;
        head = p;
        num_nodes++;
    }
}

void linked_list::print() {
    node * p = head;
    cout << endl;
    while (p != nullptr) {
        cout << p->value << " ";
        p = p->next;
    }
}

void make_random_vector(vector<int> &V, int k, int m) {
    for (int i = 0; i < k; i++) V.push_back(rand() % m);
}

//************************
void merge_sort(vector<int> &V)
{
    int fullSize = int(V.size());// the size of the whole array
    int subSize = 0; // the size of subarray, ranging from 1 to (fullSize-1)
    int subLeftHead = 0;// the first element of the left subarray
    int subRightHead = 0;// the first element of the right subarray
    int subTail = 0;;// the last element of the subarray
    
    vector<int> tempArray;
    for (int i = 0; i < fullSize; i++) tempArray.push_back(0);

    for (subSize = 1; subSize < fullSize -1 ; subSize = 2 * subSize)
    {
        for (subLeftHead = 0; subLeftHead < fullSize - 1; subLeftHead = subLeftHead + 2* subSize)
        {
            if (subLeftHead + subSize > fullSize - 1) break;
            else
            {
                //---< the first element of the right subarray
                subRightHead = subLeftHead + subSize;
                
                //---< find the last element of the subarray
                subTail = 0;
                if (subRightHead + subSize - 1 < fullSize - 1)
                    subTail = subRightHead + subSize - 1;
                else subTail = fullSize - 1;
                
                //---< merge subarrays: V[subLeftHead...(subRightHead-1)],V[subRightHead...subTail]
                int leftCount = subLeftHead;
                int rightCount = subRightHead;
                int subLength = subTail - subLeftHead + 1;
                int i = 0;
                
                while (i < subLength)
                {
                    if (leftCount >= subRightHead)
                    {
                        tempArray[i] = V[rightCount];
                        rightCount++;
                    }
                    else if (rightCount > subTail)
                    {
                        tempArray[i] = V[leftCount];
                        leftCount++;
                    }
                    else if (V[leftCount] > V[rightCount])
                    {
                        tempArray[i] = V[rightCount];
                        rightCount++;
                    }
                    else if (V[leftCount] <= V[rightCount])
                    {
                        tempArray[i] = V[leftCount];
                        leftCount++;
                    }
                    i++;
                }
                
                //---<copy elements back to the original array
                int k = subLeftHead;
                for (int i = 0; i < subLength; i++)
                {
                    V[k] = tempArray[i];
                    k++;
                }
            }
            
            
        }
    }
};
//Using iterative (i.e., non_recursive) approach to implement merge sort.


int main() {
    
    vector<int> V;
    make_random_vector(V, 50, 40);
    cout << endl;
    for (size_t i = 0; i < V.size(); i++) cout << V[i] << " ";
    merge_sort(V);
    cout << endl;
    for (size_t i = 0; i < V.size(); i++) cout << V[i] << " ";
    
    linked_list L1;
    L1.make_list(30, 20);
    L1.print();
    L1.merge_sort(L1.head, L1.num_nodes);
    L1.print();
    
    getchar();
    getchar();
    return 0;
}


