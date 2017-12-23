/**
	>>>> INDEX <<<<
	Funcoes para manipulacao de indices, 
	como criacao e buscas.
**/

#ifndef _INDEX_H_
#define _INDEX_H_

/** >> (Tipo) IndexData
	Auxilia as operacoes com arquivos de indice e guarda,
	alem do valor daquele dado do indice, a linha em que
	a informacao se encontra no arquivo de dados da tabela.
**/
typedef struct {
	char *data;
	int line;
} IndexData;

// definicoes: create index
#define CREATEI_TABLE_POS 2 // posicao da substring com o nome da tabela do indice
#define CREATEI_FIELD_POS 3 // posicao da substring com o campo da tabela do indice

// definicoes: sort
#define SORT_TABLE_POS 1 // posicao da substring com o nome da tabela do indice
#define SORT_FIELD_POS 2 // posicao da substring com o campo da tabela do indice

// definicoes: select
#define SELECT_TABLE_POS 1 // posicao da substring com o nome da tabela
#define SELECT_FIELD_POS 2 // posicao da substring com o nome do campo
#define SELECT_DATA_POS  3 // posicao da substring com o nome do valor buscado
#define SELECT_INDEX_DELIMITER ' ' // delimitador entre os dados do indice

bool createIndex(char **, System *);
bool sort(char **, System *);
bool selectInTable(char **, System *);

#endif