#include <iostream>
#include <cstring>
#include <vector>
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
void addVertex(char* in);
//Asks user to input two labels and a weight, adds it as the weight between nodes
void addEdge();
//Asks user to input label, remove it from the graph
void removeVertex();
//Asks user to input two labels, removes edge
void removeEdge(); //Uses Dijkstra's Algorithm to find a path between the first vertex and the last vertex. Return shortest path if it exists, or no paths
void findShortestPath();

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

        }else if(strcmp(in, "ADDEDGE") == 0){

        }else if(strcmp(in, "REMOVEVERTEX") == 0){

        }else if(strcmp(in, "REMOVEEDGE") == 0){

        }else if(strcmp(in, "FINDSHORTESTPATH") == 0){

        }else if(strcmp(in, "HELP") == 0){
            help();
        }else if(strcmp(in, "QUIT") == 0){
            quit(running);
        }else{
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
    cout << "\n----------\n\"addvertex\" to add a vertex to the tree,\n\"addedge\" to add an edge to the tree,\n\"removevertex\" to remove an edge,\n\"findshortestpath\" to find shortest path from one node to the other,\n\"quit\" to exit this program\n\"help\" to reprint this list.\n----------\n" << endl;
}

//Stops program execution by setting running to false
void quit(bool &running){
    running = false;
}
