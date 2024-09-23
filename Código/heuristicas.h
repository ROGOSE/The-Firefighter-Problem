#ifndef HEURISTICAS_H
#define HEURISTICAS_H

#include <queue>
#include "grafo.h"

struct registroBFS {
	int dist;
	bool contieneE;
	int numCaminosMin;

	// Constructor para facilidad
	registroBFS(int dist, bool contieneE, int numCaminosMin) : dist(dist), contieneE(contieneE), numCaminosMin(numCaminosMin){}
};

struct arista {
	int origen;
	int destino;

	// Constructor para facilidad
	arista(int n1, int n2) : origen(n1), destino(n2) {}
};

struct entrada {
    arista ar;
    int puntuacion;

    // Constructor para facilidad
    entrada(arista a, int p) : ar(a), puntuacion(p) {}

    // Sobrecargar operador < para que la cola de prioridad ordene correctamente
    bool operator<(const entrada& other) const {
        // Queremos que la cola de prioridad saque primero las puntuaciones mayores
        return puntuacion < other.puntuacion;
    }
};

void betweennessScoringMethod(unordered_map<string, Node> & graph_data, const vector<string> & random_numbers, int k);

//devuelve un mapa donde la clave es un nodo de inicio y el valor es otro unordered_map que almacena las distancias
//desde el nodo de inicio a todos los demás nodos.
void shortestPathBFSdesdeNodo(unordered_map<string, Node>& graph_data, unordered_map<string, Node>::iterator& it,
    vector<registroBFS>& caminos, string nodoLlegadaE);

int calcularCentralidad(const vector<registroBFS>& caminosBFS);

void describirCaminos(string nombre, const vector<registroBFS>& caminosBFS);

priority_queue<entrada> outdegreeMethod(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers, int k);

priority_queue<entrada> extendedOutdegreeMethod(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers, int k);

void describirGrados(string nombre, const vector<registroBFS>& caminosBFS);

#endif // ALGORITMO_H
#pragma once
