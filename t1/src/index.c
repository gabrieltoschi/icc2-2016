/**
	>>>> INDEX <<<<
	Funcoes para manipulacao de indices, 
	como criacao e buscas.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>
#include <input.h>
#include <table.h>
#include <index.h>

/** >> comparationInt
	Faz comparacao entre dois inteiros, recebidos
	por meio de strings.
**/
int comparationInt(char *a, char *b){
	int A = atoi(a);
	int B = atoi(b);
	if (A < B) return -1;
	if (A == B) return 0;
		else return 1;
}

/** >> comparationInt
	Faz comparacao entre duas strings.
**/
int comparationChar(char *a, char *b){
	return strcmp(a, b);
}

/** >> comparationDouble
	Faz comparacao entre dois numeros de ponto
	flutuante, recebidos por meio de strings.
**/
int comparationDouble(char *a, char *b){
	double A = atof(a);
	double B = atof(b);
	if (A < B) return -1;
	if (A == B) return 0;
		else return 1;
}
/** >> freeIndexData
	Libera um vetor de structs do tipo IndexData.
**/
void freeIndexData(IndexData *p, int n){
	n--;
	while (n >= 0) free(p[n--].data);
	free(p);
}

/** >> sortIndexData
	Ordena dados por meio de um campo para
	serem salvos posteriormente em arquivo de 
	indice.
**/
void sortIndexData(IndexData *idxData, int idxDataCount, FieldType fieldType){
	int (*ordCondition) (char *, char *);
	IndexData aux;
	int i, j;

	// seleciona o criterio de ordenacao, baseado no tipo do campo
	switch (fieldType){
		case INT:
			ordCondition = &comparationInt;
			break;
		case CHAR:
			ordCondition = &comparationChar;
			break;
		case DOUBLE:
		case FLOAT:
			ordCondition = &comparationDouble;
			break;
	}

	// usa bubble sort para ordenar os dados do indice
	for (i = idxDataCount - 1; i >= 1; i--){
		for (j = 0; j < i; j++){
			if (ordCondition(idxData[j].data, idxData[j+1].data) > 0){
				aux = idxData[j];
				idxData[j] = idxData[j+1];
				idxData[j+1] = aux;
			}
		}
	}
}
/** >> getIndexId
	Retorna o id de um indice a partir do
	id da tabela e o id do campo a que se refere.
**/
int getIndexId(int tableId, int fieldId, System *SGBD){
	int i;

	for (i = 0; i < SGBD->indexCount; i++){
		if (tableId == SGBD->indexes[i]->tableId &&
							fieldId == SGBD->indexes[i]->fieldId) 
			return i;
	}

	return -1;
}

/** >> indexBinarySearch
	Faz uma busca binaria em um vetor de IndexData,
	procurando pela chave key e fazendo as comparacoes
	a partir do tipo do campo do indice.
**/
int indexBinarySearch(IndexData *idxData, int idxDataCount, FieldType fieldType, char *key){
	int (*searchCondition) (char *, char *);
	int start, end, middle;

	// seleciona o criterio de ordenacao, baseado no tipo do campo
	switch (fieldType){
		case INT:
			searchCondition = &comparationInt;
			break;
		case CHAR:
			searchCondition = &comparationChar;
			break;
		case DOUBLE:
		case FLOAT:
			searchCondition = &comparationDouble;
			break;
	}

	// usa busca binaria para encontrar a primeira aparicao
	// daquele dado no indice
	start = 0;
	end = idxDataCount - 1;
	
	while (start <= end){
		middle = (start + end) / 2;
		if (searchCondition(idxData[middle].data, key) == 0) break;
			else if (searchCondition(idxData[middle].data, key) < 0) start = middle + 1;
			else end = middle - 1;
	}

	// checando para ver se a posicao encontrada e' valida
	if (searchCondition(idxData[middle].data, key) != 0) return -1;
	
	// expandindo o resultado da busca para encontrar o primeiro registro
	// no qual a key pode ser encontrada
	while (middle >= 0 && searchCondition(idxData[middle].data, key) == 0) middle--;
	middle++;

	return middle;
}

/** >> sequentialSearchComp
	Faz a comparacao de uma busca sequencial a partir
	do id do campo onde a chave poderia estar, ou seja,
	a sua posicao no registro do arquivo temporario.
**/
bool sequentialSearchComp(char *record, char *key, int fieldId){
	int found = false;

	// copiando o conteudo do registro para nao altera-lo na hora da impressao
	char *toComp = (char *) malloc(sizeof(char) * (strlen(record) + 1));
	strcpy(toComp, record);

	// dividindo o registro em substrings de dados
	char **compMatch = match(toComp, MATCH_PARAM);

	// caso a chave esteja no registro, retorne true
	if (strcmp(compMatch[fieldId], key) == 0) found = true;

	// liberando ponteiros
	free(toComp);
	freeDoublePointerTerm((void **) compMatch);

	return found;
}

/** >> printRecord
	Imprime um registro da tabela.
**/
void printRecord(char *record, int fieldCount){
	char **printMatch;
	int i;

	// passando por um pre-processamento inverso, tirando as marcas feitas pelo sistema
	preProcessing(record, PP_DATA_START, PP_DATA_END, PP_DATA_SWAP, PP_DATA_SEARCH);

	// dividindo o registro em substrings de dados
	printMatch = match(record, MATCH_PARAM);

	// imprimindo os dados do registro
	for (i = 0; i < fieldCount - 1; i++) printf("%s, ", printMatch[i]);
	printf("%s\n", printMatch[fieldCount-1]);

	// liberando substrings
	freeDoublePointerTerm((void **) printMatch);
}

/** >> createTable
	Cria um indice a partir do comando correto,
	introduzido no shell. 
**/
bool createIndex(char **cmd, System *SGBD){
	char *indexFileName, *dataFileName;
	char **tableData, **fieldData;
	IndexData *idxData = NULL;
	FILE *indexFile, *dataFile;
	int i, idxDataCount = 0;

	// checa se o comando introduzido foi "create index"
	if (checkCmd(cmd, SGBD->commands[CMD_CREATE_INDEX])){ // comando correto

		// colocando dados do arquivo temporario no arquivo definitivo
		includeTemporary(cmd[CREATEI_TABLE_POS]);

		// alocando indice no vetor de indices do SGBD
		SGBD->indexes = (Index **) realloc(SGBD->indexes, sizeof(Index *)*(SGBD->indexCount + 1));
		SGBD->indexes[SGBD->indexCount] = (Index *) malloc(sizeof(Index));

		// encontrando id da tabela no vetor de tabelas
		SGBD->indexes[SGBD->indexCount]->tableId = getTableId(cmd[CREATEI_TABLE_POS], SGBD);

		// encontrando id do campo no vetor de campos da tabela
		SGBD->indexes[SGBD->indexCount]->fieldId = getFieldId(cmd[CREATEI_FIELD_POS], 
														SGBD->indexes[SGBD->indexCount]->tableId, SGBD);

		// encontrando numero que representa o tipo do campo
		SGBD->indexes[SGBD->indexCount]->fieldType = SGBD->tables[SGBD->indexes[SGBD->indexCount]->tableId]->
															fields[SGBD->indexes[SGBD->indexCount]->fieldId]->type;

		// alocando nome do arquivo definitivo de dados da tabela
		dataFileName = (char *) malloc(sizeof(char)*(strlen(TABLE_FOLDER) + strlen(cmd[CREATEI_TABLE_POS])
														+ strlen(DATA_FILE_EXTENSION) + 1));
		strcpy(dataFileName, TABLE_FOLDER);
		strcat(dataFileName, cmd[CREATEI_TABLE_POS]);
		strcat(dataFileName, DATA_FILE_EXTENSION);

		// abrindo arquivo de dados
		dataFile = fopen(dataFileName, "r");

		// lendo o arquivo de dados
		tableData = readAllLines(dataFile);
		
		// percorrendo tableData com match, salvando apenas os enderecos de memoria dos campos
		// cuja posicao no vetor seja o mesmo que o id do campo do indice
		for (idxDataCount = 0; tableData[idxDataCount] != NULL; idxDataCount++){
			// dividindo todos os parametros em substrings
			fieldData = match(tableData[idxDataCount], MATCH_PARAM);

			// armazenando o parametro relativo ao id do campo do indice
			idxData = (IndexData *) realloc(idxData, sizeof(IndexData)*(idxDataCount + 1));
			idxData[idxDataCount].data = (char *) malloc(sizeof(char)*
										strlen(fieldData[SGBD->indexes[SGBD->indexCount]->fieldId]) + 1);
			strcpy(idxData[idxDataCount].data, fieldData[SGBD->indexes[SGBD->indexCount]->fieldId]);

			// salvando a linha em que essa entrada esta no arquivo de dados da tabela
			idxData[idxDataCount].line = idxDataCount;

			// liberando as substrings daquela linha
			freeDoublePointerTerm((void **) fieldData);
		}

		// liberando os ponteiros relativos ao arquivo de dados
		freeDoublePointerTerm((void **) tableData);
		fclose(dataFile);
		free(dataFileName);

		// ordenando os dados do indice
		sortIndexData(idxData, idxDataCount, SGBD->indexes[SGBD->indexCount]->fieldType);

		// alocando nome do arquivo de indice
		indexFileName = (char *) malloc(sizeof(char)*(strlen(INDEX_FOLDER) + strlen(cmd[CREATEI_TABLE_POS])
											+ strlen(INDEX_FILENAME_SEPARATOR) + strlen(cmd[CREATEI_FIELD_POS])
														+ strlen(INDEX_FILE_EXTENSION) + 1));
		strcpy(indexFileName, INDEX_FOLDER);
		strcat(indexFileName, cmd[CREATEI_TABLE_POS]);
		strcat(indexFileName, INDEX_FILENAME_SEPARATOR);
		strcat(indexFileName, cmd[CREATEI_FIELD_POS]);
		strcat(indexFileName, INDEX_FILE_EXTENSION);

		// abrindo arquivo de indice
		indexFile = fopen(indexFileName, "w");

		// escrevendo dados do indice no arquivo de indice
		for (i = 0; i < idxDataCount; i++)
			fprintf(indexFile, "%s %d\n", idxData[i].data, idxData[i].line);

		// liberando ponteiros relativos ao indice
		free(indexFileName);
		freeIndexData(idxData, idxDataCount);
		fclose(indexFile);

		// retorna ao shell, indicando que a operacao foi feita
		SGBD->indexCount++;
		return true;

	} else return false; // comando invalido, volte ao shell e tente o proximo
}

/** >> sort
	Recria um indice a partir do comando correto,
	introduzido no shell. 
**/
bool sort(char **cmd, System *SGBD){
	char *indexFileName, *dataFileName;
	char **tableData, **fieldData;
	IndexData *idxData = NULL;
	FILE *indexFile, *dataFile;
	int i, idxDataCount = 0;

	int tableId, fieldId, indexId;

	// checa se o comando introduzido foi "sort"
	if (checkCmd(cmd, SGBD->commands[CMD_SORT])){ // comando correto

		// colocando dados do arquivo temporario no arquivo definitivo
		includeTemporary(cmd[SORT_TABLE_POS]);

		// encontrando id da tabela no vetor de tabelas
		tableId = getTableId(cmd[SORT_TABLE_POS], SGBD);

		// encontrando id do campo no vetor de campos da tabela
		fieldId = getFieldId(cmd[SORT_FIELD_POS], tableId, SGBD);

		// encontrando id do indice no vetor de indices
		indexId = getIndexId(tableId, fieldId, SGBD);

		// alocando nome do arquivo definitivo de dados da tabela
		dataFileName = (char *) malloc(sizeof(char)*(strlen(TABLE_FOLDER) + strlen(cmd[SORT_TABLE_POS])
														+ strlen(DATA_FILE_EXTENSION) + 1));
		strcpy(dataFileName, TABLE_FOLDER);
		strcat(dataFileName, cmd[SORT_TABLE_POS]);
		strcat(dataFileName, DATA_FILE_EXTENSION);

		// abrindo arquivo de dados
		dataFile = fopen(dataFileName, "r");

		// lendo o arquivo de dados
		tableData = readAllLines(dataFile);

		// percorrendo tableData com match, salvando apenas os enderecos de memoria dos campos
		// cuja posicao no vetor seja o mesmo que o id do campo do indice
		for (idxDataCount = 0; tableData[idxDataCount] != NULL; idxDataCount++){
			// dividindo todos os parametros em substrings
			fieldData = match(tableData[idxDataCount], MATCH_PARAM);

			// armazenando o parametro relativo ao id do campo do indice
			idxData = (IndexData *) realloc(idxData, sizeof(IndexData)*(idxDataCount + 1));
			idxData[idxDataCount].data = (char *) malloc(sizeof(char)*
										strlen(fieldData[fieldId]) + 1);
			strcpy(idxData[idxDataCount].data, fieldData[fieldId]);

			// salvando a linha em que essa entrada esta no arquivo de dados da tabela
			idxData[idxDataCount].line = idxDataCount;

			// liberando as substrings daquela linha
			freeDoublePointerTerm((void **) fieldData);
		}

		// liberando os ponteiros relativos ao arquivo de dados
		freeDoublePointerTerm((void **) tableData);
		fclose(dataFile);
		free(dataFileName);

		// ordenando os dados do indice
		sortIndexData(idxData, idxDataCount, SGBD->indexes[indexId]->fieldType);

		// alocando nome do arquivo de indice
		indexFileName = (char *) malloc(sizeof(char)*(strlen(INDEX_FOLDER) + strlen(cmd[SORT_TABLE_POS])
											+ strlen(INDEX_FILENAME_SEPARATOR) + strlen(cmd[SORT_FIELD_POS])
														+ strlen(INDEX_FILE_EXTENSION) + 1));
		strcpy(indexFileName, INDEX_FOLDER);
		strcat(indexFileName, cmd[SORT_TABLE_POS]);
		strcat(indexFileName, INDEX_FILENAME_SEPARATOR);
		strcat(indexFileName, cmd[SORT_FIELD_POS]);
		strcat(indexFileName, INDEX_FILE_EXTENSION);

		// abrindo arquivo de indice
		indexFile = fopen(indexFileName, "w");

		// escrevendo dados do indice no arquivo de indice
		for (i = 0; i < idxDataCount; i++)
			fprintf(indexFile, "%s %d\n", idxData[i].data, idxData[i].line);

		// liberando ponteiros relativos ao indice
		free(indexFileName);
		freeIndexData(idxData, idxDataCount);
		fclose(indexFile);

		// retorna ao shell, indicando que a operacao foi feita
		SGBD->sortCount++;
		return true;

	} else return false; // comando invalido, volte ao shell e tente o proximo
}

/** >> selectInTable
	Busca um valor nos registros de uma tabela
	a partir do comando correto, introduzido no shell. 
**/
bool selectInTable(char **cmd, System *SGBD){
	char *indexFileName, *dataFileName, *tempFileName;
	char *indexLine;
	char **tableData, **notCharCorrection;
	IndexData *idxData = NULL;
	FILE *indexFile, *dataFile, *tempFile;
	int idxDataCount = 0;
	int tableId, fieldId, indexId, binaryStartResult;
	FieldType fieldType;

	int i;

	// checa se o comando introduzido foi "select"
	if (checkCmd(cmd, SGBD->commands[CMD_SELECT])){ // comando correto

		// encontrando id da tabela no vetor de tabelas
		tableId = getTableId(cmd[SELECT_TABLE_POS], SGBD);

		// encontrando id do campo no vetor de campos da tabela
		fieldId = getFieldId(cmd[SELECT_FIELD_POS], tableId, SGBD);

		/** BUSCA BINARIA (no arquivo de INDICE) **/
		// zerando o numero de registros encontrados em busca binaria
		SGBD->lastBinarySearch = 0;

		// alocando nome do arquivo de indice
		indexFileName = (char *) malloc(sizeof(char)*(strlen(INDEX_FOLDER) + strlen(cmd[SELECT_TABLE_POS])
											+ strlen(INDEX_FILENAME_SEPARATOR) + strlen(cmd[SELECT_FIELD_POS])
														+ strlen(INDEX_FILE_EXTENSION) + 1));
		strcpy(indexFileName, INDEX_FOLDER);
		strcat(indexFileName, cmd[SELECT_TABLE_POS]);
		strcat(indexFileName, INDEX_FILENAME_SEPARATOR);
		strcat(indexFileName, cmd[SELECT_FIELD_POS]);
		strcat(indexFileName, INDEX_FILE_EXTENSION);

		// abrindo arquivo de indice
		indexFile = fopen(indexFileName, "r");

		// caso o arquivo de indice exista, faca a busca binaria
		if (indexFile != NULL){
			// recuperando dados do arquivo de indice
			while (!feof(indexFile)){
				indexLine = readLine(indexFile, SELECT_INDEX_DELIMITER);
				if (!feof(indexFile)){
					// alocando um espaco para guardar os dados de uma entrada do indice
					idxData = (IndexData *) realloc(idxData, sizeof(IndexData) * (idxDataCount + 1));

					// guardando informacao da tabela contida no indice
					idxData[idxDataCount].data = indexLine;

					// guardando linha do indice onde a informacao se encontra
					indexLine = readLine(indexFile, '\n');
					idxData[idxDataCount++].line = atoi(indexLine);
					free(indexLine);

				} else free(indexLine);
			}

			// liberando ponteiros
			fclose(indexFile);
			free(indexFileName);

			// encontrando id do indice no vetor de indices
			indexId = getIndexId(tableId, fieldId, SGBD);

			// encontrando o tipo do campo do indice
			fieldType = SGBD->indexes[indexId]->fieldType;
			
			// checando se as aspas simples devem continuar, caso o campo seja char
			if (fieldType != CHAR){
				notCharCorrection = match(cmd[SELECT_DATA_POS], MATCH_DATANOTCHAR);
				free(cmd[SELECT_DATA_POS]);
				cmd[SELECT_DATA_POS] = (char *) malloc(sizeof(char)*(sizeof(notCharCorrection[0]) + 1));
				strcpy(cmd[SELECT_DATA_POS], notCharCorrection[0]);
				freeDoublePointerTerm((void **) notCharCorrection);				
			}
			
			// procurando o inicio dos registros por busca binaria
			binaryStartResult = indexBinarySearch(idxData, idxDataCount, fieldType, cmd[SELECT_DATA_POS]);

			// caso o registro seja encontrado no indice
			if (binaryStartResult != -1){
				// alocando nome do arquivo definitivo de dados da tabela
				dataFileName = (char *) malloc(sizeof(char)*(strlen(TABLE_FOLDER) + strlen(cmd[SELECT_TABLE_POS])
																+ strlen(DATA_FILE_EXTENSION) + 1));
				strcpy(dataFileName, TABLE_FOLDER);
				strcat(dataFileName, cmd[SELECT_TABLE_POS]);
				strcat(dataFileName, DATA_FILE_EXTENSION);

				// abrindo arquivo de dados
				dataFile = fopen(dataFileName, "r");

				// lendo o arquivo de dados
				tableData = readAllLines(dataFile);

				// liberando ponteiros
				fclose(dataFile);
				free(dataFileName);

				// percorrendo o idxData, a partir do indice retornado pela busca binaria, imprimindo as linhas
				// encontradas no registro
				for (i = binaryStartResult; strcmp(idxData[i].data, cmd[SELECT_DATA_POS]) == 0; i++){
					// imprime os dados na tela
					printRecord(tableData[idxData[i].line], SGBD->tables[tableId]->fieldCount);
					// conta um registro encontrado por busca binaria
					SGBD->lastBinarySearch++;
				}

				// liberando ponteiros
				freeDoublePointerTerm((void **) tableData);
				freeIndexData(idxData, idxDataCount);
			
			}	
		} else free(indexFileName);

		/** BUSCA SEQUENCIAL (no arquivo TEMPORARIO) **/
		// zerando o numero de registros encontrados em busca sequencial
		SGBD->lastSequentialSearch = 0;

		// alocando nome do arquivo temporario
		tempFileName = (char *) malloc(sizeof(char)*(strlen(TABLE_FOLDER) + strlen(cmd[SELECT_TABLE_POS])
														+ strlen(TEMP_FILE_EXTENSION) + 1));
		strcpy(tempFileName, TABLE_FOLDER);
		strcat(tempFileName, cmd[SELECT_TABLE_POS]);
		strcat(tempFileName, TEMP_FILE_EXTENSION);

		// abrindo arquivo temporario
		tempFile = fopen(tempFileName, "r");

		// caso o arquivo temporario exista
		if (tempFile != NULL){
			// lendo o arquivo de dados
			tableData = readAllLines(tempFile);

			// liberando ponteiros
			free(tempFileName);
			fclose(tempFile);

			// percorrendo todos os registros do arquivo temporario, procurando pela chave buscada
			for (i = 0; tableData[i] != NULL; i++){
				// caso o registro tenha a chave
				if (sequentialSearchComp(tableData[i], cmd[SELECT_DATA_POS], fieldId)){
					// imprimindo o registro
					printRecord(tableData[i], SGBD->tables[tableId]->fieldCount);

					// conta um registro encontrado por busca sequencial
					SGBD->lastSequentialSearch++;
				}
			}

			// liberando ponteiros
			freeDoublePointerTerm((void **) tableData);
		} else free(tempFileName);		

		// caso nao exista nenhum retorno da busca
		if (SGBD->lastBinarySearch == 0 && SGBD->lastSequentialSearch == 0)
						printf("null\n"); // imprima null
		
		// retorna ao shell, indicando que a operacao foi feita
		SGBD->selectCount++;
		return true;

	} else return false; // comando invalido, volte ao shell e tente o proximo
}
