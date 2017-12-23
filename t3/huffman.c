/**
	>>>> HUFFMAN <<<<
	Funcoes usadas para compactacao
	e decompactacao de Huffman.
**/

#include "huffman.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** >> createCharHuff
	Inicializa um Huff a partir de um caractere
	unico.
**/
Huff *createCharHuff(char ch, int freq){
	Huff *huff = (Huff *) malloc(sizeof(Huff));
	huff->string = (char *) malloc(sizeof(char)*2);
	huff->string[0] = ch;
	huff->string[1] = '\0';
	huff->frequence = freq;
	
	return huff;
}

/** >> createStrHuff
	Inicializa um Huff a partir de uma
	string de caracteres.
**/
Huff *createStrHuff(char *str, int freq){
	Huff *huff = (Huff *) malloc(sizeof(Huff));
	huff->string = str;
	huff->frequence = freq;
	
	return huff;
}

/** >> printHuff
	Imprime um Huff na saida padrao.
**/
void printHuff(Huff *huff){
	printf("%d - %s\n", huff->frequence, huff->string);
}

/** >> freeHuff
	Libera um Huff alocado na memoria Heap.
**/
void freeHuff(Huff *huff){
	free(huff->string);
	free(huff);
}

/** >> createFrequenceList
	A partir de uma lista de caracteres 
	ordenados pelas suas frequencias em uma string,
	cria uma lista ligada com um TreeNode contendo
	um Huff relacionado a cada caractere para
	o inicio da construcao da arvore de Huffman.
**/
List *createFrequenceList(char *chars, int *frequences, int charCount){
	List *l = listCreate();
	Huff *itHuff;
	TreeNode *itTree;
	int i;
	
	for (i = ASCII_MAX - charCount; i < ASCII_MAX; i++){
		itHuff = createCharHuff(chars[i], frequences[chars[i]]);
		itTree = treeCreateNode(itHuff);
		
		listOrdInsert(l, itTree);
	}
	
	return l;	
}

/** >> huffmanMakeTree
	Constroi uma arvore de Huffman a partir
	de uma lista ligada com os nos primarios
	da arvore (os que contem apenas um
	caractere)
**/
Tree *huffmanMakeTree(List *chars){
	Tree *compact = treeCreate();
	TreeNode *leftChild, *rightChild, *newChild;
	Huff *newHuff;
	char *newString;
	
	// enquanto ainda houver nos para serem unidos
	while (chars->size > 1){
		// remove os dois menores nos da lista
		leftChild = listRemoveFirst(chars);
		rightChild = listRemoveFirst(chars);
		
		// cria um novo no unindo os dois
		newString = concatMalloc(leftChild->item->string, rightChild->item->string);
		newHuff = createStrHuff(newString, leftChild->item->frequence + rightChild->item->frequence);	
		newChild = treeCreateNode(newHuff);
		treeAssemblyNode(newChild, leftChild, rightChild);
		
		// adiciona o novo no na lista ligada
		listOrdInsert(chars, newChild);		
	}
	
	// atribui a raiz a arvore
	treeAssemblyRoot(compact, listRemoveFirst(chars));
	
	return compact;
}

/** >> createHuffTable
	Inicializa uma HuffTable.
**/
HuffTable *createHuffTable(){
	HuffTable *t = (HuffTable *) malloc(sizeof(HuffTable));
	t->fields = NULL;
	t->size = 0;
	
	return t;
}

/** >> huffmanGetCode
	A partir de um caminho percorrido pela
	arvore de Huffman, gera uma string com o
	codigo correspondente a este caminho.
**/
char *huffmanGetCode(Bit *path, int bitCount){
	int i;
	char *code = (char *) malloc(sizeof(char)*(bitCount+1));
	
	for (i = 0; i < bitCount; i++)
		code[i] = path[i];
	
	code[bitCount] = '\0';
	return code;
}

/** >> freeHuff
	Navega recursivamente pela arvore de Huffman
	procurando caracteres unicos (folhas da 
	arvore) e gera os codigos a partir do caminho
	usado para chegar neles;
**/
void huffmanNavigate(TreeNode *node, Bit *path, HuffTable *table, int level, char direction){
	// verificar se chegou em uma folha
	if (node->left == NULL && node->right == NULL) {
		path[level] = direction;
		
		// armazena um novo codigo na HuffTable
		table->fields = (HuffUnit *) realloc(table->fields, sizeof(HuffUnit)*(table->size + 1));
		table->fields[table->size].code = huffmanGetCode(path, level+1);
		table->fields[table->size].symbol = node->item->string[0];
		table->size++;
		return;
	}
	
	// verificar se não está no primeiro no
	if (level != -1)
		path[level] = direction;
	
	// va para os dois filhos deste no
	huffmanNavigate(node->left, path, table, level+1, '0');
	huffmanNavigate(node->right, path, table, level+1, '1');
}

/** >> freeHuff
	Faz uma HuffTable a partir de uma arvore
	de Huffman, iniciando a sua navegacao
	a partir da raiz da arvore.
**/
HuffTable *huffmanMakeTable(Tree *tree){
	Bit *path = (Bit *) malloc(sizeof(Bit)*HUFF_BITS_MAX);
	HuffTable *table = createHuffTable();
		
	huffmanNavigate(tree->root, path, table, -1, '0');
	
	free(path);
	return table;
}

/** >> huffmanPrintTable
	Imprime uma HuffTable na saida padrao.
**/
void huffmanPrintTable(HuffTable *table){
	int i;
	
	for (i = 0; i < table->size; i++){
		printf("%c - %s\n", table->fields[i].symbol, table->fields[i].code);
	}
}

/** >> sortFrequences
	Ordena um vetor de frequencias de caractere
	em um texto e retorna um segundo vetor, com
	os caracteres ordenados de acordo com
	a frequencia.
**/
char *sortFrequences(int *freq){
	char *chars = (char *) malloc(sizeof(char)*ASCII_MAX);
	int i, j, key;

	for (i = 0; i < ASCII_MAX; i++) chars[i] = (char) i;

	for (i = 1; i < ASCII_MAX; i++){
		key = (int) chars[i];
		j = i-1;

		while (j >= 0 && freq[key] < freq[chars[j]]){
			chars[j+1] = chars[j];
			j--;
		}
		
		chars[j+1] = (char) key;
	}

	return chars;
}

/** >> getCharCount
	Retorna o numero de caracteres que tem
	frequencia maior que 0 em um vetor ordenado
	de frequencias.
**/
int getCharCount(int *freq){
	int i, ct = 0;

	for (i = 0; i < ASCII_MAX; i++) if (freq[i] > 0) ct++;
	return ct;
}

/** >> sortHuffTableSymbol
	Ordena uma HuffTable a partir do
	valor ASCII do simbolo de cada entrada.
**/
void sortHuffTableSymbol(HuffTable *table){
	int i, j;
	HuffUnit key;

	for (i = 1; i < table->size; i++){
		key = table->fields[i];
		j = i-1;

		while (j >= 0 && key.symbol < table->fields[j].symbol){
			table->fields[j+1] = table->fields[j];
			j--;
		}
		
		table->fields[j+1] = key;
	}
}

/** >> huffmanSearchTableSymbol
	Faz uma busca em uma HuffTable a partir
	de um simbolo, retornando o codigo
	relacionado a ele.
**/
char *huffmanSearchTableSymbol(HuffTable *table, char query){
	int i;
	for (i = table->size; i >= 0; i--)
		if (table->fields[i].symbol == query) return table->fields[i].code;
	
	return NULL;
}

/** >> huffmanCompress
	Realiza a compactacao de Huffman.
**/
void huffmanCompress(char *text, HuffTable *table){
	unsigned char aux = 0;
	int textIndex, codeIndex, byteIndex = 0;
	int textSize = strlen(text);
	int codeSize;
	char *currentCode;
	
	printf("-\n");
	for (textIndex = 0; textIndex < textSize; textIndex++){
		// recebe o codigo correspondente ao simbolo
		currentCode = huffmanSearchTableSymbol(table, text[textIndex]);
		
		codeSize = strlen(currentCode);
		for (codeIndex = 0; codeIndex < codeSize; codeIndex++){
			// converte, com bitwise, a string de codigo em valores binarios
			if (currentCode[codeIndex] == '1')
				aux |= (1 << (7 - byteIndex));
			byteIndex++;
			
			// quando um byte binario fica completo, ele e impresso na tela
			if (byteIndex == 8){
				printf("%d\n", aux);
				aux = 0;
				byteIndex = 0;				
			}			
		}
	}
	
	// caso o ultimo byte da compressao esteja incompleto,
	// usa-se um byte auxiliar para indicar quantos bits
	// foram usados nele
	if (byteIndex != 0)
		printf("%d\n%d\n", aux, byteIndex);
}

/** >> freeHuffUnit
	Libera um HuffUnit alocado na memoria Heap.
**/
void freeHuffUnit(HuffUnit u){
	free(u.code);
}

/** >> destroyHuffTable
	Libera uma HuffTable alocada na memoria Heap,
	assim como cada um dos HuffUnit que a compoe.
**/
void destroyHuffTable(HuffTable *t){
	int i;
	for (i = 0; i < t->size; i++) freeHuffUnit(t->fields[i]);
	free(t->fields);
	free(t);
}

/** >> readHuffTable
	Le os dados da compactacao de Huffman
	e os armazena em uma HuffTable.
**/
HuffTable *readHuffTable(FILE *inputFile){
	HuffTable *t = createHuffTable();

	while (!testDelimiter(inputFile, HUFFTABLE_READ_DELIMITER)){
		t->fields = (HuffUnit *) realloc(t->fields, sizeof(HuffUnit)*(t->size + 1));
		fscanf(inputFile, "%c - %ms\n", &t->fields[t->size].symbol, &t->fields[t->size].code);
		t->size++;
	}

	// pegando o delimitador
	fgetc(inputFile);
	fgetc(inputFile);

	return t;
}

/** >> readCompressedText
	Le um texto compactado por Huffman que esta
	contido em um arquivo.
**/
unsigned char *readCompressedText(FILE *inputFile, int *inputSize){
	unsigned char *str = NULL;
	int ct = 0;
	char c;

	do {
		c = fgetc(inputFile);
		if (!feof(inputFile)){	
			str = (unsigned char *) realloc(str, sizeof(unsigned char)*(ct+1));
			str[ct++] = c;
		}
	} while (!feof(inputFile));

	*inputSize = ct;
	return str;
}

/** >> sortHuffTableCode
	Ordena uma HuffTable a partir da quantidade
	de bits do codigo de cada entrada.
**/
void sortHuffTableCode(HuffTable *table){
	int i, j;
	HuffUnit key;

	for (i = 1; i < table->size; i++){
		key = table->fields[i];
		j = i-1;

		while (j >= 0 && strlen(key.code) < strlen(table->fields[j].code)){
			table->fields[j+1] = table->fields[j];
			j--;
		}
		
		table->fields[j+1] = key;
	}
}

/** >> huffmanSearchTableCode
	Faz uma busca em uma HuffTable a partir
	de um codigo, retornando o simbolo
	relacionado a ele.
**/
char huffmanSearchTableCode(HuffTable *table, char *query){
	int i;
	for (i = 0; i < table->size; i++)
		if (strcmp(table->fields[i].code, query) == 0) return table->fields[i].symbol;
	
	return -1;
}

/** >> huffmanUncompress
	Realiza a descompactacao de Huffman.
**/
void huffmanUncompress(unsigned char *text, int byteSize, HuffTable *table){
	int textIndex = 0, codeIndex = 0, bitIndex;
	// calcula a quantidade de bits do texto compactado
	int bitSize = (HUFF_BITS_MAX * (byteSize - 2)) + ((int) text[byteSize - 1]);
	char aux, currentSymbol;
	
	char *currentCode = (char *) calloc(HUFF_BITS_MAX + 1, sizeof(char));
	
	for (bitIndex = 0; bitIndex < bitSize; bitIndex++){
		// le um bit do texto comprimido
		aux = text[textIndex] & (1 << (7 - (bitIndex % 8)));
		
		// monta o codigo relacionado a esse bit
		if (aux == 0) currentCode[codeIndex] = '0';
		else currentCode[codeIndex] = '1';
		codeIndex++;
		
		// procura um correspondente a esse codigo na HuffTable
		currentSymbol = huffmanSearchTableCode(table, currentCode);
		// caso um simbolo seja encontrado
		if (currentSymbol != -1){
			// imprime o simbolo na saida padrao
			printf("%c", currentSymbol);
			// recomeca a montagem do codigo
			cleanString(currentCode, HUFF_BITS_MAX + 1);
			codeIndex = 0;
		}
		
		// pula para o proximo byte do texto comprimido
		// quando todos os bits forem vistos
		if ((bitIndex % 8) == 7) textIndex++;
	}
	
	free(currentCode);
}

//a


