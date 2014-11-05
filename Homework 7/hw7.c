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

char helpPrompt[] = "usage: ./a.out [-s | -a | -h]"
					"\n-s Substitution cipher"
						"\n\tAdditional parameters: [-e | -d] n INPUT_FILE OUTPUT_FILE"
						"\n\t\t-e Encrypt using the substitution cipher."
						"\n\t\t-d Decrypt using the substitution cipher."
						"\n\t\tn The amount of position to shift by."
						"\n\t\tINPUT_FILE This can be any file on the file system or - which specifies stdin."
						"\n\t\tOUTPUT_FILE This can be any file on the system or - which specifies stdout."
					"\n-a Autokey cipher"
						"\n\tAdditional parameters: [-e | -d] n INPUT_FILE KEY_FILE OUTPUT_FILE"
						"\n\t\t-e Encrypt using the autokey cipher"
						"\n\t\t-d Decrypt using the autokey cipher"
						"\n\t\tn The initial shift value"
						"\n\t\tINPUT_FILE This can be any file on the file system or -which specifies stdin."
						"\n\t\tKEY_FILE This can be any file on the file system or -which specifies stdin."
						"\n\t\tOUTPUT_FILE This can be any file on the system or - which specifies stdout.";

/* Function prototypes */
void handleFlags(int argc, char *argv[]);
void printHelp(int failure);
void substitution(int encrypt, int decrypt, int n, char *input, char *output);
void subEncrypt(FILE *input, FILE *output);
void subDecrypt(FILE *input, FILE *output);
FILE* openInput(char *in);
FILE* openOutput(char *out);


int main (int argc, char *argv[]){
	handleFlags(argc, argv);
}

void handleFlags(int argc, char *argv[]){
	/* Counters to count occurances of valid flags */
	int s = 0, a = 0, e = 0, d = 0, h = 0, opt = 0;
	extern int optind;
	/* Additional args needed for encryption/decryption */
	char *n;
	char *input, *output;
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

		/* Encryption */
		if(e){
			/* Ensure both encrypt and decrypt weren't requested */
			if(d)
				exit(EXIT_FAILURE);
			/* Check if there are enough args */
			if (argc != 6)
				exit(EXIT_FAILURE);

			/* Process the args for encryption */
			n = *(argv + optind);
			num = *n - '0';
			input = *(argv + optind + 1);
			output = *(argv + optind + 2);
			substitution(e, d, num, input, output);
		}

		/* Decryption */
		if(d){
			/* Ensure both encrypt and decrypt weren't requested */
			if(e)
				exit(EXIT_FAILURE);
			/* Check if there are enough remaining args */
			if (optind != argc - 3)
				exit(EXIT_FAILURE);

			/* Process the args for encryption */
			n = *(argv + optind);
			num = *n - '0';
			input = *(argv + optind + 1);
			output = *(argv + optind + 2);
			substitution(e, d, num, input, output);
		}

	}

	/* check for Autokey cipher */
	if (a){
		/* Exit success for now */
		exit(EXIT_SUCCESS);
	}
}

void substitution(int e, int d, int n, char *input, char *output){
	/* Streams for input and output files */
	FILE *in, *out;

	/* Attempt to open streams */
	in = openInput(input);
	out = openOutput(output);

	/* Print out prompts for grading */
	cse220("shift ammount: %d", n);
	if (in == stdin)
		cse220("input file: STD_IN");
	else
		cse220("input file: %s", input);
	if (out == stdout)
		cse220("output file: STD_OUT");
	else
		cse220("output file: %s", output);
	cse220("%s", "cipher type: substition cypher");
	if (e)
		cse220("cipher operation: encryption");
	else
		cse220("cipher operation: decryption");

	/* Call encryption or decryption functions based on the choice */
	if(e)
		subEncrypt(in, out);
	if(d)
		subDecrypt(in, out);
}

void subEncrypt(FILE *in, FILE *out){

	
}

void subDecrypt(FILE *in, FILE *out){
	
}

FILE* openInput(char *input){

	/* Check if the arg is a "-" */
	if (*input == '-'){
		debug("%s", "stdin was used");
		return stdin;
	}

	FILE *file = fopen(input, "r");
	if (file == NULL){
		debug("%s", "Failed to open input file");
		exit(EXIT_FAILURE);
	}

	return file;
}

FILE* openOutput(char *output){

	/* Check if the arg is a "-" */
	if (*output == '-'){
		debug("%s", "stdout was used");
		return stdout;
	}

	FILE *file = fopen(output, "w");
	if (file == NULL){
		debug("%s", "Failed to open input file");
		exit(EXIT_FAILURE);
	}

	return file;
}

