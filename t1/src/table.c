/**
	>>>> TABLE <<<<
	Funcoes para manipulacao de tabelas
	e seus dados, como criacao de tabela e
	insercao de dados. 
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>
#include <input.h>
#include <table.h>

/** >> getFieldType
	Retorna o numero relativo ao tipo de campo
	a partir do parametro.
**/
FieldType getFieldType(System *SGBD, char *input){
	int i;

	for (i = 0; i < NTYPES; i++)
		if (strcmp(input, SGBD->fieldTypes[i]) == 0) return i;

	return NTYPES;
}

/** >> createTable
	Cria uma tabela a partir do comando correto,
	introduzido no shell. 
**/
bool createTable(char **cmd, System *SGBD){
	char **fmd;
	int i;

	// checa se o comando introduzido foi "create table"
	if (checkCmd(cmd, SGBD->commands[CMD_CREATE_TABLE])){ // comando correto

		// alocando nova tabela na memoria heap
		SGBD->tables = (Table **) realloc(SGBD->tables, sizeof(Table *)*(SGBD->tableCount + 1));
		SGBD->tables[SGBD->tableCount] = (Table *) malloc(sizeof(Table));

		// registrando nome da tabela
		SGBD->tables[SGBD->tableCount]->name = (char *) malloc
												(sizeof(char)*(strlen(cmd[CREATET_NAME_POS])+1));
		strcpy(SGBD->tables[SGBD->tableCount]->name, cmd[CREATET_NAME_POS]);

		// preparando vetor de campos da tabela
		SGBD->tables[SGBD->tableCount]->fields = NULL;
		SGBD->tables[SGBD->tableCount]->fieldCount = 0;

		// separando os campos em substrings, formando o field metadata
		fmd = match(cmd[CREATET_FIELDS_POS], MATCH_PARAM);

		// adicionando os campos na tabela
		for (i = 0; fmd[i] != NULL; i += 2){

			// alocando novo campo na tabela
			SGBD->tables[SGBD->tableCount]->fields = (Field **) realloc
									(SGBD->tables[SGBD->tableCount]->fields, sizeof(Field *)
											* (SGBD->tables[SGBD->tableCount]->fieldCount + 1));
			SGBD->tables[SGBD->tableCount]->fields[SGBD->tables[SGBD->tableCount]->fieldCount] =
																	(Field *) malloc(sizeof(Field));
			
			// registrando nome do campo
			casePreProcessing(fmd[i], PP_DATA_START, PP_DATA_END);
			SGBD->tables[SGBD->tableCount]->fields[SGBD->tables[SGBD->tableCount]->fieldCount]->fieldName = 
			(char *) malloc(sizeof(char) * (strlen(fmd[i]) + 1));
			strcpy(SGBD->tables[SGBD->tableCount]->fields[SGBD->tables[SGBD->tableCount]->fieldCount]->fieldName,
																										fmd[i]);

			// registrando tipo do campo
			casePreProcessing(fmd[i+1], PP_DATA_START, PP_DATA_END);
			SGBD->tables[SGBD->tableCount]->fields[SGBD->tables[SGBD->tableCount]->fieldCount]->type = 
											getFieldType(SGBD, fmd[i+1]);

			// registrando tamanho do campo, caso seja char
			if (SGBD->tables[SGBD->tableCount]->fields[SGBD->tables[SGBD->tableCount]->fieldCount]->type == CHAR){
																							
				// verificar se foi especificado um tamanho para o campo char,
				// checando se o proximo parametro e' um numero
				if (fmd[i+2] != NULL && atoi(fmd[i+2]) != 0){ // e' um numero, o tamanho do campo
					SGBD->tables[SGBD->tableCount]->fields[SGBD->tables[SGBD->tableCount]->fieldCount]->charSize
																								= atoi(fmd[i+2]);
					i++; // como esse campo tinha tres parametros, o indice do vetor deve andar uma unidade a mais
				} else // e' um char comum, com tamanho 1
				SGBD->tables[SGBD->tableCount]->fields[SGBD->tables[SGBD->tableCount]->fieldCount]->charSize = 1;
			} else // nao e' um char
				SGBD->tables[SGBD->tableCount]->fields[SGBD->tables[SGBD->tableCount]->fieldCount]->charSize = -1;

			// aumentando o numero de campos da tabela
			SGBD->tables[SGBD->tableCount]->fieldCount++;
		}

		// liberando o field metadata
		freeDoublePointerTerm((void **) fmd);

		// aumentando o numero de tabelas
		SGBD->tableCount++;

		// retorna ao shell, indicando que a operacao foi feita
		return true;

	} else return false; // comando invalido, volte ao shell e tente o proximo
}

/** >> insert
	Adiciona um novo registro em uma tabela
	a partir do comando correto, introduzido no shell. 
**/
bool insert(char **cmd, System *SGBD){
	FILE *tempFile;
	char *tempFileName;
	int i;

	// checa se o comando introduzido foi "insert into" e "values"
	if (checkCmd(cmd, SGBD->commands[CMD_INSERT]) &&
							checkCmd(cmd+INSERT_SUBCMD_POS, SGBD->commands[CMD_VALUES])){ // comando correto

		// alocando nome do arquivo temporario
		tempFileName = (char *) malloc(sizeof(char)*(strlen(TABLE_FOLDER) + strlen(cmd[INSERT_TABLE_POS])
														+ strlen(TEMP_FILE_EXTENSION) + 1));
		strcpy(tempFileName, TABLE_FOLDER);
		strcat(tempFileName, cmd[INSERT_TABLE_POS]);
		strcat(tempFileName, TEMP_FILE_EXTENSION);

		// abrindo acesso ao arquivo temporario em modo "append", indo para o fim do arquivo
		tempFile = fopen(tempFileName, "a+");

		// adicionando novos dados ao arquivo temporario
		fprintf(tempFile, "%s\n", cmd[INSERT_DATA_POS]);

		// liberando ponteiros criados na funcao
		fclose(tempFile);
		free(tempFileName);

		// retorna ao shell, indicando que a operacao foi feita
		SGBD->insertCount++;
		return true;

	} else return false; // comando invalido, volte ao shell e tente o proximo
}

/** >> includeTemporary
	Inclui, no arquivo de dados, as entradas que 
	estavam no arquivo temporario e remove o 
	arquivo temporario.
**/
void includeTemporary(char *tableName){
	char *tempFileName, *dataFileName, *read;
	FILE *tempFile, *dataFile;

	// alocando nome do arquivo temporario
		tempFileName = (char *) malloc(sizeof(char)*(strlen(TABLE_FOLDER) + strlen(tableName)
														+ strlen(TEMP_FILE_EXTENSION) + 1));
		strcpy(tempFileName, TABLE_FOLDER);
		strcat(tempFileName, tableName);
		strcat(tempFileName, TEMP_FILE_EXTENSION);

	// alocando nome do arquivo definitivo de dados
		dataFileName = (char *) malloc(sizeof(char)*(strlen(TABLE_FOLDER) + strlen(tableName)
														+ strlen(DATA_FILE_EXTENSION) + 1));
		strcpy(dataFileName, TABLE_FOLDER);
		strcat(dataFileName, tableName);
		strcat(dataFileName, DATA_FILE_EXTENSION);

	// abrindo acesso aos arquivos temporario e definitivo
		tempFile = fopen(tempFileName, "r"); // dados serao apenas lidos do temporario... 
		dataFile = fopen(dataFileName, "a"); // ...e serao adicionados ao fim do definitivo

	if (tempFile != NULL) { // verifica se existe um arquivo temporario
		// SIM, existe um temporario
		// lendo o arquivo temporario, linha a linha, e adicionando ao definitivo
		while (!feof(tempFile)){
			read = readLine(tempFile, '\n');
			if (!feof(tempFile)){
				fprintf(dataFile, "%s\n", read);
				free(read);
			}
		}

	// apagando arquivo temporario
		fclose(tempFile);
		remove(tempFileName);
	}
	
	// fechando arquivo de dados
		fclose(dataFile);

	// liberando strings alocadas na funcao
		free(tempFileName);
		free(dataFileName);
}

/** >> getTableId
	Retorna o id de uma determinada tabela (sua posicao
	no vetor de tabelas do SGBD) a partir do nome.
**/

int getTableId(char *name, System *SGBD){
	int i;

	for (i = 0; i < SGBD->tableCount; i++)// percorra todas as tabelas
		// caso encontre a tabela com o nome procurado, retorne seu id
		if (strcmp(name, SGBD->tables[i]->name) == 0) return i;
	
	// caso nao encontre, retorne -1
	return -1;
}

/** >> getFieldId
	Retorna o id de um determinado campo (sua posicao
	no vetor de campos de sua tabela) a partir do nome.
**/

int getFieldId(char *name, int tableId, System *SGBD){
	int i;

	for (i = 0; i < SGBD->tables[tableId]->fieldCount; i++) // percorra todos os campos
		// caso encontre o campo com o nome procurado, retorne seu id
		if (strcmp(name, SGBD->tables[tableId]->fields[i]->fieldName) == 0) return i;
	
	// caso nao encontre, retorne -1
	return -1;
}
