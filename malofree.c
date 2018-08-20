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
		//Primeiro verifica se existe o espeço solicitado, caso exista segue o programa, se não retorna um erro e exit (1).
		if(LOGAR &memory[MEMORY_SIZE] - LOGAR voz > rage + 24){
				
			//Verificação se é o primeiro ALOCA solicitado. Retornando assim a posição com o espaço desejado.
			if (voz->next == NULL && voz->before == NULL && voz->size == -1) {
				voz->size = rage;
				printf("Passou Primeiro ponteiro: %d\n", voz->size);
				return (void *) &memory[x + 24];
				
			} else {
				//Verifica se não existe controle para frente, se não tiver cria-se um controle à frente, e depois retorna o primeiro endereço void do espaço.
				if(voz->next == NULL){
					//Criação do controle
					bloco *newblock = (bloco *) &memory[x + 24 + voz->size];
					newblock->size = rage;
					newblock->before = voz;
					newblock->next = NULL;
					voz->next = newblock;
					
					//x recebe o valor da posição da memória em que o primeiro endereço deve estar
					x += 24 + voz->size + 24;
					return (void *) &memory[x];
					
				} else{
					if (LOGAR voz->next - LOGAR &memory[x + 24] == voz->size){
						//Aqui é o caso perfeito, onde a memória foi solicitada uma seguida da outra, então o espaço entre elas será igual ao espaço solicitaado
						x += 24 + voz->size;
						voz = voz->next;					
					}else {
						if(LOGAR voz->next - LOGAR &memory[x + 24] < rage + 24 + voz->size){
							//É verificado se existe tamanho suficiente para a memória solicitada
							//Se não tiver,é passado a "bola" para o próximo bloco de controle e x passa a ter o valor de memory do proximo ponteiro
							x += LOGAR voz->next - LOGAR &memory[x + 24] ; 
							voz = voz->next;
						}else {
							//caso tenha espaço suficiente entre duas memórias, x toma como número o endereço de memory
							//após o numero de bytes solicitados pelo bloco controle atual
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
			fprintf(stderr, "%s", "Não há espaço!");
			exit(1);
		}
	}	
}
