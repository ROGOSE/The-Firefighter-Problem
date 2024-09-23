#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

struct Neighbor {
    string name;
    double prob;
    bool aristaActiva;

    Neighbor(const std::string& name, double prob, bool aristaActiva)
        : name(name), prob(prob), aristaActiva(aristaActiva) {}
};

class Node {

    public:
        string name;
        vector<Neighbor> neighbors; //conjunto(vector) de vecinos de cada nodo. Cada vecino es un par <nombre, probabilidad de ser infectado este nodo>
        bool incendiado; //True-> Incendiado, False -> Vivo
        string infectadoPor;
        int numVecinosIncendiados;

        Node();

        Node(const string& name);

        Node(const string& name, const vector<Neighbor>& neighbors);

        void check_node() const;

        void check_state(ofstream &salida, int & num_nodosIncendiados) const;
};

unordered_map <string, Node> get_graph();

void leerDataset(unordered_map<string, Node>& graph_data, int& num_aristas); // , int& num_nodosInfectadores, int& num_nodosNoInfectadosadores);

#endif // GRAFO_H
#pragma once
