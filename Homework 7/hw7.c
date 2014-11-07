/*
Homework #7
Name: Andrew Rosiclair
SBU ID: 109235970
*/

#include <stdlib.h>
#include "hw7.h"
#include <unistd.h>	/* Used for getopt() function*/
#include <string.h>	/* Used to compare strings */
#include <stddef.h> /* Used for NULL macro */
#include <stdio.h> /* Used for opening files */

/* Debugging macros */
#ifdef DEBUG
	#define debug(fmt, ...) printf("DEBUG: %s:%s:%d " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define debug(fmt, ...)
#endif

/* Macros required for grading */
#ifdef CSE220
	#define cse220(fmt, ...) printf(fmt, ##__VA_ARGS__)
/* Define assignment macros here */
#else
	#define cse220(fmt, ...)
/* Define assignment macros here */
#endif

/* Function prototypes */
void handleFlags(int argc, char *argv[]);
void printHelp(int failure);
void substitution(int encrypt, int decrypt, int n, char *input, char *output);
void subEncrypt(FILE *in, FILE *out, int n);
void subDecrypt(FILE *in, FILE *out, int n);
void autoKey(int encrypt, int decrypt, int n, char *input, char *output, char *key);
void autoEncrypt(FILE *in, FILE *out, FILE *key);
void autoDecrypt(FILE *in, FILE *out, FILE *key);
void createTabula(int n);
void printTabula();
FILE* openInput(char *in);
FILE* openOutput(char *out);
void removeNewline();
void toUppercase(char *stringPtr);
int validCharLength(char *stringPtr);
void padKey(char *reference);


int main (int argc, char *argv[]){
	handleFlags(argc, argv);

	exit(EXIT_FAILURE);
}

void handleFlags(int argc, char *argv[]){
	/* Counters to count occurances of valid flags */
	int s = 0, a = 0, e = 0, d = 0, h = 0, opt = 0;
	extern int optind;
	/* Additional args needed for encryption/decryption */
	char *n;
	char *input, *output, *key;
	int num;
	/* No flags or args special case */
	if(argc <= 1)
		printHelp(1);

	/* Use getopt to parse each of the given flags & args and count them */
	while ((opt = getopt(argc, argv, "saedh")) != -1){
		switch(opt){
			case 's':
				s++;
				break;
			case 'a':
				a++;
				break;
			case 'e':
				e++;
				break;
			case 'd':
				d++;
				break;
			case 'h':
				h++;
				break;
			default: /* If an invalid flag is used */
				exit(EXIT_FAILURE);
		}
	}

	/* if help flag was used, only print the help prompt */
	if(h)
		printHelp(0);

	/* Check for substitution cipher */
	if (s){
		/* Ensure multiple encryptions weren't requested */
		if (a)
			exit(EXIT_FAILURE);
		/* Ensure both encrypt and decrypt weren't requested */
		if(e && d)
			exit(EXIT_FAILURE);				
		/* Check if there are enough remaining args */
		if (optind != argc - 3)
			exit(EXIT_FAILURE);

		/* Process the args for substitution */
		n = *(argv + optind);
		num = *n - '0';
		num %= ALPHABET_SIZE;
		input = *(argv + optind + 1);
		output = *(argv + optind + 2);
		substitution(e, d, num, input, output);
	}

	/* check for Autokey cipher */
	if (a){
		/* Ensure multiple encryptions weren't requested */
		if (s)
			exit(EXIT_FAILURE);
		/* Ensure both encrypt and decrypt weren't requested */
		if(e && d)
			exit(EXIT_FAILURE);
		/* Check if there are enough remaining args */
		if (optind != argc - 4)
			exit(EXIT_FAILURE);

		/* Process the args for Autokey encoding */
		n = *(argv + optind);
		num = *n - '0';
		num %= ALPHABET_SIZE;
		input = *(argv + optind + 1);
		key = *(argv + optind + 2);
		output = *(argv + optind + 3);
		autoKey(e, d, num, input, output, key);
	}
}

void printHelp(int failure){

	if(failure){
		printf("usage: ./a.out [-s | -a | -h]");
		printf("\n\t-s Substitution cipher");
		printf("\n\t\tAdditional parameters: [-e | -d] n INPUT_FILE OUTPUT_FILE");
		printf("\n\t\t\t-e Encrypt using the substitution cipher.");
		printf("\n\t\t\t-d Decrypt using the substitution cipher");
		printf("\n\t\t\tn The amount of position to shift by.");
		printf("\n\t\t\tINPUT_FILE This can be any file on the file system or - which specifies stdin.");
		printf("\n\t\t\tOUTPUT_FILE This can be any file on the system or - which specifies stdout.");
		printf("\n\t-a Autokey cipher");
		printf("\n\t\tAdditional parameters: [-e | -d] n INPUT_FILE KEY_FILE OUTPUT_FILE");
		printf("\n\t\t\t-e Encrypt using the autokey cipher");
		printf("\n\t\t\t-d Decrypt using the autokey cipher");
		printf("\n\t\t\tn The initial shift value");
		printf("\n\t\t\tINPUT_FILE This can be any file on the file system or -which specifies stdin.");
		printf("\n\t\t\tKEY_FILE This can be any file on the file system or -which specifies stdin.");
		printf("\n\t\t\tOUTPUT_FILE This can be any file on the system or - which specifies stdout.\n");
		exit(EXIT_FAILURE);
	}

	printf("usage: ./a.out [-s | -a | -h]");
	printf("\n\t-s Substitution cipher");
	printf("\n\t\tAdditional parameters: [-e | -d] n INPUT_FILEOUTPUT_FILE");
	printf("\n\t\t\t-e Encrypt using the substitution cipher.");
	printf("\n\t\t\t-d Decrypt using the substitution cipher");
	printf("\n\t\t\tn The amount of position to shift by.");
	printf("\n\t\t\tINPUT_FILE This can be any file on the file system or - which specifies stdin.");
	printf("\n\t\t\tOUTPUT_FILE This can be any file on the system or - which specifies stdout.");
	printf("\n\t-a Autokey cipher");
	printf("\n\t\tAdditional parameters: [-e | -d] n INPUT_FILE KEY_FILE OUTPUT_FILE");
	printf("\n\t\t\t-e Encrypt using the autokey cipher");
	printf("\n\t\t\t-d Decrypt using the autokey cipher");
	printf("\n\t\t\tn The initial shift value");
	printf("\n\t\t\tINPUT_FILE This can be any file on the file system or -which specifies stdin.");
	printf("\n\t\t\tKEY_FILE This can be any file on the file system or -which specifies stdin.");
	printf("\n\t\t\tOUTPUT_FILE This can be any file on the system or - which specifies stdout.\n");
	exit(EXIT_SUCCESS);
}

void substitution(int e, int d, int n, char *input, char *output){
	/* Streams for input and output files */
	FILE *in, *out;

	/* Attempt to open streams */
	in = openInput(input);
	out = openOutput(output);

	/* Print out prompts for grading */
	cse220("shift amount: %d\n", n);
	if (in == stdin)
		cse220("input file: STD_IN\n");
	else
		cse220("input file: %s\n", input);
	if (out == stdout)
		cse220("output file: STD_OUT\n");
	else
		cse220("output file: %s\n", output);
	cse220("%s", "cipher type: substition cypher\n");
	if (e)
		cse220("cipher operation: encryption\n");
	else
		cse220("cipher operation: decryption\n");

	/* Call encryption or decryption functions based on the choice */
	if(e)
		subEncrypt(in, out, n);
	if(d)
		subDecrypt(in, out, n);
}

void subEncrypt(FILE *in, FILE *out, int n){
	/* Set up pointers to our premade arrays */
	extern char plaintext[], ciphertext[];
	char *plainPtr = plaintext, *cipherPtr = ciphertext, c;

	int i = 0, rollover;

	/* Load our plaintext array with the input */
	fgets(plainPtr, BUFFER_SIZE, in);

	/* Remove any possible new line chars*/
	removeNewline();
	/* Convert all characters to uppercase */
	toUppercase(plainPtr);

	/* Fill the ciphertext array with converted chars */
	for(i = 0; i < strlen(plaintext); i++){
		c = *(plainPtr + i);
		/* Only convert the character if it's within range */
		if (c >= 'A' && c <= ('A' + ALPHABET_SIZE - 1)){
			c += n;
			/* if c is out of range, rollover to the beginning of the alphabet */
			if (c > 'A' + ALPHABET_SIZE - 1){
				rollover = c % ('A' + ALPHABET_SIZE);
				c = 'A' + rollover;
			}
			/* Insert the converted char into the cipher array */
			*(cipherPtr + i) = c;
		}else
			*(cipherPtr + i) = c;
	}

	/* Write the converted string to the output stream */
	fputs(cipherPtr, out);

	exit(EXIT_SUCCESS);
	
}

void subDecrypt(FILE *in, FILE *out, int n){
	/* Set up pointers to our premade arrays */
	extern char plaintext[], ciphertext[];
	char *plainPtr = plaintext, *cipherPtr = ciphertext, c;

	int i = 0, rollover;

	/* Load our cipher array with the input */
	fgets(cipherPtr, BUFFER_SIZE, in);

	/* Remove any possible new line chars*/
	removeNewline();
	/* Convert all characters to uppercase */
	toUppercase(cipherPtr);

	/* Fill the plaintext array with decrypted chars */
	for(i = 0; i < strlen(ciphertext); i++){
		c = *(cipherPtr + i);
		/* Only convert the character if it's within range */
		if (c >= 'A' && c <= ('A' + ALPHABET_SIZE - 1)){
			c -= n;
			/* if c is out of range, rollover to the beginning of the alphabet */
			if (c < 'A'){
				rollover = 'A' % c;
				c = 'A' + ALPHABET_SIZE - rollover;
			}
			/* Insert the decrypted char into the plaintext array */
			*(plainPtr + i) = c;
		}else
			*(plainPtr + i) = c;
	}

	/* Write the decrypted string to the output stream */
	fputs(plainPtr, out);

	exit(EXIT_SUCCESS);
}

void autoKey(int encrypt, int decrypt, int n, char *input, char *output, char *key){
	/* Streams for input and output files */
	FILE *in, *out, *keyStream;

	/* Attempt to open streams */
	in = openInput(input);
	out = openOutput(output);
	keyStream = openInput(key);

	/* Print out prompts for grading */
	cse220("shift amount: %d\n", n);
	if (in == stdin)
		cse220("input file: STD_IN\n");
	else
		cse220("input file: %s\n", input);
	if (out == stdout)
		cse220("output file: STD_OUT\n");
	else
		cse220("output file: %s\n", output);
	cse220("%s", "cipher type: Autokey cypher\n");
	if (encrypt)
		cse220("cipher operation: encryption\n");
	else
		cse220("cipher operation: decryption\n");

	/* Create the tabula recta */
	createTabula(n);
	#ifdef CSE220
		printTabula();
	#endif

	if(encrypt)
		autoEncrypt(in, out, keyStream);
	else
		autoDecrypt(in, out, keyStream);

}

void autoEncrypt(FILE *in, FILE *out, FILE *keyStream){
	/* Set up pointers to our premade arrays */
	extern char plaintext[], ciphertext[], key[], tabula[ALPHABET_SIZE][ALPHABET_SIZE];
	char *plainPtr = plaintext, *cipherPtr = ciphertext, *keyPtr = key, (*tabulaPtr)[ALPHABET_SIZE] = tabula, c;

	int row, col, plainIndex = 0, keyIndex = 0;

	/* Load input arrays */
	if (in == stdin)
		printf("Enter the text to be encrypted:\n");
	fgets(plainPtr, BUFFER_SIZE, in);
	if (keyStream == stdin)
		printf("Enter the key to use for encryption");
	fgets(keyPtr, BUFFER_SIZE, keyStream);

	/* Remove any new line chars */
	removeNewline();

	/* Ensure chars are uppercase */
	toUppercase(plainPtr);
	toUppercase(keyPtr);

	/* Pad our key if necessary */
	padKey(plainPtr);

	/* Fill the ciphertext array with encrypted chars */
	while((c = *(plainPtr + plainIndex)) != '\0'){
		/* Only convert the character if it's within range */
		if (c >= 'A' && c <= ('A' + ALPHABET_SIZE - 1)){
			col = c - 'A';

			/* skip invalid chars in key*/
			while(*(keyPtr + keyIndex) < 'A' || *(keyPtr + keyIndex) > 'A' + ALPHABET_SIZE - 1)
				keyIndex++;
			row = *(keyPtr + keyIndex) - 'A';
			keyIndex++;
			
			*(cipherPtr + plainIndex) = *(*(tabulaPtr + row) + col);
		}else
			*(cipherPtr + plainIndex) = c;
		plainIndex++;
	}

	debug("%s\n", plainPtr);
	debug("%s\n", keyPtr);
	debug("%s\n", cipherPtr);
	/* Write the converted string to the output stream */
	fputs(cipherPtr, out);

	exit(EXIT_SUCCESS);
}

void autoDecrypt(FILE *in, FILE *out, FILE *keyStream){
	/* Set up pointers to our premade arrays */
	extern char plaintext[], ciphertext[], key[], tabula[ALPHABET_SIZE][ALPHABET_SIZE];
	char *plainPtr = plaintext, *cipherPtr = ciphertext, *keyPtr = key, (*tabulaPtr)[ALPHABET_SIZE] = tabula, c;
	int cipherIndex = 0, keyIndex = 0, row, i = 0;

	/* Load input arrays */
	fgets(cipherPtr, BUFFER_SIZE, in);
	fgets(keyPtr, BUFFER_SIZE, keyStream);

	/* Remove any new line chars */
	removeNewline();

	/* Ensure chars are uppercase */
	toUppercase(cipherPtr);
	toUppercase(keyPtr);

	/* Fill the plaintext array with decrypted chars */
	while((c = *(cipherPtr + cipherIndex)) != '\0'){
		/* Only decrypt char if it's in range */
		if(c >= 'A' && c <= 'A' + ALPHABET_SIZE - 1){
			/*Check if we need to pad the key */
			if(keyIndex >= strlen(key)){
				padKey(cipherPtr);
			}
			/* skip invalid chars in key*/
			while(*(keyPtr + keyIndex) < 'A' || *(keyPtr + keyIndex) > 'A' + ALPHABET_SIZE - 1)
				keyIndex++;
			row = *(keyPtr + keyIndex) - 'A';
			keyIndex++;

			/* Search the row for the ciphertext char */
			i = 0;
			while(*(*(tabulaPtr + row) + i) != c)
				i++;

			/* i has the index of the col of the decrypted char */
			*(plainPtr + cipherIndex) = 'A' + i;
		}else
			*(plainPtr + cipherIndex) = c;

		cipherIndex++;
	}

	/* Print the resultant plaintext string to the desired output */
	fputs(plainPtr, out);
	exit(EXIT_SUCCESS);
}

void createTabula(int n){
	extern char tabula[ALPHABET_SIZE][ALPHABET_SIZE];
	char (*tabulaPtr)[ALPHABET_SIZE] = tabula, c, *index;
	int i, j, rollover;

	/* Fill each row with the alphabet first */
	for (i = 0; i < ALPHABET_SIZE; i++){
		c = 'A';
		for(j = 0; j < ALPHABET_SIZE; j++){
			*(*(tabulaPtr + i) + j) = c + j;
		}
	}

	n--;	/* Decrement so n remains the same on first loop */
	/* Shift each row to the left a varying number of times */
	for(i = 0; i < ALPHABET_SIZE; i++){
		n++;
		n %= ALPHABET_SIZE;
		/* shift each character in the current row */
		for(j = 0; j < ALPHABET_SIZE; j++){
			index = (*(tabulaPtr + i) + j);
			*index += n;
			if(*index > 'A' + ALPHABET_SIZE - 1){
				rollover = *index % ('A' + ALPHABET_SIZE);
				*index = 'A' + rollover;
			}
		}
	}
}

void printTabula(){
	extern char tabula[ALPHABET_SIZE][ALPHABET_SIZE];
	char (*tabulaPtr)[ALPHABET_SIZE] = tabula, c;
	int i, j;

	printf("\nTABULA RECTA MATRIX");
	for (i = 0; i < ALPHABET_SIZE; i++){
		printf("\n");
		for(j = 0; j < ALPHABET_SIZE; j++){
			c = *(*(tabulaPtr + i) + j);
			printf("%c ", c);
		}
	}
	printf("\n");
}

FILE* openInput(char *input){

	/* Check if the arg is a "-" */
	if (*input == '-'){
		debug("%s\n", "stdin was used");
		return stdin;
	}

	FILE *file = fopen(input, "r");
	if (file == NULL){
		debug("%s\n", "Failed to open input file");
		exit(EXIT_FAILURE);
	}

	return file;
}

FILE* openOutput(char *output){

	/* Check if the arg is a "-" */
	if (*output == '-'){
		debug("%s\n", "stdout was used");
		return stdout;
	}

	FILE *file = fopen(output, "w");
	if (file == NULL){
		debug("%s\n", "Failed to open input file");
		exit(EXIT_FAILURE);
	}

	return file;
}

void removeNewline(){
	extern char plaintext[], ciphertext[], key[];
	char *plainPtr = plaintext, *cipherPtr = ciphertext, *keyPtr = key;

	/*remove newline chars if they are present */
	if (*(plainPtr + strlen(plaintext) - 1) == '\n')
		*(plainPtr + strlen(plaintext) - 1) = '\0';
	if (*(keyPtr + strlen(key) - 1) == '\n')
		*(keyPtr + strlen(key) - 1) = '\0';
	if (*(cipherPtr + strlen(ciphertext) - 1) == '\n')
		*(cipherPtr + strlen(ciphertext) - 1) = '\0';
}

void toUppercase(char *stringPtr){
	char c;
	int i = 0;

	/* Convert all characters to uppercase */
	while((c = *(stringPtr + i)) != '\0'){
		/* Convert if char c is a lowercase character */
		if (c >= 'a' && c <= 'z'){
			c -= 32;
			*(stringPtr + i) = c;
		}

		i++;
	}
}

int validCharLength(char *stringPtr){
	int i = 0, length = 0;
	char c;

	/* Only count valid chars */
	while((c = *(stringPtr + i)) != '\0'){
		if(c >= 'A' && c <= 'A' + ALPHABET_SIZE - 1)
			length++;
		i++;
	}

	return length;
}

void padKey(char *reference){
	extern char key[], plaintext[];
	char *keyPtr = key, *plainPtr = plaintext;
	int i = 0, j = 0, refLength, keyLength, end;

	/* get the valid char lengths */
	refLength = validCharLength(reference);
	keyLength = validCharLength(keyPtr);
	end = strlen(key);

	/* Pad chars from plaintext until the valid lengths match */
	while(keyLength < refLength){
		/* Only pad valid chars */
		while(*(plainPtr + i) < 'A' || *(plainPtr + i) > 'A' + ALPHABET_SIZE - 1)
			i++;
		/* pad a plaintext char at the end */
		*(keyPtr + end + j) = *(plainPtr + i);
		keyLength++;
		i++;
		j++;
	}
}

