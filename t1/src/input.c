/**
	>>>> INPUT <<<<
	Define todos os comandos do shell do SGBD e 
	contem as funcoes necessarias para reconhecer
	tais comandos quando digitados pelo usuário.
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <input.h>
#include <utils.h>

/** >> startCommands
	Armazena, em memoria, os comandos do shell para
	posteriores comparacoes com o que sera digitado
	no shell.
**/
char **startCommands(){
	char **commands = (char **) malloc(sizeof(char*)*NCOMMANDS);
	char *reading;
	int i;
	FILE *syntaxFile = fopen(COMMAND_SYNTAX_FILE, "r");

	for (i = 0; i < NCOMMANDS; i++){
		reading = readLine(syntaxFile, '\n');
		commands[i] = reading;
	}

	fclose(syntaxFile);
	return commands;
}

/** >> startFieldTypes
	Armazena, em memoria, os tipos de campos aceitos para
	posteriores comparacoes com o que sera digitado
	no shell.
**/
char **startFieldTypes(){
	char **types = (char **) malloc(sizeof(char*)*NTYPES);
	int i;
	FILE *typesFile = fopen(FIELDTYPE_SYNTAX_FILE, "r");

	for (i = 0; i < NTYPES; i++) types[i] = readLine(typesFile, '\n');

	fclose(typesFile);
	return types;
}

/** >> preProcessing
	Usado no caso do mesmo delimitador ser usado tanto para comandos
	quanto para separar parametros e dados. Altera o delimitador,
	usando, para isso, os caracteres de inicio e fim deste tipo
	de entrada no shell.
**/
void preProcessing(char *input, char start, char end, char search, char swap){
	int i, len = strlen(input);
	bool active = false;
	
	// corre por toda a string
	for (i = 0; i < len; i++){
		if (input[i] == start && !active){ // caso encontre o indicador de inicio
			active = true; // ative o pre-processamento
		} else if (input[i] == end && active) { // caso encontre o indicador de final                                             
			active = false; // desligue o pre-processamento
		} else if (input[i] == search && active){
			input[i] = swap;
		}
	}
}

/** >> casePreProcessing
	Usado para normalizar letras maiusculas e minusculas dentro dos
	comandos. Recebe dois caracteres, que servem como começo e inicio
	da excecao do preprocessamento, pois ele nao ocorre entre eles.
**/
void casePreProcessing(char *input, char exceptionStart, char exceptionEnd){
	int i, len = strlen(input);
	bool exception = false;

	for (i = 0; i < len; i++){
		if (input[i] == exceptionStart && !exception){
			exception = true;
		} else if (input[i] == exceptionEnd && exception) {
			exception = false;
		} else if (!exception){
			input[i] = tolower(input[i]);
		}
	}
}

/** >> match
	Aplica strtok em uma string e coleta substrings,
	de acordo com o delimitador. A ultima posicao do vetor de strings contem NULL, 
	indicando que nao ha mais comandos.
**/
char **match(char *input, char *delim){
	char **matched = NULL;
	int counter = 0;

	char *tok = strtok(input, delim);
	while (tok != NULL){
		matched = (char **) realloc(matched, sizeof(char *)*(counter+1));
		matched[counter] = (char *) malloc(sizeof(char)*(strlen(tok)+1));

		strcpy(matched[counter], tok);
		counter++;
		tok = strtok(NULL, delim);
	}

	matched = (char **) realloc(matched, sizeof(char *)*(counter+1));
	matched[counter] = NULL;

	return matched; 
}

/** >> checkCmd
	Checa se um conjunto de strings, vindas por um processo de match,
	representam um determinado comando, retornando true caso aquela
	linha do shell represente o comando determinado.
**/
bool checkCmd(char **input, char *command){
	char *toCheck = (char *) malloc(sizeof(char)*(strlen(command) + 1));
	strcpy(toCheck, command);
	char **checkMatch = match(toCheck, MATCH_CMD);
	int i;

	for (i = 0; checkMatch[i] != NULL; i++){
		if (input[i] == NULL || strcmp(input[i], checkMatch[i]) != 0){
			free(toCheck);
			freeDoublePointerTerm((void **) checkMatch);
			return false;
		}
	}

	free(toCheck);
	freeDoublePointerTerm((void **) checkMatch);
	return true;
}
