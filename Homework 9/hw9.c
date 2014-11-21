/*
Homework #9
Name: Andrew Rosiclair
SBU ID: 109235970
*/

#include "hw9.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h> /* For identifying endianess */

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
void printHelp(int failure);
void buildInstrList(char *instrFile, char *input, char *output);
void verifyStreams(char *stream1, char *stream2, char *stream3);
FILE* openInput(char *input);
FILE* openOutput(char *output);
void initializeTypes(struct InstrType **rType, struct InstrType **iType, struct InstrType **jType);
void addRInstr(struct InstrType **rType, char *line);
void addIInstr(struct InstrType **iType, char *line);
void addJInstr(struct InstrType **jType, char *line);
void printLists(InstrType *rType, InstrType *iType, InstrType *jType);
void parseBin(INSTRTYPE **instrTypes, char *input, char *output);
int verifyBOM(int word);
int toBE(int word);
int getInstrType(int word);
int getUID(int word, int type);
INSTRUCTION * findInstruction(INSTRTYPE *type, int UID);
void writeInstruction(INSTRUCTION *instr, int word, int instrType, FILE *output);
void printStats(INSTRTYPE **types);
void freeMemory(INSTRTYPE **types);

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

	/* No options given */
	if(argc <= 1)
		printHelp(1);

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
				printHelp(1);
				exit(EXIT_FAILURE);
		}
	}

	/* Print the help prompt if requested */
	if(h){
		printHelp(0);
	}

	/* Handle default instructions list */
	if (!m){
		instrFile = "instruction_mapping.txt";
	}
	/* Ensure both input and output were provided */
	if(i && o)
		buildInstrList(instrFile, input, output);
	/* Otherwise there weren't enough options and args passed */
	else{
		printHelp(1);
		exit(EXIT_FAILURE);
	}
}

void printHelp(int failure){
	extern char *helpPrompt;

	printf("%s", helpPrompt);
	if(failure)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

void buildInstrList(char *instrFile, char *input, char *output){
	FILE *instrList;
	INSTRTYPE *rType, *iType, *jType;
	INSTRTYPE *types[3];
	char line[30];

	/* Verify files are not the same */
	verifyStreams(instrFile, input, output);

	/* Attempt to open streams for parsing */
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

	/* Close the stream */
	fclose(instrList);

	/* Print lists for grading */
	#ifdef CSE220
		printLists(rType, iType, jType);
	#endif

	types[0] = rType;
	types[1] = iType;
	types[2] = jType;

	parseBin(types, input, output);
}

FILE* openInput(char *input){
	FILE *file = fopen(input, "r");
	if (file == NULL){
		debug("%s\n", "Failed to open input file");
		exit(EXIT_FAILURE);
	}

	return file;
}

FILE* openOutput(char *output){
	FILE *out;
	if(strcmp(output, "-") == 0)
		return stdout;
	out = fopen(output, "w");
	if (out == NULL){
		debug("%s\n", "Failed to open output file");
		exit(EXIT_FAILURE);
	}

	return out;
}

void verifyStreams(char *instrFile, char *input, char *output){
	struct stat *s1 = malloc(sizeof(struct stat)), *s2 = malloc(sizeof(struct stat)), *s3 = malloc(sizeof(struct stat));
	FILE *fp;
	long size;

	/* Load stat info */
	stat(instrFile, s1);
	stat(input, s2);
	stat(output, s3);

	/* Compare st_ino values */
	if(s1->st_ino == s2->st_ino){
		debug("Multiple files are the same\n");
		exit(EXIT_FAILURE);
	}else if(s2->st_ino == s3->st_ino){
		debug("Multiple files are the same\n");
		exit(EXIT_FAILURE);
	}else if(s1->st_ino == s3->st_ino){
		debug("Multiple files are the same\n");
		exit(EXIT_FAILURE);
	}

	free(s1);
	free(s2);
	free(s3);

	/* check that we can open the fieles */
	fp = fopen(instrFile, "r");
	if(fp == NULL){
		debug("Instruction file failed to open.\n");
		exit(EXIT_FAILURE);
	}

	fclose(fp);

	fp = fopen(input, "r");
	if(fp == NULL){
		debug("Binary file failed to open.\n");
		exit(EXIT_FAILURE);
	}

	/* check size of binary file */
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	if((size % 4) != 0){
		debug("Size of binary file is not a multiple of 4.\n");
		exit(EXIT_FAILURE);
	}

	fclose(fp);

	/* Don't check output if stdout was chosen */
	if(strcmp(output, "-") != 0){
		fp = fopen(output, "w");
		if(fp == NULL){
			debug("Output file failed to open.\n");
			exit(EXIT_FAILURE);
		}

		fclose(fp);
	}

	return;

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

/* Parses the binary file for each 32-bit MIPS instruction */
void parseBin(INSTRTYPE **instrTypes, char *input, char *output){
	FILE *bin, *out;
	int word, type, UID;
	INSTRUCTION *match;
	/* instrTypes[0] = rType, [1] = iType, [2] = jType */

	/* open input stream */
	bin = openInput(input);
	out = openOutput(output);


	/* read in the first 4 bytes */
	fread(&word, 1, 4, bin);

	/* Verify BOM based on endianess */
	if(!verifyBOM(word)){
		debug("BOM did not checkout. BOM: %x\n", word);
		freeMemory(instrTypes);
		fclose(bin);
		fclose(out);
		exit(EXIT_SUCCESS);
	}

	/* Read 4 bytes for each instruction until we reach the end of the file */
	while(fread(&word, 1, 4, bin) != 0){
		/* convert to big endian if necessary */
		if(htonl(42) != 42)
			word = toBE(word);
		/* Get the instr type */
		type = getInstrType(word);
		/* get UID based on instr type */
		UID = getUID(word, type);
		/* find the matching instruction from the appropriate list */
		match = findInstruction(instrTypes[type], UID);

		/*write to the output */
		writeInstruction(match, word, type, out);

	}

	#ifdef CSE220
		/* Print statistics */
		printStats(instrTypes);
	#endif

	/* Free memory */
	freeMemory(instrTypes);
	/* close streams */
	fclose(bin);
	fclose(out);

}

int verifyBOM(int word){
	if(htonl(42) == 42){
		/* Big Endian */
		if(word == 0x576F6E67)
			return 1;
	}else{
		/* Little Endian */
		if(word == 0x676E6F57)
			return 1;
	}

	return 0;
}

int toBE(int word){
	/* Holds each shifted bytes for BE */
	int one, two, three, four, result;

	one = (word & 0x000000FF) << 24;
	two = (word & 0x0000FF00) << 8;
	three = (word & 0x00FF0000) >> 8;
	four = (word & 0xFF000000) >> 24;

	result = one | two | three | four;
	return result;
}

int getInstrType(int word){
	word &= 0xFC000000;
	word >>= 26;

	switch(word){
		/* 0 = R-Type */
		case 0:
			return 0;
		/* 2 or 3 = J-Type */
		case 2:
			return 2;
		case 3:
			return 2;
		/* Anything else = I-Type */
		default:
			return 1;
	}
}

int getUID(int word, int type){
	int uid;

	switch(type){
		case 0:
			uid = word & 0x0000003F;
			break;
		case 1:
			uid = word & 0xFC000000;
			break;
		case 2:
			uid = word & 0xFC000000;
			break;
		default:
			debug("The wrong type was passed. Type: %d\n", type);
			exit(EXIT_FAILURE);
	}

	return uid;
}

INSTRUCTION * findInstruction(INSTRTYPE *type, int UID){
	INSTRUCTION *temp;

	temp = type->head;
	while(temp != NULL){
		if(temp->uid == UID)
			return temp;
		else
			temp = temp->next;
	}

	debug("Matching instruction not found. UID: %d\n", UID);
	exit(EXIT_FAILURE);
}

void writeInstruction(INSTRUCTION *instr, int word, int instrType, FILE *output){
	int rs = 0, rt = 0, rd = 0, shamt, imm = 0, addr = 0;
	/* Array of shorthand register names */
	char *registers[] = {
		"$zero","$at",
		"$v0","$v1",
		"$a0","$a1","$a2","$a3",
		"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
		"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
		"$t8","$t9","$k0","$k1",
		"$gp","$sp","$fp","$ra"
	};
	char *mnemonic = instr->mnemonic;

	/* Parse instruction fields based on type */
	switch(instrType){
		/* R-type */
		case 0:
			rs = (word & 0x03E00000) >> 21;
			rt = (word & 0x001F0000) >> 16;
			rd = (word & 0x0000F800) >> 11;
			shamt = (word & 0x000007C0) >> 6;
			break;
		/* I-Type */
		case 1:
			rs = (word & 0x03E00000) >> 21;
			rt = (word & 0x001F0000) >> 16;
			imm = (word & 0x0000FFFF);
			break;
		case 2:
			addr = (word & 0x03FFFFFF);
			break;
		default:
			debug("The wrong type was passed. Type: %d\n", instrType);
			exit(EXIT_FAILURE);
	}

	/* Write to output based on the pretty value */
	switch(instr->pretty){
		case 0:
			fprintf(output, "%s %s\n", mnemonic, registers[rd]);
			break;
		case 1:
			fprintf(output, "%s %s, %s\n", mnemonic, registers[rs], registers[rt]);
			break;
		case 2:
			fprintf(output, "%s %s, %s, 0x%X\n", mnemonic, registers[rt], registers[rs], imm);
			break;
		case 3:
			fprintf(output, "%s %s, %s, %s\n", mnemonic, registers[rd], registers[rt], registers[rs]);
			break;
		case 4:
			fprintf(output, "%s %s, 0x%X(%s)\n", mnemonic, registers[rt], imm, registers[rs]);
			break;
		case 5:
			fprintf(output, "%s\n", mnemonic);
			break;
		case 6:
			fprintf(output, "%s 0x%X\n", mnemonic, addr);
			break;
		case 7:
			fprintf(output, "%s %s, 0x%X\n", mnemonic, registers[rs], imm);
			break;
		case 8:
			fprintf(output, "%s %s, %s, 0x%X\n", mnemonic, registers[rd], registers[rs], shamt);
			break;
		case 9:
			fprintf(output, "%s %s, %s, 0x%X\n", mnemonic, registers[rs], registers[rt], imm);
			break;
	}
}

void printStats(INSTRTYPE **types){
	int numR = 0, numI = 0, numJ = 0, instrSize, typeSize, mnemonicSize = 0, total;
	INSTRUCTION *temp;

	/* count R-type */
	temp = types[0]->head;
	while(temp != NULL){
		numR++;
		mnemonicSize += sizeof(temp->mnemonic);
		temp = temp->next;
	}

	/* count I-type */
	temp = types[1]->head;
	while(temp != NULL){
		numI++;
		mnemonicSize += sizeof(temp->mnemonic);
		temp = temp->next;
	}

	/* count J-type */
	temp = types[2]->head;
	while(temp != NULL){
		numJ++;
		mnemonicSize += sizeof(temp->mnemonic);
		temp = temp->next;
	}

	instrSize = sizeof(INSTRUCTION);
	typeSize = sizeof(INSTRTYPE);
	total = 3*typeSize + (numR + numI + numJ)*instrSize + mnemonicSize;

	printf("CSE220: There is %d r-type, %d i-type, and %d j-type nodes.\n", numR, numI, numJ);
	printf("CSE220: The InstrType node takes up %d bytes in memory.\n", typeSize);
	printf("CSE220: The Instr node takes up %d bytes in memory\n", instrSize);
	printf("CSE220: Your program allocated %d nodes that take up %d bytes in memory\n", numR + numI + numJ + 3, total);
}

void freeMemory(INSTRTYPE **types){
	INSTRUCTION *next, *current;

	/* loop through each instruction list and free the mnemonic then instruction */
	/* R-type */
	next = types[0]->head;
	current = next;
	while(next != NULL){
		free(next->mnemonic);
		next = next->next;
		free(current);
		current = next;
	}


	/* I-type */
	next = types[1]->head;
	current = next;
	while(next != NULL){
		free(next->mnemonic);
		next = next->next;
		free(current);
		current = next;
	}

	/* J-type */
	next = types[2]->head;
	current = next;
	while(next != NULL){
		free(next->mnemonic);
		next = next->next;
		free(current);
		current = next;
	}

	/* Free the InstrTypes */
	free(types[0]);
	free(types[1]);
	free(types[2]);
}