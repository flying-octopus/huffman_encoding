char CountChars(char* filename, int* count);
void PrintChars(int* count);

typedef struct Node {
	int priority;
	int count;
	char character;
	Node *left, *right;
} Node;

typedef struct {
	int size, max_capacity;
	Node** array;
} PrioQueue;
