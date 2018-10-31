#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INT_MAX 200000;

int nElementos = 0;

struct cidade {
    float px;
    float py;
    float custo;
};

//Declaração das funções
float distancia(struct cidade a, struct cidade b);
float calculaCusto(float m[][nElementos], int* caminho);
void printCaminho(int *caminho);
void construcaoGulosa(float m[][nElementos], int* solucao_inicial);
void copiaCaminho(int* origem, int* destino);
void swap(float m[][nElementos], int* solucao_inicial, int* melhor_vizinho);
void twoOpt(float m[][nElementos], int* solucao_inicial, float melhor_custo, int* melhor_vizinho);
void twoOptSwap(int* entrada, int i, int k, int* saida);
void VND(float m[][nElementos], float custo_melhor_vizinho, int* melhor_vizinho, int* otimo_local);
void GRASP(float m[][nElementos], int* solucao_inicial, int* solucao_final, int alfa);
void gerarLCR(int* entrada, int a, int* saida);

int main(void) {
    int t;
    int alfa = 2;

    //FILE* fp = fopen("dijbouti.txt", "r");
    //FILE* fp = fopen("bcl380.txt", "r");
    FILE* fp = fopen("western sahara.txt", "r");
    //FILE* fp = fopen("xqg237.txt", "r");
    //FILE* fp = fopen("zimbabwe.txt", "r");
    //FILE* fp = fopen("pka379.txt", "r");
    //FILE* fp = fopen("qatar.txt", "r");
    //FILE* fp = fopen("uruguay.txt", "r");
    //FILE* fp = fopen("xqg131.txt", "r");
    //FILE* fp = fopen("pma343.txt", "r");

    //Lê numero de elementos
    fscanf(fp, "%d\n", &nElementos);  
    printf("Entrada:\n%d\n", nElementos);

    //Lê cidades e armazena em array do tipo struct cidade
    struct cidade cidades[nElementos];

    for(int i = 0; i < nElementos; i++) {
        fscanf(fp, "%f ", &t);
        fscanf(fp, "%f ", &cidades[i].px);
        fscanf(fp, "%f \n", &cidades[i].py);
        //printf("%d %.2f %.2f\n", i+1, cidades[i].px, cidades[i].py); 
    }

    fclose(fp);
    
    printf("\n");

    //Calcula distância entre cidades, armazena em matriz[][] e imprime matriz
    float matriz[nElementos][nElementos];

    //printf("Matriz gerada:\n");

    for(int i = 0; i < nElementos; i++) {
        for(int j = 0; j < nElementos; j++) {
            matriz[i][j] = distancia(cidades[i], cidades[j]);
            //printf("%.2f ", matriz[i][j]);
        }
  }

    //Cria solucao gulosa inicial 
    int solucao_inicial[nElementos + 1];
    int solucao_final[nElementos + 1];
    
    GRASP(matriz, solucao_inicial, solucao_final, alfa);
    printf("\n\nSolucao final GRASP: ");
    printCaminho(solucao_final);
    printf("\n\nCusto solucao final: %.2f",calculaCusto(matriz, solucao_final));


    return 0;
}

void GRASP(float m[][nElementos], int* solucao_inicial, int* solucao_final, int alfa){
    int improve = 0;
    int solucao_temp[nElementos];
    int otimo_local[nElementos];
    float custoVND, custoFinal, custoGuloso, novoCusto = INT_MAX;

    while(improve < 5){
        copiaCaminho(solucao_temp, solucao_inicial);
        
        construcaoGulosa(m, solucao_temp);
        //pegar x candidatos da lista gulosa e o resto aleatorio;
        gerarLCR(solucao_temp, alfa, solucao_temp);
        
        custoGuloso = calculaCusto(m, solucao_temp);
        printf("LCR: ");
        printCaminho(solucao_temp);
        
        VND(m, custoGuloso, solucao_temp, otimo_local);
        custoFinal = calculaCusto(m, otimo_local);

        if(novoCusto > custoFinal){
            copiaCaminho(otimo_local, solucao_final);
            copiaCaminho(solucao_final, solucao_temp);
            novoCusto = custoFinal;
            improve = 1;
        }
      
      improve ++;       
    }
    
    printf("\n\nSolução inicial gulosa: ");
    printCaminho(solucao_temp);
    custoGuloso = calculaCusto(m, solucao_temp);
    printf("Custo solução inicial gulosa: %.2f\n", custoGuloso);

    printf("\n\nSolução após VDN: ");
    printCaminho(solucao_temp);
    custoGuloso = calculaCusto(m, solucao_temp);
    printf("Custo solução inicial gulosa: %.2f\n", custoGuloso);
}

void gerarLCR(int* entrada, int a, int* saida){
    int k =  nElementos / a;
    int inseridos[nElementos];
    int count = 0;

    for(int i = 0; i < nElementos; i++){
      inseridos[i] = 0;
    }
    
    //adiciona os k primeiros elementos a solucao na ordem
    for(int i = 0; i < k; ++i){
        saida[i] = entrada[i];
        inseridos[i] = 1;
    }

    //randomiza os elementos de k ao fim;
    for(int i = k ; i < nElementos; i++){
        int n = rand() % nElementos; 
        
        if(n >= k && !inseridos[n]){
            saida[i] = entrada[n];
            inseridos[n] = 1;
            count++;
        }
    }
}
 
void construcaoGulosa(float m[][nElementos], int* solucao_inicial) {
    int *inseridos = malloc(nElementos * sizeof(int));

    for(int i = 0; i < nElementos; i++) {
        inseridos[i] = 0;
    }

    solucao_inicial[0] = 0;
    inseridos[0] = 1;

    for(int i = 0; i < nElementos; i++) {
        int valor_referencia = INT_MAX;
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

void twoOpt(float m[][nElementos], int* solucao_inicial, float melhor_custo, int* melhor_vizinho){
    int improve = 0;
    int* solucao_tmp = malloc((nElementos + 1) * sizeof(int));
    
    copiaCaminho(solucao_inicial, melhor_vizinho);
 
    while(improve < 5){

        for(int i = 1; i < nElementos; i++ ){
          copiaCaminho(solucao_inicial, solucao_tmp);
            for(int k = 0; k < nElementos; k++){
                if(i - k == 1){ continue;}
                twoOptSwap(solucao_inicial, i, k, solucao_tmp);

                float novoCusto = calculaCusto(m, solucao_tmp);

                if(novoCusto < melhor_custo){
                    improve = 0;
                    copiaCaminho(solucao_tmp, melhor_vizinho);
                    melhor_custo = novoCusto;
                }
            }
        }
    
    improve ++;
  }
}

void twoOptSwap(int* entrada, int i, int k, int* saida){
    // 1. take route[0] to route[i-1] and add them in order to new_route
    for(int c = 0; c <= i - 1; ++c ){
        saida[c] = entrada[c];
    }
         
    // 2. take route[i] to route[k] and add them in reverse order to new_route
    int dec = 0;
    for ( int c = i; c <= k; ++c){
        saida[c] = entrada[k - dec];
        dec++;
    }
 
    // 3. take route[k+1] to end and add them in order to new_route
    for ( int c = k + 1; c < nElementos; ++c ){
        saida[c] = entrada[c];
    }
}

void VND(float m[][nElementos], float custo_melhor_vizinho, int* melhor_vizinho, int* otimo_local){
    int improve = 0;
    int custoVND;

    while(improve < 5){
        swap(m, melhor_vizinho, otimo_local);
        twoOpt(m, otimo_local, custo_melhor_vizinho, otimo_local);
        swap(m, otimo_local, otimo_local);

        float novoCusto = calculaCusto(m, otimo_local);

        if(novoCusto > custo_melhor_vizinho){
            copiaCaminho(melhor_vizinho, otimo_local);
            novoCusto = custo_melhor_vizinho;
            printf("%.2f\n", novoCusto);
            improve = 1;
        } else { 
          improve ++;
        }
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

float calculaCusto(float m[][nElementos], int* caminho) {
    float custo = 0;

    for(int i = 0; i < nElementos; i++) {
        custo += m[caminho[i+ 1]][caminho[i]];
    }

    return custo;
}