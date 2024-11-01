
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 50
#define MAX_POKE 100

typedef struct Poke{
	char nome[MAX];
	float ataque;
	float defesa;
	float vida;
	char tipo[MAX];
}Poke;

void tipagem(Poke *p){
	int i=0;
	
	while (p->tipo[i] != '\0'){
		if (p->tipo[i]>='A' && p->tipo[i]<='Z') p->tipo[i] = p->tipo[i] - 'A' + 'a';
		i++;
	}
}

void gravacao(int q, FILE *arq, Poke *lista){
	
	Poke p;
	char buf[MAX];
	int i = 0;
	
    while (i<q){
		fgets(buf, MAX, arq);
    
		char *nome = strtok(buf," ");
		p.ataque = atof(strtok(NULL," "));
		p.defesa = atof(strtok(NULL," "));
		p.vida = atof(strtok(NULL," "));
		char *tipo = strtok(NULL," ");
		
		if (tipo[strlen(tipo) - 1] == '\n') tipo[strlen(tipo) - 1] = '\0';
		
		if(p.ataque<0 || p.defesa<0 || p.vida<0){
			printf("\nValor(es) negativos entrados. O sinal negativo sera desconsiderado");
			if (p.ataque<0) p.ataque *= (-1);
			if (p.defesa<0) p.defesa *= (-1);
			if (p.vida<0) p.vida *= (-1);
		}
		
		strcpy(p.nome, nome);
		strcpy(p.tipo, tipo);
		tipagem(&p);
		
		lista[i] = p;
		i++;
	}
}

float ataque(Poke *atacante, Poke *defensor){
	
	float ataqueTurno = atacante -> ataque;
	
	if (strcmp(atacante->tipo,"eletrico") == 0){
		if (strcmp(defensor->tipo,"agua") == 0) ataqueTurno += ataqueTurno*0.2;
		else if (strcmp(defensor->tipo,"pedra") == 0) ataqueTurno -= ataqueTurno*0.2;
	}
	else if (strcmp(atacante->tipo,"agua") == 0){
		if (strcmp(defensor->tipo,"fogo") == 0) ataqueTurno += ataqueTurno*0.2;
		else if (strcmp(defensor->tipo,"eletrico")== 0) ataqueTurno -= ataqueTurno*0.2;
	}
	else if (strcmp(atacante->tipo,"fogo") == 0){
		if (strcmp(defensor->tipo,"gelo") == 0) ataqueTurno += ataqueTurno*0.2;
		else if (strcmp(defensor->tipo,"agua") == 0) ataqueTurno -= ataqueTurno*0.2;
	}
	else if (strcmp(atacante->tipo,"gelo") == 0){
		if (strcmp(defensor->tipo,"pedra") == 0) ataqueTurno += ataqueTurno*0.2;
		else if (strcmp(defensor->tipo,"fogo") == 0) ataqueTurno -= ataqueTurno*0.2;
	}
	else if (strcmp(atacante->tipo,"pedra") == 0){
		if (strcmp(defensor->tipo,"eletrico") == 0) ataqueTurno += ataqueTurno*0.2;
		else if (strcmp(defensor->tipo,"gelo") == 0)ataqueTurno -= ataqueTurno*0.2;
	}
		
	return ataqueTurno;
}

void vida(float ataqueTurno, Poke *defensor){
	
	if (ataqueTurno > defensor->defesa){
		defensor -> vida -=  (ataqueTurno - defensor -> defesa);
	}
	else{ 
		defensor -> vida -= 1;
	}
}

void imprime(Poke p){
	printf("%s ", p.nome); 
	printf("%.0f ", p.ataque);
	printf("%.0f ", p.defesa);
	printf("%.0f ", p.vida);
	printf("%s\n", p.tipo);
}

int main(){
	
	printf("\n::- SIMULADOR DE POKEMON -::\n\n");
    
    // Definir 2 valores, que serão a quantidade de pokemon de cada jogador
    int quant1, quant2;
    int i;
    
    // Ler os daddos do arquivo txt
    /*
    char nome[MAX];
    printf("Adicione o nome do arquivo: ");
    scanf("%s", nome);
    
    FILE *arq;
    arq = fopen(nome, "r");
    if (arq == NULL){
		printf("\nErro ao abrir o arquivo");
		return 1;
	}
    char buf[MAX];
    fgets(buf, MAX, arq);
    */
    
    
	char buf[MAX];
    fgets(buf, MAX, stdin);
    
    
	// Lendo quantidade de pokemons de cada jogador
    quant1 = atoi(strtok(buf, " "));
    quant2 = atoi(strtok(NULL, "\n"));
    
    if ((quant1==0)||(quant2==0)){
		printf("\nUm dos jogadores tem 0 pokemon");
		return 1;
	}
	if ((quant1>100)||(quant2>100)){
		printf("\nUm dos jogadores tem mais de 100 pokemon");
		return 1;
	}
	
    // Lendo os dados dos pokemon
    Poke lista_p1[MAX_POKE]; //lista com pokemon do player 1
    Poke lista_p2[MAX_POKE]; //lista com pokemon do player 2
    
    
    gravacao(quant1, stdin, lista_p1);
    gravacao(quant2, stdin, lista_p2);
    
    /*
    gravacao(quant1, arq, lista_p1);
    gravacao(quant2, arq, lista_p2);
    */
    
    // quantidade total de pokémon
	int total = quant1 + quant2;
    
    // Lista com todos os pokemon
	int q = 0;
	Poke *todos;
	todos = (Poke *) malloc(sizeof(Poke) * total);
	
	for (i=0; i<quant1; i++){
		todos[q] = lista_p1[i];
		q++;
	}
	for (i=0; i<quant2; i++){
		todos[q] = lista_p2[i];
		q++;
	}
	
	
	// Imprimindo os dados do arquivo
	// printf("\n-DADOS DO ARQUIVO-\n");
	fprintf(stdout, "\n-DADOS DO ARQUIVO-\n");
	// printf("%d %d\n", quant1, quant2);
	fprintf(stdout, "%d %d\n", quant1, quant2);
	for (i=0; i<total; i++){
		imprime(todos[i]);
	}
	
	// Criando lista para armazenar pokemon derrotados
	int *derrotados;
	derrotados = (int *) malloc(sizeof(int) * total);
	// a princípio, nenhum pokémon foi derrotado
	for (i=0; i<total; i++) derrotados[i] = 0;

	// Fechando o arquivo, pois ja recolhemos todas as informações 
	// fclose(arq);
	
	// Simulando as batalhas
	Poke atacante, defensor, novoAtacante;
	int j = 0, k = 0;
	
	
	// Adicionei variáveis "booleanas" de controle para evitar erros com nomes iguais
	
	atacante = lista_p1[j];
	// sempre que o atacante estiver com o j1, a = 0
	int a = 0;
	
	defensor = lista_p2[k];
	// sempre que o defensor estiver com o j2, d = 1
	int d = 1; 
	
	float ataqueTurno;
	
	int t1 = quant1;
	// printf("\n-RESULTADO DAS BATALHAS-\n");
	fprintf(stdout, "\n-RESULTADO DAS BATALHAS-\n");
	while ((quant1 > 0) && (quant2 > 0)){
		
		ataqueTurno = ataque(&atacante, &defensor);
		vida(ataqueTurno, &defensor);
		
		if (defensor.vida <= 0){

			// printf("%s venceu %s\n", atacante.nome, defensor.nome);
			fprintf(stdout,"%s venceu %s\n", atacante.nome, defensor.nome);
			if (!d){
				derrotados[j] = 1;
				quant1--;
				j++;
				novoAtacante = lista_p1[j];
			}
			else if (d) {
				derrotados[t1+k] = 1;
				quant2--;
				k++;
				novoAtacante = lista_p2[k];
			}
		}
		else{
			// printf("Sem vencedor\n");
			novoAtacante = defensor;
		}
		
		// sempre troco quem é o jogador defensor e atacante, então troco o controle para 1 ou 0
		d = !d;
		a = !a;
		defensor = atacante;
		atacante = novoAtacante;
		
	}
	
	// Imprimindo o vencedor
	// printf("\n");
	fprintf(stdout,"\n");
	if (quant2 <= 0){
		// printf("Jogador 1 venceu\n");
		fprintf(stdout, "Jogador 1 venceu\n");
	}
	else if (quant1 <= 0){ 
		// printf("Jogador 2 venceu\n");
		fprintf(stdout, "Jogador 2 venceu\n");
	}
	else printf("ERRO: Sem vencedores\n");
	
	// Lista de sobreviventes
	// printf("\n-POKEMON SOBREVIVENTES-\n");
	fprintf(stdout, "\n-POKEMON SOBREVIVENTES-\n");

	// Imprimo os nomes daqueles que estão na lista com todos os nomes mas não estão na lista de derrotados
	
	for(i=0; i<total; i++){
		if (!derrotados[i]){ 
			// printf("%s\n", todos[i].nome);
			fprintf(stdout, "%s\n", todos[i].nome);
		}
	}
	
	// imprimir lista de derrotados
	// printf("\n-POKEMON DERROTADOS-\n");
	fprintf(stdout, "\n-POKEMON DERROTADOS-\n");
	for (i=0; i<total; i++){
		// imprimo primeiro todos do jogador 1
		if (derrotados[i]){ 
			// printf("%s\n", todos[i].nome);
			fprintf(stdout, "%s\n", todos[i].nome);
		}
	}
	
	free(derrotados);
	free(todos);

	
	return 0;
}

