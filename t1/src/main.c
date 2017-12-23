#include <stdlib.h>
#include <stdio.h>

#include <input.h>
#include <utils.h>
#include <table.h>
#include <statistics.h>
#include <index.h>

/** >> exitSystem
	Desaloca toda a memoria Heap usada para guardar
	as informacoes do sistema.
**/
void exitSystem(System *SGBD){
	int i, j;

	// liberando comandos e tipos de dados
	freeDoublePointerSize((void **) SGBD->commands, NCOMMANDS);
	freeDoublePointerSize((void **) SGBD->fieldTypes, NTYPES);

	// liberando tabelas
	for (i = 0; i < SGBD->tableCount; i++){
		free(SGBD->tables[i]->name);
		// liberando campos da tabela
		for (j = 0; j < SGBD->tables[i]->fieldCount; j++){
			free(SGBD->tables[i]->fields[j]->fieldName);
			free(SGBD->tables[i]->fields[j]);
		}
		free(SGBD->tables[i]->fields);
		free(SGBD->tables[i]);
	}
	free(SGBD->tables);

	// liberando indices
	for (i = 0; i < SGBD->indexCount; i++) free(SGBD->indexes[i]);
	free(SGBD->indexes);

	// liberando sistema
	free(SGBD);
}

/** >> SGBDshell
	Recebe comandos do usuario e direciona-os para as
	devidas acoes dentro do SGBD, até o fim do fluxo
	de dados.
**/
void SGBDshell (FILE *stream, System* SGBD){
	char *input = NULL, **cmd;
	int i;

	// enquanto os comandos ainda existam, rode o shell
	while (!feof(stream)){

		// liberando comandos anteriores
		if (input != NULL){
			free(input);
			freeDoublePointerTerm((void **) cmd);
		}

		// leitura do comando
		input = readLine(stream, '\n');

		// enquanto os comandos ainda existam, rode o shell
		if (!feof(stream)){
			// etapa de pre-processamento do comando
			preProcessing(input, PP_DATA_START, PP_DATA_END, PP_DATA_SEARCH, PP_DATA_SWAP);
			preProcessing(input, PP_PARAM_START, PP_PARAM_END, PP_PARAM_SEARCH, PP_PARAM_SWAP);
			casePreProcessing(input, PP_PARAM_START, PP_PARAM_END);
			
			// reconhecimento do comando em substrings
			cmd = match(input, MATCH_CMD);

			// chamada das funcoes do SGBD
			if (createTable(cmd, SGBD)) continue;
			if (insert(cmd, SGBD)) continue;
			if (createIndex(cmd, SGBD)) continue;
			if (selectInTable(cmd, SGBD)) continue;
			if (sort(cmd, SGBD)) continue;
			if (showAllTables(cmd, SGBD)) continue;
			if (showAllIndexes(cmd, SGBD)) continue;
			if (showStatistics(cmd, SGBD)) continue;
		} else free(input);
	}
}

int main (int argc, char *argv[]){
	char *inputFileName;
	FILE *inputFile;	

	// inicialização do SGBD
	System *SGBD = (System *) calloc(1, sizeof(System));
	SGBD->commands = startCommands();
	SGBD->fieldTypes = startFieldTypes();
	
	// ler nome do arquivo com as entradas
	inputFileName = readLine(stdin, '\n');
	
	// abrir arquivo com as entradas
	inputFile = fopen(inputFileName, "r");

	// chamada do shell
	SGBDshell(inputFile, SGBD);

	// encerramento do SGBD
	exitSystem(SGBD);
	free(inputFileName);
	fclose(inputFile);
	return 0;
}
