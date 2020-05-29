#ifndef NODE_H
#define NODE_H

class Node{
        public:
                //Constructor for a node
                Node(Node* newNext, char* newLabel, int newWeight);
                //Destructor for a node
                ~Node();
                //Getters and Setters for values
                Node* getNext();
                void setNext(Node* newNext);
                char* getLabel();
                void setLabel(char* newLabel);
                int getWeight();
                void setWeight(int newWeight);

        private:
                //I chose to implement a node to hold both a vertex/edge, but there are many other ways to do it.
                //You could have seperate classes for each, or not even use classes at all (which probably would've been more efficient)
                Node* next;
                char* label;
                int weight;
};

#endif /*NODE_H*/
