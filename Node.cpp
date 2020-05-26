#include <iostream>
#include <cstring>
#include "Node.h"

//Constructor for edge
Node::Node(Node* newNext, char* newLabel, int newWeight){
    next = newNext;
    label = new char[strlen(newLabel)];
    strcpy(label, newLabel);
    weight = newWeight;
}
//Destructor for edge
Node::~Node(){
    delete[] label;
}
//Getters and Setters for values
Node* Node::getNext(){
    return next;
}
void Node::setNext(Node* newNext){
    next = newNext;

}
int Node::getWeight(){
    return weight;
}
void Node::setWeight(int newWeight){
    weight = newWeight;
}
