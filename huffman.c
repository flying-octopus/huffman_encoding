#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#define MAX_CAPACITY 2000 /* Max capacity of priority queue used to build a Huffman's tre */
/*========================================*/
FILE* f;
unsigned char bufor, mask;
/*========================================*/

char CountChars(char* filename, int* count) {
	FILE* file;

	file = fopen(filename, "r"); /* Read file, name is function's argument */
	if(!file) {
		fprintf(stderr, "Nie udało się otworzyć pliku: \"%s\"\n", filename);
		return 0;
	} /* If file doesn't exist or we don't have the permission to open it (error management) */

	char c;
	while ((c = fgetc(file))) {
		if (c == EOF)
			break;
		else
			count[c] += 1;
	}

	fclose(file);
	return 1;
} /* This function will return 1 if counting the characters was succesfull and 0 otherwise */

void PrintChars(int* count) {
	int i;
	for(i = 0; i < 256; i++) {
		if(count[i] > 0) {
			if(i == '\n')
				printf("char \'\\n\': %d razy\n", count[i]);
			else
				printf("char \'%c\': %d razy\n", i, count[i]);
		}
	}
} /* Debugging purpose only */

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
		printf("Błąd, kolejka jest pełna.\n");
		return 0;
	}
	prioqueue->array[prioqueue->size] = new_node;
	prioqueue->size++;
	UpHeap(prioqueue->size -1, prioqueue);
}

Node* PrioQueueRemoveNode(PrioQueue* prioqueue) {
	if(PrioQueueEmpty(prioqueue)) {
		printf("Błąd, kolejka jest pusta nie ma co usuwać.\n");
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
	newnode->character = NULL;
	newnode->left = NULL;
	newnode->right = NULL;
	return newnode;
}

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
	Node *leaf1, *leaf2, *node;
	encode_prioqueue = InitPrioQueue();
	/* Step 1 of algorithm */
	for(i = 0; i < 256; i++)
		if(count[i] > 0) {
			leaf1 = CreateNode(count[i]);
			leaf1->character = i;
			PrioQueueInsertNode(leaf1, encode_prioqueue);
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
	/*if(CheckCount(count, HuffmanTreeRoot) == 1)
		printf("Wszystko OK.\n");
	else
		printf("Coś jest nie tak.\n"); */ /* Debugging purpose only! */
	return HuffmanTreeRoot;
} /* HuffmanEncode takes a file and creates a Huffman's Tree for characters in the given file, also returning pointer to root of that tree */

void FindBinaryCodes(Node* root, int level, unsigned int binary_code, Code* binary_codes) {
	if(root->left == NULL) {
		binary_codes[root->character].code = binary_code;
		binary_codes[root->character].lenght = level;
	}
	else {
		binary_code &= ~(1 << level);
		FindBinaryCodes(root->left, level + 1, binary_code, binary_codes);
		binary_code |= 1 << level;
		FindBinaryCodes(root->right, level + 1, binary_code, binary_codes);
	}
} /* FindBinaryCodes searches through entire Huffman's tree recursivly starting by it's root given as an argument. It fills given array of type Code (that has place for each of 256 characters in it) with Huffman's codes for each character as well as the lenght of the code (lengts is needed since codes in this method ar written backwards and when outputted to a file need to be written in a backward direction -- we need lenght of a code for that). First induction of this function should be with level == 0, as this level is a level of a binary tree and starting with a root level is 0 */

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
		/* fprintf(f, "%d", bit); */
		fwrite(&bufor, 1, 1, f);
		bufor = 0;
		mask = 1;
	}
} /* WriteBit */

void FinishWriting(void) {
	if(mask != 1)
		/* fprintf(f, "%d", 1); */
		fwrite(&bufor, 1, 1, f);
	fclose(f);
} /* FinishWriting */
/*========================================*/
/* End of functions concnerning bits */
/*========================================*/

void Encode(char* to_encode_filename, char* encoded_filename) {
	int count[256] = {0}; /* array of occourance of each character, it has 256 spaces since there are 256 characters */
	Code codes[256]; /* array of binary codes for each character */
	Node* huffman_root; /* pointer to the root of Huffman's tree */
	int i;
	char read_char;
	FILE* to_encode;
	CountChars(to_encode_filename, count);
	huffman_root = CreateHuffmanTree(count);
	FindBinaryCodes(huffman_root, 0, NULL, codes);

	to_encode = fopen(to_encode_filename, "r");
	BeginWriting(encoded_filename);
	for(i = 0; i < 256; i++)
		/* fprintf(f, "%d\n", count[i]); */
		fwrite((const void*) & count[i], sizeof(int), 1, f);
	/* This loop handles writing wrting occurances of each character from source file to compressed file it takes 4 bytes * 256 space in the beggining of compressed file (4 bytes since int's size is 4 bytes) */
	while((read_char = fgetc(to_encode))) {
		if(read_char == EOF)
			break;
		else
			for(i = 0; i < codes[read_char].lenght; i++)
				WriteBit((codes[read_char].code & 1 << i) != 0);
		/* This for loop is where we needed our code's lenght. Since we need to write them down backwards */
	}
	FinishWriting();
	fclose(to_encode);
} /* Encode takes a filename and makes a compressed version of that file called "encoded" */

void Decode(char* to_decode_filename, char* decoded_filename) {
	printf("lol\n");
}

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
