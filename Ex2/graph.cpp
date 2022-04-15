#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

/*
    Realiza uma busca em largura começando pelo vértice 'x';
    mark = marca se o vértice já foi visitado (precisa ter todos
    os seus valores em 0 antes de chamar a função);
    matriz = matriz que vai ser preenchida com os valores da distância de
    de i a j, ou seja, matriz[i-1][j-1] = dist(i,j).
*/
void buscaLargura(int x,int *mark,vector <int> *listaAdjacencia, int **matriz){
    queue <int> fila;

    matriz[x-1][x-1] = 0;

    fila.push(x);
    mark[x-1]=1;

    while(fila.size()>0){
        int primeiro = fila.front(), sizeList = (int)listaAdjacencia[primeiro-1].size();
        fila.pop();


        for(int i=0;i < sizeList;i++){
            int y = listaAdjacencia[primeiro-1][i];

            if(mark[y-1] == 0){
                matriz[x-1][y-1] = matriz[x-1][primeiro-1]+1;
                fila.push(y);
                mark[y-1] = 1;
            }
        }
    }
}

/*
    Lê um arquivo pajek e cria uma lista de adjacencia do grafo correspondente;
    *nVertices = número de vértices a serem passados por referência;
    flag = se for true, então o grafo é direcionado
    se false, não é direcionado.
*/
vector <int> *readPajekGraph(int *nVertices, bool flag){
    fstream file;
    string fileName;
    vector <int> *listaAdjacencia;

    getline(cin,fileName);

    file.open(fileName,ios::in);

    if(!file){
        cout << "Arquivo inexistente.\n";
        return nullptr;
    }
    file.seekp(10, std::ios_base::beg); //Pula o *Vertices

    file >> (*nVertices);
    listaAdjacencia = new vector <int> [(*nVertices)];
    
    file.seekp(8, std::ios_base::cur); //Pula o *Edges

    while(!file.eof()){
        int node1,node2;

        file >> node1 >> node2;

        if(file.eof()){
            break;
        }
        
        listaAdjacencia[node1-1].push_back(node2);
        if(flag){ //Se for direcionado, vai colocar o vertice nos dois sentidos
            listaAdjacencia[node2-1].push_back(node1);
        }
    }
    file.close();

    return listaAdjacencia;
}

/*
    Cria a matriz NxN que representa o grafo
    matriz[i-1][j-1] = dist(i,j);
    Se não houver caminho de i para j, então dist(i,j) = -1
*/
int **createMatriz(int nVertices,vector <int> *listaAdjacencia){

    int *mark = new int[nVertices];
    int **matriz = new int*[nVertices];
    for(int i=0;i<nVertices;i++){
        matriz[i] = new int[nVertices];
    }

    for(int i=0;i<nVertices;i++){
        for(int j=0;j<nVertices;j++){
            mark[j]=0;
            matriz[i][j] = -1; //cada valor da linha i vai ser inicializado com -1 (caso o grafo seja desconexo)
        }
        buscaLargura(i+1,mark,listaAdjacencia,matriz);
    }
    delete mark;

    return matriz;
}

void printMatrix(int **matrix,int size){
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

void freeMatriz(int **matriz,int nVertices){
    for(int i=0;i<nVertices;i++){
        delete matriz[i];
    }
    delete matriz;
}

void freeList(vector <int> *listaAdjacencia, int nVertices){
    for(int i=0;i<nVertices;i++){
        listaAdjacencia[i].clear();
    }
    delete[] listaAdjacencia;
}

int main(){
    int **matrizGrafo,nVertices;
    vector <int> *listaAdjacencia;

    listaAdjacencia = readPajekGraph(&nVertices,true);
    if(listaAdjacencia == nullptr){
        return 0;
    }

    matrizGrafo = createMatriz(nVertices,listaAdjacencia);

    printMatrix(matrizGrafo,nVertices);

    freeMatriz(matrizGrafo,nVertices);
    freeList(listaAdjacencia,nVertices);

    return 0;
}