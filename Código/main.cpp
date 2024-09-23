#include <random>
#include <deque>
#include <cstdlib>
#include <time.h>
#include "algoritmo.h"
#include "heuristicas.h"

using namespace std;

vector<string> generateRandomNumbers(int min, int max, int cantidad, mt19937& gen) {
    vector<string> resultado;
    uniform_int_distribution<> dis(min, max);
    for (int i = 0; i < cantidad; ++i) {
        int random_num = dis(gen);
        resultado.push_back(to_string(random_num));
    }
    return resultado;
}

int main() {

    //para generar numeros aleatorios. Opcion 1
    mt19937 gen(random_device{}());

    int num_aristas = 0, num_nodos = 4039; // = 4039; //475
    unordered_map<string, Node> graph_data; 

    //----------Leer datos y generar el grafo a partir del FaceBook dataset--------------------
    leerDataset(graph_data, num_aristas);

    cout << "Numero de aristas: " << num_aristas << '\n';
    cout << "Numero de nodos: " << num_nodos << '\n'; //4039

    //----------Mostrar grafo: vertices con sus aristas---------------------
    //for (const auto& it : graph_data)  it.second.check_node(); 

    //PARÁMETROS
    int k = 5;  //50 es el número de aristas que se pueden bloquear (k = 50)
    int nodosIniciales = 3; //número de nodos que se prenden inicialmente

    //Generacion aleatoria de nodos inicialmente incendiados
    //vector<string> random_numbers = generateRandomNumbers(0, 4038, nodosIniciales, gen);
    //vector<string> random_numbers = {"0", "4"}; // {"3"}; para probar ejemplo OutDegree //{"0"}; //para probar ejemplo basico
    /*vector<string> random_numbers = { "175", "237", "171", "80", "211", "15", "239", "202",
        "236", "102", "219", "136", "8", "208", "183", "45", "163", "138", "9", "232", "97",
        "199", "247", "105", "31", "24", "36", "212", "238", "210", "197", "26", "122", "234",
        "196", "235", "73", "186", "44", "191", "86", "206", "96", "192", "153", "38", "216",
        "65", "6", "16" 
    }; //este es el conjunto fijo 2
    vector<string> random_numbers = { "240", "1679", "2916", "2312", "3176", "3533", "1210", "3562", "4001", "12",
        "2470", "1386", "3979", "3513", "1676", "3854", "11", "540", "3637", "1435",
        "2896", "1819", "3006", "3866", "1300", "1014", "1802", "2161", "1835", "1323",
        "2036", "2861", "1214", "3164", "2476", "2156", "3696", "2006", "1817", "2686",
        "2838", "1491", "1790", "1037", "2258", "3869", "518", "2924", "2265", "381" 
    }; //este es el conjunto fijo 1
    */
    vector<string> random_numbers = { "175", "0", "6" };
    cout << "Newly generated numbers: ";
    int j = 0;
    for (string num : random_numbers) {
        cout << num << " ";
        j++;
    }
    cout << "\nSe han generado " << j << " numero(s).\n";

    //PARA CALCULAR LA INFLUENCIA SIN BLOQUEAR (comentar linea en simularIC)
    int total = 0, media = 0;
    entrada e = { {0,1},1 }; //{{0,1},1} para FB //{{0,4},1} para twitter
    //para ir borrando aristas
    //simularIC(graph_data, random_numbers, gen, e);
    //e = { {238,274},1 };
    //simularIC(graph_data, random_numbers, gen, e);
    //e = { {17,198},1 };
    //simularIC(graph_data, random_numbers, gen, e);
    //e = { {17,198},1 };
    //simularIC(graph_data, random_numbers, gen, e);
    //e = { {17,198},1 };
    
    for (int i = 0; i < 10000; i++) {
        int influencia = simularIC(graph_data, random_numbers, gen, e);
        //cout << influencia << '\n';
        total += influencia;
    }
    media = total / 10000;
    cout << media << '\n';

    //EXTRA PARA PROBAR LAS 2 HEURISTICAS A LA VEZ
    //unordered_map<string, Node> graph_data2 = graph_data;

    
    //-----------------------------------------------HEURÍSTICAS-----------------------------------------------
    //1: Betweenness scoring method --------------------> sin terminar
    //betweennessScoringMethod(graph_data, random_numbers, k);
    //cout << "\nBetweenness method finished." << '\n';

    /*
    //-----------------------------2: Out-degree scoring method (solo las aristas de O(S))-----------------------------
    priority_queue<entrada> solucionOD;
    solucionOD = outdegreeMethod(graph_data, random_numbers, k);
    cout << "\nOutdegree method finished." << '\n';
    //para extraer las aristas con las puntuaciones más altas:
    int x = 0;
    ofstream archivoOD("salidaOD.txt");
    while (!solucionOD.empty() && x < k) {
        entrada e = solucionOD.top();

        int media = 0, total = 0;
        for (int i = 0; i < 1000; i++) {
            int influencia = simularIC(graph_data, random_numbers, gen, e);
            total += influencia;
        }
        media = total / 1000;

        cout << "\nArista bloqueada: (" << e.ar.origen << ", " << e.ar.destino
            << "), Puntuacion: " << e.puntuacion << '\n' << "Simulando... Numero de nodos incendiados: " <<
            media << '\n';


        archivoOD << "\nArista bloqueada: (" << e.ar.origen << ", " << e.ar.destino
            << "), Puntuacion: " << e.puntuacion << '\n' << "Simulando... Numero de nodos incendiados: " << 
            media << '\n';

        solucionOD.pop();
        x++;
    }
    archivoOD.close();
    //*/
    
    /*
    //---------------3: Out-degree scoring method extendido: para todo arista y no solo los de O(S)-----------------------
    priority_queue<entrada> solucionODExt;
    solucionODExt = extendedOutdegreeMethod(graph_data, random_numbers, k);
    cout << "\nExtended outdegree method finished." << '\n';
    //para extraer las aristas con las puntuaciones más altas:
    int y = 0;
    ofstream archivoODExt("salidaODExt.txt");
    while (!solucionODExt.empty() && y < k) {
        entrada e = solucionODExt.top();

        int media = 0, total = 0;
        for (int i = 0; i < 1000; i++) {
            int influencia = simularIC(graph_data, random_numbers, gen, e);
            total += influencia;
        }
        media = total / 1000;

        cout << "\nArista bloqueada: (" << e.ar.origen << ", " << e.ar.destino
            << "), Puntuacion: " << e.puntuacion << '\n' << "Simulando... Numero de nodos incendiados: " <<
            media << '\n';

        archivoODExt << "\nArista bloqueada: (" << e.ar.origen << ", " << e.ar.destino
            << "), Puntuacion: " << e.puntuacion << '\n' << "Simulando... Numero de nodos incendiados: " <<
            media << '\n';

        solucionODExt.pop();
        y++;
    }
    archivoODExt.close();
    //*/


    /*
    //--------------------------------ALGORITMO VORAZ (GREEDY)------------------------------------------------
    vector <pair<string, string>> conjuntoSolucion(k);

    ofstream resultados("resultados.txt");
    for (int i = 0; i < k; i++){
        //elegir arista a bloquear y eliminarla del grafo
        cout << "Eliminando arista " << i + 1 << ":" << '\n';
        pair<string, string> arista = seleccionarArista(graph_data, random_numbers, gen, resultados);

        //añadir arista a solucion
        conjuntoSolucion[i] = arista;
    }
    resultados.close();

    cout << '\n' << "El conjunto de aristas bloqueadas es ";
    for (int i = 0; i < k; i++) {
        cout << conjuntoSolucion[i].first << "--->" << conjuntoSolucion[i].second << " ";
    }
    cout << '\n';
    //*/
    
    return 0;
}

//--------------------------------APUNTES EXTRA-----------------------------
//FUNCIONES DE INFO PARA GRAFOS PEQUEÑOS 
/*for (const auto& it : graph_data) {
    it.second.check_node();
}
for (const auto& it : graph_data) {
    it.second.check_state();
}*/

//para mostrar que nodos estan quemados y cuales no
/*ofstream salida("salida.txt");
for (const auto& it : graph_data) {
    it.second.check_state(salida, num_nodosIncendiados);
}
salida.close();*/

//COMPROBACION DE QUE SE HAN BLOQUEADO LOS ARISTAS
/*int numAristasBloqueadas = 0;
for (auto& it : graph_data) {
    Node& node = it.second;
    for (int i = 0; i < node.neighbors.size(); i++) {
        Neighbor& neighbor = node.neighbors[i];
        if (neighbor.aristaActiva == false) {
            numAristasBloqueadas++;
        }
    }
}
cout << "Numero total de aristas bloqueadas = " << numAristasBloqueadas << '\n';*/
//*/

//----------------------NOTAS----------------------------------------------------------------
//NOTA 1: eficiencia de la propagacion
//creo una copia del grafo cada vez  FATAL EN TERMINOS DE EFICIENCIA
//unordered_map<string, Node> graph_data_copy = graph_data;
//OTRAS OPCIONES:
//recuperar la parte que cambiemos de graph_data (implementada ahora mismo en start) VER

//NOTA 2: para guardar resultados
//ofstream resultados("resultados.txt");
//resultados << "Numero de nodos incendiados al quitar el arista " << node.name << "--->" << neighbor.name << ": " << numNodosIncendiados << '\n';
//resultados.close();

//NOTA 3: 
//PRUEBA con dataset facebook -> funciona y para en 175 !!!!!!!!!!! 
//if (numNodosIncendiados != 176) {
//    cin.get();
//}
