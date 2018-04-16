// Author: Beier Chen
// 554 HW3
// Date: February 25 2018

#include <iostream>
#include <list>
#include <map>
#include <string>
using namespace std;

class course {
public:
    string name;
    int section;
    int credits;
    course() {}
    course(string n, int s, int c) { name = n; section = s; credits = c; }
    bool operator<(const course &c) { return (name < c.name); }
    bool operator==(const course &c) { return (name == c.name); }
};
void add_student(map<int, map<int, list<course *> * > *> &DB, int id);
void remove_student(map<int, map<int, list<course *> * > *> &DB, int id);
void add_course(map<int, map<int, list<course *> * > *> &DB, int semester, int id, course c);
    //20171 Spring semester of 2017; 20172: Fall semester of 2017

void drop_course(map<int, map<int, list<course *> * > *> &DB, int semester, int id, course c);
void print_student_semester_courses(map<int, map<int, list<course *> * > *> &DB, int semester, int id);
void print_student_all_courses(map<int, map<int, list<course *> * >  *> &DB, int id);
void print_DB(map<int, map<int, list<course *> * >  *> &DB);

int main() {
    map<int, map<int, list<course  *> *>   *> DB;
    add_student(DB, 11111);
    course C1("CIS554", 1, 3), C2("CSE674", 1, 3), C3("MAT296", 8, 4), C4("WRT205", 5, 3);
    
    add_course(DB, 20171, 11111, C1);
    add_course(DB, 20171, 11111, C4);
    add_course(DB, 20171, 11111, C3);
    add_course(DB, 20171, 11111, C2);
    print_student_semester_courses(DB, 20171, 11111);
    
    drop_course(DB, 20171, 11111, C1);
    print_student_semester_courses(DB, 20171, 11111);
    
    add_course(DB, 20172, 11111, C2);
    add_course(DB, 20172, 11111, C4);
    add_course(DB, 20172, 11111, C3);
    add_course(DB, 20172, 11111, C1);
    print_student_all_courses(DB, 11111);
    
    add_student(DB, 11112);
    add_course(DB, 20171, 11112, C2);
    add_course(DB, 20171, 11112, C4);
    add_course(DB, 20171, 11112, C3);
    add_course(DB, 20171, 11112, C1);
    print_student_semester_courses(DB, 20171, 11112);
    
    add_course(DB, 20172, 11112, C2);
    add_course(DB, 20172, 11112, C4);
    add_course(DB, 20172, 11112, C3);
    add_course(DB, 20172, 11112, C1);
    print_student_semester_courses(DB, 20172, 11112);
    
    print_student_all_courses(DB, 11112);
    
    print_DB(DB);
    remove_student(DB, 11112);
    print_DB(DB);
    getchar();
    getchar();
    return 0;
}

void add_student(map<int, map<int, list<course *> * > *> &DB, int id) {
    
    if (DB.size() == 0) {
        DB[id] = new map<int, list<course *> * >;
        return;
    }
    
    auto it1 = DB.find(id);//note the find syntax; different from find for list and vector
    if (it1 == DB.end()) {
        DB[id] = new map<int, list<course *> * >;
        return;
    }
}

void remove_student(map<int, map<int, list<course *> * > *> &DB, int id) {
    
    if(DB.find(id) != DB.end()){
        
        auto it_semester = DB[id]->begin();
        while(it_semester != DB[id]->end()){
            
            auto it_list = (*it_semester).second->begin();
            while(it_list != (*it_semester).second->end()){
                
                // ---< problem >---
                // use this for VS:
                // delete *it_list;
                
                // but don't why this is different than drop_corse
                // confusing...

                (*it_semester).second->erase(it_list);
                it_list++;
            }

	    // don't use this: it_semester->second->erase(it_semester->first);
            delete it_semester->second;
            it_semester++;
        }
        DB.erase(id);
    }
}

void add_course(map<int, map<int, list<course *> * > *> &DB, int semester, int id, course c) {
    
    // check if student exists in DB
    if(DB.find(id) == DB.end()){
        return;
    }
    
    map<int, list<course *> * >::iterator it_semester_map = DB[id]->find(semester);
    
    // check is this semester exist in semester_map
    // creat a new one if not exist
    if (it_semester_map == DB[id]->end()) {
        (*DB[id])[semester] = new list<course *>;
    }
    
    list<course *>::iterator it_course_list = ((*DB[id])[semester])->begin();
    
    while (it_course_list != ((*DB[id])[semester])->end()) {
        
        // ---< problem >---
        // don't use this : *it_course_list == &c
        // &c is location of c, not a pointer to c
        if (**it_course_list == c) {
            return;
        }
        
        it_course_list++;
    }
    
    // ---< problem >---
    // don't use this: (*DB[id])[semester]->push_back(&c);
    // when leave this function, c will be delete, so DB cannot acess it again.
    // Xcode maybe don't delete stack, so it could, but cannot work on VS
    (*DB[id])[semester]->push_back(new course(c));
}

void drop_course(map<int, map<int, list<course *> * > *> &DB, int semester, int id, course c) {
    
    auto it_list = (*DB[id])[semester]->begin();
    
    while (it_list != (*DB[id])[semester]->end()){
        
        if( **it_list == c ){
            
            // ---< problem >---
        
             (*DB[id])[semester]->erase(it_list); // this work for Xcode, but not for VS
            
            // erase will also remove the iterator, thus it_list++ will give error
            // don't know why it works in Xcode
            // had to add this in VS: delete *it_list;
            
            return;
        }
        it_list++;
    }
}

void print_student_semester_courses(map<int, map<int, list<course *> * > *> &DB, int semester, int id) {
    cout << endl;
    cout << "student id = " << id << endl;
    cout << "semester = " << semester << endl;
    
    list<course *>::iterator it_course_list = ((*DB[id])[semester])->begin();
    
    while (it_course_list != ((*DB[id])[semester])->end()) {
        
        course * curr = *it_course_list;
        cout << curr->name << " " << curr->section << " " << curr->credits << " " ;
        
        it_course_list++;
    }
    cout << endl;
}

void print_student_all_courses(map<int, map<int, list<course *> * > *> &DB, int id) {
    
    auto it_semester = DB[id]->begin();
    cout << endl;
    cout << "student id = " << id;
    
    while (it_semester != DB[id]->end()) {
        cout << endl;
        cout << "semester = " << (*it_semester).first << endl;
        
        auto it_list = (*it_semester).second->begin();
        
        while (it_list != (*it_semester).second->end()) {
            
            cout << (**it_list).name << " " << (**it_list).section << " " << (**it_list).credits << " ";
            
            it_list++;
        }
        
        it_semester++;
    }
    cout << endl;
}

void print_DB(map<int, map<int, list<course *> * >  *> &DB) {
    cout << endl;
    auto it_stu_map = DB.begin();
    
    while(it_stu_map != DB.end()){
        cout << "student id = " << (*it_stu_map).first;
        
        auto it_semester_map = (*it_stu_map).second->begin();
        
        while(it_semester_map != (*it_stu_map).second->end()){
            
            cout << endl;
            cout << "semester = " << (*it_semester_map).first << endl;
            
            auto it_list = (*it_semester_map).second->begin();
            
            while (it_list != (*it_semester_map).second->end()){
                
                cout << (**it_list).name << " " << (**it_list).section << " " << (**it_list).credits << " ";
                it_list++;
            }
            it_semester_map++;
        }
        cout << endl;
        it_stu_map++;
    }
}

