/*
Homework #6
name: Andrew Rosiclair
SBU ID: 109235970
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	/* Used for getopt() function*/
#include <string.h>	/* Used to compare strings */
#include <stddef.h> /* Used for NULL macro */


/* Error exit codes */
#define ERROR_REG 1
#define ERROR_INSTR 2
#define ERROR_IMMEDIATE 3
#define ERROR_FLAGS 4
#define ERROR_HUMAN 5

/*
-h Help Menu
-i Prints MIPS instruction stats
-r Prints register stats
-m Prints immediate values used
-u Prints a label at the top of stats. Requires use of another flag.
*/
char validArgs[] = "irmu";
char helpPrompt[] = "Usage: ./a.out [-himru]"
						"\n\t-h Displays this help menu."
						"\n\t-i Displays statistics about instruction type usages."
						"\n\t-m Displays all the immediate values used in I-Type and J-Type instructions"
						"\n\t-r Displays information about the registers."
						"\n\t-u Displays human readable headers for the different information displayed."
							"\n\t\tShould only be used with the -i, -r, and -m flags.";

/* Function prototypes */
void handleFlags(int argc, char *argv[]);
void printHelp(int failure);
void instrStats(int label);
void regStats(int label);
void immStats(int label);


int main(int argc, char *argv[]){

	/* First, analyze the flags */
	handleFlags(argc, argv);
	return EXIT_SUCCESS;
}

/* Uses the getopt function to parse each of the given args and calls functions to handle each flag. */
void handleFlags(int argc, char *argv[]){
	/* Used for looping through argv */
	int i;
	/* Used in conjunction with getopt to process flags */
	int opt = 0, label = 0;
	extern char validArgs[];

	/* If there are no given flags, print -h with ERROR_FLAGS */
	if (argc <= 1){
		printHelp(ERROR_FLAGS);
	}

	/* Search argv elements for the letters h and u for the help menu and labels flag */
	for (i = 1; i < argc; i++){
		/* first check if the arg has a '-' */
		if (strchr(argv[i], '-') != 0){
			if (strchr(argv[i], 'h') != 0)
				printHelp(0);
			if (strchr(argv[i], 'u') != 0)
				label = 1;
		}
	}

	/* h was not found so we parse each of the flags with getopt.
	We call a function based on the returned option and use the label int to signal whether the printed stats should be labeled. */
	while ((opt = getopt(argc, argv, validArgs)) != 0){	
		switch(opt){
			case 'i':
				instrStats(label);
				break;
			case 'r':
				regStats(label);
				break;
			case 'm':
				immStats(label);
				break;
			}
	}
	
	if (label)
		printHelp(ERROR_HUMAN);

	/* Shouldn't even be able to make it this far */
	exit(EXIT_FAILURE);
}

/* Prints the Help prompt and exits failure if there were no flags given */
void printHelp(int failCode){
	extern char helpPrompt[];

	if (failCode){
		printf("%s", helpPrompt);
		exit(failCode);
	}

	printf("%s", helpPrompt);
	exit(EXIT_SUCCESS);
}

/* Reads through the stdin stream and analyzes each line for MIPS instructions and then prints out a table of stats for different MIPS Instruction types
Args:	label = A conditional int signifying whether a header label 				should be printed.
*/
void instrStats(int label){
	/* Used to store each line that will be read from stdin */
	int instruction = 0, opcode = 0, i;
	char line[10];
	/* Used for counting stats */
	float numOfLines = 0, numItype = 0, numRtype = 0, numJtype = 0, rUsage, iUsage, jUsage; 

	/* Condition reads in new instruction and checks if was successfully read */
	while(fgets(line, 11, stdin) != NULL){
		/* Validate each char in the input string */
		for(i = 2; i < 10; i++){
			if(line[i] >= '0' && line[i] <= '9')
				continue;
			if (line[i] >= 'a' && line[i] <= 'f')
				continue;
			if (line[i] >= 'A' && line[i] <= 'F')
				continue;
			/* char didn't checkout so exit with error */
			printf("%s", line);
			printf("%c\n", line[i]);
			printf("%.0f\n", numOfLines);
			exit(ERROR_INSTR);
		}

		/* Scan in hex string as integer into instruction */
		sscanf(line, "%10x", &instruction);

		numOfLines++;

		/* Pull first 6 bits from the instruction and shift them over */
		opcode = instruction & 0xFC000000;
		opcode >>= 26;

		/* Increment counters based on opcode */
		switch(opcode){
			case 0:
				numRtype++;
				break;
			case 2:
				numJtype++;
				break;
			case 3:
				numJtype++;
				break;
			default:
				numItype++;
				break;				
		}	
	}

	/* Empty file case */
	if (numOfLines <= 0)
		exit(ERROR_INSTR);

	/* Calcualte stats */
	rUsage = (numRtype/numOfLines) * 100;
	iUsage = (numItype/numOfLines) * 100;
	jUsage = (numJtype/numOfLines) * 100;

	/* print label according to the argument */
	if (label)
		printf("%6s %6s %9s \n", "TYPE", "COUNT", "PERCENT");
	
	/* Print the stats */
	printf("%6s %6d %8.1f%c \n", "R-type", (int)numRtype, rUsage, '%');
	printf("%6s %6d %8.1f%c \n", "I-type", (int)numItype, iUsage, '%');
	printf("%6s %6d %8.1f%c \n", "J-type", (int)numJtype, jUsage, '%');

	exit(EXIT_SUCCESS);
}

void regStats(int label){
	/* 2D array registers[i][j] that holds info for each register
		i: register, j: 0 = num of times used
						1 = times called by r instruction
						2 = times called by i instruction
						3 = times called by j instruction */
	int registers[32][4];
	/* Used to pull info about each instruction */
	int instruction = 0, RS = 0, RT = 0, RD = 0, opcode = 0, numOfLines = 0, i, j;
	char line[10];
	/* Used to calculate percent stats */
	float totalUses = 0, percent;

	/* Initialize the 2D array to 0 */
	for (i = 0; i < 32; i ++){
		for (j = 0; j < 4; j++){
			registers[i][j] = 0;
		}
	}

	/* Loop through each instruction, analyze the registers used and update stats */
	while(fgets(line, 11, stdin) != NULL){
		/* Validate each char in the input string */
		for(i = 2; i < 10; i++){
			if(line[i] >= '0' && line[i] <= '9')
				continue;
			if (line[i] >= 'a' && line[i] <= 'f')
				continue;
			if (line[i] >= 'A' && line[i] <= 'F')
				continue;
			/* char didn't checkout so exit with error */
			printf("%s", line);
			printf("%c\n", line[i]);
			printf("%d\n", numOfLines);
			exit(ERROR_REG);
		}

		/* Scan in hex string as integer into instruction */
		sscanf(line, "%10x", &instruction);

		numOfLines++;
		/* Check instruction type using opcode */
		opcode = instruction & 0xFC000000;
		opcode >>= 26;

		/* Check if the instruction is not R-type or I-type */
		if(opcode != 0){
			if(opcode == 2 || opcode == 3)
				continue;
		}

		/* Extract the register bits from the instruction */
		RS = instruction & 0x03E00000;
		RS >>= 21;
		RT = instruction & 0x001F0000;
		RT >>= 16;
		RD = instruction & 0x0000F800;
		RD >>= 11;

		/* Increment number of uses for the registers */
		registers[RS][0]++;
		registers[RT][0]++;
		if (opcode == 0)
			registers[RD][0]++;

		/* If the instruction is R-type, increment R-type usages */
		if (opcode == 0){
			registers[RS][1]++;
			registers[RT][1]++;
			registers[RD][1]++;
			totalUses += 3;
		}
		/* It's not R-type so it must be an I-type instruction */
		else{
			registers[RS][2]++;
			registers[RT][2]++;
			totalUses += 2;
		}
	}

	/* Empty file case */
	if (numOfLines <= 0)
		exit(ERROR_REG);

	/* Print label if required */
	if (label)
		printf("%-5s %4s %8s %7s %7s %8s\n", "REG", "USE", "R-TYPE", "I-TYPE", "J-TYPE", "PERCENT");
	/* Print stats for all of the labels */
	for(i = 0; i < 32; i++){
		percent = (registers[i][0] / totalUses) * 100;

		printf("%c%-4d %4d %8d %7d %7d %8.1f\n", '$', i, registers[i][0], registers[i][1], registers[i][2], registers[i][3], percent);
	}

	exit(EXIT_SUCCESS);

}

void immStats(int label){
	/* To hold all immediate values used */
	int immList[400];
	/* Used to pull info about each instruction */
	int instruction = 0, opcode = 0, immediate, numOfLines = 0, numValues = 0, i;
	char line[10];

	/* Loop through each instruction, analyze the registers used and update stats */
	while(fgets(line, 11, stdin) != NULL){
		if((feof(stdin)) != 0)
			break;
		/* Validate each char in the input string */
		for(i = 2; i < 10; i++){
			if(line[i] >= '0' && line[i] <= '9')
				continue;
			if (line[i] >= 'a' && line[i] <= 'f')
				continue;
			if (line[i] >= 'A' && line[i] <= 'F')
				continue;
			/* char didn't checkout so exit with error */
			exit(ERROR_IMMEDIATE);
		}

		/* Scan in hex string as integer into instruction */
		sscanf(line, "%10x", &instruction);
		numOfLines++;

		opcode = instruction & 0xFC000000;
		opcode >>= 26;

		/* skip instruction if it's R-type */
		if(opcode == 0)
			continue;
		/* Check and process if J-type */
		else if(opcode == 2 || opcode == 3){
			immediate = instruction & 0x03FFFFFF;
			immList[numValues] = immediate;
			numValues++;
			continue;
		/* Else it must be I-type */
		}else{
			immediate = instruction & 0x0000FFFF;
			immList[numValues] = immediate;
			numValues++;
			continue;
		}
	}

	/* Empty file case */
	if (numOfLines <= 0)
		exit(ERROR_IMMEDIATE);

	if(label)
		printf("%s\n", "IMMEDIATE-VALUE");

	/* Loop through and print our list of immediates */
	for (i = 0; i <= numValues; i++){
		printf("%s%x\n", "0x", immList[i]);
	}

	exit(EXIT_SUCCESS);
}