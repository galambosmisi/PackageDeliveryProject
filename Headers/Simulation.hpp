/*
 * Simulation.hpp
 * Created on: Oct 31, 2017
 * Author: galambosmihaly
 */

#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include <vector>

///Gráf
class Graph {
private:
    struct Node {
        struct Road {
            Node* target;
            unsigned value;
            
            Road(Node* t, unsigned v) : target(t), value(v) {}
        };
        
        std::vector<Road> connects;
        bool storage;
        int ID;
        
        Node(int k, bool st) : ID(k), storage(st) {}
        Node(int k, bool st, std::vector<Road> c) : ID(k), storage(st), connects(c) {}
    };
    
    std::vector<Node*> peaks;
    
public:
    Graph(std::string file_name) {loadGraph(file_name);}
    void initialization(int size);
    void loadGraph(std::string);
    void insert(int _ID, bool _storage, std::vector<Node::Road> _connects);
    unsigned shortestRoad(int src, int dst);
};


///Termék
class Product {
private:
    int ID;
    unsigned place; //ID alapján
    int quantity;
    bool added;
    bool deliverd;

public:
    Product(unsigned p, int q, int _ID) : ID(_ID), place(p), quantity(q), added(false), deliverd(false) {}
    int getQuantity() const;
    int getPlace() const;
    int getID() const;
    bool isAdded() const { return added;}
    bool isDeliverd() const {return deliverd;}
    void setAdded();
    void setDeliverd();
    void printProduct(std::ostream&) const;
};


///Kamion
class Truck {
    int ID;
    unsigned position; //ID alapján
    unsigned max_capacity;
    unsigned payload;
    unsigned distance;
    std::vector<Product> orders;
    
public:
    Truck(int _ID, int p, int c) : ID(_ID), position(p), max_capacity(c), payload(0), distance(0) {}
    void move(int, Graph, std::ostream&);
    void addProduct(Product&);
    int freeSpace() const;
    size_t numberOfProducts() const;
    bool isFit(int) const;
    int getID() const;
    void sortOrders(Graph graph);
    void printTruck(std::ostream&) const;
};


///Szimuláció
class Simulation {
private:
    
    std::vector<Truck> trucks;
    std::vector<Product> orders;
    unsigned deliveredProducts;
    Graph city;
    
public:
    Simulation(std::string graph, std::string simulation) : city(graph), deliveredProducts(0) {loadSimulation(simulation);}
    void loadSimulation(std::string);
    void uploadTrucks(std::ostream&);
    void deliveryProducts(std::ostream&);
    void printSimulation(std::ostream&) const;
    
    //Segéd függvények
    void sortTrucks();
    void sortProducts();
    unsigned shortestRoad(int src, int dst) {return city.shortestRoad(src, dst);}
    
    void printOrders(std::ostream&) const;
    void printTrucks(std::ostream&) const;
};

#endif /* SIMULATION_HPP_ */
