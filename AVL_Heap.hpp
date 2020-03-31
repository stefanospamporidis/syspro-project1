#ifndef AVL_Heap_hpp
#define AVL_Heap_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <set>
#include <utility>
#include <math.h>
#include "Database.hpp"
#include "Structs.hpp"

using namespace std;

class CaseListNode;

class AVL_node{
public:
    size_t entry_date;
    AVL_node *left;
    AVL_node *right;
    int height;
    CaseListNode *patient_node;
    AVL_node(size_t entry_date, CaseListNode *patient_node){
        this->entry_date = entry_date;
        left = NULL;
        right = NULL;
        height = 1;
        this->patient_node = patient_node;
    }
    ~AVL_node(){}

};

class AVL_tree{
public:
    AVL_node *root;
    size_t counter; //to pli8os twn komvwn tou dentrou
    AVL_tree(){
        root = NULL;
        counter = 0;
    }
    ~AVL_tree();
    void delete_recursive(AVL_node * root);
    void insert(size_t entry_date,CaseListNode *patient_node);
    int node_counter(AVL_node* node,size_t date_size1,size_t date_size2);
    int get_patient_frequency(size_t date_size1,size_t date_size2);

    int node_counter(AVL_node* node,size_t date_size1,size_t date_size2,string country);
    int get_patient_frequency(size_t date_size1,size_t date_size2,string country);

    int active_cases_counter(AVL_node* node);
    int get_active_cases();

    AVL_node* insert(AVL_node* node, size_t entry_date,CaseListNode *patient_node);
    int getBalance(AVL_node* node);
    AVL_node* leftRotate(AVL_node* node);
    AVL_node* rightRotate(AVL_node* node);
    int max(int a, int b);
    int height(AVL_node* node);

};



class Heap_element{
public:
    Heap_element * left;
    Heap_element * right;
    Heap_element * parent;
    int key;
    string disease_country;
};

class MaxHeap{
public:
    Heap_element * root;
    Heap_element *last_node;
    int current_index;
    MaxHeap(){
        root = NULL;
        last_node = NULL;
        current_index = 0;
    }
    bool isEmpty(){
        return root == NULL ;
    }
    void fillHeap_Countries(AVL_node* node,size_t date_size1,size_t date_size2); //gemizei to heap me vasi tis imerominies pou dwsame me xwres kai krousmata
    void fillHeap_Countries(AVL_node* node); //gemizei to heap me ola ta nodes AVL tree me xwres kai krousmata stis xwres apo to sugkekrimeno io
    void fillHeap_Diseases(AVL_node* node,size_t date_size1,size_t date_size2);
    void fillHeap_Diseases(AVL_node* node);
    void insertKey(int key,string disease_country);
    Heap_element * nextPlaceInHeap();
    Heap_element * parent(Heap_element * root, int i);  
    void increaseKey(Heap_element * leaf); // "anevazei" ena fulo an to key tou einai megalutero apo tou "goniou" tou mexri na min einai
    void decreaseKey(Heap_element * element); //"vu8izei" ena fulo an to key tou einai mikrotero apo ena apo ta paidia tou
    void swap(Heap_element * a, Heap_element * b); //allazei to periexomeno meta3u twn antikeimenwn a kai b pou einai fula tou dentrou
    bool update(int key, string disease_country);
    Heap_element * findNode(Heap_element * element, string disease_country);
    void getMax();
    void print_inorder(){
        inorder(root);
    }

    void inorder(Heap_element* p){
        if(p != NULL)
        {
            if(p->left) inorder(p->left);
            cout<<" "<<p->key<<" ";
            if(p->right) inorder(p->right);
        }
        else return;
    }
    ~MaxHeap(){
        deleteHeap(root);
    }
    void deleteHeap(Heap_element * element){
        if(element != NULL){
            if(element->left) deleteHeap(element->left);
            if(element->right) deleteHeap(element->right);
            delete(element);
        }   
        else return;
    }


};


#endif
