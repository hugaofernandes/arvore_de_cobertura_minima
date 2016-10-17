
//autor: hugo rafael de m. fernandes
//resolver apenas o problema da árvore de cobertura mínima
//disciplina: Estrutura de dados

//para compilar: gcc fonte.c -lm -o fonte


/*
para gerar o grafico:
set grid
set size ratio 1
unset border
set style arrow 1 nohead lw 1

plot "tree.data" using 1:2:($3-$1):($4-$2) with vectors arrowstyle 1 notitle, \
     "tree.data" using 1:2 lt -1 pt 4 notitle, \
     "tree.data" using 3:4 lt -1 pt 4 notitle

pause -1
*/




#include <stdio.h>
#include <stdlib.h>
#include <math.h>



struct vertice{

	int grupo;
	int coordenadaX;
	int coordenadaY;

};


struct aresta{

	float peso;
	struct vertice* verticeA;
	struct vertice* verticeB;
};


float distanciaEntreDoisPontos(struct vertice* a, struct vertice* b){

	float distancia = 0.0;
	distancia = pow((pow(a->coordenadaX - b->coordenadaX, 2.0)) + (pow(a->coordenadaY - b->coordenadaY, 2.0)), 0.5);

	return distancia;
}



struct vertice* novoVertice(int grupo, int coordenadaX, int coordenadaY){

	struct vertice* novo;
	novo = (struct vertice*)malloc(sizeof(struct vertice));
	novo->grupo = grupo;
	novo->coordenadaX = coordenadaX;
	novo->coordenadaY = coordenadaY;
	return novo;
}


struct aresta* novoAresta(float peso, struct vertice* verticeA, struct vertice* verticeB){

	struct aresta* novo;
	novo = (struct aresta*)malloc(sizeof(struct aresta));
	novo->peso = peso;
	novo->verticeA = verticeA;
	novo->verticeB = verticeB;
	return novo;
}


int quantidadeDeVertices(char arquivo[]){

	FILE* arq = fopen(arquivo, "r");
	int coordenadaX, coordenadaY, contador = 0;
	while (fscanf(arq, "%d %d\n", &coordenadaX, &coordenadaY) != EOF){
		contador++;
	}
	fclose(arq);
	return contador;

}


int quantidadeDeArestas(int quantidadeDeVertices){

	int i, contador = 0;
	for (i = 1; i < quantidadeDeVertices; i++){
		contador += i;
	}
	return contador;
}


void inserirVertices(char arquivo[], struct vertice* vertices[]){
	//lendo arquivo
	FILE* arq = fopen(arquivo, "r");
	int coordenadaX, coordenadaY;
	int contador = 0;
	while (fscanf(arq, "%d %d\n", &coordenadaX, &coordenadaY) != EOF){
		//colocando vertices no vetor de vertices
		vertices[contador] = novoVertice(contador, coordenadaX, coordenadaY);
		contador++;
	}
	fclose(arq);

}


void inserirArestas(struct vertice* vertices[], struct aresta* arestas[], int numeroDeVertices){

	int contador, contador2, contador3 = 0;
	struct vertice* verticeA;
	struct vertice* verticeB;
	
	for (contador = 0; contador < (numeroDeVertices - 1); contador++){
		for (contador2 = contador + 1; contador2 < numeroDeVertices; contador2++){
		
			verticeA = vertices[contador];
			verticeB = vertices[contador2];
			arestas[contador3] = novoAresta(distanciaEntreDoisPontos(verticeA, verticeB), verticeA, verticeB);
			contador3++;

		}
	}
}



//função insertion Sort
void insertionSort(struct aresta* arestas[], int tamanho){

	int contador, j;
	struct aresta* eleito;

	for(contador = 1; contador < tamanho; contador++){

		eleito = arestas[contador];
		j = contador - 1;
		
		while((j >= 0) && (eleito->peso < arestas[j]->peso)){

			arestas[j + 1] = arestas[j];
			j = j - 1;
		}

		arestas[j + 1] = eleito;
	}
}


void imprimirArestas(struct aresta* arestas[], int numeroDeArestas){

	int i;
	for (i = 0; i < numeroDeArestas; i++){
		printf("\n%d %d - %d %d - Peso: %f", arestas[i]->verticeA->coordenadaX, arestas[i]->verticeA->coordenadaY, arestas[i]->verticeB->coordenadaX, arestas[i]->verticeB->coordenadaY, arestas[i]->peso);

	}
	printf("\nTotal de Arestas: %d\n", i);

}



void alterarGrupo(struct vertice* vertices[], int grupo, int novoGrupo, int tamanho){

	int i;
	for (i = 0; i < tamanho; i++){
		if (vertices[i]->grupo == grupo){
			vertices[i]->grupo = novoGrupo;
		}
	}
}



//algoritmo de kruskal
void kruskal(struct aresta* arestas[], struct vertice* vertices[], int numeroDeArestas, int numeroDeVertices){

	FILE* arq = fopen("arvoreDeCoberturaMinima.txt", "w");
	int contador; 
	float totalDoCusto = 0;
	for (contador = 0; contador < numeroDeArestas; contador++){
		
		if (arestas[contador]->verticeA->grupo != arestas[contador]->verticeB->grupo){

			fprintf(arq, "%d %d	%d %d\n", arestas[contador]->verticeA->coordenadaX, arestas[contador]->verticeA->coordenadaY, arestas[contador]->verticeB->coordenadaX, arestas[contador]->verticeB->coordenadaY);
			
			totalDoCusto += arestas[contador]->peso;

			alterarGrupo(vertices, arestas[contador]->verticeB->grupo, arestas[contador]->verticeA->grupo, numeroDeVertices);
			
		}
	}	

	fclose(arq);
	printf("\nTotal do Custo das Arestas: %f\n", totalDoCusto);
}




void main(){

	char arquivo[] = "cidades.txt";
	int numeroDeVertices = quantidadeDeVertices(arquivo);
	int numeroDeArestas = quantidadeDeArestas(quantidadeDeVertices(arquivo));

	struct vertice* vertices[numeroDeVertices];
	struct aresta* arestas[numeroDeArestas];

	//inserindo
	inserirVertices(arquivo, vertices);
	inserirArestas(vertices, arestas, numeroDeVertices);


	//imprimindo antes da ordenção
	//imprimirArestas(arestas, numeroDeArestas);

	//ordenando
	insertionSort(arestas, numeroDeArestas);

	//imprimindo depois da ordenção
	//imprimirArestas(arestas, numeroDeArestas);

	kruskal(arestas, vertices, numeroDeArestas, numeroDeVertices);

	
}



