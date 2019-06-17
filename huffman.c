#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#define MAX_CAPACITY 2000 /* Max capacity of priority queue used to build a Huffman's tre */
/*========================================*/
/* Following global variables are used for reading/writing bits */
/*========================================*/
FILE* f;
unsigned char bufor, mask;
char end; /* variable end is needed for reading bits in binary file */
/*========================================*/
FILE* codes_file;

char CountChars(char* filename, int* count) {
	FILE* file;

	file = fopen(filename, "r"); /* Read file, name is function's argument */
	if(!file) {
		fprintf(stderr, "Couldn't open file or it doesn't exist: \"%s\"\n", filename);
		return 0;
	} /* If file doesn't exist or we don't have the permission to open it (error management) */

	char c;
	while ((c = fgetc(file))) {
		if (c == EOF)
			break;
		else
			count[(int)(c)] += 1;
	}

	fclose(file);
	return 1;
} /* This function will return 1 if counting the characters was succesfull and 0 otherwise */

void PrintChars(int* count) {
	int i;
	for(i = 0; i < 256; i++) {
		if(count[i] > 0) {
			if(i == '\n')
				printf("char \'\\n\': %d times\n", count[i]);
			else
				printf("char \'%c\': %d times\n", i, count[i]);
		}
	}
} /* Debugging purpose only, it will print all the occurrences of characters from read file*/

/*========================================*/
/* Everything in the brackets is related to Priority Queue */
/*========================================*/
PrioQueue* InitPrioQueue(void) {
	PrioQueue* new_prioqueue;
	new_prioqueue = malloc(sizeof(PrioQueue));
	new_prioqueue->size = 0;
	new_prioqueue->max_capacity = MAX_CAPACITY;
	new_prioqueue->array = malloc(MAX_CAPACITY * sizeof(Node));
	return new_prioqueue;
} /* PriorityQueue init */

void Swap(int i, int j, PrioQueue* prioqueue) {
	Node* temporary;
	temporary = prioqueue->array[i];
	prioqueue->array[i] = prioqueue->array[j];
	prioqueue->array[j] = temporary;
}

void UpHeap(int i, PrioQueue* prioqueue) {
	if(i != 0 && prioqueue->array[i]->priority > prioqueue->array[(i - 1) / 2]->priority) {
		Swap(i, (i - 1) / 2, prioqueue);
		UpHeap((i - 1) / 2, prioqueue);
	}
}

void DownHeap(int i , PrioQueue* prioqueue) {
  int bigger_child = i;
  if(2 * i + 1 < prioqueue->size &&
    prioqueue->array[2 * i + 1]->priority > prioqueue->array[bigger_child]->priority)
    bigger_child = 2 * i + 1;
  if(2 * i + 2 < prioqueue->size &&
     prioqueue->array[2 * i + 2]->priority > prioqueue->array[bigger_child]->priority)
     bigger_child = 2 * i + 2;
  if(bigger_child != i) {
    Swap(i, bigger_child, prioqueue);
    DownHeap(bigger_child, prioqueue);
  }
}
char PrioQueueEmpty(PrioQueue* prioqueue) {
	if(prioqueue->size < 1)
		return 1;
	else
		return 0;
}

char PrioQueueInsertNode(Node* new_node, PrioQueue* prioqueue) {
	if(prioqueue->size == MAX_CAPACITY) {
		printf("Error, queue is full.\n");
		return 0;
	}
	prioqueue->array[prioqueue->size] = new_node;
	prioqueue->size++;
	UpHeap(prioqueue->size -1, prioqueue);
	return 1;
}

Node* PrioQueueRemoveNode(PrioQueue* prioqueue) {
	if(PrioQueueEmpty(prioqueue)) {
		printf("Error, queue is empty, there is nothing to remove.\n");
		return NULL;
	}
	else {
		Node* removed_node = prioqueue->array[0];
		Swap(0, prioqueue->size - 1, prioqueue);
		prioqueue->size--;
		DownHeap(0, prioqueue);
		return removed_node;
	}
} /* Removes node from PriorityQueue and also returns pointer to that node */

/*========================================*/
/* End of Priority Queue functions */
/*========================================*/

Node* CreateNode(int count) {
	Node* newnode;
	newnode = malloc(sizeof(Node));
	newnode->count = count;
	newnode->priority = (-1) * count;
	newnode->character = 0;
	newnode->left = NULL;
	newnode->right = NULL;
	return newnode;
} /* CreateNode does all the assigning to create a node, all it needs is number of occurances  -- a vertex of Huffman's tree */

char CheckCount(int* count, Node* HuffmanTreeRoot) {
	int i, all;
	all = 0;
	for(i = 0; i < 256; i++)
		all = count[i] + all;
	if(all == HuffmanTreeRoot->count)
		return 1;
	else
		return 0;
} /* Debugging purpose only, checks if count of characters in root of Huffman's tree is equal to count of characters returned by function Count Chars */

Node* CreateHuffmanTree(int* count) {
	int i;
	PrioQueue* encode_prioqueue;
	Node *leaf0, *leaf1, *leaf2, *node;

	encode_prioqueue = InitPrioQueue();

	Node *my_EOF;
	my_EOF = CreateNode(count[256]);
	my_EOF->character = 256;
	PrioQueueInsertNode(my_EOF, encode_prioqueue);
	/* the 256th place in our array is reserved for EOF */

	/* Step 1 of algorithm */
	for(i = 0; i < 256; i++)
		if(count[i] > 0) {
			leaf0 = CreateNode(count[i]);
			leaf0->character = i;
			PrioQueueInsertNode(leaf0, encode_prioqueue);
		}
	/* End of step 1 of algorithm */
	/* Step 2 of algorithm */
	while(encode_prioqueue->size > 1) {
		leaf1 = PrioQueueRemoveNode(encode_prioqueue);
		leaf2 = PrioQueueRemoveNode(encode_prioqueue);
		node = CreateNode(leaf1->count + leaf2->count);
		node->left = leaf2;
		node->right = leaf1;
		PrioQueueInsertNode(node, encode_prioqueue);
	}
	/* End of step 2 */
	/* Step 3 of algorithm */
	Node* HuffmanTreeRoot;
	HuffmanTreeRoot = PrioQueueRemoveNode(encode_prioqueue);
	/* End of step 3 - now HuffmanTreeRoot is a root to our huffman tree*/
	/*if(CheckCount(count, HuffmanTreeRoot))
		printf("Everything's OK.\n");
	else
		printf("Something's wrong...\n");*/ /* Debugging purpose only! Checks if created Huffman's tree is valid */
	return HuffmanTreeRoot;
} /* HuffmanEncode takes a file and creates a Huffman's Tree for characters in the given file, also returning pointer to root of that tree */

void FindBinaryCodes(Node* vertex, int level, unsigned int binary_code, Code* binary_codes) {
	if(vertex->left == NULL) {
		binary_codes[vertex->character].code = binary_code;
		binary_codes[vertex->character].lenght = level;
	}
	else {
		binary_code &= ~(1 << level);
		FindBinaryCodes(vertex->left, level + 1, binary_code, binary_codes);
		binary_code |= 1 << level;
		FindBinaryCodes(vertex->right, level + 1, binary_code, binary_codes);
	}
} /* FindBinaryCodes searches through entire Huffman's tree recursivly starting by it's root given as an argument. It fills given array of type Code (that has place for each of 256 characters in it) with Huffman's codes for each character as well as the lenght of the code (lengt is needed since codes in this method ar written backwards and when outputted to a file need to be written in a backward direction -- we need lenght of a code for that). First induction of this function should be with level == 0, as this level is a level of a binary tree and starting with a root level is 0 */

/*========================================*/
/* Functions below deal with writing each bits */
/*========================================*/

void BeginWriting(char* filename) {
	f = fopen(filename, "wb+");
	mask = 1;
	bufor = 0; /* Set all 8 bits to 0 */
} /* BeginWriting */

void WriteBit(char bit) {
	if(bit)
		bufor += mask;
	mask *= 2;
	if(!mask) {
		/* mask == 0 if there  is bits surplus -- after writing 8 bits (1 byte) */
		//fprintf(f, "%d", bit); /* Debugging purpose only, it will print int represetning bit (1 or 0) instead of actual bit to binary file */
		fwrite(&bufor, 1, 1, f);
		bufor = 0;
		mask = 1;
	}
} /* WriteBit */

void FinishWriting(void) {
	if(mask != 1)
		/* fprintf(f, "%d", 1); */ /* Debugging purpose only */
		fwrite(&bufor, 1, 1, f);
	fclose(f);
} /* FinishWriting */

void BeginReading(char* filename) {
	f = fopen(filename, "r+");
	if(!f) {
		fprintf(stderr, "Couldn't open file or it doesn't exist: \"%s\"\n", filename);
		exit(10);
	}
	mask = end = 0;
} /* BeginReading */

char ReadBit(void) {
	char bit;
	if(!mask) {
		if(!fread(&bufor, 1, 1, f))
			end = 1;
		else
			mask = 1;
	}
	bit = bufor & 1;
	bufor /= 2;
	mask *= 2;
	return bit;
} /* ReadBit */

void FinishReading(void) {
	fclose(f);
} /* FinishReading */

/*========================================*/
/* End of functions concnerning bits */
/*========================================*/

void Encode(char* to_encode_filename, char* encoded_filename) {
	int count[257] = {0}; /* array of occourance of each character, it has 256 spaces since there are 256 characters (257 spaces since last one is reserved for EOF -- not a character */
	Code codes[257]; /* array of binary codes for each character */
	Node* huffman_root; /* pointer to the root of Huffman's tree */
	int i, j;
	char read_char;
	FILE* to_encode;
	CountChars(to_encode_filename, count);
	count[256] = 1; /* 1 occurence of EOF */
	huffman_root = CreateHuffmanTree(count);
	FindBinaryCodes(huffman_root, 0,(unsigned int)NULL, codes);

	to_encode = fopen(to_encode_filename, "r");
	BeginWriting(encoded_filename);

	codes_file = fopen("codes", "wb+");
	for(j = 0; j < 257; j++) {
		if(count[j] != 0) {
			//printf("wpisuje kod chara :%c\n", j);
			if(j == 256)
				fprintf(codes_file, "char: \'EOF\' code: \'");
			else
				fprintf(codes_file, " char : \'%c\' code: \'", j);
			for(i = 0; i < codes[j].lenght; i++) {
				fprintf(codes_file, "%d", (codes[j].code & 1 << i) != 0);
			}
			fprintf(codes_file, "\'\n");
		}
	}
	fclose(codes_file);
	/* This part is responsible for creating file "codes" that cointains huffman codes for each character in an input file */


//	PrintChars(count); /*Debugging purpose only */
	for(i = 0; i < 256; i++)
		//fprintf(codes_file, "%d ", count[i]); /* Debugging purpose only, will write integers to file instead of bytes represetning integers */
		fwrite((const void*) & count[i], sizeof(int), 1, f);
	/* This loop handles writing wrting occurances of each character from source file to compressed file it takes 4 bytes * 256 space in the beggining of compressed file (4 bytes since int's size is 4 bytes) */
	while((read_char = fgetc(to_encode))) {
		if(read_char == EOF) {
			for(i = 0; i < codes[256].lenght; i++)
//				fprintf(f, "%d", (codes[256].code & 1 << i) != 0);
				WriteBit((codes[256].code & 1 << i) != 0);
			break;
		}
		else {
			for(i = 0; i < codes[(int)read_char].lenght; i++)
//				fprintf(f, "%d", (codes[(int)read_char].code & 1 << i) != 0); /* Debugging purpose only, will display ASCII char 1 or 0 representing the bit instead of actual bit */
				WriteBit((codes[(int)read_char].code & 1 << i) != 0);

		}
		/* This loop handles writing single bits of each character to binary file, this is where we needed our code's lenght. Since we need to write them down backwards */
	}
	FinishWriting();
	fclose(to_encode);
	exit(0);
} /* Encode takes a filename and makes a compressed version of that file called "encoded" */

void Decode(char* to_decode_filename, char* decoded_filename) {
	int count[257] = {0}; /* similarly to Encode */
	Node *huffman_root, *helper; /* similarly to Encode helper is here just for a while loop since after we find leaf of Huffman's tree we need to go back to the root and repeat the process */
	int i;
	char read_bit;
	FILE* decoded;

	BeginReading(to_decode_filename);
	for(i = 0; i < 256; i++) {
		if(fread(&count[i], sizeof(int), 1, f) != 1)
        fprintf(stderr, "error writing to binary file\n");
/*		else {
			if(count[i] != 0) {
				if(i == '\n')
					printf("char \'\\n\': %d times\n", count[i]);
				else
					printf("char \'%c\': %d times\n", i, count[i]);
			}
		}*/ /* Debugging purpose only, prints the "formula" for Huffman's tree read from encoded binary file, the "formula" is list of occurrences of each character */
	} /* This loop reads first 1024 bytes represetning 256 integers that represent occurrence of each of 256 characters */
	count[256] = 1; /*1 occurence of EOF */
	huffman_root = CreateHuffmanTree(count);
	helper = huffman_root;
	decoded = fopen(decoded_filename, "wb+");
	while(1) {
		read_bit = ReadBit();
		if(helper->left == NULL) {
			if(helper->character == 256)
				break; /* if character is == 256 that means it's EOF */
			fputc(helper->character, decoded);
			printf("%c", helper->character); /* Debugging purpose only */
			helper = huffman_root;
		}
		else if(helper->left != NULL) {
			if(read_bit == 1)
				helper = helper->right;
			else if(read_bit == 0)
				helper = helper->left;
		}
	}
	/* This while loop reads encoded file bit by bit and if bit is a 0 we move left on Huffman's tree and if it's 1 we move right until a node is missing left(right, any will work since this is Huffman's tree), then it means we are in a leaf and we output character to decoded file and reset helper to point a the root of the Huffman's tree again and we do that until we encounter end of binary file */
	printf("\n");
	fclose(decoded);
	FinishReading();
	exit(0);
	/* Closing the files and exiting program after finished work */
} /* Decode decodes compressed file with "to_decode_filename" name and outputs decoded characters to file "decoded_filename" */

int main(int argc, char** argv) {
	if(argc != 4) {
		exit(1);
	}
	else if(strcmp("-c", argv[1]) == 0)
		Encode(argv[2], argv[3]);
	else if(strcmp("-d", argv[1]) == 0)
		Decode(argv[2], argv[3]);
	else
		exit(2);
}
