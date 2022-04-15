#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

/*
    Struct da lista de adjacencia
    Guarda o vertice que está no final do arco em 'vertex' e o peso em 'weight'
*/
struct node{
    int vertex;
    int weight;
};

/*
    Struct da fila de prioridade
    Guarda o arco de um grafo, sendo vertex1 -> vertex 2, com peso weight
*/
struct arc{
    int vertex1;
    int vertex2;
    int weight;

    arc(int vertex1, int vertex2, int weight)
        : vertex1(vertex1),vertex2(vertex2),weight(weight)
    {
    }
};

struct compareWeight{ // Comparador para determinar a ordem da fila de prioridade
    bool operator()(arc const& a1, arc const& a2){
        return a1.weight > a2.weight;
    }
};

/*
    Lê um arquivo pajek e depois cria/retorna uma lista de adjacencia do grafo correspondente;
    *nVertices = número de vértices a serem passados por referência;
    undirected = se for true, então o grafo é não direcionado; se false, é direcionado.
*/
vector <node> *readPajekGraph(int *nVertices){
    fstream file;
    string fileName, connectionType;
    vector <node> *listaAdjacencia;
    bool undirected;

    getline(cin,fileName);

    file.open(fileName,ios::in);

    if(!file){
        cout << "Arquivo inexistente.\n";
        return nullptr;
    }
    file.seekp(10, std::ios_base::beg); //Pula o *Vertices

    file >> (*nVertices);
    listaAdjacencia = new vector <node> [(*nVertices)];
    
    file.seekp(2, std::ios_base::cur); //Pula o \n

    getline(file,connectionType);

    string str("*Edges\r");
    
    if(connectionType.compare(str)==0){
        undirected = true;
    }else{
        undirected = false;
    }

    while(!file.eof()){
        int node1,node2,weight;
        node newArc;

        file >> node1 >> node2 >> weight;

        if(file.eof()){
            break;
        }

        newArc.vertex = node2;
        newArc.weight = weight;

        listaAdjacencia[node1-1].push_back(newArc);

        if(undirected){ //Se for direcionado, vai colocar o vertice nos dois sentidos
            newArc.vertex = node1;

            listaAdjacencia[node2-1].push_back(newArc);
        }
    }
    file.close();

    return listaAdjacencia;
}

/*
    Realiza o algoritmo de Prim para um dado vertice 'x';
    Retorna a soma dos pesos da árvore geradora mínima formada
*/
int prim(int x, int nVertices, bool *mark, vector <node> *listaAdjacencia){
    int sumWeight=0;
    priority_queue<arc, vector<arc>, compareWeight> pQueue;

    for(int i=1;i<nVertices;i++){ // Executa nVertices-1 vezes (número de lados presentes na árvore geradora mínima)
        int sizeList = (int) listaAdjacencia[x-1].size();

        mark[x-1]=true; // Marca o vertice 'x' como visitado

        for(int i=0;i<sizeList;i++){ // Adiciona na lista de prioridades os arcos saindo de 'x'
            node y = listaAdjacencia[x-1][i];

            pQueue.push(arc(x,y.vertex,y.weight));
        }

        do{ 
            arc aux = pQueue.top(); // Pega os arcos com menor peso

            if(mark[aux.vertex2-1] == false){ // Se o arco aponta para um vetice já visitado, então ele é descartado
                // Se não, o peso desse arco é somado a soma dos pesos da arvore geradora mínima
                sumWeight += aux.weight;
                x = aux.vertex2; // Agora, o algoritmo irá se repetir no vertice apontado pelo arco
                pQueue.pop();
                break;
            }
            pQueue.pop();
        }while(!pQueue.empty());
    }

    return sumWeight;

}

// Libera a lista da memória
void freeList(vector <node> *listaAdjacencia, int nVertices){
    for(int i=0;i<nVertices;i++){
        vector<node>().swap(listaAdjacencia[i]);
    }
    delete[] listaAdjacencia;
}

int main(){
    int nVertices, sumWeight;
    vector <node> *listaAdjacencia;
    bool *mark;

    listaAdjacencia = readPajekGraph(&nVertices);
    if(listaAdjacencia == nullptr || nVertices<=0){
        return 0;
    }

    mark = new bool[nVertices];
    for(int i=0;i<nVertices;i++){ // Marca todos os vertices como não visitados
        mark[i]=false;
    }

    sumWeight = prim(1,nVertices,mark,listaAdjacencia); // Realiza uma arvore geradora a partir do vertice 1

    delete mark;

    cout << sumWeight << endl;
    
    freeList(listaAdjacencia,nVertices);

    return 0;
}