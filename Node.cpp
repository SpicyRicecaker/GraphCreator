#include <iostream>
#include <cstring>
#include "Node.h"

//Constructor for edge
Node::Node(Node* newNext, char* newLabel, int newWeight){
    next = newNext;
    //We have to allocate space for label as we're initializing it in a function at times and not in main
    label = new char[strlen(newLabel)];
    strcpy(label, newLabel);
    weight = newWeight;
}
//Destructor for edge
Node::~Node(){
    //delete[] for [] objects, just delete for regular
    delete[] label;
}
//Getters and Setters for values
Node* Node::getNext(){
    return next;
}
void Node::setNext(Node* newNext){
    next = newNext;

}

char* Node::getLabel(){
    return label;
}
void Node::setLabel(char* newLabel){
    //Deallocate contents of current label
    delete[] label;
    //Reallocate space for new label
    label = new char[strlen(newLabel)];
    //Copy values over
    strcpy(label, newLabel);
}

int Node::getWeight(){
    return weight;
}
void Node::setWeight(int newWeight){
    weight = newWeight;
}
