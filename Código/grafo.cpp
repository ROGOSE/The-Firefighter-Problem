#include "grafo.h"
using namespace std;

//CONSTRUCTOR TRIVIAL
//Node() {}
// Constructor definition
Node:: Node() 
    :incendiado(false), numVecinosIncendiados(0) {}

Node::Node(const string& name)
    : name(name), incendiado(false), infectadoPor("nadie"), numVecinosIncendiados(0) {}

//CONSTRUCTOR
Node:: Node(const string& name, const vector<Neighbor>& neighbors)
    : name(name), neighbors(neighbors), incendiado(false), infectadoPor("nadie"), numVecinosIncendiados(0){}

void Node:: check_node() const {
    bool noAristasSalida = true;
    cout << name << '\n';
    for (const auto& neighbor : neighbors) {
        cout << "  --" << name << " --> " << neighbor.name << '\n';
        noAristasSalida = false;
    }
    if (noAristasSalida) {
        cout << "No tiene aristas de salida" << '\n';
    }
}

void Node:: check_state(ofstream &salida, int & num_nodosIncendiados) const {
    if (incendiado) {
        cout << name << " fue incendiado por " << infectadoPor << "." << '\n';
        salida << name << " fue incendiado por " << infectadoPor << "." << '\n';
        num_nodosIncendiados++;

    }
    else {
        cout << name << " continua a salvo." << '\n';
        salida << name << " continua a salvo." << '\n';
    }
}

unordered_map<string, Node> get_graph() {

    //grafo predefinido (imagen en carpeta del proyecto)
    unordered_map<string, vector<Neighbor>> node_info = {
            {"a", {{"c", 0.7, true}, {"b", 0.5, true}}},
            {"b", {{"d", 0.5, true}, {"c", 0.4, true}}},
            {"c", {{"d", 0.6, true}, {"e", 0.2, true}}},
            {"d", {{"e", 0.7, true}}},
            {"e", {{"f", 0.8, true}}},
            {"f", {{"d", 0.6, true}, {"b", 0.4, true}, {"e", 0.6, true}}}
    };

    unordered_map<string, Node> graph;

    for (const auto& it : node_info) {
        graph.emplace(it.first, Node(it.first, it.second));
    }

    return graph;
}

//IMPORTANTE: El archivo Facebook tiene un total de 88,234 líneas y hay 3,663 números distintos en la primera columna (nodos salida)
//Tiene en total 4039 nodos, es decir, 376 nodos a los que solo llegan aristas (no pueden contagiar).
//----------: El archivo Twitter tiene 475 nodos y 9289 arcos, con 6 nodos a los que solo les llegan aristas
void leerDataset(unordered_map<string, Node>& graph_data, int& num_aristas){ 

    int i = 0, parcial_aristas = 0;
    //Vector para diferenciar 2 tipos de nodos; si es true, el nodo puede infectar. False -> no infecta (no tiene aristas de salida) 
    vector <bool> nodosInfectadores(4039, false); //4039 Facebook //475 Twitter
    bool finArchivo = false;

    ifstream inputFile("facebook_dataset.txt");   //facebook_dataset.txt //congress_twitter_dataset1.txt
    if (!inputFile.is_open()) {
        cerr << "Error opening the file!" << endl;
    }
    else {
        cout << "Inicio de lectura:" << '\n';
        string nodo, vecino, nodoBase;
        while (i < 88234) { //(ARISTAS) //88234 Facebook // 9289 Twitter
            if (i == 0) { //para la primera vuelta
                inputFile >> nodo >> vecino;
                nodoBase = nodo;
            }

            vector<Neighbor> vecinos;

            while (nodo == nodoBase && !finArchivo) {
                //ATENCIÓN, HABRÍA QUE MODIFICAR LA PROBABILIDAD PARA CADA ARISTA más adelante
                double probabilidad = 0.05; //0.05 o 0.1
                //-----------------------------------
                vecinos.push_back({ vecino, probabilidad, true }); //{name, prob, aristaActiva}
                parcial_aristas++;

                if (!(inputFile >> nodo >> vecino)) {
                    finArchivo = true;
                }
                i++;
            }

            graph_data.emplace(nodoBase, Node(nodoBase, vecinos));
            nodosInfectadores[stoi(nodoBase)] = true;
            num_aristas += parcial_aristas;
            parcial_aristas = 0;
            nodoBase = nodo;
        }

        cout << "Fin de lectura:" << '\n';
    }
    inputFile.close();

    //DECIDIR
    //añadimos los nodos que no incendian a nadie (solo tienen aristas de entrada) --> PENSAR PARA MEJORAR EFICIENCIA
    for (int i = 0; i < 4039; i++) { //4039 Facebook //475 Twitter
        if (nodosInfectadores[i] == false) {
            graph_data.emplace(to_string(i), Node(to_string(i)));
            //num_nodosNoInfectadosadores++;
        }
    }
}
