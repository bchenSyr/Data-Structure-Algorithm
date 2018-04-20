// Author: Beier Chen
// Selection Sort, Bubble Sort, Insertion Sort

#include <iostream>
using namespace std;

class node {
public:
    int value;
    node * next;
    node() { next = nullptr;}
    node(int k) { value = k; next = nullptr;}
};

class linked_list {
public:
int num_nodes;
    node * head;
    
    linked_list() {
        num_nodes = 0;
        head = nullptr;
    }
    void make_random_list(int k);
    void print();
    
    void reverse(); //Reverse the order of nodes of linked list
    //void remove_all(int k); //Remove all nodes whose node values are k
    //void insert(int k); //create a new node with value k and insert it to an already sorted list. After the insert, the linked list is still sorted.
    
    void next_func(); // Insert-Sort the nodes, based on ascending order of node values
    void selection_sort();
    void bubble_sort();
    void insertion_sort();
    
    void reverseFor554();
    void makeListTail(int k);
};

void linked_list::selection_sort(){
    node * p1 = head;
    node *p2 = head->next;
    node *p3 = nullptr;
    int min;
    int temp;
    int flag = 0;
    
    for(int i = 0; i < num_nodes - 1; i++)
    {
        min = p1->value;
        for(int j = num_nodes-1; j > i;  j-- )
        {
            if(min > p2->value)
            {
                p3 = p2;
                min = p2->value;
                flag = 1;
            }
            if (p2->next != nullptr ) p2 = p2->next;
        }
        
        if(flag == 1)
        {
            temp = p1->value;
            p1->value = p3->value;
            p3->value = temp;
        }
        p1 = p1->next;
        p2 = p1->next;

    }
}

void linked_list::bubble_sort(){
    
    node * p1, *p2;
    int temp;
    
    for (int i = 0; i < num_nodes; i++) {
        p1 = head;
        p2 = head->next;
        for (int j = num_nodes-1; j > i;  j--){
            if(p1->value >= p2->value){
                temp = p2->value;
                p2->value = p1->value;
                p1->value = temp;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }
}

void linked_list::insertion_sort()
{
    node *current = head->next;
    node *sorted_end = head;
    node *index_pre = head;
    node *index = head;
    
    int flag = 0;
    int temp = 0;
    int k = 0;
    
    for(int  i = 0; i < num_nodes - 1; i++)
    {
//        cout<<"\ni = "<< i;
//        cout<<"\ncurrent = "<< current->value;
        
        if(current->next == nullptr) { sorted_end->next = nullptr; } //the last node
        
        for( int j = i; j >= 0 && flag != 1; j--)
        {
            temp = i;
            if(index->value > current->value)
            {
//                cout<<"\n> k = "<< k;
//                cout<<"\n> temp = "<< temp;
                k = temp - j - 1;
                flag = 1;
//                cout<<"\n> k = "<< k;
            }
            if (flag == 0) {index = index->next;}
        }
    
        if(flag == 1) /* need to insert */
        {
            if(k == -1)/* insert to the head */
            {
                sorted_end->next = current->next;
                current->next = head;
                head = current;
                k = 0;
            }
            else
            {
                while(k >0)
                {
                    index_pre = index_pre->next;
                    k--;
                }
                /* insert */
                sorted_end->next = current->next;
                index_pre->next = current;
                current->next = index;
            }
        }
        else /* no need to insert */
        {
            if(current->next != nullptr) sorted_end = sorted_end->next;
            else sorted_end->next = nullptr;
        }
        
        /* update all the parameters */
        if(current->next != nullptr) { current = sorted_end->next;}
        else(sorted_end->next = nullptr);
        
        index = head;
        index_pre = head;
        flag = 0;
//        cout<<"\nsorted_end = "<< sorted_end->value;
//        linked_list::print();
    }

}

void linked_list::next_func()//to learn how to ues pointer
{
    node * one = head;
    node * two = one->next;
    node * three = two->next;
    
    two->next = three->next;
    three->next = two;
    one->next = three;
    
    while(one->next != nullptr){
        cout << "test ";
        one = one->next;
    }
}

void linked_list::make_random_list(int k) {
    node * p;
    for (int i = 0; i < k; i++) {
        p = new node(rand() % 100);
        p->next = head;
        head = p;
        num_nodes++;
    }
}

// creat linked list with every new node added to the tail of the list
// Jan 29, 2018
void linked_list::makeListTail(int k) {
    node * newNode;
    node * tail = head;
    for (int i = 0; i < k; i++) {
        newNode = new node(rand() % 100);
        if(head == nullptr){ 
            head = newNode;
            tail = head;
        }
        else{
            tail->next = newNode;
            tail = newNode;
        }
        num_nodes++;
    }
}



void linked_list::reverseFor554(){
    node * current =  head->next;
    node * before = head;
    node * nextOfcurrent;
    
    if(head->next == nullptr) return;
    head->next = nullptr;
    while(current != nullptr){
        nextOfcurrent = current->next;
        current ->next = before;
        before = current;
        current = nextOfcurrent;
    }
    head = before;
}
// array is always called by reference

void linked_list::print() {
    node * p = head;
    cout << endl;
    while (p != nullptr) {
        cout << p->value << " ";
        p = p->next;
    }
    cout << "\n";
}

void linked_list::reverse() {
    if (num_nodes <= 1) return;
    node * p1 = head, *p2 = head->next, *p3;
     while (p2 != nullptr) {
        p3 = p2->next;
        p2->next = p1;
        if (p1 == head) p1->next = nullptr;
        p1 = p2;
        p2 = p3;
    }
    head = p1;
}

int main() {
    
    linked_list L1;
    L1.make_random_list(11);

    cout<<"original list:";
    L1.print();
    
    linked_list ListTest;
    ListTest.makeListTail(11);
    ListTest.print();
    
//    L1.reverse();
//    L1.reverseFor554();
//    cout<<"\n reverse:";
//    L1.print();
//    L1.bubble_sort();
//    cout<<"\n bubble sort:";
//    L1.print();
    
    
    linked_list L2;
    L2.make_random_list(11);
    cout<<"\n original list:";
    L2.print();
    L2.selection_sort();
    cout<<"\n selection sort:";
    L2.print();

    linked_list L3;
    L3.make_random_list(11);
    cout<<"\n original list:";
    L3.print();
    L3.insertion_sort();
    cout<<"\n insertion sort:";
    L3.print();
    
    getchar();
    getchar();
    return 0;
}
