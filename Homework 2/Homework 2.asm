# Homework #2
# Name: Andrew Rosiclair
# SBU ID: 109235970

.text
.globl main
main:
	#-------------------------------------PROMPT THE USER----------------------------------------
	la $a0,prompt		#load and print prompt.	
	li $v0,4
	syscall
	
	li $v0,8		#Read the input
	la $a0,file		#Save the directory in the "file" label
	li $a1,65		#Accepting max 64 characters for no particular reason.
	syscall			
	
	li $s0,0		#Use $s0 as a counter and $s1 as the number of max bytes for null removal process.
	li $s1,32
	j removeNull
	
openFile:
	#-------------------------------------OPEN THE FILE------------------------------------------
	li $v0,13		#Open the file
	la $a0,file		#Use the entered directory		
	li $a1,0		#Set flag to read
	li $a2,0		#Ignore mode	
	syscall			#The result of opening the file is stored in $v0
	
	bltz $v0,main		#If the file failed to open, try again.
	move $s6, $v0		#Load $s7 with the file descriptor for later.
	
	j checkBOM		#Continue to check the Byte Order Marker

removeNull:
		
	lb $t6,file($s0)	#Load byte at index
	addi $s0,$s0,1		#Increment the index
	bnez $t6,removeNull	#Repeat until we reach the end of the string.
	beq $s0,$s1,openFile 	#If the length of the directory is equal to 32 characters skip to file load.
	addiu $s0,$s0,-2
	sb	$0,file($s0)

	j openFile		#Now attempt to open the file.
	
checkBOM:
	
	li $t2,0xBFBBEF		#The BOM byte for UTF-8 encoding in little endian order
	li $v0,14 		#Prepare to read the BOM bytes
	move $a0, $s6		#Load file descriptor
	la $a1,bom		#We'll save the bytes in the bom space
	li $a2,3		#We'll only accept 3 bytes in order to verify a BOM for a UTF-8 encoding.
	syscall
	bltz $v0,byteReadError	#If $v0 is negative, reading the byte failed.
	
	la $s2,bom		#Save the address to $s2
	lw $t3,0($s2)		#load the BOM word from $s2 to $t3
	
	bne $t2,$t3,notUTF	#check if the read bytes are equivalent to the required BOM. Quit if not.
	j processFirstByte
	
processFirstByte:
	
	li $v0,14		#Prepare to read the first byte of a char/glyph
	move $a0,$s6		#Load the file descriptor
	la $a1,char		#Save the byte in our char space
	li $a2,1		#Only read 1 byte
	syscall
	bltz $v0,byteReadError	#Quit if an error occured
	beqz $v0,finish		#Print a finish message and quit.
	
	la $s2,char
	lbu $t3,0($s2)		#The byte should be loaded into $t3
	andi $t4,$t3,10000000	#Set $t4 to a copy of only $t3's first bit.

	bnez $t4,processMoreBytes	#The first bit is 1 so process more than 1 byte.
	
	#If we haven't branched, the character is 1 byte and can be converted directly into hex and a codepoint.
	li $v0,4		#Print the one byte label
	la $a0,oneByte
	syscall
	li $v0,34		#Print the 1 byte in hex
	la $a0,($t3)
	syscall
	li $v0,4		#Tab over
	la $a0,tab
	syscall
	la $a0,cpHeader		#Print the code point header, "U+"
	syscall
	li $v0,34		#print the byte in hex again after CP header.
	la $a0,($t3)
	syscall
	
	j processFirstByte	#Read the next byte.
	
processMoreBytes

	li $t5,00000110
	li $t6,00001110
	li $t7,00011110
	srl $t4,$t3,5		#Shift the bits to leave just the first 3 bits
	beq $t4,$t5,processTwoB	#branch and process based on how many 1's the byte header has
	
	
byteReadError:

	li $v0,4
	la $a0,byteError
	syscall
	j quit
		
notUTF:

	li $v0,4
	la $a0,notUTFBOM
	syscall	
	j quit

finish:
	li $v0,4
	la $a0,finishStr
	syscall
	j quit

quit:

	li $v0,10
	syscall
	
.data
	prompt: .asciiz "Enter the name/directory of the file encoded in UTF-8: \n"
	oneByte: .asciiz "\n1 byte:\t"
	twoBytes: .asciiz "\n2 bytes:\t"
	threeBytes: .asciiz "\n3 bytes:\t"
	tab: .asciiz "\t"
	cpHeader: .asciiz "U+"
	notUTFBOM: .asciiz "The entered file does not have a UTF-8 Byte Order Marking"
	byteError: .asciiz "There was an error reading in the next byte(s)"
	finishStr: .asciiz "\nFinshed reading the file. Quitting..."
	.align 2
	file: .space 64
	.align 2
	bom: .space 3
	.align 2
	char: .space 1
	
