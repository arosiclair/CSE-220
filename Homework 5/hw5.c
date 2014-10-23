/*
Homework #5
Name: Andrew Rosiclair
SBU ID: 109235970
*/

#include <stdlib.h>
#include <stdio.h>

/* Function prototypes */
int promptForId(void);
int scrambleId(int id);
int xor(int v, int id);
int shift(int v, int n);
int printResults(int original, int scrambled);

/* Ask the user for their SBUID */
int promptForId(void) {
	int id, valid;
	char line[256];
	
	/* Loop until valid input is given */
	do {
		printf("Please enter your SBUID: ");
		/* Read in the whole line */
		fgets(line, sizeof(line), stdin);
		/* If we were not able to parse the string, tell the user */
		if(!(valid = sscanf(line, "%d", &id))) {
			printf("Invalid id entered.\n");
		}
	} while(!valid);
	return id;
}

/* Scramble the SBUID */
int scrambleId(int id) {
	int scrambled = id, i, r, rounds;
	/* Seeds and generates a random number */
	srand(id);
	r = rand();
	/* Bound this random number between 1-10 */
	rounds = (rand() % 10) + 1;
	printf("Performing %d rounds of shifting and xoring.\n", rounds);
	/* Go through some crazy shifting operations */
	for(i = 0; i < rounds; i++);
		scrambled = shift(scrambled, r >>= 1);
	return scrambled;
}

/* Perform bitwise XOR on the SBU ID */
int xor(int v, int id) {
	int result;
	result = v ^ id;
	return result;
}

/* Shift the value right and xor */
int shift(int v, int r) {
	int shamt = 0x08000000;
	int result;
	/* while shamt is still not zero */
	while(shamt) {
		result += xor(v, r); /* add the return value of xor to result */
		shamt >>= 1;	 /* Shift right by 1 */
	}
	return result;
}

int printResults(int original, int scrambled) {
	int isEven;

	/* If the scrambled number is even */
	printf("%d -> %d\n", original, scrambled);

	isEven = scrambled % 2; 
	if((isEven = 0))
		printf("The scrambled result is even!\n");
		return 0;
	/* else it is odd */
	printf("The scrambled result is odd!\n");
	return 127;
}

/* The program starts here */
int main() {
	int id, scrambled, result;
	id = promptForId();
	printf("SBUID: %d\n", id);
	/* Scramble the SBUID */
	scrambled = scrambleId(id);
	/* Print out the scrambled value */
	result = printResults(id, scrambled);

	return result;
}

/* @@GDB_OUTPUT_HERE@@

Reading symbols from hw5...done.
(gdb) clear
No source file specified.
(gdb) break 22
Breakpoint 1 at 0x10c08: file CSE220/hw5.c, line 22.
(gdb) break 40
Breakpoint 2 at 0x10d18: file CSE220/hw5.c, line 40.
(gdb) break 47
Breakpoint 3 at 0x10d54: file CSE220/hw5.c, line 47.
(gdb) run
Starting program: /export/home2/a/r/arosicla/hw5 
[Thread debugging using libthread_db enabled]
[New Thread 1 (LWP 1)]
Please enter your SBUID: 109235970
[Switching to Thread 1 (LWP 1)]

Breakpoint 1, promptForId () at CSE220/hw5.c:22
22                      if(!(valid = sscanf(line, "%d", &id))) {
(gdb) print line
$1 = "109235970\n", '\000' <repeats 46 times>, "\377\377\377\377\377\377\377\377\377:\000\000\377\277\373\320\377:@L\000\000\000\f\000\001\000\064\377\277\377\331\377\377\377\377\000\000\000\000\377\277\374\274\000\000\000\002\377\377\377\377\377\377\377\377\377\277\373\330\377:X\350\000\000\000\000\000\000\000\000\377\277\375\b\000\000\000\000\000\000\000\001\000\001\005\v\377\067\np\377 \267\200\000\001\005\v\377\067\004 \377\067\004 \000\000\000\002\006\212\317\004\000\000\000\021\000\001\003L\025\001\377\377\000\001\000\000\000\000\000\000\000\000\000\002\377\277\373\320"...
(gdb) continue
Continuing.
SBUID: 109235970
Performing 5 rounds of shifting and xoring.

Breakpoint 2, scrambleId (id=109235970) at CSE220/hw5.c:40
40                      scrambled = shift(scrambled, r >>= 1);
(gdb) info frame
Stack level 0, frame at 0xffbffbe8:
 pc = 0x10d18 in scrambleId (CSE220/hw5.c:40); saved pc = 0x10e80
 called by frame at 0xffbffc58
 source language c.
 Arglist at 0xffbffbe8, args: id=109235970
 Locals at 0xffbffbe8, Previous frame's sp in fp
 Saved registers:
  l0 at 0xffbffbe8, l1 at 0xffbffbec, l2 at 0xffbffbf0, l3 at 0xffbffbf4,
  l4 at 0xffbffbf8, l5 at 0xffbffbfc, l6 at 0xffbffc00, l7 at 0xffbffc04,
  i0 at 0xffbffc08, i1 at 0xffbffc0c, i2 at 0xffbffc10, i3 at 0xffbffc14,
  i4 at 0xffbffc18, i5 at 0xffbffc1c, fp at 0xffbffc20, i7 at 0xffbffc24
(gdb) clear xor
Deleted breakpoint 3 
(gdb) continue
Continuing.
109235970 -> -1249671751
[Inferior 1 (process 20408    ) exited normally]


@@GDB_OUTPUT_HERE@@ */