// CSE674 HW7 2-3-4 Tree
// Date: Tuesday Dec 5 2017
// Author: Beier Chen, bchen22@syr.edu

//#include "stdafx.h"
#include <iostream>
#include <vector>

using namespace std;

class node
{
//for simplicity, we again assume all numbers are distinct
public:
    int num_values;
    vector<int> value;  //it can contain 1 ... 3 values
    node * parent;
    vector<node *> child;  //child[i] ( i = 0 ... 3) is a pointer to child node i
    int child_state; //a node can have up to 4 child nodes: child 0, 1, 2, 3.  child_state i (0 ... 3) 
                     //means this node is child i of its parent
    int is_leaf; //1 if this is a leaf node; otherwise 0
    node() { num_values = 0; is_leaf = 1; child.assign(4, nullptr); parent = nullptr; value.assign(3, -999); }
    void add_value(int k);  //add value to a node of less than 3 value
    void remove_value(int k); //remove value from a node of more than 1 value
};

class two34_tree
{
public:
    node * root;
    two34_tree() { root = nullptr; }
    void add(int k);
    node * find(int k); //find a node to add value k; invoked by add
    void break_3_value_node(node * p); //to be invoked by find
    void remove(int k);
    node * find_1(int k); //find a node to replace k once k is removed; invokde by remove
    void expand_1_value_node(node *p); //to be invoked by find_1
    void fusion(node *p); //to be invoked by exapnd_1_value_node
    void rotation(node *p); //to be invoked by exapnd_1_value_node
    void in_order_traversal(node * p);
};

void node::remove_value(int k)
{
    // # remove the first or second value
    if (value[0] == k || value[1] == k)
    {
        if (value[0] == k)
            value[0] = value[1];
        
        if (num_values == 2) // ## if node has two values
        {
            value[1] = -999;
        }
        else // ## if node has three values
        {
            value[1] = value[2];
            value[2] = -999;
        }
    }
    // # remove the third value
    else
    {
        value[2] = -999;
    }
    num_values--;
}

void two34_tree::fusion(node *p)
{
    node * parentOfP = p->parent;
    node * sibling = nullptr;
    bool isLeftFusion = true;
    int childState = p->child_state;
    
    // # if p is first child node of its parent, uses its right sibling to perform right fusion
    // # otherwise, uses its left sibling to perform left fusion
    if (childState == 0)
    {
        isLeftFusion = false;
    }
    
    /* ---< perform left fusion, using left sibling >--- */
    if(isLeftFusion)
    {
        sibling = parentOfP->child[childState -1]; // use left sibling
        
        //---< update p's value
        p->value[2] = p->value[0];
        p->value[1] = parentOfP->value[childState -1];
        p->value[0] = sibling->value[0];
        
        //---< update parentOfP's value
        if(childState == parentOfP->num_values)     // if p is the last child node of its parent
        {
            parentOfP->value[childState -1] = -999;
        }
        else     // if p is not the last child node of its parent
        {
            if(childState == 1)
            {
                parentOfP->value[0] = parentOfP->value[1];
            }
            
            if(parentOfP->num_values == 3)
            {
                parentOfP->value[1] = parentOfP->value[2];
                parentOfP->value[2] = -999;
            }
            else
            {
                parentOfP->value[1] = -999;
            }
        }
        
        //---< if parentOfP is not a 1-value root node, update parentOfP's child relationship
        if(parentOfP->num_values != 1)
        {
            // update parentOfP's child relationship
            if(childState == 1)
            {
                parentOfP->child[0] = parentOfP->child[1];
                parentOfP->child[0]->child_state = 0;
                
                parentOfP->child[1] = parentOfP->child[2];
                parentOfP->child[1]->child_state = 1;
                
            }
            else if (childState == 2)
            {
                parentOfP->child[1] = parentOfP->child[2];
                parentOfP->child[1]->child_state = 1;
            }
            
            if(parentOfP->num_values == 3)
            {
                parentOfP->child[2] = parentOfP->child[3];
                parentOfP->child[2]->child_state = 2;
                parentOfP->child[3] = nullptr;
            }
            else
            {
                parentOfP->child[2] = nullptr;
            }
            
            parentOfP->num_values--;
        }
        else // if parentOfP is not a 1-value root node, p is the new root
        {
            root = p;
            p->parent = nullptr;
            p->child_state = -1;
        }
        
        //---< if p is not a leaf node, update p's realated children relationship
        if(p->is_leaf == 0)
        {
            p->child[2] = p->child[0];
            p->child[2]->child_state = 2;
            
            p->child[3] = p->child[1];
            p->child[3]->child_state = 3;
            
            p->child[0] = sibling->child[0];
            p->child[0]->parent = p;
            
            p->child[1] = sibling->child[1];
            p->child[1]->parent = p;
        }
        
        //---< update num_values
        p->num_values = 3;
    }
    
    /* ---< when p is the first child node of its parent, perform right fusion using right sibling >--- */
    else
    {
        sibling = parentOfP->child[1];
        
        //---< update p's value
        p->value[1] = parentOfP->value[0];
        p->value[2] = sibling->value[0];
        
        //---< if parentOfP is not a 1-value root node, update parentOfP's child relationship and value
        if(parentOfP->num_values != 1)
        {
            // update parentOfP's value
            parentOfP->value[0] = parentOfP->value[1];
            
            if(parentOfP->num_values == 3)
            {
                parentOfP->value[1] = parentOfP->value[2];
                parentOfP->value[2] = -999;
            }
            else
            {
                parentOfP->value[1] = -999;
            }
            
            // update parentOfP's child relationship
            parentOfP->child[1] = parentOfP->child[2];
            parentOfP->child[1]->child_state = 1;
            
            if(parentOfP->num_values == 3)
            {
                parentOfP->child[2] = parentOfP->child[3];
                parentOfP->child[2]->child_state = 2;
                parentOfP->child[3] = nullptr;
            }
            else
            {
                parentOfP->child[2] = nullptr;
            }
            
            parentOfP->num_values--;
        }
        else // if parentOfP is not a 1-value root node, p is the new root
        {
            root = p;
            p->parent = nullptr;
            p->child_state = -1;
        }
       
        //---< if p is not a leaf node, update p's realated children relationship
        if(p->is_leaf == 0)
        {
            p->child[2] = sibling->child[0];
            p->child[2]->child_state = 2;
            p->child[2]->parent = p;
            
            p->child[3] = sibling->child[1];
            p->child[3]->child_state = 3;
            p->child[3]->parent = p;
        }
        
        //---< update num_values
        p->num_values = 3;
        sibling->num_values--;
    }
}

void two34_tree::rotation(node *p)
{
    node * parentOfP = p->parent;
    bool isLeftRotation = false;
    node * sibling = nullptr;
    p->num_values++;
    int childState = p->child_state;
    
    // # if p is the first child node of its parent, only left rotation is available
    if(p->child_state == 0)
    {
        isLeftRotation = true;
    }
    // # if p is the last child node of its parent, only right rotation is available
    else if (p->child_state == parentOfP->num_values)
    {
        // only right rotation is available
        isLeftRotation = false;
    }
    // # if p is the middle child node(s) of its parent
    else
    {
        // always consider left sibling first, if available, perform right rotation
        // ## if left sibling is not available(only has one value), perform left rotation
        if(parentOfP->child[childState -1]->num_values == 1)
        {
            isLeftRotation = true;
        }
    }
    // perform left rotation
    if(isLeftRotation)
    {
        sibling = parentOfP->child[childState + 1];
        
        p->value[1] = parentOfP->value[childState];
        parentOfP->value[childState] = sibling->value[0];
        sibling->value[0] = sibling->value[1];
        
        if(sibling->num_values == 3)
        {
            sibling->value[1] = sibling->value[2];
            sibling->value[2] = -999;
        }
        else
            sibling->value[1] = -999;
        
        // if p and its sibling has child nodes, update their relationship
        if(p->is_leaf == 0)
        {
            p->child[2] = sibling->child[0];
            p->child[2]->child_state = 2;
            sibling->child[0]->parent = p;
            
            sibling->child[0] = sibling->child[1];
            sibling->child[0]->child_state = 0;
            sibling->child[1] = sibling->child[2];
            sibling->child[1]->child_state = 1;
            
            if(sibling->child[3] != nullptr)
            {
                sibling->child[2] = sibling->child[3];
                sibling->child[2]->child_state = 2;
                sibling->child[3] = nullptr;
            }
            else
                sibling->child[2] = nullptr;
        }
        
        sibling->num_values--;
    }
    // perform right rotation
    else
    {
        sibling = parentOfP->child[childState - 1];
        
        p->value[1] = p->value[0];
        p->value[0] = parentOfP->value[childState -1];
        parentOfP->value[childState -1] = sibling->value[sibling->num_values - 1];
        sibling->value[sibling->num_values - 1] = -999;
        
        // if p and its sibling has child nodes, update their relationship
        if(p->is_leaf == 0)
        {
            p->child[2] = p->child[1];
            p->child[2]->child_state = 2;
            
            p->child[1] = p->child[0];
            p->child[1]->child_state = 1;
            
            p->child[0] = sibling->child[sibling->num_values];
            sibling->child[sibling->num_values]->parent = p;
            sibling->child[sibling->num_values] = nullptr;
            p->child[0]->child_state = 0;
        }
        sibling->num_values--;
    }
}

void two34_tree::expand_1_value_node(node *p)
{
    node * parentOfP = p->parent;
    bool siblingAvailable = false;
    
    int childState = p->child_state;
    
    //# if p is the first child node of its parent, check its right sibling
    if(childState == 0)
    {
        if(parentOfP->child[1]->num_values != 1)
        {
            siblingAvailable = true;
        }
    }
    // # if p is the last child node of its parent, check its left sibling
    else if (childState == parentOfP->num_values)
    {
        if(parentOfP->child[childState-1]->num_values != 1)
        {
            siblingAvailable = true;
        }
    }
    // # if p is the middle child node of its parent, check sibings in both side
    else
    {
        if(parentOfP->child[childState-1]->num_values != 1 || parentOfP->child[childState+1]->num_values != 1)
            siblingAvailable = true;
    }
    
    if(siblingAvailable)
        rotation(p);
    else
        fusion(p);
}

node * two34_tree::find_1(int k)
{
    node * curr = root;
    bool finishFlag = false;
    // Finish means after all the nessesary explasion:
    //either: k is a leaf node, and return this leaf node
    //or: located the leaf node that has the replace value for k, and return this leaf node
    
    while (finishFlag == false)
    {
        if(curr != root && curr->num_values == 1)
        {
            expand_1_value_node(curr);
        }
        
        if(curr->is_leaf == 1 && curr->num_values != 1)
        {
            finishFlag = true;
        }
        else
        {
            for (int i = 0; i < curr->num_values; i++)
            {
                // if k is equals or less than the values that are not the last value of *curr
                if (k <= curr->value[i])
                {
                    if (curr->value[i] == k)
                    {
                        //k is in a leaf node that contains more that one value, search process finished
                        if(curr->is_leaf == 1)  // curr->num_values >=2 &&
                        {
                            finishFlag = true;
                            break;
                        }
                    }
                    
                    curr = curr->child[i];
                    break;
                }
                // if k is largger than the last value of current pointed node, *curr moves to its last child node
                else if (i == curr->num_values - 1)
                {
                    curr = curr->child[i + 1];
                    break;
                }
            }
        }
    }
    return curr;
}

void two34_tree::remove(int k)
{
    node * curr = root;
    bool finishSearch = false;
    int childState = -1;
    
    node* returnNode = find_1(k);
    
    while (finishSearch == false)
    {
        for (int i = 0; i < curr->num_values; i++)
        {
            childState = i;
            if (k <= curr->value[i])
            {
                if (curr->value[i] == k)
                {
                    finishSearch = true;
                    break;
                }
                else
                {
                    curr = curr->child[i];
                    break;
                }
            }
            else if (i == curr->num_values - 1)
            {
                curr = curr->child[i + 1];
                break;
            }
        }
    }
    
    // # if replace value and value to be removed is in the same leaf node
    if(returnNode == curr)
    {
        curr->remove_value(k);
    }
    // # if value to be removed is in a non-leaf node
    else
    {
        // ## if replace value in a 2-value node
        if(returnNode->num_values == 2)
        {
            curr->value[childState] =  returnNode->value[1];
            returnNode->value[1] = -999;
        }
        // ## if replace value in a 3-value node
        else
        {
            curr->value[childState] =  returnNode->value[2];
            returnNode->value[2] = -999;
        }
        returnNode->num_values--;
    }
}

void two34_tree::break_3_value_node(node * p)
{
    // if p is a root node, after breaking, p is the first child node of root, and slipNode is the second child of root
    // otherwise, after breaking, p remains the original child node of its parent, and slipNode is a new child node of parentOfP
    
    node * splitNode = new node();
    p->num_values = 1;
    splitNode->num_values = 1;
    splitNode->value[0] = p->value[2]; // splitNode's value is the third value number of p
    
    // if p has child node, update their relationship
    if (p->is_leaf == 0) {
        splitNode->is_leaf = 0;
        splitNode->child[0] = p->child[2];
        p->child[2]->parent = splitNode;
        p->child[2]->child_state = 0;
        p->child[2] = nullptr;
        
        splitNode->child[1] = p->child[3];
        p->child[3]->parent = splitNode;
        p->child[3]->child_state = 1;
        p->child[3] = nullptr;
    }
    
    // # p is the root
    if (p == root)
    {
        node * newRoot = new node();
        root = newRoot;
        newRoot->num_values = 1;
        newRoot->value[0] = p->value[1];
        newRoot->child_state = -1;
        newRoot->is_leaf = 0;
        
        newRoot->child[0] = p;
        p->parent = newRoot;
        
        newRoot->child[1] = splitNode;
        splitNode->parent = newRoot;
        
        p->child_state = 0;
        splitNode->child_state = 1;
    }
    // # p isn't the root
    else
    {
        node * parentOfP = p->parent; // the parent node of p
        parentOfP->num_values++;
        
        splitNode->parent = parentOfP;
        
        // ## p the third child node of its parent
        if (p->child_state == 2)
        {
            splitNode->child_state = 3;
            
            parentOfP->value[2] = p->value[1]; // promote the middle value of p node to upper layer
            parentOfP->child[3] = splitNode;
        }
        // ## p is the second child node of its parent
        else if (p->child_state == 1)
        {
            parentOfP->value[2] = parentOfP->value[1];
            parentOfP->value[1] = p->value[1];
            
            if (parentOfP->child[2] != nullptr) {
                parentOfP->child[3] = parentOfP->child[2];
                parentOfP->child[3]->child_state = 3;
            }
            
            parentOfP->child[2] = splitNode;
            splitNode->child_state = 2;
        }
        // ## p is the first child node of its parent
        else if (p->child_state == 0)
        {
            parentOfP->value[2] = parentOfP->value[1];
            parentOfP->value[1] = parentOfP->value[0];
            parentOfP->value[0] = p->value[1];
            
            if (parentOfP->child[2] != nullptr) {
                parentOfP->child[3] = parentOfP->child[2];
                parentOfP->child[3]->child_state = 3;
            }
            
            parentOfP->child[2] = parentOfP->child[1];
            parentOfP->child[2]->child_state = 2;
            
            parentOfP->child[1] = splitNode;
            splitNode->child_state = 1;
        }
    }
    
    p->value[1] = 0;
    p->value[2] = 0;
    
}

void node::add_value(int k)
{
    // # the node has two values
    if (num_values == 2)
    {
        int temp0 = value[0];
        int temp1 = value[1];
        
        if (k < value[0]) // ## k is smaller that the smallest value of p
        {
            value[0] = k;
            value[1] = temp0;
            value[2] = temp1;
        }
        else if (k > value[0] && k < value[1]) // ## k is in between
        {
            value[1] = k;
            value[2] = temp1;
        }
        else // ## k is largger that the larggest value of p
        {
            value[2] = k;
        }
    }
    // # the node has one value
    else if (num_values == 1)
    {
        if (k > value[0])
        {
            value[1] = k;
        }
        else
        {
            int temp = value[0];
            value[0] = k;
            value[1] = temp;
        }
    }
    // # the node has no value
    else if (num_values == 0)
    {
        value[0] = k;
    }
    
    num_values++;
}

node * two34_tree::find(int k)
{
    node * curr;
    if (root == nullptr)
    {
        curr = new node();
        root = curr;
        curr->child_state = -1;
        return curr;
    }
    else
    {
        curr = root;
        while (curr->is_leaf != 1 || curr->num_values == 3)
        {
            if (curr->num_values == 3)
            {
                break_3_value_node(curr);
                curr = curr->parent;
            }
            
            for (int i = 0; i < curr->num_values; i++)
            {
                if (k < curr->value[i])
                {
                    curr = curr->child[i];
                    break;
                }
                else if (i == curr->num_values - 1)
                {
                    curr = curr->child[i + 1];
                    break;
                }
            }
        }
        return curr;
    }
}

void two34_tree::add(int k)
{
    node * returnNode = find(k);
    returnNode->add_value(k);
}

void two34_tree::in_order_traversal(node * p){
    cout << endl;
    node * p1 = p;
    if (p1 == nullptr) return;
    int i;
    for (i = 0; i < p1->num_values; i++) {
        in_order_traversal(p1->child[i]);
        cout << "  " << p1->value[i] << "  " << "child_state = " << p1->child_state;
    }
    in_order_traversal(p1->child[i]);
}


int main() {
    //The following is just an example.
    //Different test casess can be used during grading.
    
    two34_tree t1;
    t1.add(40);
    t1.add(30);
    t1.add(20);
    t1.in_order_traversal(t1.root);
    t1.add(100);
    t1.add(120);
    t1.in_order_traversal(t1.root);
    t1.add(200);
    t1.in_order_traversal(t1.root);
    t1.add(400);
    t1.in_order_traversal(t1.root);
    t1.add(600);
    t1.in_order_traversal(t1.root);
    t1.remove(20);
    t1.in_order_traversal(t1.root);
    t1.remove(200);
    t1.in_order_traversal(t1.root);
    t1.remove(100);
    t1.in_order_traversal(t1.root);
    getchar();
    getchar();
    return 0;
}


