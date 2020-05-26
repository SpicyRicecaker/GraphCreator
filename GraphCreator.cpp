#include <iostream>
#include <cstring>
#include <vector>
#include <iterator>
#include "Node.h"

using namespace std;

//Gets user input
void getInput(char* in);
//Prints list of commands
void help();
//Quits the program
void quit(bool &running);
//Takes in user input and returns an int
int getAction();
//Asks user to input label, adds it as a vertex to the garph
void addVertex(vector<Node*> &nodeList, char* in);
//Asks user to input two labels and a weight, adds it as the weight between nodes
void addEdge();
//Asks user to input label, remove it from the graph
void removeVertex();
//Asks user to input two labels, removes edge
void removeEdge();
//Uses Dijkstra's Algorithm to find a path between the first vertex and the last vertex. Return shortest path if it exists, or no paths
void findShortestPath();
//Prints out the adjacency list
void graph(vector<Node*> &nodeList);

int main(){
    //Holds input
    char inArray[999];
    char* in = &inArray[0];
    //Holds list of nodes
    vector<Node*> nodeList;

    //Running!
    bool running = true;

    cout << "Welcome to Graph Creator. Enter \"help\" for a list of commands." << endl;
    while(running){
        //Get user input then chooses the appropriate function to call
        getInput(in);
        int inLen = strlen(in);
        for(int a = 0; a < inLen; ++a){
            in[a] = toupper(in[a]);
        }
        //addVertex()
        if(strcmp(in, "ADDVERTEX") == 0){
            addVertex(nodeList, in);
        }
        //addEdge()
        else if(strcmp(in, "ADDEDGE") == 0){

        }
        //addEdge()
        else if(strcmp(in, "REMOVEVERTEX") == 0){

        }
        //removeEdge()
        else if(strcmp(in, "REMOVEEDGE") == 0){

        }
        //findShortestPath()
        else if(strcmp(in, "FINDSHORTESTPATH") == 0){

        }
        //graph()
        else if(strcmp(in, "GRAPH") == 0){
            graph(nodeList);
        }
        //help()
        else if(strcmp(in, "HELP") == 0){
            help();
        }
        //quit()
        else if(strcmp(in, "QUIT") == 0){
            quit(running);
        }
        //reprompt
        else{
            cout << "The command \"" << in << "\" was not recognized. Please type a new command, or enter \"help\" for a list of commands!" << endl;
        }
    }
    return 0;
}

void getInput(char* in){
    while(true){
        cin.getline(in,999);
        if(strcmp(in, "") != 0){
            break;
        }
        cout << "No input detected. Please enter something..." << endl;
    }
}

//Prints list of valid commands.
void help(){
    cout << "\n----------\n\"addvertex\" to add a vertex to the tree,\n\"addedge\" to add an edge to the tree,\n\"removevertex\" to remove an edge,\n\"findshortestpath\" to find shortest path from one node to the other,\n\"graph\" to print the adjacency list for this graph,\n\"quit\" to exit this program\n\"help\" to reprint this list.\n----------\n" << endl;
}

//Stops program execution by setting running to false
void quit(bool &running){
    running = false;
}

//Pushes back a new node into the list of vertices
void addVertex(vector<Node*> &nodeList, char* in){
    cout << "Please enter the label for this new vertex" << endl;
    getInput(in);
    nodeList.push_back(new Node(NULL, in, -1));
    cout << "\"" << in << "\" was successfully entered into the graph!" << endl;
}

//Prints out the adjacency list by iterating through the vector of nodes and traversing the linked list for each
void graph(vector<Node*>& nodeList){
    //Create iterator
    vector<Node*>::iterator it;
    //Iterate through vertices
    if(it == nodeList.end()){
        cout << "The graph is empty!" << endl;
    }
    for(it = nodeList.begin(); it != nodeList.end(); it++){
        //Iterate through linked list
        //We have curr and past here just to make the syntax look a bit better (i.e. ", and x")
        bool neighbors;
        Node* curr = (*it)->getNext();
        if(curr == NULL){
            cout << "For node \"" << (*it)->getLabel() << "\", there are no adjacent nodes." << endl;
        }else{
            //Print vertex
            cout << "For node \"" << (*it)->getLabel() << "\", the adjacent nodes are: ";
            while(curr!=NULL){
                cout << curr->getLabel() << " ";
                curr = curr->getNext();
            }
            cout << endl;
        }
    }
}
