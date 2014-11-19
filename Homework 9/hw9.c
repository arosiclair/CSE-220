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
void initializeTypes(struct InstrType **rType, struct InstrType **iType, struct InstrType **jType, int *numInstructions);
void addRInstr(struct InstrType *rType, char *line, int numOfInstr);
void addIInstr(struct InstrType *iType, char *line, int numOfInstr);
void addJInstr(struct InstrType *jType, char *line, int numOfInstr);
void printLists(InstrType rType, InstrType iType, InstrType jType);

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
	int numR = 0, numI = 0, numJ = 0, *numInstructions;

	/* Attempt to open and count the number of instructions */
	numInstructions = countNumInstr(instrFile);

	/* Open a new stream for parsing */
	instrList = openInput(instrFile);

	/* Initialize InstrType structs values and malloc space for instructions*/
	initializeTypes(&rType, &iType, &jType, numInstructions);

	/* Loop through each instruction in the stream*/
	while(fgets(line, 30, instrList) != NULL){
		/* Check first char first */
		switch(*line){
			case 'r':
				addRInstr(rType, line, numR);
				numR++;
				break;
			case 'i':
				addIInstr(iType, line, numI);
				numI++;
				break;
			case 'j':
				addJInstr(jType, line, numJ);
				numJ++;
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

void initializeTypes(struct InstrType **rType, struct InstrType **iType, struct InstrType **jType, int *numInstructions){
	
	(*rType) = malloc(sizeof(INSTRTYPE));
	(*rType)->type = 'R';
	(*rType)->count = 0;
	(*rType)->head = malloc(numInstructions[0]*sizeof(INSTRUCTION));

	(*iType) = malloc(sizeof(INSTRTYPE));
	(*iType)->type = 'I';
	(*iType)->count = 0;
	(*iType)->head = malloc(numInstructions[1]*sizeof(INSTRUCTION));

	(*jType) = malloc(sizeof(INSTRTYPE));
	(*jType)->type = 'J';
	(*jType)->count = 0;
	(*jType)->head = malloc(numInstructions[0]*sizeof(INSTRUCTION));

	(*rType)->prev = NULL;
	(*rType)->next = *iType;
	(*iType)->prev = *rType;
	(*iType)->next = *jType;
	(*jType)->prev = *iType;
	(*jType)->next = NULL;
}

void addRInstr(struct InstrType *rType, char *line, int numOfInstr){
	INSTRUCTION *temp, *prev;
	char *token, *mnemonic;
	int UID, pretty, scanResult;

	/* Parse UID */
	token = strtok(line, " ");
	token = strtok(NULL, " ");
	if(token == NULL){
		debug("Parsing UID failed.\n");
		exit(EXIT_FAILURE);
	}
	/* Scan in hex UID */
	scanResult = sscanf(token, "%8x", &UID);
	if(scanResult <= 0){
		debug("Scanning hex UID failed.\n");
		exit(EXIT_FAILURE);
	}

	/* Parse mnemonic */
	mnemonic = strtok(NULL, " ");
	/* Parse pretty print format */
	token = strtok(NULL, " ");
	scanResult = sscanf(token, "%d", &pretty);


	/* Add the instruction to the r-type list alphabetically */
	/* Check if this is the first instruction */
	if(numOfInstr == 0){
		rType.head->uid = &instruction;
		instruction.next = NULL;
		instruction.prev = NULL;
	}else{
		/* Otherwise insert instruction alphabetically*/
		temp = rType.head;
		prev = NULL;
		while(temp != NULL){
			/* compare to the next instruction in the list */
			/* Insert if the mnemonic is comes before */
			if(strcmp(instruction.mnemonic, temp->mnemonic) < 0){
				temp->prev = &instruction;
				instruction.next = temp;
				instruction.prev = prev;
				/* If we are inserting to the front */
				if(prev == NULL){
					rType.head = &instruction;
					return;
				}
				else{
					prev->next = &instruction;
					return;
				}
			/* current instruction is greater so continue */
			}else
				prev = temp;
				temp = prev->next;
		}
		/* have not inserted yet so we add to the end */
		prev->next = &instruction;
		instruction.prev = prev;
		return;
	}

}

void addIInstr(struct InstrType *iType, char *line, int numOfInstr){
	INSTRUCTION instruction, *temp, *prev;
	char *token, *mnemonic;
	int UID, pretty, scanResult;

	/* Parse UID */
	token = strtok(line, " ");
	token = strtok(NULL, " ");
	if(token == NULL){
		debug("Parsing UID failed.\n");
		exit(EXIT_FAILURE);
	}
	/* Scan in hex UID */
	scanResult = sscanf(token, "%8x", &UID);
	if(scanResult <= 0){
		debug("Scanning hex UID failed.\n");
		exit(EXIT_FAILURE);
	}

	/* Parse mnemonic */
	mnemonic = strtok(NULL, " ");
	/* Parse pretty print format */
	token = strtok(NULL, " ");
	scanResult = sscanf(token, "%d", &pretty);

	/* Initialize new instruction */
	instruction.uid = UID;
	instruction.pretty = pretty;
	instruction.mnemonic = mnemonic;


	/* Add the instruction to the r-type list alphabetically */
	/* Check if this is the first instruction */
	if(iType.head == NULL){
		iType.head = &instruction;
		instruction.next = NULL;
		instruction.prev = NULL;
	}else{
		/* Otherwise insert instruction alphabetically*/
		temp = iType.head;
		prev = NULL;
		while(temp != NULL){
			/* compare to the next instruction in the list */
			/* Insert if the mnemonic is comes before */
			if(strcmp(instruction.mnemonic, temp->mnemonic) < 0){
				temp->prev = &instruction;
				instruction.next = temp;
				instruction.prev = prev;
				/* If we are inserting to the front */
				if(prev == NULL){
					iType.head = &instruction;
					return;
				}
				else{
					prev->next = &instruction;
					return;
				}
			/* current instruction is greater so continue */
			}else
				prev = temp;
				temp = prev->next;
		}
		/* have not inserted yet so we add to the end */
		prev->next = &instruction;
		instruction.prev = prev;
		return;
	}

}

void addJInstr(struct InstrType *jType, char *line, int numOfInstr){
	INSTRUCTION instruction, *temp, *prev;
	char *token, *mnemonic;
	int UID, pretty, scanResult;

	/* Parse UID */
	token = strtok(line, " ");
	token = strtok(NULL, " ");
	if(token == NULL){
		debug("Parsing UID failed.\n");
		exit(EXIT_FAILURE);
	}
	/* Scan in hex UID */
	scanResult = sscanf(token, "%8x", &UID);
	if(scanResult <= 0){
		debug("Scanning hex UID failed.\n");
		exit(EXIT_FAILURE);
	}

	/* Parse mnemonic */
	mnemonic = strtok(NULL, " ");
	/* Parse pretty print format */
	token = strtok(NULL, " ");
	scanResult = sscanf(token, "%d", &pretty);

	/* Initialize new instruction */
	instruction.uid = UID;
	instruction.pretty = pretty;
	instruction.mnemonic = mnemonic;


	/* Add the instruction to the r-type list alphabetically */
	/* Check if this is the first instruction */
	if(jType.head == NULL){
		jType.head = &instruction;
		instruction.next = NULL;
		instruction.prev = NULL;
	}else{
		/* Otherwise insert instruction alphabetically*/
		temp = jType.head;
		prev = NULL;
		while(temp != NULL){
			/* compare to the next instruction in the list */
			/* Insert if the mnemonic is comes before */
			if(strcmp(instruction.mnemonic, temp->mnemonic) < 0){
				temp->prev = &instruction;
				instruction.next = temp;
				instruction.prev = prev;
				/* If we are inserting to the front */
				if(prev == NULL){
					jType.head = &instruction;
					return;
				}
				else{
					prev->next = &instruction;
					return;
				}
			/* current instruction is greater so continue */
			}else
				prev = temp;
				temp = prev->next;
		}
		/* have not inserted yet so we add to the end */
		prev->next = &instruction;
		instruction.prev = prev;
		return;
	}

}

void printLists(InstrType rType, InstrType iType, InstrType jType){
	INSTRUCTION *temp;

	printf("CSE220: R-Type List:\n");
	temp = rType.head;
	while(temp != NULL){
		printf("CSE220: %p uid: %d pretty: %d mnemonic: %s next: %p prev: %p\n", temp, temp->uid, temp->pretty, temp->mnemonic, temp->next, temp->prev);
	}

	printf("CSE220: I-Type List:\n");
	temp = iType.head;
	while(temp != NULL){
		printf("CSE220: %p uid: %d pretty: %d mnemonic: %s next: %p prev: %p\n", temp, temp->uid, temp->pretty, temp->mnemonic, temp->next, temp->prev);
	}

	printf("CSE220: J-Type List:\n");
	temp = jType.head;
	while(temp != NULL){
		printf("CSE220: %p uid: %d pretty: %d mnemonic: %s next: %p prev: %p\n", temp, temp->uid, temp->pretty, temp->mnemonic, temp->next, temp->prev);
	}
}