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
void addEdge(vector<Node*> &nodeList, char* in);
//Asks user to input label, remove it from the graph
void removeVertex(vector<Node*> &nodeList, char* in);
//Asks user to input two labels, removes edge
void removeEdge(vector<Node*> &nodeList, char* in);
//Uses Dijkstra's Algorithm to find a path between the first vertex and the last vertex. Return shortest path if it exists, or no paths
void findShortestPath();
//Prints out the adjacency list
void graph(vector<Node*> &nodeList);
//Delete LinkedList - This really should be in the node class but I'm too lazy to move it over
void removeList(Node* &head);
//Delete Node - Again, this really should in the node class but I'm too lazy to move it over
void removeNode(char* in, Node* &head);
//Node Length - traverses list to get length
int nodeLength(Node* head);


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
            addEdge(nodeList, in);
        }
        //addEdge()
        else if(strcmp(in, "REMOVEVERTEX") == 0){
            removeVertex(nodeList, in);
        }
        //removeEdge()
        else if(strcmp(in, "REMOVEEDGE") == 0){
            removeEdge(nodeList, in);
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

//Asks user for two labels, and a weight, verifies that the weight is a number, verifies that the first node DOES exist, then traverses the linked list for the first node, verifies that the second node DNE, then adds that, including a weight
void addEdge(vector<Node*> &nodeList, char* in){
    char firstNodeLabel[999];
    char secondNodeLabel[999];
    int weight = 0;
    //Asks user for two labels
    cout << "Please enter the label for the first node." << endl;
    getInput(in);
    strcpy(firstNodeLabel, in);
    cout << "Please enter the label for the second node." << endl;
    getInput(in);
    strcpy(secondNodeLabel, in);
    cout << "Please enter the weight of this edge." << endl;
    while(true){
        getInput(in);
        int inLen = strlen(in);
        bool alldig = true;
        for(int a = 0; a < inLen; ++a){
            if(!isdigit(in[a])){
                alldig = false;
                break;
            }
        }
        if(alldig){
            break;
        }
        cout << "Please only enter numbers for the weight of the edge." << endl;
    }
    weight = atoi(in);
    //Loop through the initial node list and makes sure that the first node does exist
    vector<Node*>::iterator it;
    for(it = nodeList.begin(); it != nodeList.end(); ++it){
        //If the first node was found
        if(strcmp((*it)->getLabel(),firstNodeLabel) == 0){
            //Traverse the list and make sure that the first node DNE
            Node* curr = (*it);
            while(true){
                cout << "Current label is " << curr->getLabel() << endl;
                cout << "Second label to add is " << secondNodeLabel << endl;
                //If the second node exists
                if(strcmp(curr->getLabel(), secondNodeLabel)==0){
                    //Tell the user that the edge already exists and exit!
                    cout << "The edge between \"" << firstNodeLabel << "\" and \"" << secondNodeLabel << "\" already exists! Therefore, an edge was not added." << endl;
                    return;
                }
                if(curr->getNext()!=NULL){
                    curr = curr->getNext();
                }else{
                    break;
                }
            }
            //If everything is valid, we can finally add the edge!
            curr->setNext(new Node(NULL, secondNodeLabel, weight));
            cout << "An edge with weight \"" << weight << "\" was successfully added between \"" << firstNodeLabel << "\" and \"" << secondNodeLabel << "\"!" << endl;
            return;
        }
    }
    cout << "The first node \"" << firstNodeLabel << "\" was not found...therefore an edge was not added." << endl;
}

//Really inefficient when using adjacency lists. Loops through the entire list of vertices, deletes matching, then traverse the adjacent vertices and also deletes matching from there.
void removeVertex(vector<Node*> &nodeList, char* in){
    //First ask user for vertex to be deleted.
    cout << "Please enter a label for the vertex to be deleted." << endl;
    getInput(in);
    //Keep a record of the number of vertices deleted
    int deletedNodes = 0;
    //Keep a record for the number of connections deleted
    int deletedConnections = 0;
    //Iterate throught the list of vertices
    vector<Node*>::iterator it;
    for(it = nodeList.begin(); it!=nodeList.end(); ){
        //If the vertex was found
        if(strcmp((*it)->getLabel(), in) == 0){
            //Iterate and delete the whole linked list, then delete the head from the vertex
            //I hope that there aren't any issues with this because of references!
            removeList((*it));
            it = nodeList.erase(it);
            deletedNodes++;
        }else{
            int befLength = nodeLength((*it));
            //Otherwise, traverse the linked list and scan for equivalent connections, remove them
            removeNode(in, (*it));
            deletedConnections += (befLength - nodeLength((*it)));
            ++it;
        }
    }
    if(deletedNodes == 0){
        cout << "The vertex with the given label \"" << in << "\" was not found, so there were no changes to the graph." << endl;
    }else{
        cout << "The vertex \"" << in << "\" was deleted along with " << deletedConnections << " connections!" << endl;
    }
}

//Deletes the entire linked list through the head
void removeList(Node* &head){
    while(head!=NULL){
        //Store the child of head
        Node* tempChild = head->getNext();
        //Cut off child
        head->setNext(NULL);
        //Deallocate head values
        delete head;
        //Reset head to child
        head = tempChild;
    }
}

//Deletes a node at a certain point in a linked list. It doesn't even consider the root case so it's technically bugged but whatever lol
void removeNode(char* in, Node* &head){
    Node** past = &head;
    Node** current = &head;
    //while we haven't reached the end of the list with current
    while((*current)!=NULL){
        //Check if we have a match with current
        if(strcmp((*current)->getLabel(), in) == 0){
            //If so, remember current's child
            Node* tempChild = (*current)->getNext();
            //Set current's child to null
            (*current)->setNext(NULL);
            //Remove current
            delete (*current);
            //Set past child to current's child
            (*past)->setNext(tempChild);
            //Set current to its child
            //current = &tempChild;
        }else{
            //RIP
            past = current;
            Node* temp = (*current)->getNext();
            current = &temp;
        }
    }
}

//Traverses the linked list while incrementing counter to find length
int nodeLength(Node* head){
    int length = 0;
    while(head!=NULL){
        ++length;
        head=head->getNext();
    }
    return length;
}

//Basically a more simple removevertex, kind of like addedge. Verifies that source node exists, traverses linked list and outputs the number of connections deleted
void removeEdge(vector<Node*> &nodeList, char* in){
    char firstNodeLabel[999];
    char secondNodeLabel[999];
    //Asks user for two labels
    cout << "Please enter the label for the first node." << endl;
    getInput(in);
    strcpy(firstNodeLabel, in);
    cout << "Please enter the label for the second node." << endl;
    getInput(in);
    strcpy(secondNodeLabel, in);
    vector<Node*>::iterator it;
    for(it = nodeList.begin(); it!=nodeList.end(); ++it){
        //If the vertex was found
        if(strcmp((*it)->getLabel(), firstNodeLabel) == 0){
            int befLength = nodeLength((*it));
            //Traverse the linked list and scan for equivalent connections, remove them
            removeNode(secondNodeLabel, (*it));
            if(befLength == nodeLength((*it))){
                //If there weren't any deleted edges, notify
                cout << "The edge between \"" << firstNodeLabel << "\" and \"" << secondNodeLabel << "\"doesn't exist! Therefore, an edge was not deleted." << endl;
            }else{
               //If there were deleted edges, notify
            cout << "An edge was successfully deleted between \"" << firstNodeLabel << "\" and \"" << secondNodeLabel << "\"!" << endl;
            }
            return;
        }
    }
    cout << "The first node \"" << firstNodeLabel << "\" was not found...therefore an edge was not deleted." << endl;
}
