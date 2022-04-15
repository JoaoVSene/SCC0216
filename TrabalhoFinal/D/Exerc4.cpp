/* Trabalho Final - Modelagem Computacional em Grafos
    (Parte D)
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


/* Percorre o grafo e marca em um vetor booleano os vértices sorvedouros.
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
void buscaProfundidade(int vertex,int *marked,vector <int> *listaAdjacencia,int** matrizDistancias, int* parentsDistance){

    int sizeList = (int)listaAdjacencia[vertex-1].size(); 

    for(int i=0 ; i < sizeList ; i++){
        int y = listaAdjacencia[vertex-1][i];
        
        marked[y-1]++;
        parentsDistance[y-1] += matrizDistancias[0][vertex-1];

        if(marked[y-1] == 1) {
            buscaProfundidade(y,marked,listaAdjacencia,matrizDistancias,parentsDistance);
        }
    }
}

/*  Imprime os vértices desejados, aqueles em que a soma da distância dos pais (em relação ao vértice 1) é mínima, que são 
    sorvedouros e que os pais são descendentes do vértice 1.
        sinks: vetor com os vérties sorvedouros marcados como true
        descendants: vetor que marcam os vértices que possuem ambos os pais descendentes de 1. Todos os valores precisam ser inicializados com 0
        nVertices: número total de vértices do grafo
        parentsDistance: vetor com a soma das distâncias dos pais em relação ao vértice 1, de cada vértice.Todos os valores precisam ser inicializados com 0
            Ex: parentsDistance[i] = dist(1,a) + dist(1,b) , sendo a e b os pais de i

*/
void printTargets(bool* sinks, int* descendants, int nVertices, int* parentsDistance) {

    int minDistance = INT_MAX;

    for (int i = 0; i < nVertices; i++) {
        // Se o vértice for um sorvedouro e os seus pais forem descendentes de 1
        if ((sinks[i] == true) && (descendants[i] == 2) && (parentsDistance[i] < minDistance)) {
            minDistance = parentsDistance[i];
        }
    }

    for (int i = 0; i < nVertices; i++) {
        // Se o vértice for um sorvedouro e os seus pais forem descendentes de 1
        if ((sinks[i] == true) && (descendants[i] == 2)  && (parentsDistance[i] == minDistance)) {
            cout << i + 1 << endl;
        }
    }
}

/*  Realiza uma busca em largura começando pelo vértice 'x';
        mark = marca se o vértice já foi visitado (precisa ter todos
        os seus valores em 0 antes de chamar a função);
        matriz = matriz que vai ser preenchida com os valores da distância
        de i a j, ou seja, matriz[i-1][j-1] = dist(i,j).
*/
void buscaLargura(int x,bool *mark,vector <int> *listaAdjacencia, int **matriz){
    queue <int> fila;

    matriz[x-1][x-1] = 0;

    fila.push(x);
    mark[x-1]=true;

    while(fila.size()>0){
        int primeiro = fila.front();
        int sizeList = (int)listaAdjacencia[primeiro-1].size();
        fila.pop();

        for(int i=0;i < sizeList;i++){
            int y = listaAdjacencia[primeiro-1][i];

            if(mark[y-1] == false){
                matriz[x-1][y-1] = matriz[x-1][primeiro-1]+1;
                fila.push(y);
                mark[y-1] = true;
            }
        }
    }
}

/*  Cria a matriz NxN que representa o grafo
        matriz[i-1][j-1] = dist(i,j);
        Se não houver caminho de i para j, então dist(i,j) = -1
*/
int **createMatriz(int nVertices,vector <int> *listaAdjacencia){

    bool *mark = new bool[nVertices];
    int **matriz = new int*[nVertices];
    
    for(int i=0;i<nVertices;i++){
        matriz[i] = new int[nVertices];
    }
    
    for(int i=0;i<nVertices;i++){
        for(int j=0;j<nVertices;j++){
            mark[j]=false;
            matriz[i][j] = -1; //cada valor da linha i vai ser inicializado com -1 (caso o grafo seja desconexo)
        }
        buscaLargura(i+1,mark,listaAdjacencia,matriz);
    }

    delete mark;

    return matriz;
}

void freeMatriz(int **matriz,int nVertices){
    for(int i=0;i<nVertices;i++){
        delete matriz[i];
    }
    delete matriz;
}

int main(){
    int nVertices; 
    vector <int> *listaAdjacencia; 
    int* descendants; // vetor que marcam os vértices que possuem ambos os pais descendentes de 1
    bool* sinks; // vetor com os vérties sorvedouros marcados como true
    int** matrizDistancias; // matriz com as distâncias de todos os vértices em relação a todos
    int* parentsDistance; // vetor com a soma das distâncias dos pais em relação ao vértice 1, de cada vértice.
   

    // Lendo o arquivo pajek
    listaAdjacencia = readPajekGraph(&nVertices);
    if(listaAdjacencia == nullptr || nVertices<=0){
        return 0;
    }

    // Criamos uma matriz de distancias, fazendo uma busca em largura em todos os vertices
    matrizDistancias = createMatriz(nVertices,listaAdjacencia);

    descendants = new int[nVertices];
    parentsDistance = new int[nVertices];

    for (int i = 0;i < nVertices; i++) {
        descendants[i] = 0;
        parentsDistance[i] = 0;
    }

    sinks = markSinks(listaAdjacencia,nVertices);
    buscaProfundidade(1,descendants,listaAdjacencia,matrizDistancias,parentsDistance);
    
    // Imprimos os vertices desejados
    printTargets(sinks,descendants,nVertices,parentsDistance);
       
    freeList(listaAdjacencia,nVertices);
    freeMatriz(matrizDistancias,nVertices);
    delete descendants;
    delete sinks;
    delete parentsDistance;

    return 0;
}