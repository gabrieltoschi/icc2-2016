/**
	>>>> STATISTICS <<<<
	Funcoes para apresentacao de
	estatisticas e informacoes sobre o SGBD,
	como tabelas, indices e chamadas de comandos.
**/

#include <stdlib.h>
#include <stdio.h>

#include <utils.h>
#include <input.h>

/** >> getFieldSize
	Retorna o tamanho de um campo a partir de seu codigo
	FieldType.
**/
int getFieldSize(Field *f){
	switch (f->type){
		case INT:
			return sizeof(int);
		case CHAR:
			return sizeof(char) * f->charSize;
		case DOUBLE:
			return sizeof(double);
		case FLOAT:
			return sizeof(float);
	}
}

/** >> showAllTables
	Imprime, na saida padrao, as informacoes
	relativas a todas as tabelas do SGBD, assim
	como os campos que as compoem.
**/
bool showAllTables(char **cmd, System *SGBD){
	int i, j;

	// checa se o comando introduzido foi "showalltables"
	if (checkCmd(cmd, SGBD->commands[CMD_SHOWALL_TABLE])){ // comando correto
		// para todas as tabelas do SGBD
		for (i = 0; i < SGBD->tableCount; i++){
			// imprimindo nome da tabela
			printf("\nTablename: %s\n", SGBD->tables[i]->name);
			// para todos os campos da tabela
			for (j = 0; j < SGBD->tables[i]->fieldCount; j++){
				// imprimindo informacoes do campo
				printf("\tField: %s Type: %s Size %d\n", 
					SGBD->tables[i]->fields[j]->fieldName,
					SGBD->fieldTypes[SGBD->tables[i]->fields[j]->type],
					getFieldSize(SGBD->tables[i]->fields[j]));
			}
			printf("\n");
		}

		// retorna ao shell, indicando que a operacao foi feita
		SGBD->showAllTablesCount++;
		return true;

	} else return false; // comando invalido, volte ao shell e tente o proximo
}

/** >> showAllIndexes
	Imprime, na saida padrao, as informacoes
	relativas a todos os indices do SGBD, assim
	como as tabelas e campos a que se referem.
**/
bool showAllIndexes(char **cmd, System *SGBD){
	int i, j;

	// checa se o comando introduzido foi "showallindexes"
	if (checkCmd(cmd, SGBD->commands[CMD_SHOWALL_INDEX])){ // comando correto
		// para todas as tabelas do SGBD
		for (i = 0; i < SGBD->indexCount; i++){
			printf("\nIndex information\n");
			// nome da tabela relacionada ao indice i
			printf("\tTablename: %s\n", SGBD->tables[SGBD->indexes[i]->tableId]->name);
			// nome do campo indexado
			printf("\tFieldname: %s\n\n", SGBD->tables[SGBD->indexes[i]->tableId]->
				fields[SGBD->indexes[i]->fieldId]->fieldName);
		}

		// retorna ao shell, indicando que a operacao foi feita
		SGBD->showAllIndexesCount++;
		return true;

	} else return false; // comando invalido, volte ao shell e tente o proximo
}

/** >> showStatistics
	Imprime, na saida padrao, estatisticas
	sobre o estado atual do SGBD.
**/
bool showStatistics(char **cmd, System *SGBD){
	
	// checa se o comando introduzido foi "statistics"
	if (checkCmd(cmd, SGBD->commands[CMD_STAT])){ // comando correto
		
		printf("#Tables: %d\n", SGBD->tableCount); // numero de tabelas criadas ate entao
		printf("#Indexes: %d\n", SGBD->indexCount); // numero de indices criados ate entao
		printf("#Inserts: %d\n", SGBD->insertCount); // numero de inserts feitos ate entao
		printf("#Selects: %d\n", SGBD->selectCount); // numero de selects feitos ate entao
		printf("#Sorts: %d\n", SGBD->sortCount); // numero de sorts feitos ate entao
		// numero de 'showalltables' feitos ate entao
		printf("#ShowAllTables: %d\n", SGBD->showAllTablesCount);
		// numero de 'showallindexes' feitos ate entao
		printf("#ShowAllIndexes: %d\n", SGBD->showAllIndexesCount);
		// numero de instancias recuperadas via busca binaria na ultima consulta feita
		printf("#Records in last select (binary search): %d\n",	SGBD->lastBinarySearch);
		// numero de instancias recuperadas via busca sequencial na ultima
		// consulta feita
		printf("#Records in last select (sequential search): %d\n",	SGBD->lastSequentialSearch);

		// retorna ao shell, indicando que a operacao foi feita
		return true;

	} else return false; // comando invalido, volte ao shell e tente o proximo
}