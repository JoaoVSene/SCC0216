//João Victor Sene Araújo - BCC 020 - 05/2021

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Vai gerar uma matrix M quadrada NxN de um grafo modelo Erdos-Renyi
int **generateMatrix(int N, float p){
    int **matrix = malloc(sizeof(int*)*N);
    
    srand(time(NULL));
    
    for(int i=0;i<N;++i){
        matrix[i] = malloc(sizeof(int)*N);
        for(int j=0;j<N;++j){
            if(i==j){
                matrix[i][j]=0;
            }else if(i<j){
                if(p > (rand() / (RAND_MAX + 1.))){
                    //negado
                    matrix[i][j]=0;
                }else{
                    //permitido
                    matrix[i][j]=1;
                }
            }else{
                matrix[i][j]=matrix[j][i];
            }
        }
    }
    
    return matrix;
}

//Imprime na tela quais vertices são adjacentes a um determinado vertice dado e qual o grau desse vertice,
//retornando o grau do vertice
int vertexDegree(int **matrix, int N, int vertex){
    int count=0;

    printf("Vertices adjacentes a %d:\n",vertex);
    for(int i=0;i<N;++i){
        if(matrix[vertex-1][i]>=1){
            printf("%d ",i+1);
            count++;
        }
    }
    printf("\nGrau do vertice %d: %d.\n",vertex,count);

    return count;
}

//Verifica e imprime se existe aresta entre dois vertices (retorna 1 se existe, 0 se não)
//Retorna -1 se não existir um ou mais vertices
int existEdge(int **matrix, int N, int vertexI, int vertexJ){
    if(vertexI > N || vertexJ > N || vertexI <= N || vertexJ <= 0){
        printf("Não existe um ou mais vertices\n");
        return -1;
    }else if(matrix[vertexI-1][vertexJ-1]>=1){
        printf("Existe aresta entre o vertice %d e o vertice %d\n",vertexI, vertexJ);
        return 1;
    }else{
        printf("Não existe aresta entre o vertice %d e o vertice %d\n",vertexI, vertexJ);
        return 0;
    }
}

//Imprime e libera da memória a matrix criada
void printAndFreeMatrix(int **matrix, int N){
    
    for(int i=0;i<N;++i){
        for(int j=0;j<N;++j){
            printf("%d  ",matrix[i][j]);
        }
        printf("\n");
        free(matrix[i]);
    }
    free(matrix);
}

int main(){
    int N, **matrix;
    float p;

    printf("Digite o tamanho 'N' da matriz NxN e o parâmetro 'p' flutuante (separe por espaço):\n");
    scanf("%d %f",&N,&p);

    matrix = generateMatrix(N,p);
    //Testes estáticos:
    vertexDegree(matrix,N,1);
    existEdge(matrix,N,1,2);
    printAndFreeMatrix(matrix,N);

    return 0;
}