#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>

#define MEMORY_SIZE 4096
#define LOGAR (long long int)


uint8_t memory[MEMORY_SIZE];

typedef struct chunck{
	unsigned int size;
	struct chunck * next;
	struct chunck * before;
}bloco;

void *aloca(unsigned int);

int main(void){
	
	bloco *voz = (bloco *) &memory[0];
	voz->size = -1;
		
	setlocale(LC_ALL, "");
	/*
	bloco *voz, *vo2;
	voz = (bloco*) &memory[0];
	vo2 = (bloco*) &memory[sizeof(bloco) - 1 + sizeof(int)];
	voz->size = sizeof(int);
	vo2->size = 8;
	voz->before = NULL;
	vo2->before = voz;
	voz->next = vo2;
	vo2->next = NULL;
	*/
	
	int * p = (int *) aloca(sizeof(int) + 4000	);
	*p = 79;
	printf("%d\n\n", *p);
	
	int *w = (int *) aloca(sizeof(int));
	*w = 2;
	printf("%d\n\n", *w);
	
}

void *aloca(unsigned int rage){
	int x = 0;
	bloco *voz = (bloco *) &memory[x];	
	/*if (voz->next != NULL){
		LOGAR voz->next  - ((LOGAR voz) + 23) == voz->size ? printf("Chupa") : printf("Porran"); 
	}*/
	
	while(1){
		//Primeiro verifica se existe o espe�o solicitado, caso exista segue o programa, se n�o retorna um erro e exit (1).
		if(LOGAR &memory[MEMORY_SIZE] - LOGAR voz > rage + 24){
				
			//Verifica��o se � o primeiro ALOCA solicitado. Retornando assim a posi��o com o espa�o desejado.
			if (voz->next == NULL && voz->before == NULL && voz->size == -1) {
				voz->size = rage;
				printf("Passou Primeiro ponteiro: %d\n", voz->size);
				return (void *) &memory[x + 24];
				
			} else {
				//Verifica se n�o existe controle para frente, se n�o tiver cria-se um controle � frente, e depois retorna o primeiro endere�o void do espa�o.
				if(voz->next == NULL){
					//Cria��o do controle
					bloco *newblock = (bloco *) &memory[x + 24 + voz->size];
					newblock->size = rage;
					newblock->before = voz;
					newblock->next = NULL;
					voz->next = newblock;
					
					//x recebe o valor da posi��o da mem�ria em que o primeiro endere�o deve estar
					x += 24 + voz->size + 24;
					return (void *) &memory[x];
					
				} else{
					if (LOGAR voz->next - LOGAR &memory[x + 24] == voz->size){
						//Aqui � o caso perfeito, onde a mem�ria foi solicitada uma seguida da outra, ent�o o espa�o entre elas ser� igual ao espa�o solicitaado
						x += 24 + voz->size;
						voz = voz->next;					
					}else {
						if(LOGAR voz->next - LOGAR &memory[x + 24] < rage + 24 + voz->size){
							//� verificado se existe tamanho suficiente para a mem�ria solicitada
							//Se n�o tiver,� passado a "bola" para o pr�ximo bloco de controle e x passa a ter o valor de memory do proximo ponteiro
							x += LOGAR voz->next - LOGAR &memory[x + 24] ; 
							voz = voz->next;
						}else {
							//caso tenha espa�o suficiente entre duas mem�rias, x toma como n�mero o endere�o de memory
							//ap�s o numero de bytes solicitados pelo bloco controle atual
							x += 24 + voz->size;
							
							bloco *newblock = (bloco *) &memory[x];
							newblock->size = rage;
							newblock->next = voz->next;
							newblock->before = voz;
							
							voz->next->before = newblock;
							voz->next = newblock;
							
							x += 24;
							return (void *) &memory[x];						
						}
					}
				}
			}
		}else {
			printf("\n%d\n", LOGAR &memory[MEMORY_SIZE] - LOGAR voz);
			fprintf(stderr, "%s", "N�o h� espa�o!");
			exit(1);
		}
	}	
}
