#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>

#define malloc aloca
#define free dealoca
#define MEMORY_SIZE 4096
#define LOGAR (long long int)

uint8_t memory[MEMORY_SIZE];

typedef struct chunck{
	unsigned int size;
	struct chunck* next;
	struct chunck* before;
}bloco;

void* aloca(unsigned int);
void dealoca(void*);
bloco* createlist(int);
bloco* insertlastdata(bloco*, int);
void insertdata(bloco*, int);
void imprimetudo(bloco*);
void expulsar(bloco*, int);

int main(void){
	// ******************* N�o apagar *****************
	setlocale(LC_ALL, "");
	bloco *voz = (bloco *) &memory[0];
	voz->size = -1;
	// ****************** N�o apagar ******************
	
	bloco *k = createlist(8);
	insertdata(k, 90);
	insertdata(k, 13);
	insertdata(k, 45);
	insertdata(k, 23);
	insertdata(k, 54);
	imprimetudo(k);
	expulsar(k, 13);
	imprimetudo(k);
}

void* aloca(unsigned int rage){
	int x = 0;
	bloco* voz = (bloco*) &memory[x];	

	while(1){
		//Primeiro verifica se existe o espe�o solicitado, caso exista segue o programa, se n�o retorna um erro e exit (1).
		if(LOGAR &memory[MEMORY_SIZE] - LOGAR voz > rage + (2 * sizeof(bloco))){
				
			//Verifica��o se � o primeiro ALOCA solicitado. Retornando assim a posi��o com o espa�o desejado.
			if (voz->next == NULL && voz->before == NULL && voz->size == -1) {
				voz->size = rage;
				return (void*) &memory[x + sizeof(bloco)];
				
			} else {
				//Verifica se n�o existe controle para frente, se n�o tiver cria-se um controle � frente, e depois retorna o primeiro endere�o void do espa�o.
				if(voz->next == NULL){
					//Cria��o do controle
					bloco *newblock = (bloco *) &memory[x + sizeof(bloco) + voz->size];
					newblock->size = rage;
					newblock->before = voz;
					newblock->next = NULL;
					voz->next = newblock;
					
					//x recebe o valor da posi��o da mem�ria em que o primeiro endere�o deve estar
					x += voz->size + (2 * sizeof(bloco));
					return (void*) &memory[x];
					
				} else{
					if (LOGAR voz->next - LOGAR &memory[x + sizeof(bloco)] == voz->size){
						//Aqui � o caso perfeito, onde a mem�ria foi solicitada uma seguida da outra, ent�o o espa�o entre elas ser� igual ao espa�o solicitaado
						x += 24 + voz->size;
						voz = voz->next;					
						
					}else {
						if(voz->size == -1){
							if(LOGAR voz->next - LOGAR &memory[x] < rage + sizeof(bloco)){
								x += LOGAR voz->next - LOGAR &memory[0] ; 
								voz = voz->next;
							}else{		
								voz->size = rage;				
								x += sizeof(bloco);
								return (void*) &memory[x];	
							}
							
						}else{
							if(LOGAR voz->next - LOGAR &memory[x + sizeof(bloco)] < rage + 24 + voz->size){
								//� verificado se existe tamanho suficiente para a mem�ria solicitada
								//Se n�o tiver,� passado a "bola" para o pr�ximo bloco de controle e x passa a ter o valor de memory do proximo ponteiro
								x += LOGAR voz->next - LOGAR &memory[0] ; 
								voz = voz->next;
								
								
							}else {
								//caso tenha espa�o suficiente entre duas mem�rias, x toma como n�mero o endere�o de memory
								//ap�s o numero de bytes solicitados pelo bloco controle atual
								x += sizeof(bloco) + voz->size;
								
								bloco* newblock = (bloco*) &memory[x];
								newblock->size = rage;
								newblock->next = voz->next;
								newblock->before = voz;
								
								voz->next->before = newblock;
								voz->next = newblock;
								
								x += sizeof(bloco);
								return (void*) &memory[x];						
							}
						}
					}
				}
			}
			
		}else {
			//Cado o espa�o seja insuficiente, retorna um erro.
			fprintf(stderr, "%s", "N�o h� espa�o!");
			exit(1);
		}
	}	
}

void dealoca(void* freedom){
	int x = 0;
	while(freedom != &memory[x]) x++;
	
	bloco* voz;
	voz = (bloco*) &memory[x - sizeof(bloco)];
	
	voz->before != NULL ? (voz->before->next = voz->next) : (voz->before = NULL);
	voz->next != NULL ? (voz->next->before = voz->before): (voz->next = NULL);

	voz->before = NULL;
	voz->size = -1;
}

bloco* createlist(int x){
	
	//Create a new list
	bloco* newblock = (bloco*) malloc(sizeof(bloco));
	newblock->next = NULL;
	newblock->before = NULL;
	newblock->size = x;
	return newblock;
}

bloco* insertlastdata(bloco *last, int x){
	
	bloco* newblock = (bloco *) malloc(sizeof(bloco));
	newblock->next = NULL;
	newblock->before = last;
	newblock->size = x;
	return newblock;
}

void insertdata(bloco* voz, int x){
	while(voz->next != NULL){
		voz = voz->next;
	}
	voz->next = insertlastdata(voz, x);	
}

void imprimetudo(bloco* voz){
	while(voz != NULL){
		printf("Valor do item: %d\n", voz->size);
		voz = voz->next;
	}
}

void expulsar(bloco* voz, int x){
	while(voz->size != x){
		if(voz== NULL)
			printf("N�o existe o valor\n");
		else
			voz = voz->next;
	}
	
	voz->before->next = voz->next;
	voz->next->before = voz->before;
	
	free(voz);
}

