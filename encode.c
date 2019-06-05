#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
