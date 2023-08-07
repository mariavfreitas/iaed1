/*
 * File:  project1.c
 * Author:  ist196757 - iaed-23
 * Description: A program for a public transport management system.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project1.h"

/* The maximum and minimun number of characters the 'aux' in main can have. */
#define MIN_INVERSO 3
#define MAX_INVERSO 7

/* The maximum number of characters a Carreira and Paragem names can have. */
#define MAX_NOME_CARREIRA 20
#define MAX_NOME_PARAGEM 50

/* The maximum number of Carreiras, Paragens and Ligacoes that can exist. */
#define MAX_CARREIRAS 200
#define MAX_PARAGENS 10000
#define MAX_LIGACOES 30000

/* Array of Carreiras. */
Carreira carreiras[MAX_CARREIRAS];
/* Array of Paragens. */
Paragem paragens[MAX_PARAGENS];
/* Array of Ligacoes. */
Ligacao ligacoes[MAX_LIGACOES];
/* Auxiliar array of Carreiras. */
Carreira aux_carreiras[MAX_CARREIRAS];

/* The number of paragens, carreiras and ligacoes stored. */
int cont_p = 0, cont_c = 0, cont_l = 0;
/* The values of custo and duracao each time. */
double custo = 0, duracao = 0;

/* Receives the array aux and if it is a prefix with more than 3 characters of the word 'inverso' it returns 0. */
int verificaInverso(char aux[MAX_INVERSO]){
    int i;

    if(strcmp(aux, "inverso") == 0) return 0;
    for(i = MIN_INVERSO; i < MAX_INVERSO; i++){
        if(strncmp(aux, "inverso", i) == 0) return 0;
    }
    printf("incorrect sort option.\n");
    return -1;
}

/* Receives the name of one Carreira and returns 0 if it is already registered. */
int existeCarreira(char nome[MAX_NOME_CARREIRA]){
    int i;
    for(i = 0; i < cont_c; i++)
        if(strcmp(carreiras[i].nome, nome) == 0) return 0;
    return -1;
}

/* Receives the name of one Paragem and returns 0 if it is already registered. */
int existeParagem(char nome[MAX_NOME_PARAGEM]){
    int i;
    for(i = 0; i < cont_p; i++)
        if(strcmp(paragens[i].nome, nome) == 0) return 0;
    return -1;
}

/* Reads one Paragem name from stdin, inserts it into the vector nome and returns the last writen character. */
char leParagem(char nome[MAX_NOME_PARAGEM]){
    char c;
    int i = 0;

    if((c = getchar()) == '"'){      
        while((c = getchar()) != '"'){
            nome[i] = c;
            i++;
        }
        c = getchar();
        nome[i] = '\0';
    }
    else {
        nome[0] = c;
        i++;
        while((c = getchar()) != ' ' && c != '\n'){
            nome[i] = c;
            i++;
        }  
        nome[i] = '\0';
    }
    return c;
}

/* Reads one Carreira name from stdin, inserts it into the vector nome and returns the last writen character. */
char leCarreira(char nome[MAX_NOME_CARREIRA]){
    char c;
    int i = 0;

    c = getchar();
    nome[i] = c;
    i++;

    while((c = getchar()) != ' ' && c != '\n'){     
        nome[i] = c;
        i++;
    }
    nome[i] = '\0';
    return c;
}

/* Receives one Carreira name and returns the number of stops it has. */
int contaParagens(char nome[MAX_NOME_CARREIRA]){
    int i;
    int p = 0, flag = 0;

    for(i = 0; i < cont_l; i++){
        if(strcmp(nome, ligacoes[i].carreira.nome) == 0) {
            flag = 1;
            p++;
            custo += ligacoes[i].custo;
            duracao += ligacoes[i].duracao;
        }
    }
    if(flag == 1) p++;
    return p;
}

/* Receives one Paragem name and returns the number of Carreiras that go through it. */
int contaCarreiras(char nome[MAX_NOME_PARAGEM]){
    int cont = 0, flag = 0;
    int i, j;

    for(i = 0; i < cont_l; i++){
        if(strcmp(ligacoes[i].origem.nome, nome) == 0 || strcmp(ligacoes[i].destino.nome, nome) == 0){
            flag = 0;
            for(j = 0; j < cont; j++){
                if(strcmp(aux_carreiras[j].nome, ligacoes[i].carreira.nome) == 0) flag = 1;
            }
            if(flag == 0){
                strcpy(aux_carreiras[cont].nome, ligacoes[i].carreira.nome);
                cont++;
            }
        }
    }
    return cont;
}

/* Receives the number of Carreiras in the auxiliar array of Carreiras and sorts it to alphabetical order. */
void ordenaAux_carreiras(int cont){
    int i, j;
    Carreira c;

    for (i = 0; i < cont; i++){
        for (j = i + 1; j < cont; j++){
            if (strcmp(aux_carreiras[i].nome, aux_carreiras[j].nome) > 0){
                c = aux_carreiras[i];
                aux_carreiras[i] = aux_carreiras[j];
                aux_carreiras[j] = c;
            }
        }
    }
}

/* Receives a Carreira name and adds it to the carreiras array. */
void adicionaCarreira(char nome[MAX_NOME_CARREIRA]){
    strcpy(carreiras[cont_c].nome, nome);
    
    carreiras[cont_c].origem.nome[0] = '\0';
    carreiras[cont_c].origem.latitude = 0;
    carreiras[cont_c].origem.longitude = 0;

    carreiras[cont_c].destino.nome[0] = '\0';
    carreiras[cont_c].destino.latitude = 0;
    carreiras[cont_c].destino.longitude = 0;

    cont_c++;
}

/* Receives a Carreira name and an auxiliar array and it prints all of the Carreiras stops. 
If the aux array is a valid prefix for 'inverso' it prints it in the inverse order. */
void listaCarreira(char nome[MAX_NOME_CARREIRA], char aux[MAX_INVERSO]){
    int i, paragens, l = 0, cont = 1;
    char origem[MAX_NOME_PARAGEM], destino[MAX_NOME_PARAGEM];

    for(i = 0; i < cont_c; i++){
        if(strcmp(carreiras[i].nome, nome) == 0){
            strcpy(origem, carreiras[i].origem.nome);
            strcpy(destino, carreiras[i].destino.nome);
        }
    }

    paragens = contaParagens(nome);

    if(aux[0] == '\0'){
        while(cont < paragens){
            if(strcmp(ligacoes[l].carreira.nome, nome) == 0 && strcmp(ligacoes[l].origem.nome, origem) == 0){
                printf("%s, ", origem);
                strcpy(origem, ligacoes[l].destino.nome);
                l = 0;
                cont++;
            }
            else l++;
        }
        printf("%s\n", destino);
    }

    else if(verificaInverso(aux) == 0){
        while(cont < paragens){
            if(strcmp(ligacoes[l].carreira.nome, nome) == 0 && strcmp(ligacoes[l].destino.nome, destino) == 0){
                printf("%s, ", destino);
                strcpy(destino, ligacoes[l].origem.nome);
                l = 0;
                cont++;
            }
            else l++;
        }
        printf("%s\n", origem);
    }
}

/* Prints all of the registered Carreiras. */
void listaCarreiras(){
    int i, p;
    custo = 0, duracao = 0;

    for(i = 0; i < cont_c; i++){
        p = contaParagens(carreiras[i].nome);

        if(p == 0) printf("%s %d %.2f %.2f\n", carreiras[i].nome, p, custo, duracao);
        else printf("%s %s %s %d %.2f %.2f\n", carreiras[i].nome, carreiras[i].origem.nome, 
            carreiras[i].destino.nome, p, custo, duracao);

        custo = 0;
        duracao = 0;
    }
}

/* Receives a Paragem name, its latitude and longitude values and adds it to the paragens array. */
int adicionaParagem(char nome[MAX_NOME_PARAGEM], double latitude, double longitude){
    int i;

    for(i = 0; i < cont_p; i++){
        if(strcmp(paragens[i].nome, nome) == 0){
            printf("%s: stop already exists.\n", nome);
            return -1;
        }
    }

    strcpy(paragens[cont_p].nome, nome);
    paragens[cont_p].latitude = latitude;
    paragens[cont_p].longitude = longitude;

    cont_p++;
    return 0;
}

/* Receives a Paragem name and prints its latitude and longitude values. */
int listaParagem(char nome[MAX_NOME_PARAGEM]){
    int i, cont = 0;

    for(i = 0; i < cont_p; i++){
        if(strcmp(paragens[i].nome, nome) == 0){
            printf("%16.12f %16.12f\n", paragens[i].latitude, paragens[i].longitude);
            cont = 1;
        }
    }
    if(cont == 0){
        printf("%s: no such stop.\n", nome);
        return -1;
    }
    return 0;
}

/* Prints all of the registered Paragens. */
void listaParagens(){
    int i, cont;
    for(i = 0; i < cont_p; i++){
        cont = contaCarreiras(paragens[i].nome);
        printf("%s: %16.12f %16.12f %d\n", paragens[i].nome, paragens[i].latitude, paragens[i].longitude, cont);
    }
}

/* Receives two arrays with a Carreira and Paragem names sizes. Reads from the input and adds a Ligacao with it. */
int adicionaLigacao(char nome_c[MAX_NOME_CARREIRA], char nome_p[MAX_NOME_PARAGEM]){
    int i, flag = 0;
    double custo, duracao;

    leCarreira(nome_c);
    if(existeCarreira(nome_c) != 0){
        printf("%s: no such line.\n", nome_c);
        return -1;
    }
    strcpy(ligacoes[cont_l].carreira.nome, nome_c);

    leParagem(nome_p);
    if(existeParagem(nome_p) != 0){
        printf("%s: no such stop.\n", nome_p);
        return -1;
    }
    strcpy(ligacoes[cont_l].origem.nome, nome_p);

    leParagem(nome_p);
    if(existeParagem(nome_p) != 0){
        printf("%s: no such stop.\n", nome_p);
        return -1;
    }
    strcpy(ligacoes[cont_l].destino.nome, nome_p);

    scanf(" %lf %lf", &custo, &duracao);
    if(custo < 0 || duracao < 0){
        printf("negative cost or duration.\n");
        return -1;
    }
    ligacoes[cont_l].custo = custo;
    ligacoes[cont_l].duracao = duracao;

    for(i = 0; i < cont_c; i++){
        if(strcmp(ligacoes[cont_l].carreira.nome, carreiras[i].nome) == 0){
    
            if((strcmp(carreiras[i].origem.nome, "\0") == 0 && (strcmp(carreiras[i].destino.nome, "\0") == 0))){
                flag = 1;
                strcpy(carreiras[i].origem.nome, ligacoes[cont_l].origem.nome);
                strcpy(carreiras[i].destino.nome, ligacoes[cont_l].destino.nome);
            }
            else if(strcmp(ligacoes[cont_l].origem.nome, carreiras[i].destino.nome) == 0){
                flag = 1;
                strcpy(carreiras[i].destino.nome, ligacoes[cont_l].destino.nome);
            }
            else if(strcmp(ligacoes[cont_l].destino.nome, carreiras[i].origem.nome) == 0){
                flag = 1;
                strcpy(carreiras[i].origem.nome, ligacoes[cont_l].origem.nome);
            }
        }
    }
    if(flag == 0){
        printf("link cannot be associated with bus line.\n");
        return -1;
    }
    cont_l++;
    return 0;  
}

/* Prints all of the registered Ligacoes. */
void listaIntersecoes(){
    int i, cont = 0;
    int j;

    for(i = 0; i < cont_p; i++){
        cont = contaCarreiras(paragens[i].nome);
        ordenaAux_carreiras(cont);

        if(cont > 1){
            printf("%s %d:", paragens[i].nome, cont);
            for(j = 0; j < cont; j++){
                printf(" %s", aux_carreiras[j].nome);
            }
            printf("\n");
        }
    }
}

/* Reads commands from stdin. */
int main(){
    char c, nome_p[MAX_NOME_PARAGEM], nome_c[MAX_NOME_CARREIRA], aux[MAX_INVERSO];
    double lat, lon;
    aux[0] = '\0';

	while((c = getchar()) != EOF){
		switch(c){
			case 'q':
				exit(0);
				break;

			case 'c':
                if((c = getchar()) == '\n') listaCarreiras();
                else{    
                    if(leCarreira(nome_c) == ' '){
                        scanf("%s", aux);  
                    }
                    if(existeCarreira(nome_c) == 0) listaCarreira(nome_c, aux);
                    else adicionaCarreira(nome_c);
                }
				break;

			case 'p':
                if((c = getchar()) == '\n') listaParagens();
                else {
                    if(leParagem(nome_p) == '\n') {   
                        listaParagem(nome_p);
                    } else{
                        scanf("%lf %lf", &lat, &lon);
                        adicionaParagem(nome_p, lat, lon);
                    }
                }
				break;

			case 'l':
                c = getchar();
                adicionaLigacao(nome_c, nome_p);
				break;

			case 'i':
                listaIntersecoes();
				break;
		}
	}
	return 0;
}


