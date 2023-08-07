/*
 * File:  project1.h
 * Author:  ist196757 - iaed-23
 * Description: Structures for a program for a public transport management system.
*/

#define MAX_NOME_CARREIRA 20
#define MAX_NOME_PARAGEM 50

typedef struct{
    char nome[MAX_NOME_PARAGEM];
    double latitude;
    double longitude;
} Paragem;

typedef struct{
    char nome[MAX_NOME_CARREIRA];
    Paragem origem;
    Paragem destino;
} Carreira;

typedef struct{
    Carreira carreira;
    Paragem origem;
    Paragem destino;
    double custo;
    double duracao;
} Ligacao;


