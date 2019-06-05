#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#define MAX_CAPACITY 2000

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
	if(prioqueue->size = MAX_CAPACITY) {
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

void HuffmanEncode(char* filename, int* count) {
	int i;
	PrioQueue* encode_prioqueue;
	Node *leaf1, *leaf2, *node;
	CountChars(filename, count);
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
	/* End of step 3*/
}

int main(int argc, char** argv) {
	int count[256] = {0};
	if(argc != 2) {
		printf("Podaj nazwe pliku.\n");
		exit(1);
	}
	else {
		CountChars(argv[1], count);
		PrintChars(count);
	}
}
