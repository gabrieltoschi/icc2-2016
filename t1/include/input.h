/**
	>>>> INPUT <<<<
	Define todos os comandos do shell do SGBD e 
	contem as funcoes necessarias para reconhecer
	tais comandos quando digitados pelo usuário.
**/

#ifndef _INPUT_H_
#define _INPUT_H_

#include <utils.h>

/** >> (Tipo) Command
	Enum para facilitar o acesso aos comandos
	do shell, armazenados em arquivo em disco
	e lidos pelo SGBD pela funcao startCommands.
**/
typedef enum {
	CMD_CREATE_TABLE,
	CMD_INSERT,
	CMD_VALUES,
	CMD_CREATE_INDEX,
	CMD_SELECT,
	CMD_SORT,
	CMD_SHOWALL_TABLE,
	CMD_SHOWALL_INDEX,
	CMD_STAT,
	NCOMMANDS
} Command;

// Definicoes: delimitadores para funcao match
#define MATCH_CMD " ;()\n" // delimitadores entre comandos
#define MATCH_PARAM "$,[]" // delimitadores entre parametros
#define MATCH_DATANOTCHAR "\'" // delimitadores entre strings de dados

// Definicoes: delimitadores para funcao preProcessing
/* START: inicio da area de pre-processamento
   END: final da area de pre-processamento
   SEARCH: caractere buscado
   SWAP: caractere que substituira */

// pre-processamento de parametros
#define PP_PARAM_START '('
#define PP_PARAM_END ')'
#define PP_PARAM_SEARCH ' '
#define PP_PARAM_SWAP '$'

// pre-processamento de dados
#define PP_DATA_START '\''
#define PP_DATA_END '\''
#define PP_DATA_SEARCH ' '
#define PP_DATA_SWAP '|'

char **startCommands();
char **startFieldTypes();
char **match(char *, char *);
void preProcessing(char *, char, char, char, char);
void casePreProcessing(char *, char, char);
bool checkCmd(char **, char *);

#endif
