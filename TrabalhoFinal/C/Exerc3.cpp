/* Trabalho Final - Modelagem Computacional em Grafos
    (Parte C)
Data de Entrega: 03/08/2021
Professor: Kuruvilla Joseph Abraham
Alunos:
    João Victor Sene Araújo - 11796382
    Pedro Augusto Ribeiro Gomes - 11819125
*/

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

/*
    Lê um arquivo pajek e depois cria/retorna uma lista de adjacencia do grafo correspondente;
    *nVertices = número de vértices a serem passados por referência;
    undirected = se for true, então o grafo é não direcionado; se false, é direcionado.
*/
vector <int> *readPajekGraph(int *nVertices){
    fstream file;
    string fileName, connectionType;
    vector <int> *listaAdjacencia;
    bool undirected;

    getline(cin,fileName);

    file.open(fileName,ios::in);

    if(!file){
        cout << "Arquivo inexistente.\n";
        return nullptr;
    }
    file.seekp(10, std::ios_base::beg); //Pula o *Vertices

    file >> (*nVertices);
    listaAdjacencia = new vector <int> [(*nVertices)];
    
    file.seekp(2, std::ios_base::cur); //Pula o \n

    getline(file,connectionType);

    string str("*Edges\r");
    
    if(connectionType.compare(str)==0){
        undirected = true;
    }else{
        undirected = false;
    }

    while(!file.eof()){
        int node1,node2;

        file >> node1 >> node2;

        if(file.eof()){
            break;
        }

        listaAdjacencia[node1-1].push_back(node2);

        if(undirected){ //Se for não-direcionado, vai colocar o vertice nos dois sentidos
            listaAdjacencia[node2-1].push_back(node1);
        }
    }
    file.close();

    return listaAdjacencia;
}

// Libera a lista da memória
void freeList(vector <int> *listaAdjacencia, int nVertices){
    for(int i=0;i<nVertices;i++){
        vector<int>().swap(listaAdjacencia[i]);
    }
    delete[] listaAdjacencia;
}

/*  Percorre o grafo e marca em um vetor booleano os vértices sorvedouros.
        listaAdjacencia: lista com as listas de adjacências de cada vértice do grafo
        nVertices: número de vértices do grafo
*/
bool* markSinks(vector <int> *listaAdjacencia, int nVertices){
    
    bool *mark; // vetor boolean com quantidade de nós do grafo, marcando se o nó possui (true) ou não (false) filho(s)
    int sizeList;

    mark = new bool[nVertices];

    for(int i=0;i<nVertices;i++){ // Marca todos os vertices com false
        mark[i]=false;
    }
    
    for(int i=0;i<nVertices;i++){
        sizeList = (int)listaAdjacencia[i].size();

        if(sizeList == 0) {
            mark[i] = true;
        }
    }

    // Retorna o vetor com os vértices sorvedouros marcados como true
    return mark;
}

/*
    Busca em profundidade que marca os vertices no qual ambos os pais são descendentes de um dado vertice
*/
void buscaProfundidade(int vertex,int *marked,vector <int> *listaAdjacencia){

    int sizeList = (int)listaAdjacencia[vertex-1].size(); 

    for(int i=0 ; i < sizeList ; i++){
        int y = listaAdjacencia[vertex-1][i];
        
        marked[y-1]++;

        if(marked[y-1] == 1) {
            buscaProfundidade(y,marked,listaAdjacencia);
        }
    }
}

/*
    Imprime os vértices sorvedouros e que os pais são descendentes do vértice 1
*/
void printTargets(bool* sinks, int* descendants, int nVertices) {

    for (int i = 0; i < nVertices; i++) {
        // Se o vértice for um sorvedouro e os seus pais forem descendentes de 1
        if ((sinks[i] == true) && (descendants[i] == 2)) {
            cout << i + 1 << endl;
        }
    }
}

int main(){
    int nVertices;
    vector <int> *listaAdjacencia;
    int* descendants; // vetor que marcam os vértices que possuem ambos os pais descendentes de 1
    bool* sinks; // vetor que marcam os vértices que possuem ambos os pais descendentes de 1

    // Lendo o grafo do pajek
    listaAdjacencia = readPajekGraph(&nVertices);
    if(listaAdjacencia == nullptr || nVertices<=0){
        return 0;
    }

    descendants = new int[nVertices];

    for (int i = 0;i < nVertices; i++) {
        descendants[i] = 0;
    }

    sinks = markSinks(listaAdjacencia,nVertices);
    buscaProfundidade(1,descendants,listaAdjacencia);

    printTargets(sinks,descendants,nVertices);
       
    freeList(listaAdjacencia,nVertices);

    return 0;
}