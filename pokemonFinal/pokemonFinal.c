#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 50
#define MAX_POKE 100
#define ALTERACAO_PODER 0.2

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
		if (strcmp(defensor->tipo,"agua") == 0) ataqueTurno += ataqueTurno*ALTERACAO_PODER;
		else if (strcmp(defensor->tipo,"pedra") == 0) ataqueTurno -= ataqueTurno*ALTERACAO_PODER;
	}
	else if (strcmp(atacante->tipo,"agua") == 0){
		if (strcmp(defensor->tipo,"fogo") == 0) ataqueTurno += ataqueTurno*ALTERACAO_PODER;
		else if (strcmp(defensor->tipo,"eletrico")== 0) ataqueTurno -= ataqueTurno*ALTERACAO_PODER;
	}
	else if (strcmp(atacante->tipo,"fogo") == 0){
		if (strcmp(defensor->tipo,"gelo") == 0) ataqueTurno += ataqueTurno*ALTERACAO_PODER;
		else if (strcmp(defensor->tipo,"agua") == 0) ataqueTurno -= ataqueTurno*ALTERACAO_PODER;
	}
	else if (strcmp(atacante->tipo,"gelo") == 0){
		if (strcmp(defensor->tipo,"pedra") == 0) ataqueTurno += ataqueTurno*ALTERACAO_PODER;
		else if (strcmp(defensor->tipo,"fogo") == 0) ataqueTurno -= ataqueTurno*ALTERACAO_PODER;
	}
	else if (strcmp(atacante->tipo,"pedra") == 0){
		if (strcmp(defensor->tipo,"eletrico") == 0) ataqueTurno += ataqueTurno*ALTERACAO_PODER;
		else if (strcmp(defensor->tipo,"gelo") == 0)ataqueTurno -= ataqueTurno*ALTERACAO_PODER;
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
	
	// Alguns comentários adicionados servem para que o programa receba do usuário o nome do arquivo
	// a partir de solicitação ou pelo stdin. 
	
	// O arquivo "pokemon.txt", na pasta compactada, é o caso de teste 3
	
	printf("\n::- SIMULADOR DE POKEMON -::\n\n");
    
    // Definir 2 valores, que serão a quantidade de pokemon de cada jogador
    int quant1, quant2;
    int i;
    
    // Ler os daddos do arquivo txt
    // char nome[MAX];
    // printf("Adicione o nome do arquivo: ");
    // scanf("%s", nome);
    
    char nome[] = "pokemon.txt";
    
    FILE *arq;
    arq = fopen(nome, "r");
    if (arq == NULL){
		printf("\nErro ao abrir o arquivo");
		fclose(arq);
		return 1;
	}
    char buf[MAX];
	
	
	// Mova o ponteiro do arquivo para o final menos um byte (ver se tenho \n no final)
    if (fseek(arq, -1, SEEK_END) != 0) {
        perror("\nErro ao posicionar o ponteiro do arquivo");
        fclose(arq);
        return 1;
    }
    
    // Ler o último caractere do arquivo
    char ultimoCaractere = fgetc(arq);
    
    // Volto com o marcador para o início do arquivo
    rewind(arq);
    
    // Leio quantas linhas o arquivo tem
	int linhas = 0;
    char ch;
    while ((ch = fgetc(arq)) != EOF) {
        if (ch == '\n') {
            linhas++;
        }
    }
    
    // Volto com o marcador para o início do arquivo
    rewind(arq);
    
    fgets(buf, MAX, arq);
    
    /*
	char buf[MAX];
    fgets(buf, MAX, stdin);
    */
    
	// Lendo quantidade de pokemons de cada jogador
    quant1 = atoi(strtok(buf, " "));
    quant2 = atoi(strtok(NULL, "\n"));
    
    if ((quant1==0)||(quant2==0)){
		printf("\nUm dos jogadores tem 0 pokemon");
		fclose(arq);
		return 1;
	}
	if ((quant1>100)||(quant2>100)){
		printf("\nUm dos jogadores tem mais de 100 pokemon");
		fclose(arq);
		return 1;
	}
    
    // Quantidade total de pokémon
	int total = quant1 + quant2;
    
    // Se a quantidade de linhas não for igual ao total de pokemon, temos um erro
    // Se tiver \n no fim, devemos considerar linha - 1
    if (ultimoCaractere == '\n'){
		if (linhas - 1 != total){
			printf("\nA quantidade de pokemon informada nao corresponde a quantidade de linhas do arquivo");
			fclose(arq);
			return 1;
		}
	}
	// Se não, o valor de linha já ignora uma linha
	else{
		if (linhas != total){
			printf("\nA quantidade de pokemon informada nao corresponde a quantidade de linhas do arquivo");
			fclose(arq);
			return 1;
		}
	}
    
	
    // Lendo os dados dos pokemon
    Poke lista_p1[MAX_POKE]; //lista com pokemon do player 1
    Poke lista_p2[MAX_POKE]; //lista com pokemon do player 2
    
    /*
    gravacao(quant1, stdin, lista_p1);
    gravacao(quant2, stdin, lista_p2);
    */
    
    gravacao(quant1, arq, lista_p1);
    gravacao(quant2, arq, lista_p2);
    
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
	printf("\n-DADOS DO ARQUIVO-\n");
	// fprintf(stdout, "\n-DADOS DO ARQUIVO-\n")
	printf("%d %d\n", quant1, quant2);
	// fprintf(stdout, "%d %d\n", quant1, quant2);
	for (i=0; i<total; i++){
		imprime(todos[i]);
	}
	
	// Criando lista para armazenar pokemon derrotados
	int *derrotados;
	derrotados = (int *) malloc(sizeof(int) * total);
	// a princípio, nenhum pokémon foi derrotado
	for (i=0; i<total; i++) derrotados[i] = 0;

	// Fechando o arquivo, pois ja recolhemos todas as informações 
	fclose(arq);
	
	// Simulando as batalhas
	Poke atacante, defensor, novoAtacante;
	int j = 0, k = 0;
	
	
	// Adicionei variáveis "booleanas" de controle para evitar erros com nomes iguais
	
	atacante = lista_p1[j];
	// Sempre que o atacante ou defensor estiver com o jogador 1, temos um controle que indica 0
	// É como se eu estivesse perguntando: o atacante/defensor é do jogador 2?
	// Se verdadeiro (jogador 2), 1, e se falso (jogador 1), 0.
	int atacanteDeQuem = 0;
	
	defensor = lista_p2[k];
	int defensorDeQuem = 1; 
	
	float ataqueTurno;
	
	int totalDePokeJogador1 = quant1;
	printf("\n-RESULTADO DAS BATALHAS-\n");
	// fprintf(stdout, "\n-RESULTADO DAS BATALHAS-\n");
	while ((quant1 > 0) && (quant2 > 0)){
		
		ataqueTurno = ataque(&atacante, &defensor);
		vida(ataqueTurno, &defensor);
		
		if (defensor.vida <= 0){

			printf("%s venceu %s\n", atacante.nome, defensor.nome);
			// fprintf(stdout,"%s venceu %s\n", atacante.nome, defensor.nome);
			if (!defensorDeQuem){
				derrotados[j] = 1;
				quant1--;
				j++;
				novoAtacante = lista_p1[j];
			}
			else if (defensorDeQuem) {
				derrotados[totalDePokeJogador1+k] = 1;
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
		defensorDeQuem = !defensorDeQuem;
		atacanteDeQuem = !atacanteDeQuem;
		defensor = atacante;
		atacante = novoAtacante;
		
	}
	
	// Imprimindo o vencedor
	printf("\n");
	// fprintf(stdout,"\n");
	if (quant2 <= 0){
		printf("Jogador 1 venceu\n");
		// fprintf(stdout, "Jogador 1 venceu\n");
	}
	else if (quant1 <= 0){ 
		printf("Jogador 2 venceu\n");
		// fprintf(stdout, "Jogador 2 venceu\n");
	}
	else printf("ERRO: Sem vencedores\n");
	
	// Lista de sobreviventes
	printf("\n-POKEMON SOBREVIVENTES-\n");
	// fprintf(stdout, "\n-POKEMON SOBREVIVENTES-\n");

	// Imprimo os nomes daqueles que estão na lista com todos os nomes mas não estão na lista de derrotados
	
	for(i=0; i<total; i++){
		if (!derrotados[i]){ 
			printf("%s\n", todos[i].nome);
			// fprintf(stdout, "%s\n", todos[i].nome);
		}
	}
	
	// imprimir lista de derrotados
	printf("\n-POKEMON DERROTADOS-\n");
	// fprintf(stdout, "\n-POKEMON DERROTADOS-\n");
	for (i=0; i<total; i++){
		// imprimo primeiro todos do jogador 1
		if (derrotados[i]){ 
			printf("%s\n", todos[i].nome);
			// fprintf(stdout, "%s\n", todos[i].nome);
		}
	}
	
	free(derrotados);
	free(todos);

	
	return 0;
}

