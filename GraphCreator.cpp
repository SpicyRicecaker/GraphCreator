/*
  Author: Shengdong Li
  Date: 3/28/2020
  GraphCreator:
  A program that can create vertices and edges between vertices,
  remove vertices along with all edges connected to it,
  remove edges between two vertices,
  and finally, use dijkstra's algorithm to find the shortest path from one vertex to another.
 */
#include <iostream>
#include <cstring>
#include <vector>
#include <iterator>
#include <map> //Maps are used mainly in Dijkstra's algorithm where we need to retrieve distance and previous node
#include "Node.h"

using namespace std;

#define INFINITY 2147483647

//From https://stackoverflow.com/questions/4157687/using-char-as-a-key-in-stdmap User: GWW
//Compares two char* by their string value instead of their pointer. This code was modified to account for null values of char*
struct cmp_str
{
    bool operator()(const char* a, const char* b) const {
        return ((a != nullptr) && (b != nullptr) && (strcmp(a, b) < 0));
    }};
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
//Add edge except with no user inputs. Used this for debug
void addEdgeLite(vector<Node*> &nodeList, char* src, char* dest, int weight);
//Asks user to input label, remove it from the graph
void removeVertex(vector<Node*> &nodeList, char* in);
//Asks user to input two labels, removes edge
void removeEdge(vector<Node*> &nodeList, char* in);
//Uses Dijkstra's Algorithm to find a path between the first vertex and the last vertex. Return shortest path if it exists, or no paths
//Graph, Distance, Prev, Visited, Starting node
void findShortestPath(vector<Node*> &nodeList, char* in);
//Prints out the adjacency list
void dijkstraAlgorithm(vector<Node*> &nodeList, map<char*, char*, cmp_str> &parent, map<char*, int, cmp_str> &distance, char* starting);
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
    //Holds list of vertices
    vector<Node*> nodeList;

    //Running!
    bool running = true;

    //Beginning prompt
    cout << "Welcome to Graph Creator. Enter \"help\" for a list of commands." << endl;

    //Main program loop, handles all functions
    while(running){
        //Get user input then chooses the appropriate function to call
        getInput(in);
        //Upper case so user can mix cases
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
            findShortestPath(nodeList, in);
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

//Get input takes user input and stores it into char* in, with some minimal input checking
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
    //Edges are NULL, label is in, and weight is -1 (weight is never accessed tho)
    nodeList.push_back(new Node(NULL, in, -1));
    cout << "\"" << in << "\" was successfully entered into the graph!" << endl;
}

//Prints out the adjacency list by iterating through the vector of nodes and traversing the linked list for each
void graph(vector<Node*>& nodeList){
    //Create iterator
    vector<Node*>::iterator it;
    //If iterator already is at the end, graph is empty
    if(it == nodeList.end()){
        cout << "The graph is empty!" << endl;
    }
    //Iterate through vertices
    for(it = nodeList.begin(); it != nodeList.end(); it++){
        //Iterate through linked list
        //We have curr and past here just to make the syntax look a bit better (i.e. ", and x")
        Node* curr = (*it)->getNext();
        if(curr == NULL){
            cout << "For node \"" << (*it)->getLabel() << "\", there are no adjacent nodes." << endl;
        }else{
            //Print each vertex by traversing linked list
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
    //Ask user for number as weight
    cout << "Please enter the weight of this edge." << endl;
    while(true){
        getInput(in);
        int inLen = strlen(in);
        bool alldig = true;
        //Check to make sure that all digits are ints. No segfaults here!
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
    //Loop through the initial node list and makes sure that the first node exists
    vector<Node*>::iterator it;
    //This stores the spot at which the first node was found. First program to be doing this!
    vector<Node*>::iterator firstNodeSpot;
    bool firstFound = false;
    bool secondFound = false;
    for(it = nodeList.begin(); it != nodeList.end(); ++it){
        //If the first node was found
        if(strcmp((*it)->getLabel(),firstNodeLabel) == 0){
            //Record spot and update
            firstFound = true;
            firstNodeSpot = it;
        }else if(strcmp((*it)->getLabel(),secondNodeLabel) == 0){
            //Update
            secondFound = true;
        }
    }
    //Different debug messages for each scenario
    if(firstFound == false){
        if(secondFound == false){
            cout << "The node first and second \"" << firstNodeLabel << "\" and \"" << secondNodeLabel << "\" were not found...therefore an edge was not added." << endl;
            return;
        }
        cout << "The first node \"" << firstNodeLabel << "\" was not found...therefore an edge was not added." << endl;
    }else if(secondFound == false){
        cout << "The second node \"" << secondNodeLabel << "\" was not found... therefore and edge was not added." << endl;
    }else{
        //Traverse the list and make sure that the second node DNE, so that we don't add duplicate edges
        Node* curr = (*firstNodeSpot);
        while(true){
            //If the second node exists
            if(strcmp(curr->getLabel(), secondNodeLabel) == 0){
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
        //If everything is valid, make sure that the second node exists, then add. (Really efficient I know)
        curr->setNext(new Node(NULL, secondNodeLabel, weight));
        cout << "An edge with weight \"" << weight << "\" was successfully added between \"" << firstNodeLabel << "\" and \"" << secondNodeLabel << "\"!" << endl;
        return;
    }
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
            //I hope that there aren't any issues with this because of references! (There weren't, yay!)
            //Iterate and delete the whole linked list
            removeList((*it));
            //Then delete the head from the node list
            it = nodeList.erase(it);
            deletedNodes++;
        }else{
            //We also need to delete all possible connections to this vertex, so we go through the linked list of any vertex (really inefficient but you have to do this with adjacency lists)
            int befLength = nodeLength((*it));
            //Otherwise, traverse the linked list and scan for equivalent connections, remove them
            removeNode(in, (*it));
            deletedConnections += (befLength - nodeLength((*it)));
            ++it;
        }
    }
    //If there were no deleted nodes, notify, otherwise present good news
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
            //Set current to NULL
            (*current) = NULL;
            //Set past child to current's child
            (*past)->setNext(tempChild);
            //Set current to its child
            //current = &tempChild;
        }else{
            //Move up
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
                cout << "The second node \"" << secondNodeLabel << "\" was not found...therefore an edge was not deleted." << endl;
            }else{
                //If there were deleted edges, notify
                cout << "An edge was successfully deleted between \"" << firstNodeLabel << "\" and \"" << secondNodeLabel << "\"!" << endl;
            }
            return;
        }
    }
    cout << "The first node \"" << firstNodeLabel << "\" was not found...therefore an edge was not deleted." << endl;
}

//Uses dijkstra's to find shortest path - so many ways to implement, I chose priority queue
void findShortestPath(vector<Node*> &nodeList, char* in){
    char firstNodeLabel[999];
    char secondNodeLabel[999];
    //Asks user for two labels
    cout << "Please enter the label for the source node." << endl;
    getInput(in);
    strcpy(firstNodeLabel, in);
    cout << "Please enter the label for the destination node." << endl;
    getInput(in);
    strcpy(secondNodeLabel, in);
    //Our maps need cmp_str because otherwise the map would look at the address of char* instead of the actual string value
    map<char*, char*, cmp_str> parent;
    map<char*, int, cmp_str> distance;
    //Loop through the initial node list and makes sure that the first node does exist
    bool firstNodeValid = false;
    bool secondNodeValid = false;
    vector<Node*>::iterator it;
    for(it = nodeList.begin(); it != nodeList.end(); ++it){
        //If the first node was found
        if(strcmp((*it)->getLabel(),firstNodeLabel) == 0){
            //Update bool!
            firstNodeValid = true;
        }
        //Traverse the list and make sure that the second node exists
        Node* curr = (*it);
        while(true){
            //If the second node exists
            if(strcmp(curr->getLabel(), secondNodeLabel)==0){
                //Update bool!
                secondNodeValid = true;
                break;
            }
            if(curr->getNext()!=NULL){
                curr = curr->getNext();
            }else{
                break;
            }
        }
    }
    //Output bad news if needed
    if(firstNodeValid == false){
        cout << "The first node \"" << firstNodeLabel << "\" was not found...therefore an edge was not added." << endl;
        return;
    }else if(secondNodeValid == false){
        cout << "The second node \"" << secondNodeLabel << "\" was not found...therefore an edge was not added." << endl;
        return;
    }
    //
    //
    //
    // ----------We pass in our graph, starting node, then previous nodes of current node and distance to get to node!----------
    //
    dijkstraAlgorithm(nodeList, parent, distance, firstNodeLabel);
    //
    // ----------We should get back *parent*, the the node before a given node in a graph that eventually leads back to the shortest path if recursed enough, along with *distance*, the shortest possible distance that each node is away from the start.
    //
    //
    //
    //Make sure that there is a valid path
    if(distance[secondNodeLabel] != INFINITY){
        //Print shortest path
        cout << "The shortest path from \"" << firstNodeLabel << "\" to \"" << secondNodeLabel << "\" is ";
        //To get the shortest path, we start at our destination, then keep on calling distance[] while pushing it into the vector to get a path
        char* current = secondNodeLabel;
        vector<char*> path;
        while(true){
            //push into vector
            path.push_back(current);
            //If we get back to the beginning, our path is done
            if(strcmp(current, firstNodeLabel) == 0){
                break;
            }
            //Reiterate
            current = parent[current];
        }
        //We need a reverse vector to go from end to beginning, along with its syntax
        vector<char*>::reverse_iterator pathIt;
        for(pathIt = path.rbegin(); pathIt != path.rend(); ++pathIt){
            cout << (*pathIt) << " ";
        }
        //The distance we already know, because that's the distance that a specific node is away from the starting node
        cout << "with a cost of " << distance[secondNodeLabel] << "!" << endl;
    }else{
        cout << "There is no path from \"" << firstNodeLabel << "\" to \"" << secondNodeLabel << "\"." << endl;
    }
}

//A clone of add edge except without user input. Mostly for debugging purposes.
void addEdgeLite(vector<Node*> &nodeList, char* firstNodeLabel, char* secondNodeLabel, int weight){
    //Loop through the initial node list and makes sure that the first node does exist
    vector<Node*>::iterator it;
    for(it = nodeList.begin(); it != nodeList.end(); ++it){
        //If the first node was found
        if(strcmp((*it)->getLabel(),firstNodeLabel) == 0){
            //Traverse the list and make sure that the first node DNE
            Node* curr = (*it);
            while(true){
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

//Returns a map of the parent and distance for all nodes by using dijkstra's algorithm to find the shortest length from a beginning node to all nodes. Uses (scuffed) priority queue for implementation!
void dijkstraAlgorithm(vector<Node*> &nodeList, map<char*, char*, cmp_str> &parent, map<char*, int, cmp_str> &distance, char* starting){
    //Define visited map, true = visited, false = unvisited.
    //We should be able to just plugin the label for a node and get if it has been visited or not.
    map<char*, bool, cmp_str> visited;
    //This queue is so big brain. If we pick the min from the queue, it's always BFS
    vector<char*> queue;
    //Initialize our maps with default values as specified in Dijkstra's Algorithm
    vector<Node*>::iterator nodeListIt;
    for(nodeListIt = nodeList.begin(); nodeListIt != nodeList.end(); ++nodeListIt){
        //First mark all nodes as unvisited
        visited[(*nodeListIt)->getLabel()] = false;
        //Then define distance to all other nodes as infinity
        distance[(*nodeListIt)->getLabel()] = INFINITY;
        //Then define previous nodes for all nodes as null, as nothing has been visited yet; we don't have a previous
        parent[(*nodeListIt)->getLabel()] = NULL;
        //Then add the node to queue.
        //We add all vertices to the queue so that they will all be visited eventually.
        queue.push_back((*nodeListIt)->getLabel());
    }
    //Set starting node to current node by setting the distance to that node as 0.
    //This makes sense, because if we want to find the path from "A" to "A" it is just 0
    distance[starting] = 0;
    //The line below can be used for different implementations of Dijkstra's, like recursion
    //char* currentNode = starting;
    //
    //
    //Ok I get it we learned heaps to find max/min very efficiently what's below is REALLY INEFFICIENT but idc ehehe
    //
    //
    //While queue is not empty, this means that while we haven't visited every node yet
    while(!queue.empty()){
        //Choose the node with the least distance from the starting node in the queue (this makes it BFS!)
        //Implementation for finding the smallest number in this queue is really inefficient but we don't talk about that
        vector<char*>::iterator queueIt;
        int leastDist = INFINITY;
        char* leastLabel = (char*)"";
        for(queueIt = queue.begin(); queueIt != queue.end(); ++queueIt){
            //If distance is smaller
            if(distance[(*queueIt)] <= leastDist){
                //Set it as the least, and update least dist
                leastDist = distance[(*queueIt)];
                leastLabel = (*queueIt);
            }
        }
        //Remove the node from the queue as we're about to visit it rn
        for(queueIt = queue.begin(); queueIt != queue.end(); ++queueIt){
            //Simple iteration through vector and value matching deletion
            if(strcmp(leastLabel, (*queueIt)) == 0){
                queueIt = queue.erase(queueIt);
                break;
            }
        }
        //Mark node as visited also, so we don't add the neighbors of this node into the queue again
        visited[leastLabel] = true;
        //Add all unvisited neighboring nodes of this node to queue, to do this we have to traverse linked list (like what???? Why did I use linked lists at all??????????)
        for(nodeListIt = nodeList.begin(); nodeListIt != nodeList.end(); ++nodeListIt){
            //Find the least label node's location in the nodeList
            if(strcmp((*nodeListIt)->getLabel(), leastLabel) == 0){
                //Traverse list of neighbors
                Node* temp = (*nodeListIt);
                //For each neighbor
                while(true){
                    //If we're at the end of the list of neighbors, leavee
                    if(temp == NULL){
                        break;
                    }
                    //Make sure that the neighbor has not been visited!
                    if(!visited[temp->getLabel()]){
                        //Calculate the distance to node
                        int distToNode = distance[leastLabel] + temp->getWeight();
                        //If distance to node is less than the current distance of the node (in other words, if this new path to the neighbor is less cost)
                        if(distToNode < distance[temp->getLabel()]){
                            //Then update distance.
                            distance[temp->getLabel()] = distToNode;
                            //Update previous node, so we can trace back the most efficient path later!
                            parent[temp->getLabel()] = leastLabel;
                        }
                    }
                    //Move onto the next neighbor
                    temp = temp->getNext();
                }
            }
        }
    }
    //Below is a really early pseudocode for Dijkstra's
    //
    //Add all list of unvisited neighbors to queue
    //Calculate the distance to the neighbor node by adding the distance of the current node to the weight of the edge that connects the current node to the neighboring node
    //Compare edge distance to current distance assigned to neighboring node.
    //If edge weight is smaller than the weight of neighboring node, set it as new weight, otherwise keep previous weight
    //Mark current node as visited
    //Select unvisited node with smallest distance
    //Set it as new current node, return to step 4
}
