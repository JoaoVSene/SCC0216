#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

/*
    Lê um arquivo pajek e cria uma lista de adjacencia do grafo correspondente;
    *nVertices = número de vértices a serem passados por referência;
    undirected = se for true, então o grafo é não direcionado se false, é direcionado.
    Retorna por referência a matriz de adjacencia, com matriz[i][j] = dist(i+1,j+1);
    Se matriz[i][j] == -1, então dist(i+1,j+1) == infinito.
*/
vector <int> *readPajekGraph(int *nVertices, int ***matrizAdjacencia){
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

    if(connectionType.compare("*Edges") == 0){
        undirected = true;
    }else{
        undirected = false;
    }

    (*matrizAdjacencia) = new int* [(*nVertices)];
    for(int i=0;i<(*nVertices);i++){
        (*matrizAdjacencia)[i] = new int [(*nVertices)];
        for(int j=0;j<(*nVertices);j++){
            (*matrizAdjacencia)[i][j] = -1;
        }
    }

    while(!file.eof()){
        int node1,node2;  

        file >> node1 >> node2 >> (*matrizAdjacencia)[node1-1][node2-1];

        if(file.eof()){
            break;
        }
        
        listaAdjacencia[node1-1].push_back(node2);
        if(undirected){ //Se for direcionado, vai colocar o vertice nos dois sentidos
            listaAdjacencia[node2-1].push_back(node1);
            (*matrizAdjacencia)[node2-1][node1-1] = (*matrizAdjacencia)[node1-1][node2-1];
        }
    }
    file.close();

    return listaAdjacencia;
}

/*
    Realiza o algoritmo de Dijkstra para um dao vertice 'x';
    Atualiza a matriz de adjacencia com os valores dos menores caminhos,
    de modo que matriz[x-1][i] = dist(x,i+1);
*/
void dijkstra(int x, int nVertices, bool *mark, vector <int> *listaAdjacencia, int **matriz){
    int smallerDistNode,smallerDist;
    bool verify;

    matriz[x-1][x-1] = 0;

    mark[x-1]=true; // Marca o vertice inicial como visitado

    // Visita(marca) todos os nVertices-1
    for(int i=0;i<nVertices-1;i++){

        smallerDist=INT_MAX;
        verify = false; // Variável auxiliar que verifica se ainda há vertices alcançáveis

        // Encontra o menor distancia entra x e um nó (até o momento)
        for(int j=0;j<nVertices;j++){
            if(matriz[x-1][j] < smallerDist && mark[j]==false && matriz[x-1][j]!=-1){
                verify = true;
                smallerDistNode = j;
                smallerDist = matriz[x-1][j];
            }
        }
        mark[smallerDistNode]=true; // Marca o nó encontrado acima

        if(verify==false){ // Se não há vertices alcançáveis, então o algoritmo para
            break;
        }

        int sizeList = (int) listaAdjacencia[smallerDistNode].size();
        // Vê os vertices adjacentes ao nó que possui menor caminho e substitui a distancia desses vertices
        // se a distancia atual for inf(-1) ou maior que a distancia descoberta;
        for(int j=0;j<sizeList;j++){
            int y = listaAdjacencia[smallerDistNode][j];
            if(matriz[x-1][y-1] == -1 || smallerDist + matriz[smallerDistNode][y-1] < matriz[x-1][y-1]){
                matriz[x-1][y-1] = smallerDist + matriz[smallerDistNode][y-1];
            }
        }
    }
}

void printMatrix(int **matrix,int nVertices){
    int maxNumber=-1;

    // Guarda em maxNumber o maior número presente na matriz
    for(int i=0;i<nVertices;i++){
        for(int j=0;j<nVertices;j++){
            if(matrix[i][j]>maxNumber)
                maxNumber = matrix[i][j];
        }
    }
    int k;
    // Guarda em k a quantidade de algarismos que maxNumber tem (considerando maxNumber positivo)
    for(k=1;maxNumber>10;k++)
        maxNumber = maxNumber/10;

    // Imprime de maneira formatada a matriz
    for(int i=0;i<nVertices;i++){
        for(int j=0;j<nVertices;j++){
            cout << setw (k)<< matrix[i][j] << ' ';
        }
        cout << '\n';
    }
}

// Libera a lista da memória
void freeList(vector <int> *listaAdjacencia, int nVertices){
    for(int i=0;i<nVertices;i++){
        vector<int>().swap(listaAdjacencia[i]);
    }
    delete[] listaAdjacencia;
}

// Libera a matriz da memória
void freeMatrix(int **matrizAdjacencia,int nVertices){
    for(int i=0;i<nVertices;i++){
        delete matrizAdjacencia[i];
    }
    delete matrizAdjacencia;
}

int main(){
    int nVertices, **matrizAdjacencia;
    vector <int> *listaAdjacencia;
    bool *mark;

    listaAdjacencia = readPajekGraph(&nVertices,&matrizAdjacencia);
    if(listaAdjacencia == nullptr || nVertices<=0){
        return 0;
    }

    mark = new bool[nVertices];

    for(int i=1;i<=nVertices;i++){
        for(int j=0;j<nVertices;j++){ // Inicializa todos os vertices como não visitados(marcados)
            mark[j]=false;
        }
        dijkstra(i,nVertices,mark,listaAdjacencia,matrizAdjacencia);
    }
    delete mark;

    printMatrix(matrizAdjacencia,nVertices);
    freeList(listaAdjacencia,nVertices);
    freeMatrix(matrizAdjacencia,nVertices);

    return 0;
}