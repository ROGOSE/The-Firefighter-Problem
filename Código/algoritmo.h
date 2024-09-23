#ifndef ALGORITMO_H
#define ALGORITMO_H

#include <deque>
#include <random>
#include "grafo.h"
#include "heuristicas.h"


double randfrom(double min, double max);

void iniciarCola(deque<string>& dcola_incendiados, unordered_map<string, Node>& graph_data, const vector<string>& random_numbers);

//Simula la influencia de un nodo sobre sus vecinos usando una distribución uniforme de números aleatorios
vector<string> incendiarVecinosDe(Node& node, unordered_map<string, Node>& graph_data, int& numNodosIncendiados, mt19937& gen);

//Implementa el algoritmo de propagación de la infección en el grafo. Usa una deque para manejar la cola de nodos a procesar su difusión
void start(unordered_map<string, Node>& graph_data, deque<string>& dcola_incendiados, int& numNodosIncendiados, mt19937& gen);

pair <string, string> seleccionarArista(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers, 
	mt19937& gen, ofstream &resultados);

int simularIC(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers, mt19937& gen, entrada e);

#endif // ALGORITMO_H
#pragma once
