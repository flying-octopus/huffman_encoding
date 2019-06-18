typedef struct Node {
	int priority;
	int count;
	int character;
	struct Node *left, *right;
} Node;

typedef struct {
	int size, max_capacity;
	Node** array;
} PrioQueue;

typedef struct {
	char length;
	unsigned int code;
} Code;

char CountChars(char* filename, int* count);
void PrintChars(int* count);
PrioQueue* InitPrioQueue(void);
void Swap(int i, int j, PrioQueue* prioqueue);
void UpHeap(int i, PrioQueue* prioqueue);
void DownHeap(int i , PrioQueue* prioqueue);
char PrioQueueEmpty(PrioQueue* prioqueue);
char PrioQueueInsertNode(Node* new_node, PrioQueue* prioqueue);
Node* PrioQueueRemoveNode(PrioQueue* prioqueue);
Node* CreateNode(int count);
char CheckCount(int* count, Node* HuffmanTreeRoot);
Node* CreateHuffmanTree(int* count);
void FindBinaryCodes(Node* vertex, int level, unsigned int binary_code, Code* binary_codes);
void BeginWriting(char* filename);
void WriteBit(char bit);
void FinishWriting(void);
void BeginReading(char* filename);
char ReadBit(void);
void FinishReading(void);
void Encode(char* to_encode_filename, char* encoded_filename);
void Decode(char* to_decode_filename, char* decoded_filename);
