#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

/*
    Lê um arquivo pajek e cria uma lista de adjacencia do grafo correspondente;
    *nVertices = número de vértices a serem passados por referência;
    undirected = se for true, então o grafo é não direcionado
    se false, é direcionado.
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

    if(connectionType.compare("*Edges")){
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
        if(undirected){ //Se for direcionado, vai colocar o vertice nos dois sentidos
            listaAdjacencia[node2-1].push_back(node1);
        }
    }
    file.close();

    return listaAdjacencia;
}

//  Libera a lista da memória
void freeList(vector <int> *listaAdjacencia, int nVertices){
    for(int i=0;i<nVertices;i++){
        vector<int>().swap(listaAdjacencia[i]);
    }
    delete[] listaAdjacencia;
}

/*
    Busca em profundidade que contabiliza quantos vertices são encontras na busca 
    a partir de um vértice dado.
*/
void buscaProfundidade(int vertex,bool *visited,vector <int> *listaAdjacencia,int *nVertexPerComponent){
    visited[vertex-1] = 1;

    (*nVertexPerComponent)++;

    int size = (int)listaAdjacencia[vertex-1].size();

    for(int i=0;i<size;i++){
        int y = listaAdjacencia[vertex-1][i];
        
        if(visited[y-1] == 0){
            buscaProfundidade(y,visited,listaAdjacencia,nVertexPerComponent);
        }
    }
}

/*
    Cria a lista que vai conter a quantidade de vértices de cada componente do grafo;
    Retorna a lista criada.
*/
vector <int> setComponents(vector <int> *listaAdjacencia, int nVertices){
    bool *visited = new bool[nVertices];
    vector <int> listaComponentes;

    for(int i=0;i<nVertices;i++){ //Marca os vertices como não visitados
        visited[i]=false;
    }

    for(int i=0;i<nVertices;i++){
        if(visited[i]==0){ //Se o Vertice não foi visitado, então é iniciada uma busca em profundidade
                        // com o objetivo de marcar os vertices pertencentes a componente do vértice iniciado
            int nVertexPerComponent=0;

            buscaProfundidade(i+1,visited,listaAdjacencia,&nVertexPerComponent);

            listaComponentes.push_back(nVertexPerComponent); //Coloca na lista o número de vértices do componente encontrado
        }
    }

    sort(listaComponentes.begin(),listaComponentes.end(),greater<int>()); //Ordena em ordem decrescente os valores de cada componente

    return listaComponentes;
}

void printComponentList(vector <int> listaComponentes){
    cout << listaComponentes.size() << "\n";
    int size = listaComponentes.size();

    for(int i=0;i<size;i++){
        cout << listaComponentes[i] << "\n";
    }
}

int main(){
    int nVertices;
    vector <int> *listaAdjacencia,listaComponentes;

    listaAdjacencia = readPajekGraph(&nVertices);
    if(listaAdjacencia == nullptr || nVertices<=0){
        return 0;
    }

    listaComponentes = setComponents(listaAdjacencia, nVertices);

    printComponentList(listaComponentes);
    
    freeList(listaAdjacencia,nVertices);
    listaComponentes.clear();

    return 0;
}