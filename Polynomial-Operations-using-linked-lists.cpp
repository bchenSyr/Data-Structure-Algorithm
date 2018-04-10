//  Author: Beier Chen, bchen22@syr.edu
//  Date: Jan 29, 2018
//  Applicaiton: 554 HW1 - Polynomial Operations using linked lists

//A polynomial is implemented using a linked list.  Only non-zero coefficients are included in linked list.
//Implment member functions operator+ , operator* , and add term

#include <iostream>

using namespace std;

class term {
public:
    int power;
    int coef;
    term * next;
    term() { next = nullptr; }
    term(int c, int p) { power = p; coef = c; next = nullptr; }
};

class polynomial {
public:
    int num_terms;
    term * head;
    polynomial() { num_terms = 0; head = nullptr; }
    
    void add_term(int c, int p);
    //If the polynomial does not contain a term with power p,
    //then add this new term.
    //If the polynomial contains a term with power p,
    //then add c to the existing coef.
    
    polynomial operator+(polynomial P);
    polynomial operator*(polynomial P);
    void print_polynomial();
};


void polynomial::print_polynomial() {
    cout << endl;
    term * p = head;
    while (p != nullptr) {
        cout << p->coef << " " << p->power << "   ";
        p = p->next;
    }
}

void polynomial::add_term(int c, int p) {
    term * current = head;
    term * beforeCurr;
    term * newTerm = new term(c,p);
    
    // # the polynomial is empty, the new term is head
    if(current == nullptr){
        head = newTerm;
        num_terms++;
    }
    // # the polynomial is not empty
    else if(current->power >= p){ // ## current->power >= p,
        while(true){
            // ### the polynomial contains a term with power p, then add c to the existing coef.
            if(current->power == p){
                current->coef += c;
                break;
            }
            // ### add the new term to the end of the polynomial
            else if(current->next == nullptr){
                current->next = newTerm;
                num_terms++;
                 break;
            }
            // ### add the new term to the middle of the polynomial
            else if(current->next->power < p){
                newTerm->next = current->next;
                current->next = newTerm;
                num_terms++;
                break;
            }
            current = current->next;
        }
    }
    else { // ## current->power < p, the new term becomes head
        newTerm->next = head;
        head = newTerm;
        num_terms++;
    }
    
    // ---< delete the term with coef is 0
    beforeCurr = head;
    current = head;
    while(current != nullptr){
        if(current == head){
            if(current->coef == 0){
                head = current->next;
                beforeCurr = head;
                num_terms--;
            }
        }
        else{
            if(current->coef == 0){
                beforeCurr->next = current->next;
                num_terms--;
            }
            else{
                beforeCurr = beforeCurr->next;
            }
        }
        current = current->next;
    }
}

polynomial polynomial::operator+(polynomial P) {
    
    polynomial returnPoly;
    term * currThis = this->head;
    term * currP = P.head;

    while(currThis != nullptr || currP != nullptr){
        
        if(currThis == nullptr && currP != nullptr){
            returnPoly.add_term(currP->coef, currP->power);
            currP = currP->next;
        }
        else if(currThis != nullptr && currP == nullptr){
            returnPoly.add_term(currThis->coef, currThis->power);
            currThis = currThis->next;
        }
        else if(currThis->power == currP->power){
            returnPoly.add_term(currThis->coef + currP->coef, currThis->power);
            currThis = currThis->next;
            currP = currP->next;
        }
        else if(currThis->power != currP->power){
            if(currThis->power > currP->power){
                returnPoly.add_term(currThis->coef, currThis->power);
                currThis = currThis->next;
            }
            else{
                returnPoly.add_term(currP->coef, currP->power);
                currP = currP->next;
            }
        }
    }
    return returnPoly;
}

polynomial polynomial::operator*(polynomial P) {
    polynomial returnPoly;
    term * currThis = this->head;
    term * currP = P.head;
    
    for(currThis = this->head; currThis != nullptr; currThis = currThis->next){
        for(currP = P.head; currP != nullptr; currP = currP->next){
            returnPoly.add_term(currThis->coef * currP->coef, currThis->power + currP->power);
        }
    }
    return returnPoly;
}


int main() {
    polynomial p1, p2, p3, p4, p5, p6;
    int num_t;
    
    cout << "Enter numer of terms" << endl;
    cin >> num_t;
    cout << "Enter all terms" << endl;
    for (int i = 0; i < num_t; i++) { //You don't need to check for input error
        int c, p;
        cin >> c >> p;
        p1.add_term(c, p);
    }
    p1.print_polynomial();
    cout << endl;
    
    cout << "Enter numer of terms" << endl;
    cin >> num_t;
    cout << "Enter all terms" << endl;
    for (int i = 0; i < num_t; i++) { //You don't need to check for input error
        int c, p;
        cin >> c >> p;
        p2.add_term(c, p);
    }
    p2.print_polynomial();
    
    p3 = p1 + p2;
    p3.print_polynomial();
    p4 = p1 * p2;
    p4.print_polynomial();
    
    getchar();
    getchar();
    return 0;
}

/*
 //An example screenshot is given below:
 //Different cases may be used during grading
 
 Enter numer of terms
 3
 Enter all terms
 2 2 1 1 4 0
 
 2 2   1 1   4 0
 Enter numer of terms
 2
 Enter all terms
 -6 1 3 0
 
 -6 1   3 0
 2 2   -5 1   7 0
 -12 3   -21 1   12 0
*/
