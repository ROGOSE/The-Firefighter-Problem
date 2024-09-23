#include "heuristicas.h"

/*vector<registroBFS> shortestPathBFSdesdeNodo(unordered_map<string, Node>& graph_data) {

    vector<registroBFS> shortestPaths;

    for (const auto& itNode : graph_data) {
        const string& startNode = itNode.first;
        //cout << "Nodo inicial: " << startNode << '\n';
        queue<string> q;
        q.push(startNode);
        shortestPaths[startNode][startNode] = 0;

        while (!q.empty()) {
            string currentNode = q.front();
            //cout << currentNode << '\n';
            q.pop();
            int currentDistance = shortestPaths[startNode][currentNode];

            for (const Neighbor& neighbor : graph_data[currentNode].neighbors) {
                //si no he visto ese nodo todavia, continuo. Si lo he visto, el resultado anterior sera mejor (BREATH F S)
                if (shortestPaths[startNode].find(neighbor.name) == shortestPaths[startNode].end()) { 
                    shortestPaths[startNode][neighbor.name] = currentDistance + 1;
                    cout << "Distancia " << startNode << " a " << neighbor.name << ": " << currentDistance + 1 << '\n';
                        //<< " (via " << currentNode << ")" << '\n';
                    q.push(neighbor.name);
                }
            }
        }
    }

    return shortestPaths;
}*/

void shortestPathBFSdesdeNodo(unordered_map<string, Node>& graph_data, unordered_map<string, Node>::iterator& itNodo,
    vector<registroBFS>& shortestPaths, string nodoLlegadaE) {
    //el arista E esta formado por startNode y 
    const string& startNode = itNodo->first;
    cout << "Nodo inicial: " << startNode << '\n';
    queue<string> q;
    q.push(startNode);
    registroBFS& elementoInicio = shortestPaths[stoi(startNode)];
    elementoInicio.dist = 0;
    elementoInicio.numCaminosMin = 1;

    while (!q.empty()) {
        string currentNode = q.front();
        //cout << currentNode << '\n';
        q.pop();
        int currentDistance = shortestPaths[stoi(currentNode)].dist;

        for (const Neighbor& neighbor : graph_data[currentNode].neighbors) {
            //si no he visto(modificado) ese nodo todavia, continuo. Si lo he visto, el resultado anterior sera mejor (BREATH F S)
            if (shortestPaths[stoi(neighbor.name)].dist == -1) {
                shortestPaths[stoi(neighbor.name)].dist = currentDistance + 1;
                cout << "Viendo " << currentNode << " a " << neighbor.name << '\n';
                shortestPaths[stoi(neighbor.name)].numCaminosMin = 1;
                q.push(neighbor.name);
            }
            else if (shortestPaths[stoi(neighbor.name)].dist == currentDistance + 1) {
                    shortestPaths[stoi(neighbor.name)].numCaminosMin += 1;
            }
            else {

            }
        }
    }

}

void describirCaminos(string nombre, const vector<registroBFS>& caminosBFS) {
    int tamano = caminosBFS.size();
    for (int i = 0; i < tamano; i++) {
        cout << "Distancia mas corta de " << nombre << " a " << i << ": " << caminosBFS[i].dist 
            << " (" << caminosBFS[i].numCaminosMin << " camino(s))" << '\n';
    }
}

int calcularCentralidad(const vector<registroBFS>& caminosBFS) {
    int suma = 0, tamano = caminosBFS.size();
    for (int i = 0; i < tamano; i++) {
        suma += caminosBFS[i].numCaminosMin;
    }
    return suma;
}

void betweennessScoringMethod(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers, int k) {
    int numCaminos = 0, numCaminosConE = 0, tamanoVector = random_numbers.size();
    vector <vector<int>> centralidades(tamanoVector);
    for (int i = 0; i < tamanoVector; i++) {
        string nombre = random_numbers[i];
        auto initialItNode = graph_data.find(nombre);

        if (initialItNode != graph_data.end()) {
            cout << "Nodo encontrado: " << nombre << '\n';
            int tamano = initialItNode->second.neighbors.size();
            for (int i = 0; i < tamano; i++) {
                string nombreVecino = initialItNode->second.name;
                Neighbor& neighbor = initialItNode->second.neighbors[i];

                vector<registroBFS> caminosBFS(6, { -1, false, 0 }); //4039 es el numero total de nodos

                shortestPathBFSdesdeNodo(graph_data, initialItNode, caminosBFS, "a");
                describirCaminos(nombre, caminosBFS);
                //para el nodoX (de los inicialmente incendiados), tenemos N(nodoX, v) para todo v de V\S
                numCaminos = calcularCentralidad(caminosBFS);
                //numCaminosConE = sumarCaminosConE(caminosBFS);
                //centralidades[0].push_back(numCaminosConE / numCaminos);
                //calcular b(e) siendo e el arista que une cada initialNode con cada uno de sus nodos prendidos
            }
            

        }
        else {
            cout << "Nodo " << nombre << " no tiene aristas de salida" << '\n';
        }
        cin.get();
    }
   
}

void describirGrados(string nombre, const vector<registroBFS>& caminosBFS) {
    int tamano = caminosBFS.size();
    for (int i = 0; i < tamano; i++) {
        cout << "Distancia mas corta de " << nombre << " a " << i << ": " << caminosBFS[i].dist
            << " (" << caminosBFS[i].numCaminosMin << " camino(s))" << '\n';
    }
}

void iniciarPrendidos(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers) {
    for (const auto& num : random_numbers) {
        auto it = graph_data.find(num);
        if (it != graph_data.end()) {
            it->second.incendiado = true;
            it->second.infectadoPor = "el problema";
        }
        else {
            //habria que tener en cuenta a los nodos incendiados que no teien aristas de salida
        }
    }
}

void cuantosVecinosPrendidos(unordered_map<string, Node>& graph_data) {
    for (const auto& it : graph_data) {
        cout << "Nodo " << it.second.name << " tiene " << it.second.numVecinosIncendiados << " vecino(s) incendiados." << '\n';
    }
}

void actualizarNumVecinosIncendiados(unordered_map<string, Node>& graph_data) {

    for (auto& it : graph_data) { //para cada nodo X
        for (const auto& vecino : it.second.neighbors) { //para cada nodo vecino Yj de X
            auto it3 = graph_data.find(vecino.name);
            if (it3->second.incendiado) { //si nodo Yj esta prendido, sumamos 1 al numVecinosIncendiados de X
                it.second.numVecinosIncendiados++;
            }
        }
    }
}

priority_queue<entrada> outdegreeMethod(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers, int k) {

    int tamanoVector = random_numbers.size();
    priority_queue<entrada> outdegrees;
    //cuantosVecinosPrendidos(graph_data);
    iniciarPrendidos(graph_data, random_numbers);
    actualizarNumVecinosIncendiados(graph_data);
    //cuantosVecinosPrendidos(graph_data);

    for (int i = 0; i < tamanoVector; i++) { //iteramos sobre los nodos prendidos de inicio(porq solo queremos las aristas de O(S))
        string nombre = random_numbers[i];
        auto initialItNode = graph_data.find(nombre); //iterador de los nodos inicialmente prendidos

        cout << "Nodo incendiado: " << nombre << '\n';
        int tamano = initialItNode->second.neighbors.size();

        for (int i = 0; i < tamano; i++) { //sacamos cada vecino para tener arista e=(nombre, nombrevecino) y calcular o(e)
            string nombreVecino = initialItNode->second.neighbors[i].name;
            auto itNodeVecino = graph_data.find(nombreVecino); //iterador de cada uno de los vecinos
            if (itNodeVecino->second.incendiado) {
                cout << "Nodo de llegado incendiado. Siguiente..." << '\n';
                continue;
            }
            //vamos sacando las aristas e de O(S)
            int oe = itNodeVecino->second.neighbors.size() - itNodeVecino->second.numVecinosIncendiados;
            cout << "o(" << nombre << "," << nombreVecino << ") = " << itNodeVecino->second.neighbors.size() <<
                " - " << itNodeVecino->second.numVecinosIncendiados << " = " << oe << '\n';
            outdegrees.emplace(arista(stoi(nombre), stoi(nombreVecino)), oe);
        }

    }

    return outdegrees;
}

priority_queue<entrada> extendedOutdegreeMethod(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers, int k) {

    int tamanoVector = random_numbers.size();
    priority_queue<entrada> outdegrees;
    //cuantosVecinosPrendidos(graph_data);
    iniciarPrendidos(graph_data, random_numbers);
    actualizarNumVecinosIncendiados(graph_data);
    //cuantosVecinosPrendidos(graph_data);

    for (auto& it : graph_data) { //iteramos sobre todos los nodos (porq queremos todas las aristas)
        Node& node = it.second;
        //cout << "Analizando nodo: " << node.name << '\n';

        for (int i = 0; i < node.neighbors.size(); i++) { //sacamos cada vecino para tener toda arista e del grafo y calcular o(e)
            Neighbor& neighbor = node.neighbors[i];
            auto itNodeVecino = graph_data.find(neighbor.name);//iterador de cada uno de los vecinos
            //vamos sacando las aristas e de O(S)
            int oe = itNodeVecino->second.neighbors.size() - itNodeVecino->second.numVecinosIncendiados;
            //cout << "o(" << node.name << "," << neighbor.name << ") = " << itNodeVecino->second.neighbors.size() <<
            //    " - " << itNodeVecino->second.numVecinosIncendiados << " = " << oe << '\n';
            outdegrees.emplace(arista(stoi(node.name), stoi(neighbor.name)), oe);
        }
    }


    return outdegrees;
}
