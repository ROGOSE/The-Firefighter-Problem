#include "algoritmo.h"

double randfrom(double min, double max) {
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}


void iniciarCola(deque<string>& dcola_incendiados, unordered_map<string, Node>& graph_data, const vector<string>& random_numbers) {
    for (const auto& num : random_numbers) {
        dcola_incendiados.push_back(num);
        auto it = graph_data.find(num);
        if (it != graph_data.end()) {
            it->second.incendiado = true;
            it->second.infectadoPor = "el problema";
        }
    }
}


vector<string> incendiarVecinosDe(Node& node, unordered_map<string, Node>& graph_data, int& numNodosIncendiados, mt19937& gen) {

    //opcion 1 de aleatoriedad (total)
    uniform_real_distribution<> dis(0.0, 1.0);
    //opcion 2 (semilla fija para cada llamada aqui) //srand(stoi(node.name) + 2);

    vector<string> nodosIncendiados;
    //cout << '\n' << node.name << " intenta incendiar a: " << '\n';

    for (auto& neighbor : node.neighbors) {
        auto it = graph_data.find(neighbor.name);

        double w = dis(gen); //OPCION 1 //double w = randfrom(0, 1); //OPCION 2:

        //cout << neighbor.name << " con fuerza " << w << " ";

        //si el vecino que estamos mirando ya está infectado o tiene arista bloqueada, se pasa al siguiente
        if (it->second.incendiado) {
            //cout << "Arista esta ya incendiado.\n";
            continue;
        }
        if (!neighbor.aristaActiva) {
            //cout << "Arista esta ya bloqueado.\n";
            continue;
        }

        //cout << neighbor.name << " con w=" << w;
        if (w <= neighbor.prob) {  //si el vecino tiene probabilidad de incendio >= numero generado aleatorio, se incendia el vecino
            nodosIncendiados.push_back(neighbor.name); //Ahora mismo tambien se añaden los nodos sin aristas de salida
            //cout << " <= " << neighbor.prob << " --> lo consigue." << '\n';
            it->second.incendiado = true;
            it->second.infectadoPor = node.name;
            numNodosIncendiados++;
            //contabilizamos los vecinos de node que infectamos
            node.numVecinosIncendiados++;
        }
        else {
            //cout << " > " << neighbor.prob << " --> falla." << '\n';
        }
    }
    return nodosIncendiados;
}


void start(unordered_map<string, Node>& graph_data, deque<string>& dcola_incendiados, int& numNodosIncendiados, mt19937& gen) {

    vector<pair<string, string>> cambios; //vector para guardar los cambios
    //cout << "Comienza la etapa " << etapa << " en la que se difunde el incendio desde " << nodosXetapa << " nodo(s):" << '\n';

    while (!dcola_incendiados.empty()) { //para todo nodo que se va a intentar extender:

        string node_name = dcola_incendiados.back();
        dcola_incendiados.pop_back();
        auto node_it = graph_data.find(node_name);       
        Node& node = node_it->second;
       
        //intentamos incendiar todos los vecinos de node. Aqui hay que tener en cuenta las aristas que ya hayamos bloqueado
        vector<string> nodosIncendiados = incendiarVecinosDe(node, graph_data, numNodosIncendiados, gen);

        //vamos guardando los cambios que se hagan (nodos que se incendian)
        for (const auto& vecino : nodosIncendiados) {
            cambios.push_back({vecino, graph_data[vecino].infectadoPor});
        }

        //añadimos a la doble cola los nuevos nodos prendidos
        dcola_incendiados.insert(dcola_incendiados.end(), nodosIncendiados.begin(), nodosIncendiados.end());
    }

    //recuperar el grafo inicial, revirtiendo los cambios, para la siguiente iteración
    for (auto& it : cambios) {
        auto graph_it = graph_data.find(it.first);
        if (graph_it != graph_data.end()) {
            graph_it->second.incendiado = false;
            graph_it->second.infectadoPor = "nadie";
        }
    }

}


pair <string, string> seleccionarArista(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers, 
    mt19937& gen, ofstream & resultados) {

    //se elimina 1 arista, se calcula la influencia y se recupera la arista. Así con todas. Se guarda la que mejor lo haya hecho
    int minNumNodosIncendiados = 5000, indiceVecinoSelec = 0;
    pair <string, string> aristaSeleccionado;
    int g = 0;

    for (auto& it : graph_data) {
        Node& node = it.second;
        //cout << "\nComprobamos el bloqueo de las aristas del nodo " << node.name << '\n';

        for (int i = 0; i < node.neighbors.size(); i++) {
            if (g % 100 == 0) cout << g << '\n';
            g++;
            Neighbor& neighbor = node.neighbors[i];
            //cout << "\nBloqueo arista " << node.name << "--->" << neighbor.name << ": ";

            if (!neighbor.aristaActiva) { //si ya hemos quitado ese arista no se hace nada
                //cout << "Ya bloqueada." << '\n';
                continue;
            }

            //En otro caso, bloqueamos el arista temporalmente (mientras calculamos propagación)
            neighbor.aristaActiva = false;

            //NOTA 1 (ver final)

            deque<string> dcola_incendiados;

            //-------- Cálculo de la propagación del incendio sin el arista:
            int total = 0, media = 0;
            for (int i = 0; i < 30; i++) {
                //-------- Creación de la cola de incendiados (x inicialmente y en cada etapa se van sumando al final los que se prenden)
                iniciarCola(dcola_incendiados, graph_data, random_numbers);

                int numNodosIncendiados = random_numbers.size();
                start(graph_data, dcola_incendiados, numNodosIncendiados, gen);
                total = total + numNodosIncendiados;
                //cout << "\nNumero de nodos incendiados: " << numNodosIncendiados << '\n';
            }
            media = total / 30;

            //cout << "media: " << media << '\n';

            //NOTA 2 (ver final)

            //NOTA 3 (ver final)

            //-------- Recuperación del arista bloqueado temporalmente
            neighbor.aristaActiva = true;

            //-------- Comprobación de si es el mejor resultado hasta el momento (menor influencia provoca) y si lo es, lo guardo
            if (media < minNumNodosIncendiados) {
                minNumNodosIncendiados = media;
                aristaSeleccionado.first = node.name;
                aristaSeleccionado.second = neighbor.name;
                indiceVecinoSelec = i;
                //cout << "Mejorando...\n";
            }

        } //final del for de vecinos
    }//final del for de nodos

    //----- Eliminar el arista más influyente de manera definitiva
    auto it = graph_data.find(aristaSeleccionado.first); //O(1)
    Node& node = it->second;
    node.neighbors[indiceVecinoSelec].aristaActiva = false; //O(1)

    //----- Mostrar por pantalla
    cout << "\n ----- Arista bloqueado definitivamente " << aristaSeleccionado.first << "--->" << aristaSeleccionado.second <<
        ". Influencia: " << minNumNodosIncendiados << '\n';
    cout << '\n';
    
    //----- Guardar en txt
    resultados << "\n ----- Arista bloqueado definitivamente " << aristaSeleccionado.first << "--->" << aristaSeleccionado.second <<
        ". Influencia: " << minNumNodosIncendiados << '\n';

    return aristaSeleccionado;
}


int simularIC(unordered_map<string, Node>& graph_data, const vector<string>& random_numbers, mt19937& gen, entrada e) {
    bool encontrado = false;
    int numNodosIncendiados = random_numbers.size();
    //se elimina 1 arista (la de la entrada e) y se calcula la influencia
    auto it = graph_data.find(to_string(e.ar.origen));
    Node& node = it->second;

    for (int i = 0; (i < node.neighbors.size()) && !encontrado; i++) {
        Neighbor& neighbor = node.neighbors[i];

        if (stoi(neighbor.name) != e.ar.destino) { 
            continue;
        }
        
        //cuando llegamos al vecino (arista) que queremos:
        encontrado = true;
        //-------- Bloqueamos el arista que nos da e. COmentar linea para calcular influencia de la red
        neighbor.aristaActiva = false;
        //-------- Creación de la cola de incendiados (x inicialmente y en cada etapa se van sumando al final los que se prenden)
        deque<string> dcola_incendiados;
        iniciarCola(dcola_incendiados, graph_data, random_numbers);

        //-------- Cálculo de la propagación del incendio sin el arista:
        start(graph_data, dcola_incendiados, numNodosIncendiados, gen);
        //cout << "Simulando... Numero de nodos incendiados: " << numNodosIncendiados << '\n';
    }

    return numNodosIncendiados;
}
