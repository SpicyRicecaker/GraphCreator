#ifndef NODE_H
#define NODE_H

class Node{
        public:
                //Constructor for edge
                Node(Node* newNext, char* newLabel, int newWeight);
                //Destructor for edge
                ~Node();
                //Getters and Setters for values
                Node* getNext();
                void setNext(Node* newNext);
                char* getValue();
                void setValue(char newValue);
                int getWeight();
                void setWeight(int newWeight);

        private:
                Node* next;
                char* label;
                int weight;
};

#endif /*NODE_H*/
