#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INT_MAX 2000;
#define TRUE 1;
#define FALSE 0;

int nElementos = 0;

struct cidade {
    float px;
    float py;
    float custo;
};

float distancia(struct cidade a, struct cidade b);
void printCaminho(int *caminho);
void construcaoGulosa(float m[][nElementos], int* solucao_inicial);
void construcaoAleatoria(float m[][nElementos], int* caminho);
void copiaCaminho(int* origem, int* destino);
float calculaCusto(float m[][nElementos], int* caminho);
void swap(float m[][nElementos], int* solucao_inicial, int* melhor_vizinho);
void twoOpt(float m[][nElementos], int* solucao_inicial, float custo_solucao_inicial, int* melhor_vizinho);
void VND(float m[][nElementos], float custo_melhor_vizinho, int* melhor_vizinho, int* otimo_local);

int main(void) {

      int t;

      FILE* fp = fopen("dijbouti.txt", "r");
      //FILE* fp = fopen("bcl380.txt", "r");
      //FILE* fp = fopen("western sahara.txt", "r");
      //FILE* fp = fopen("dijbouti.txt", "r");
      //FILE* fp = fopen("dijbouti.txt", "r");
      //FILE* fp = fopen("dijbouti.txt", "r");
      //FILE* fp = fopen("dijbouti.txt", "r");
      //FILE* fp = fopen("dijbouti.txt", "r");
      //FILE* fp = fopen("dijbouti.txt", "r");
      //FILE* fp = fopen("dijbouti.txt", "r");
      //FILE* fp = fopen("dijbouti.txt", "r");

      //Lê numero de elementos
      fscanf(fp, "%d\n", &nElementos);  
      printf("Entrada:\n%d\n", nElementos);

      //Lê cidades e armazena em array do tipo struct cidade
      struct cidade cidades[nElementos];

      for(int i = 0; i < nElementos; i++) {
        fscanf(fp, "%f ", &t);
        fscanf(fp, "%f ", &cidades[i].px);
        fscanf(fp, "%f \n", &cidades[i].py);
        printf("%d %.2f %.2f\n", i+1, cidades[i].px, cidades[i].py); 
      }

      fclose(fp);
      
      printf("\n");

      //Calcula distância entre cidades, armazena em matriz[][] e imprime matriz
      float matriz[nElementos][nElementos];

      printf("Matriz gerada:\n");

      for(int i = 0; i < nElementos; i++) {
          for(int j = 0; j < nElementos; j++) {
              matriz[i][j] = distancia(cidades[i], cidades[j]);
              printf("%.2f ", matriz[i][j]);
          }
    }

    //Cria solucao gulosa inicial 
    int solucao_inicial[nElementos + 1];

    construcaoGulosa(matriz, solucao_inicial);
    printf("\n\nSolucao inicial gulosa: ");
    printCaminho(solucao_inicial);
    printf("Custo solução inicial gulosa: %d\n", calculaCusto(matriz, solucao_inicial));

    //Cria melhor vizinho a partir da solucao inicial com movimento de swap
    int melhor_vizinho[nElementos];
    
    printf("\n\nSolucao melhor vizinho (swap): ");
    swap(matriz, solucao_inicial, melhor_vizinho);
    printCaminho(melhor_vizinho);

    float custo_melhor_vizinho = calculaCusto(matriz, melhor_vizinho);
    printf("Custo solução melhor vizinho: %d\n", custo_melhor_vizinho);

    /*Cria caminho aleatorio e cria solucao com movimento de 2-opt
    printf("\n\nSolucao melhor vizinho (2-opt): ");
    twoOpt(matriz, solucao_inicial, custo_solucao_inicial, melhor_vizinho);
    
    printf("Custo solução melhor vizinho: %d\n", calculaCusto(matriz, melhor_vizinho));
    */

    //Cria otimo local e calcula com o VND
    int otimo_local[nElementos];
    
    VND(matriz, custo_melhor_vizinho, melhor_vizinho, otimo_local);
    printf("Custo ótimo local: %d", calculaCusto(matriz, otimo_local));

    return 0;
}

void GRASP(){
  
}


void construcaoGulosa(float m[][nElementos], int* solucao_inicial) {
    int *inseridos = malloc(nElementos * sizeof(int));

    for(int i = 0; i < nElementos; i++) {
        inseridos[i] = 0;
    }

    solucao_inicial[0] = 0;
    inseridos[0] = 1;

    for(int i = 0; i < nElementos; i++) {
        int valor_referencia = 222222222;
        int vizinho_selecionado = 0;

        for(int j = 0; j < nElementos; j++) {
            if(!inseridos[j] && valor_referencia > m[i][j]) {
                vizinho_selecionado = j;
                valor_referencia = m[i][j];
            }
        }

        solucao_inicial[i + 1] = vizinho_selecionado;
        inseridos[vizinho_selecionado] = 1;
    }

    solucao_inicial[nElementos] = 0;

    free(inseridos);
}

void swap(float m[][nElementos], int* solucao_inicial, int* melhor_vizinho) {

    int* solucao_tmp = malloc((nElementos + 1) * sizeof(int));
    float custo_referencia = calculaCusto(m, solucao_inicial);

    copiaCaminho(solucao_inicial, melhor_vizinho);

    for(int i = 1; i < nElementos; i++) {
        copiaCaminho(solucao_inicial, solucao_tmp);

        for(int j = i + 1; j < nElementos; j++) {
            int tmp = solucao_tmp[i];
            solucao_tmp[i] = solucao_tmp[j];
            solucao_tmp[j] = tmp;

            int custo_solucao_tmp = calculaCusto(m, solucao_tmp);

            if(custo_solucao_tmp < custo_referencia){
                custo_referencia = custo_solucao_tmp;
                copiaCaminho(solucao_tmp, melhor_vizinho);
            }
        }
    }
}

/*void twoOpt(float m[][nElementos], int* caminho, float melhor, int* melhor_vizinho){
    int improve = 0;
 
    while(improve < 20){

        for(int i = 0; i < nElementos; i++ ){
            for(int k = 0; k < nElementos; k++){
                TwoOptSwap(caminho, i, k);
 
                float novoCusto = calculaCusto(m, caminho);

                if(novoCusto < melhor){
                    improve = 0;
                    melhor_vizinho = vizinho;
                    melhor = novoCusto;
                }
            }
        }
    
    improve ++;
    }
}*/

/*
current_tour:=create_random_initial_tour()
repeat
modified_tour:=apply_2opt_move(current_tour)
if length(modified_tour) <length(current_tour)
then current_tour:=modified_tour
until no further improvement or a specified number of iterations

2optSwap(route, i, k) {
      1. take route[0] to route[i-1] and add them in order to new_route
      2. take route[i] to route[k] and add them in reverse order to new_route
      3. take route[k+1] to end and add them in order to new_route
      return new_route;
  }


*/
void VND(float m[][nElementos], float custo_melhor_vizinho, int* melhor_vizinho, int* otimo_local){
    int improve = 0;
    float novoCusto;

    while(improve < 20){
        swap(m, melhor_vizinho, otimo_local);

        novoCusto = calculaCusto(m, melhor_vizinho);

        if(novoCusto < custo_melhor_vizinho){
            otimo_local = melhor_vizinho;
            novoCusto = custo_melhor_vizinho;
            improve = 1;
        }

    improve ++;
    }
}

void printCaminho(int* caminho){
    for(int i = 0; i <= nElementos; i++) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
}

float distancia(struct cidade a, struct cidade b){
    return sqrt(((b.px-a.px)*(b.px-a.px))+((b.py-a.py)*(b.py-a.py)));
}

void copiaCaminho(int* origem, int* destino){
    for(int i = 0; i <= nElementos; i++) {
        destino[i] = origem[i];
    }
}

float calculaCusto(float m[][nElementos], int* caminho){
    float custo = 0;

    for(int i = 0; i < nElementos; i++) {
        custo += m[caminho[i]][caminho[i + 1]];
    }
    return custo;
}