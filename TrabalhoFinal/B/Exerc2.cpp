/* Trabalho Final - Modelagem Computacional em Grafos
    (Parte B)
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

/*  Retorna a quantidade de nós do grafo que não possuem filhos (nós sorvedouros)
        listaAdjacencia: lista com a lista de adjacência de cada vertice do grafo
        nVertices: número total de vértices do grafo
*/
int countSinks(vector <int> *listaAdjacencia, int nVertices){
    
    bool *mark; // vetor boolean com quantidade de nós do grafo, marcando se o nó possui (true) ou não (false) um pai
    int counter = 0; // conta o número de sorvedouros
    int sizeList;

    mark = new bool[nVertices];

    for(int i=0;i<nVertices;i++){ // Marca todos os vertices com false
        mark[i]=false;
    }
    
    for(int i=0;i<nVertices;i++){
        sizeList = (int)listaAdjacencia[i].size();

        for(int j=0;j<sizeList;j++){
            int vertex = listaAdjacencia[i][j];
            mark[vertex - 1] = true;
        }
    }

    for (int i=0;i<nVertices;i++) {
        sizeList = (int)listaAdjacencia[i].size();
        
        if (mark[i] == true && sizeList == 0) {
            counter++;
        } 
    }

    delete mark;

    // Retorna o número de vértices sorvedouros
    return counter;
}

int main(){
    int nVertices;
    vector <int> *listaAdjacencia;

    listaAdjacencia = readPajekGraph(&nVertices);
    if(listaAdjacencia == nullptr || nVertices<=0){
        return 0;
    }

    cout << countSinks(listaAdjacencia,nVertices) << endl;

    freeList(listaAdjacencia,nVertices);

    return 0;
}