/*Nicholas Marble
 *CS 4720 HW 4-2
 *NOTE: Some ideas taken from http://www.geeksforgeeks.org/ 
 */

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <math.h>
#include <cmath>
#include <stdlib.h>   
#include <time.h>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <limits.h>
#include <chrono>

using namespace std;
class Coords 
{
public:
    int x, y;
    Coords() {
        x = 0;
        y = 0;
    }
    double getDistanceTo(Coords to) {
        return sqrt(pow(this->x - to.x, 2) + pow(this->y - to.y, 2));
    }
};

class Node {
public:
    int nodeNumber; //Which node this is
    Coords coords;  //The x and y coords
    vector<int> connectingNodes; //All the connecting nodes
    map<int, double> distanceToNodes; //All the distances to these nodes
    
    //Used when creating random nodes
    Node(int number, int sizeOfMap) {
        nodeNumber = number;
        coords.x = rand() % sizeOfMap + 1;
        coords.y = rand() % sizeOfMap + 1;
        
    }
    //used when creating static nodes
    Node(double x, double y, int number) {
        nodeNumber = number;
        coords.x = x;
        coords.y = y;
    }
    
    //Checks what the current distance is to a node
    double getDistanceToo(int node) {
        for(auto elem : distanceToNodes){
            if (elem.first == node) {
                return elem.second;
            }
        }
        return 0;
    }
    
    //Sets the distance to another node
    void setDistance(int node, double distance) {
        distanceToNodes[node] = distance;
    }
    
    //Adds a given node to the list of connected nodes
    void addConnectingNode(int newNode) {
        connectingNodes.push_back(newNode);
    }
    
    //Used to remove a node once it is connected NOT USED
    void removeConnectingNode(int toRemove) {
        vector<int> nodesToKeep;
        for (auto node : connectingNodes) {
            if (node != toRemove) {
                nodesToKeep.push_back(node);
            }
        }
        connectingNodes = nodesToKeep;
    }
    
    //Loops through all connected nodes and checks if given is part of it
    bool isConnected(int findNode){
        
        for (auto &node : connectingNodes) {
            if (node == findNode) {
                return true;
            }
        }
    }
    
    //Check if there is any points that this is connecting to
    bool isAlone() {
        return connectingNodes.empty();
    }
    //Prints and connecting Nodes NOT USED
    void printConnecting() {
        for (int node : connectingNodes) {
            cout << node;
        }
    }
};

class NodeFactory {
public:
    int nodeNumber; //Holds current size of data points
    vector<Node> nodes; //Holds all the nodes
    int sizeOfMap; //Holds the size of the map for random generation
    
    NodeFactory(int sizeOfMap) {
        nodeNumber = 0;
        this->sizeOfMap = sizeOfMap;
    }
    NodeFactory() {
        nodeNumber = 0;
    }
    
    //Used to make a random point
    void createNode() {
        nodeNumber++;
        Node newNode(nodeNumber, sizeOfMap);
        nodes.push_back(newNode);
    }
    
    //Used to insert predetermined points
    void insertNode(double x, double y) {
        nodeNumber++;
        Node newNode(x, y, nodeNumber);
        nodes.push_back(newNode);
    }
    
    //Connects two nodes togethers and sets their individual connections
    void connectNodes(int nodeOne, int nodeTwo) {
        for (auto &node : nodes) {
            if (node.nodeNumber == nodeOne) {
                node.addConnectingNode(nodeTwo);
            }
            if (node.nodeNumber == nodeTwo) {
                node.addConnectingNode(nodeOne);
            }
        }
    }
    
    //Disconnects nodes NOT USED
    void disconnectNodes(int nodeOne, int nodeTwo) {
        for (auto &node : nodes) {
            if (node.nodeNumber == nodeOne) {
                node.removeConnectingNode(nodeTwo);
            }
            if (node.nodeNumber == nodeTwo) {
                node.removeConnectingNode(nodeOne);
            }
        }
    }
    
    //Tells weather two nodes are connected
    bool isConnected(int nodeOne, int nodeTwo) {
        for (Node node : nodes) {
            if (node.nodeNumber == nodeOne) {
                return node.isConnected(nodeTwo);
            }
            if (node.nodeNumber == nodeTwo) {
                return node.isConnected(nodeOne);
            }
        }
    }
    
    //Used to get the distances on all connected nodes.
    void getDistances() {
        for (int index = 0; index < nodeNumber; index++) {
            for (int to = 0; to < nodeNumber; to++) {
                if (isConnected(index, to)) {
                    if (nodes.at(index).coords.getDistanceTo(nodes.at(to).coords) != 0) {
                        nodes.at(index).setDistance(to + 1, nodes.at(index).coords.getDistanceTo(nodes.at(to).coords));
                    }
                }
            }
        }
    }
    //Runs through all nodes and makes sure they are all connected at least once
    void connectAllNodes(int runs) {
        do {
            vector<int> visitedNodes;
            visitedNodes.push_back(0);

            for (int index = 1; index < nodeNumber; index++) {
                bool visited; 
                int to;
                do {
                    to = rand() % visitedNodes.size();
                }
                while (to == index);
                connectNodes(index, to);
                visitedNodes.push_back(index);
            }
            runs--;
        }while (runs > 0);
    }
    int getNumberOfNodes() {return nodeNumber;}
    
    //Prints all the nodes with corresponding connections
    void printInfo() {
        for (auto &node : nodes ) {
            cout << "Node: " << node.nodeNumber << " Coords:" << node.coords.x << "," << node.coords.y << "\n";
            for (auto info : node.connectingNodes) {
               // cout << "Connected to " << info << " with a distance of " << node.distanceToNodes.find(info)->second << "\n";
            }
            for(auto elem : node.distanceToNodes)
                {
                   std::cout << elem.first << " " << elem.second << "\n";
                }
            cout << "**********\n";
        }
    }
    //Creates the matrix to be used with dij alg
    double** createAdjMatrix() {
        double** matrix = new double*[nodeNumber];
        for (int i = 0; i < nodeNumber; ++i) {
            matrix[i] = new double[nodeNumber];
        }
        for (int i = 0; i < nodeNumber; ++i) {
            for (int j = 0; j < nodeNumber; ++j) {
                matrix[i][j] = nodes.at(i).getDistanceToo(j+1);
            }
        }
        return matrix;
    }
}; 

//Used to read the data from http://www.math.uwaterloo.ca/
NodeFactory readFile(string fileLocation) {
    NodeFactory newFactory(1);
    string line;
    ifstream myfile (fileLocation);
    double x, y;
    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
        std::string::size_type sz;     // alias of size_t
        istringstream iss(line);
        vector<string> tokens{istream_iterator<string>{iss},
                      istream_iterator<string>{}};
        x = atol(tokens.at(1).c_str());
        y = atol(tokens.at(2).c_str());
        
        newFactory.insertNode(x,y);
      }
      myfile.close();
    }
    else cout << "Unable to open file"; 
    return newFactory; 
}

//Used in conjunction with dijkstra to find the next best value
int minDistance(double dist[], bool sptSet[], int size)
{
   // Initialize min value
   int min = INT_MAX, min_index;
 
   for (int v = 0; v < size; v++)
     if (sptSet[v] == false && dist[v] <= min)
         min = dist[v], min_index = v;
 
   return min_index;
}

//The ALGORITHM
double dijkstra(double** matrix, int src, int dest, int size)
{
    double dist[size];  
    bool visited[size];
    int path[size];
    int position = 0;
    path[0] = src;
    position++;
    for (int i = 0; i < size; i++) {
        dist[i] = INT_MAX, visited[i] = false;
    }
    //set distance to itself
    dist[src] = 0;
 
    // Find shortest path for points
    for (int index = 0; index < size - 1; ++index)
    {
        // Min dist in flex
        int i = minDistance(dist, visited, size);

        // set as completed
        visited[i] = true;

        // Update distance value if need be
        for (int j = 0; j < size; j++)
 
            //Check if there is actually a edge and if it was not visited then add if it is less
            if (!visited[j] && matrix[i][j] && dist[i] != INT_MAX && dist[i]+matrix[i][j] < dist[j]) {
                dist[j] = dist[i] + matrix[i][j];
                path[position] = i;
                position++;
            }
    }
    for (int index = 0; index <= position; index++) {
        cout << path[index + 1] << " ";
    }
    cout << "\n";
    return dist[dest - 1];
}

//Used to run the algorithm 6 times and get the average time
void run(double** matrix, int size) {
    double total = 0;
    int count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            //Set the start time
            chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
            cout << dijkstra(matrix, i, size - j, size);
            
            //Set end time
            chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();

            //Gets difference and adds to the average
            chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2-t1);
            total += time_span.count();
            count++;
            cout << " Distance form " << i + 1 << " to " << size - j << "\n";
        }
    }
    cout << "Average time for " << size << " sized node sets " << (total / count) << "\n"; 
}

//Main
int main(int argc, char** argv) {
    srand (time(NULL));
    double** matrix; // Hold Adj matrix
    NodeFactory nodeFactory(100);
    for (int index = 0; index < 10; index++) {
        nodeFactory.createNode();
    }
    nodeFactory.connectAllNodes(2);
    nodeFactory.getDistances();
    nodeFactory.printInfo();
    matrix = nodeFactory.createAdjMatrix();
    run(matrix, nodeFactory.getNumberOfNodes());
    
    NodeFactory saharaNodes(1);
    saharaNodes = readFile("sahara.txt");
    saharaNodes.connectAllNodes(2);
    saharaNodes.getDistances();    
    matrix = saharaNodes.createAdjMatrix();
    run(matrix, saharaNodes.getNumberOfNodes());
       
    NodeFactory djiboutiNodes(1);
    djiboutiNodes = readFile("djibouti.txt");
    djiboutiNodes.connectAllNodes(2);
    djiboutiNodes.getDistances();
    matrix = djiboutiNodes.createAdjMatrix();
    run(matrix, djiboutiNodes.getNumberOfNodes());
    
    NodeFactory qatarNodes(1);
    qatarNodes = readFile("qatar.txt");
    qatarNodes.connectAllNodes(2);
    qatarNodes.getDistances();
    matrix = qatarNodes.createAdjMatrix();
    run(matrix, qatarNodes.getNumberOfNodes());
    
    NodeFactory zimbabweNodes(1);
    zimbabweNodes = readFile("zimbabwe.txt");
    zimbabweNodes.connectAllNodes(2);
    zimbabweNodes.getDistances();
    matrix = zimbabweNodes.createAdjMatrix();
    run(matrix, zimbabweNodes.getNumberOfNodes());
    
    NodeFactory swedenNodes(1);
    swedenNodes = readFile("sweden.txt");
    swedenNodes.connectAllNodes(2);
    swedenNodes.getDistances();
    matrix = swedenNodes.createAdjMatrix();
    run(matrix, swedenNodes.getNumberOfNodes());
    return 0;
}

