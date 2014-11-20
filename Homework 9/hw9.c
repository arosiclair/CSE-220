/*
Homework #9
Name: Andrew Rosiclair
SBU ID: 109235970
*/

#include "hw9.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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

/* Shorthand types */
typedef struct InstrType INSTRTYPE;
typedef struct Instr INSTRUCTION;


char *helpPrompt = "Usage: ./a.out [-h] [-m INSTRUCTION_MAPPING] -i INPUT_FILE -o OUTPUT_FILE\n\n"
					"-h Displays this help menu.\n\n"
					"-m INSTRUCTION_MAPPING File that contains the instruction mapping.\n"
						"\t\t\tIf this option is not provided it defaults to\n"
						"\t\t\tinstruction_mapping.txt.\n\n"
					"-i INPUT_FILE This should be a binary file that contains the .text\n"
						"\t\tsection of a Mars220 MIPS binary.\n\n"
					"-o OUTPUT_FILE This can be any file on the system or - which\n"
						"\t\tspecifies stdout\n";

/* Function prototypes */
void handleFlags(int argc, char *argv[]);
void printHelp();
void buildInstrList(char *instrFile, char *input, char *output);
int* countNumInstr(char *file);
FILE* openInput(char *input);
void initializeTypes(struct InstrType **rType, struct InstrType **iType, struct InstrType **jType);
void addRInstr(struct InstrType **rType, char *line);
void addIInstr(struct InstrType **iType, char *line);
void addJInstr(struct InstrType **jType, char *line);
void printLists(InstrType *rType, InstrType *iType, InstrType *jType);

int main(int argc, char *argv[]){

	/* Process args first */
	handleFlags(argc, argv);

	exit(EXIT_FAILURE);
}

/* Processes the flags and arguments passed to the program */
void handleFlags(int argc, char *argv[]){
	/* Used for processing options */
	int h = 0, m = 0, i = 0, o = 0, opt;
	char *validOpts = "m:i:o:h";
	extern int optind;
	extern char *optarg;

	char *instrFile = NULL, *input, *output;

	/* Handle no options given */
	if(argc <= 1)
		printHelp();

	/* Process each opt and arg with getopt */
	while((opt = getopt(argc, argv, validOpts)) != -1){
		switch(opt){
			case 'h':
				h++;
				break;
			case 'm':
				m++;
				instrFile = optarg;
				break;
			case 'i':
				i++;
				input = optarg;
				break;
			case 'o':
				o++;
				output = optarg;
				break;
			default:
				exit(EXIT_FAILURE);
		}
	}

	/* Print the help prompt if requested */
	if(h){
		printHelp();
	}

	/* Handle default instructions list */
	if (!m){
		instrFile = "instruction_mapping.txt";
	}
	/* Ensure both input and output were provided */
	if(i && o)
		buildInstrList(instrFile, input, output);
	/* Otherwise there weren't enough options and args passed */
	else
		exit(EXIT_FAILURE);
}

void printHelp(){
	extern char *helpPrompt;

	printf("%s", helpPrompt);
	exit(EXIT_SUCCESS);
}

void buildInstrList(char *instrFile, char *input, char *output){
	FILE *instrList;
	INSTRTYPE *rType, *iType, *jType;
	char line[30];

	/* Attempt to open and count the number of instructions */
	/* numInstructions = countNumInstr(instrFile); */

	/* Open a new stream for parsing */
	instrList = openInput(instrFile);

	/* Initialize InstrType structs values and malloc space for instructions*/
	initializeTypes(&rType, &iType, &jType);

	/* Loop through each instruction in the stream*/
	while(fgets(line, 30, instrList) != NULL){
		/* Check first char first */
		switch(*line){
			case 'r':
				addRInstr(&rType, line);
				break;
			case 'i':
				addIInstr(&iType, line);
				break;
			case 'j':
				addJInstr(&jType, line);
				break;
			default:
				debug("incorrect instruction type parsed.\n");
				break;
		}
	}

	/* Print lists for grading */
	#ifdef CSE220
		printLists(rType, iType, jType);
	#endif

}

int* countNumInstr(char *file){
	char c, prev;
	/* Index 0: r-type, 1: i-type, 2: j-type. */
	int *numInstructions = (int *)malloc(3*sizeof(int));

	/* Attempt to open stream */
	FILE *stream = fopen(file, "r");
	if (stream == NULL){
		debug("%s\n", "Failed to open instrlist file\n");
		exit(EXIT_FAILURE);
	}

	/* count first instruction */
	c = fgetc(stream);
	switch(c){
		case 'r':
			numInstructions[0]++;
			break;
		case 'i':
			numInstructions[1]++;
			break;
		case 'j':
			numInstructions[2]++;
			break;
		default:
			debug("Failed to count first instr type\n");
			exit(EXIT_FAILURE);
	}

	prev = c;

	/* count number of each instr type */
	while((c = fgetc(stream)) != EOF){
		/* Check if we previously read a new line char */
		if (prev == '\n'){
			/* count based on the char we just read */
			switch(c){
				case 'r':
					numInstructions[0]++;
					break;
				case 'i':
					numInstructions[1]++;
					break;
				case 'j':
					numInstructions[2]++;
					break;
				default:
					debug("Failed to count instr type\n");
					exit(EXIT_FAILURE);
			}
		}
		prev = c;
	}

	/* Close the stream */
	fclose(stream);
	return numInstructions;
}

FILE* openInput(char *input){
	FILE *file = fopen(input, "r");
	if (file == NULL){
		debug("%s\n", "Failed to open input file");
		exit(EXIT_FAILURE);
	}

	return file;
}

void initializeTypes(struct InstrType **rType, struct InstrType **iType, struct InstrType **jType){
	
	(*rType) = malloc(sizeof(INSTRTYPE));
	(*rType)->type = 'R';
	(*rType)->count = 0;
	(*rType)->head = NULL;

	(*iType) = malloc(sizeof(INSTRTYPE));
	(*iType)->type = 'I';
	(*iType)->count = 0;
	(*iType)->head = NULL;

	(*jType) = malloc(sizeof(INSTRTYPE));
	(*jType)->type = 'J';
	(*jType)->count = 0;
	(*jType)->head = NULL;

	(*rType)->prev = NULL;
	(*rType)->next = *iType;
	(*iType)->prev = *rType;
	(*iType)->next = *jType;
	(*jType)->prev = *iType;
	(*jType)->next = NULL;
}

void addRInstr(struct InstrType **rType, char *line){
	INSTRUCTION *temp, *prev, *new;
	char mnemonic[10];
	int UID, pretty;

	/* Parse the line for UID, mnemonic, and pretty value */
	sscanf(line, "r %x %s %d", &UID, mnemonic, &pretty);

	/* Modify linkedlist to reference alphabetically */
	temp = (*rType)->head;
	prev = NULL;
	/* Check if this is the first instruction */
	if(temp == NULL){
		/* Allocate memory at the head */
		(*rType)->head = malloc(sizeof(INSTRUCTION));
		/* initialize each of the values */
		((*rType)->head)->uid = UID;
		((*rType)->head)->pretty = pretty;
		((*rType)->head)->mnemonic = malloc(strlen(mnemonic)*sizeof(char));
		strcpy(((*rType)->head)->mnemonic, mnemonic);
		((*rType)->head)->next = NULL;
		((*rType)->head)->prev = NULL;
	}else{
		/* Otherwise insert instruction alphabetically*/
		new = malloc(sizeof(INSTRUCTION));
		new->uid = UID;
		new->pretty = pretty;
		new->mnemonic = malloc(strlen(mnemonic)*sizeof(char));
		strcpy(new->mnemonic, mnemonic);

		while(temp != NULL){
			/* compare to the next instruction in the list */
			/* Insert if the mnemonic is comes before */
			if(strcmp(mnemonic, temp->mnemonic) < 0){
				/* If we are inserting to the front */
				if(prev == NULL){
					((*rType)->head)->prev = new;
					new->next = (*rType)->head;
					(*rType)->head = new;
					new->prev = NULL;
					return;
				}
				else{
					prev->next = new;
					new->prev = prev;
					new->next = temp;
					temp->prev = new;
					return;
				}
			/* current instruction is greater so continue */
			}else
				prev = temp;
				temp = prev->next;
		}
		/* have not inserted yet so we add to the end */
		prev->next = new;
		new->prev = prev;
		new->next = NULL;
		return;
	}

}

void addIInstr(struct InstrType **iType, char *line){
	INSTRUCTION *temp, *prev, *new;
	char mnemonic[10];
	int UID, pretty;

	/* Parse the line for UID, mnemonic, and pretty value */
	sscanf(line, "i %x %s %d", &UID, mnemonic, &pretty);

	/* Modify linkedlist to reference alphabetically */
	temp = (*iType)->head;
	prev = NULL;
	/* Check if this is the first instruction */
	if(temp == NULL){
		/* Allocate memory at the head */
		(*iType)->head = malloc(sizeof(INSTRUCTION));
		/* initialize each of the values */
		((*iType)->head)->uid = UID;
		((*iType)->head)->pretty = pretty;
		((*iType)->head)->mnemonic = malloc(strlen(mnemonic)*sizeof(char));
		if(((*iType)->head)->mnemonic == NULL){
			debug("malloc failed for mnemonic\n");
			exit(EXIT_FAILURE);
		}
		strcpy(((*iType)->head)->mnemonic, mnemonic);
		((*iType)->head)->next = NULL;
		((*iType)->head)->prev = NULL;
	}else{
		/* Otherwise insert instruction alphabetically*/
		new = malloc(sizeof(INSTRUCTION));
		new->uid = UID;
		new->pretty = pretty;
		new->mnemonic = malloc(strlen(mnemonic)*sizeof(char));
		strcpy(new->mnemonic, mnemonic);

		while(temp != NULL){
			/* compare to the next instruction in the list */
			/* Insert if the mnemonic is comes before */
			if(strcmp(mnemonic, temp->mnemonic) < 0){
				/* If we are inserting to the front */
				if(prev == NULL){
					((*iType)->head)->prev = new;
					new->next = (*iType)->head;
					(*iType)->head = new;
					new->prev = NULL;
					return;
				}
				else{
					prev->next = new;
					new->prev = prev;
					new->next = temp;
					temp->prev = new;
					return;
				}
			/* current instruction is greater so continue */
			}else
				prev = temp;
				temp = prev->next;
		}
		/* have not inserted yet so we add to the end */
		prev->next = new;
		new->prev = prev;
		new->next = NULL;
		return;
	}

}

void addJInstr(struct InstrType **jType, char *line){
	INSTRUCTION *temp, *prev, *new;
	char mnemonic[10];
	int UID, pretty;

	/* Parse the line for UID, mnemonic, and pretty value */
	sscanf(line, "j %x %s %d", &UID, mnemonic, &pretty);

	/* Modify linkedlist to reference alphabetically */
	temp = (*jType)->head;
	prev = NULL;
	/* Check if this is the first instruction */
	if(temp == NULL){
		/* Allocate memory at the head */
		(*jType)->head = malloc(sizeof(INSTRUCTION));
		/* initialize each of the values */
		((*jType)->head)->uid = UID;
		((*jType)->head)->pretty = pretty;
		((*jType)->head)->mnemonic = malloc(strlen(mnemonic)*sizeof(char));
		if(((*jType)->head)->mnemonic == NULL){
			debug("malloc failed for mnemonic\n");
			exit(EXIT_FAILURE);
		}
		strcpy(((*jType)->head)->mnemonic, mnemonic);
		((*jType)->head)->next = NULL;
		((*jType)->head)->prev = NULL;
	}else{
		/* Otherwise insert instruction alphabetically*/
		new = malloc(sizeof(INSTRUCTION));
		new->uid = UID;
		new->pretty = pretty;
		new->mnemonic = malloc(strlen(mnemonic)*sizeof(char));
		strcpy(new->mnemonic, mnemonic);

		while(temp != NULL){
			/* compare to the next instruction in the list */
			/* Insert if the mnemonic is comes before */
			if(strcmp(mnemonic, temp->mnemonic) < 0){
				/* If we are inserting to the front */
				if(prev == NULL){
					((*jType)->head)->prev = new;
					new->next = (*jType)->head;
					(*jType)->head = new;
					new->prev = NULL;
					return;
				}
				else{
					prev->next = new;
					new->prev = prev;
					new->next = temp;
					temp->prev = new;
					return;
				}
			/* current instruction is greater so continue */
			}else
				prev = temp;
				temp = prev->next;
		}
		/* have not inserted yet so we add to the end */
		prev->next = new;
		new->prev = prev;
		new->next = NULL;
		return;
	}

}

void printLists(InstrType *rType, InstrType *iType, InstrType *jType){
	INSTRUCTION *temp;

	printf("\nCSE220: R-Type List:\n");
	temp = rType->head;
	while(temp != NULL){
		printf("CSE220: %8p uid: %3d pretty: %2d mnemonic: %7s next: %8p prev: %8p\n", temp, temp->uid, temp->pretty, temp->mnemonic, temp->next, temp->prev);
		temp = temp->next;
	}

	printf("\nCSE220: I-Type List:\n");
	temp = iType->head;
	while(temp != NULL){
		printf("CSE220: %8p uid: %3d pretty: %2d mnemonic: %7s next: %8p prev: %8p\n", temp, (temp->uid) >> 26, temp->pretty, temp->mnemonic, temp->next, temp->prev);
		temp = temp->next;
	}

	printf("\nCSE220: J-Type List:\n");
	temp = jType->head;
	while(temp != NULL){
		printf("CSE220: %8p uid: %3d pretty: %2d mnemonic: %7s next: %8p prev: %8p\n", temp, (temp->uid) >> 26, temp->pretty, temp->mnemonic, temp->next, temp->prev);
		temp = temp->next;
	}
}