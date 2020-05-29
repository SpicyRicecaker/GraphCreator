#include <iostream>
#include <cstring>
#include <vector>
#include <iterator>
#include <map>
#include "Node.h"

using namespace std;

#define INFINITY 2147483647

//From https://stackoverflow.com/questions/4157687/using-char-as-a-key-in-stdmap User: GWW
//Compares two char* by their string value instead of their pointer.
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
    //Holds list of nodes
    vector<Node*> nodeList;

    //Running!
    bool running = true;

    cout << "Welcome to Graph Creator. Enter \"help\" for a list of commands." << endl;


    //DEBUG DEBUG DEBUG

      nodeList.push_back(new Node(NULL, (char*)"A", -1));
      nodeList.push_back(new Node(NULL, (char*)"B", -1));
      nodeList.push_back(new Node(NULL, (char*)"C", -1));
      nodeList.push_back(new Node(NULL, (char*)"D", -1));
      nodeList.push_back(new Node(NULL, (char*)"E", -1));
      nodeList.push_back(new Node(NULL, (char*)"F", -1));
      nodeList.push_back(new Node(NULL, (char*)"G", -1));

      addEdgeLite(nodeList, (char*)"A", (char*)"C", 5);
      addEdgeLite(nodeList, (char*)"A", (char*)"B", 10);
      addEdgeLite(nodeList, (char*)"C", (char*)"D", 50);
      addEdgeLite(nodeList, (char*)"B", (char*)"D", 25);
      addEdgeLite(nodeList, (char*)"D", (char*)"E", 100);
      addEdgeLite(nodeList, (char*)"E", (char*)"F", 10);
      addEdgeLite(nodeList, (char*)"E", (char*)"G", 200);
    //DEBUG DEBUG DEBUG
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
    vector<Node*>::iterator firstNodeSpot;
    bool firstFound = false;
    bool secondFound = false;
    for(it = nodeList.begin(); it != nodeList.end(); ++it){
        //If the first node was found
        if(strcmp((*it)->getLabel(),firstNodeLabel) == 0){
            firstFound = true;
            firstNodeSpot = it;
        }else if(strcmp((*it)->getLabel(),secondNodeLabel) == 0){
            secondFound = true;
        }
    }
    if(firstFound == false){
        if(secondFound == false){
            cout << "The node first and second \"" << firstNodeLabel << "\" and \"" << secondNodeLabel << "\" were not found...therefore an edge was not added." << endl;
            return;
        }
        cout << "The first node \"" << firstNodeLabel << "\" was not found...therefore an edge was not added." << endl;
    }else if(secondFound == false){
        cout << "The second node \"" << secondNodeLabel << "\" was not found... therefore and edge was not added." << endl;
    }else{
        //Traverse the list and make sure that the second node DNE
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
            }else{
                //If there were deleted edges, notify
                cout << "An edge was successfully deleted between \"" << firstNodeLabel << "\" and \"" << secondNodeLabel << "\"!" << endl;
            }
            return;
        }
    }
    cout << "The first node \"" << firstNodeLabel << "\" was not found...therefore an edge was not deleted." << endl;
}

//Uses dijkstra's to find shortest path
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
    map<char*, char*, cmp_str> parent;
    map<char*, int, cmp_str> distance;
    //Because maps pass in by pointer and not value, we need to make sure that the given node exists and set starting equal to it, killing two birds with one stone
    //
    //
    //Loop through the initial node list and makes sure that the first node does exist
    bool firstNodeValid = false;
    bool secondNodeValid = false;
    vector<Node*>::iterator it;
    for(it = nodeList.begin(); it != nodeList.end(); ++it){
        //If the first node was found
        if(strcmp((*it)->getLabel(),firstNodeLabel) == 0){
            firstNodeValid = true;
        }
        //Traverse the list and make sure that the second node exists
        Node* curr = (*it);
        while(true){
            //If the second node exists
            if(strcmp(curr->getLabel(), secondNodeLabel)==0){
                //Otherwise, we can go onto dijstra's
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
    if(firstNodeValid == false){
        cout << "The first node \"" << firstNodeLabel << "\" was not found...therefore an edge was not added." << endl;
        return;
    }
    if(secondNodeValid == false){
        cout << "The second node \"" << secondNodeLabel << "\" was not found...therefore an edge was not added." << endl;
        return;
    }
    dijkstraAlgorithm(nodeList, parent, distance, firstNodeLabel);
    cout << distance[secondNodeLabel] << endl;
    //Case that there is no path
    if(distance[secondNodeLabel] != INFINITY){
        cout << "No infinity" << endl;
        //Print shortest path
        cout << "The shortest path from \"" << firstNodeLabel << "\" to \"" << secondNodeLabel << "\" is ";
        char* current = secondNodeLabel;
        vector<char*> path;
        //Go backwards to trace path
        while(true){
            path.push_back(current);
            if(strcmp(current, firstNodeLabel) == 0){
                break;
            }
            current = parent[current];
        }
        vector<char*>::reverse_iterator pathIt;
        for(pathIt = path.rbegin(); pathIt != path.rend(); ++pathIt){
            cout << (*pathIt) << " ";
        }
        cout << "with a weight of " << distance[secondNodeLabel] << "!" << endl;
    }else{
        cout << "There is no path from \"" << firstNodeLabel << "\" to \"" << secondNodeLabel << "\"." << endl;
    }
}

//Asks user for two labels, and a weight, verifies that the weight is a number, verifies that the first node DOES exist, then traverses the linked list for the first node, verifies that the second node DNE, then adds that, including a weight
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

//Returns a map of parent and distance by using dijkstra's algorithm to find the shortest length from a beginning node to all nodes
void dijkstraAlgorithm(vector<Node*> &nodeList, map<char*, char*, cmp_str> &parent, map<char*, int, cmp_str> &distance, char* starting){
    //Define visited map, true = visited, false = unvisited
    map<char*, bool, cmp_str> visited;
    //This queue is so big brain. If we pick the min, it's always BFS.
    vector<char*> queue;
    //Initialize default map values
    vector<Node*>::iterator nodeListIt;
    for(nodeListIt = nodeList.begin(); nodeListIt != nodeList.end(); ++nodeListIt){
        //First mark all nodes as unvisited
        visited[(*nodeListIt)->getLabel()] = false;
        //Then define distance to all other nodes as infinity
        distance[(*nodeListIt)->getLabel()] = INFINITY;
        //Then define previous as null, as nothing has been visited yet
        parent[(*nodeListIt)->getLabel()] = NULL;
        //Then add to queue
        queue.push_back((*nodeListIt)->getLabel());
    }
    //Set start initial node to current node, distance is 0
    distance[starting] = 0;
    //
    //char* currentNode = starting;
    //
    //
    //
    //
    //Ok I get it we learned heaps to find max/min very efficiently what you see below is REALLY INEFFICIENT but idc
    //
    //
    //
    //
    //While queue not empty
    while(!queue.empty()){
        //Choose the minimum object in the queue
        vector<char*>::iterator queueIt;
        int leastDist = INFINITY;
        char* leastLabel = (char*)"";
        for(queueIt = queue.begin(); queueIt != queue.end(); ++queueIt){
            cout << (*queueIt) << endl;
            cout << distance[(*queueIt)] << endl;
            //If distance is smaller
            if(distance[(*queueIt)] <= leastDist){
                //Check if it hasn't been visited
                //if(!visited[(*queueIt)]){
                //Then set it as the least, and update least dist
                leastDist = distance[(*queueIt)];
                leastLabel = (*queueIt);
            }
        }
        //DELETE LEAST LABEL FROM THE QUEUE DARN IT.
        for(queueIt = queue.begin(); queueIt != queue.end(); ++queueIt){
            cout << "Still Scanningn" << endl;
            if(strcmp(leastLabel, (*queueIt)) == 0){
                cout << "We're going to delete " << (*queueIt) <<". "<< endl;
                cout << "JDSKLFJDSLKFJSDLKFJDSLKFJLk" << endl;
                queueIt = queue.erase(queueIt);
                break;
            }
        }
        cout << "leastLabel is:" << leastLabel << "." << endl;
        //Mark node as visited. I don't know if this might throw an error
        visited[leastLabel] = true;
        //Add all nodes of this node to queue, to do this we have to traverse linked list (like what???? Why did I use linked lists at all??????????)
        for(nodeListIt = nodeList.begin(); nodeListIt != nodeList.end(); ++nodeListIt){
            //Find the least label node
            if(strcmp((*nodeListIt)->getLabel(), leastLabel) == 0){
                //Traverse list of neighbors
                Node* temp = (*nodeListIt);
                while(true){
                    //If no neighbors / the end, leave
                    if(temp == NULL){
                        break;
                    }
                    //Make sure that it isn't visited
                    if(!visited[temp->getLabel()]){
                        //Calculate the distance to node
                        int distToNode = distance[leastLabel] + temp->getWeight();
                        //If distance to node is less than the current distance of the node
                        if(distToNode < distance[temp->getLabel()]){
                            //Then update distance
                            distance[temp->getLabel()] = distToNode;
                            //Update prev
                            parent[temp->getLabel()] = leastLabel;
                        }
                    }
                    //Move onto the next neighbor
                    temp = temp->getNext();
                }
            }
        }
    }

    //Add all list of unvisited neighbors to queue
    //Calculate the distance to the neighbor node by adding the distance of the current node to the weight of the edge that connects the current node to the neighboring node
    //Compare edge distance to current distance assigned to neighboring node.
    //If edge weight is smaller than the weight of neighboring node, set it as new weight, otherwise keep previous weight
    //Mark current node as visited
    //Select unvisited node with smallest distance
    //Set it as new current node, return to step 4
}
