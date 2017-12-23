/**
	>>>> UTILS <<<<
	Define funcoes e tipos que auxiliam em 
	varias funcoes ou sao uteis em varias
	partes do SGBD.
**/

#ifndef _UTILS_H_
#define _UTILS_H_

// definicoes: pastas, arquivos e extensoes
#define COMMAND_SYNTAX_FILE "syntax/cmd.syntax" // arquivo com os comandos do shell
#define FIELDTYPE_SYNTAX_FILE "syntax/type.syntax" // arquivo com os tipos de campos aceitos

#define TABLE_FOLDER "table/" // pasta com os arquivos de dados de tabela
#define TEMP_FILE_EXTENSION ".tmp" // extensao dos arquivos temporarios de tabela
#define DATA_FILE_EXTENSION ".dat" // extensao dos arquivos definitivos de tabela

#define INDEX_FOLDER "index/" // pasta com os arquivos de indice
#define INDEX_FILE_EXTENSION ".idx" // extensao dos arquivos de indice
#define INDEX_FILENAME_SEPARATOR "-" // separador entre tabela e campo no nome de arquivo do indice

/** >> (Tipo) bool
	Variaveis de verdadeiro ou falso,
	para facilitar o trabalho com condicoes
	e operacoes booleanas.
**/
typedef enum {
	false,
	true
} bool;

/** >> (Tipo) FieldType
	Facilita o acesso aos diversos tipos possiveis
	de um campo de uma tabela por meio de um enum.
**/
typedef enum {
	INT,
	CHAR, 
	DOUBLE, 
	FLOAT,
	NTYPES
} FieldType;

/** >> (Tipo) Field
	Armazena informacoes sobre o campo de uma
	tabela, como o seu nome e o seu tipo.
**/
typedef struct {
	char *fieldName; // nome do campo
	FieldType type; // tipo do campo
	int charSize; // tamanho do campo, caso seja char
} Field;

/** >> (Tipo) Table
	Armazena informacoes sobre uma tabela,
	como seu nome e os campos que a compoe.
**/
typedef struct {
	char *name; // nome da tabela
	Field **fields; // campos da tabela
	int fieldCount; // quantidade de campos
} Table;

/** >> (Tipo) Index
	Armazena as informacoes sobre a tabela e o
	campo a que se refere um index.
**/
typedef struct {
	int tableId;
	int fieldId;
	FieldType fieldType;
} Index;

/** >> (Tipo) System
	Armazena dados sobre o SGBD: lista de tabelas,
	quantidade de tabelas e dados úteis para a funcao
	statistics.
**/ 
typedef struct {
	char **commands; // comandos do shell
	char **fieldTypes; // tipos aceitos de campos
	Table** tables;// lista de tabelas
	Index** indexes; // lista de indices
	int tableCount; // numero de tabelas
	int indexCount; // numero de indices
	int insertCount; // numero de inserts
	int selectCount; // numero de selects
	int sortCount; // numero de sorts
	int showAllTablesCount; // numero de "showalltables"
	int showAllIndexesCount; // numero de "showallindexes"
	int lastBinarySearch; // numero de instancias recuperadas na ultima busca binaria
	int lastSequentialSearch; // numero de instancias recuperadas na ultima busca sequencial
} System;

void freeDoublePointerSize(void **, int);
void freeDoublePointerTerm(void **);
char *readLine(FILE *, char);
char **readAllLines(FILE *stream);

#endif
