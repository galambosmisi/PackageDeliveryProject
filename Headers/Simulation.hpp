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
        
        int ID;                                                                                     //Csúcs azonosító
        bool storage;                                                                               //Raktár-e
        std::vector<Road> connects;                                                                 //Szomszédós csúcsok és a hozzájuk vezetú élek súlya
        
        Node(int k, bool st) : ID(k), storage(st) {}
        Node(int k, bool st, std::vector<Road> c) : ID(k), storage(st), connects(c) {}
    };
    
    std::vector<Node*> peaks;                                                                       //Gráf csúcsai
    
public:
    //Konstruktor, destruktor
    Graph(std::string file_name) {loadGraph(file_name);}
    ~Graph() {for(unsigned i =0; i<peaks.size(); i++) {delete peaks[i];};}
    //Függvények
    void initialization(unsigned size);                                                                  //Peaks vektor feltöltése üres Node* -al a hivatkozások miatt
    void loadGraph(std::string);                                                                    //Gráf beolvasása
    void insert(int _ID, bool _storage, std::vector<Node::Road> _connects);                         //Új csúcs "hozzáadása"
    unsigned Dijkstra(int src, int dst) const;                                                            //Dijkstra algoritmus
};


///Termék
class Product {
private:
    // |
    // v   Ezt a mezőt (ID) const -ra szerettem volna állítani de a fordító hibát dobott a sortOrders() függvényben, arra hivatkozva, hogy nem tudja swap -elni
    int ID;                         			                                                    //Termék azonosító
    unsigned place;                                                                                 //Megrendelés helye Peak_ID alapján
    unsigned quantity;                                                                              //Mennyiség
    bool added;                                                                                     //Kamionhoz adva?
    bool deliverd;                                                                                  //Kiszállítva?

public:
    //Konstruktor
    Product(unsigned p, int q, int _ID) : ID(_ID), place(p), quantity(q), added(false), deliverd(false) {}
    //Függvények
    int getID() const {return ID;}                                                                  //Melyik csomag?
    int getPlace() const {return place;}                                                            //Hova kell vinni?
    int getQuantity() const {return quantity;}                                                      //Mekkora a csomag?
    bool isAdded() const {return added;}                                                            //Kamionban van már?
    bool isDeliverd() const {return deliverd;}                                                      //Ki lett szállítva?
    void setAdded() {added = true;}                                                                 //Kamionhoz adtam!
    void setDeliverd() {deliverd = true;}                                                           //Kiszállítottam!
    void printProduct(std::ostream&) const;                                                         //Termék adatainak kiírása
};


///Kamion
class Truck {
private:
    // |
    // v   A következő két mezőt (ID, startPosition) is const -ra szerettem volna állítani de a fordító hibát dobott a sortOrders() függvényben, arra hivatkozva, hogy nem tudja swap -elni
    int ID;                                                                                         //Kamion azonosító
    unsigned startPosition;                                                                         //Kezdeti pozíció
    unsigned position;                                                                              //Aktuális pozíció
    unsigned max_capacity;                                                                          //Maximál kapacitás
    unsigned payload;                                                                               //Jelenleg felrakott súly
    unsigned distance;                                                                              //Eddigi megtett távolság
    std::vector<Product> orders;                                                                    //Felpakolt termékek
    
public:
    //Konstruktor
    Truck(int _ID, int p, int c) : ID(_ID), startPosition(p), position(p), max_capacity(c), payload(0), distance(0) {}
    //Függvények
    int getID() const {return ID;}                                                                  //Melyik kamion?
    int freeSpace() const {return max_capacity-payload;}                                            //Mennyi üres hely van még?
    size_t numberOfProducts() const {return orders.size();}                                         //Hány db termék van felrakva?
    bool isFit(int p) const {return freeSpace() >= p;}                                              //Belefér "p" termék?
    void addProduct(Product&);                                                                      //Termék hozzáadása
    void sortOrders(Graph graph);                                                                   //Termékek rendezése kiszállítási távolság alapján
    void move(int, Graph, std::ostream&);                                                           //Mozgás
    void printTruck(std::ostream&) const;                                                           //kamion adatainak kiírása
};


///Szimuláció
class Simulation {
private:
    Graph city;                                                                                     //Várost reprezentáló gráf
    std::vector<Truck> trucks;                                                                      //Kamionok
    std::vector<Product> orders;                                                                    //Termékek
    
    unsigned deliveredProducts;                                                                     //Össz kiszállított termék
    
public:
    //Konstruktor
    Simulation(std::string graph, std::string simulation) : city(graph), deliveredProducts(0) {loadSimulation(simulation);}
    //Függvények
    void loadSimulation(std::string);                                                               //Szimuláció betöltése
    void uploadTrucks(std::ostream&);                                                               //Kamionok feltöltése
    void deliveryProducts(std::ostream&);                                                           //Termékek kiszállítása
    void printSimulation(std::ostream&) const;                                                      //Szimuláció adatainak kiírása
    
    //Segéd függvények
    void sortTrucks();                                                                              //Kamionok rendezése üres hely szerint
    void sortProducts();                                                                            //Termékek rendezése mennyiség szerint
    void printOrders(std::ostream&) const;                                                          //Az összes kaion adatainak kiírása
    void printTrucks(std::ostream&) const;                                                          //Az összes termék adatainak kiírása
    unsigned shortestRoad(int src, int dst) {return city.Dijkstra(src, dst);}                       //Két pont közötti legrövidebb út meghatározása (Dijkstra segítségével)
};

#endif /* SIMULATION_HPP_ */
