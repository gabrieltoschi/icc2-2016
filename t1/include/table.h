/**
	>>>> TABLE <<<<
	Funcoes para manipulacao de tabelas
	e seus dados, como criacao de tabela e
	insercao de dados. 
**/

#ifndef _TABLE_H_
#define _TABLE_H_

#include <utils.h>

// definicoes: create table
#define CREATET_NAME_POS 2 // posicao da substring com o nome da tabela a ser criada
#define CREATET_FIELDS_POS 3 // posicao da substring com os campos da tabela a ser criada

// definicoes: insert
#define INSERT_TABLE_POS 2 // posicao da substring com o nome da tabela a ser inserido um registro
#define INSERT_FIELDS_POS 3 // posicao da substring com os campos da tabela que vao receber dados
#define INSERT_SUBCMD_POS 4 // posicao da substring com o subcomando "values"
#define INSERT_DATA_POS 5 // posicao da substring com os dados a serem inseridos na tabela

bool createTable(char **, System *);
bool insert(char **, System *);
void includeTemporary(char *);
int getTableId(char *, System *);
int getFieldId(char *, int, System *);

#endif