# Homework #2
# Name: Andrew Rosiclair
# SBU ID: 109235970

.text
.globl main
main:
	#-------------------------------------PROMPT THE USER----------------------------------------
	la $a0,prompt	#load and print prompt.	
	li $v0,4
	syscall
	
	li $v0,8		#Read the input
	la $a0,file		#Save the directory in the "file" label
	li $a1,33		#Accepting max 32 characters for no particular reason.
	syscall			
	
	li $s0,0		#Use $s0 as a counter and $s1 as the number of max bytes for null removal process.
	li $s1,32
	j removeNull
	
openFile:
	#-------------------------------------OPEN THE FILE------------------------------------------
	li $v0,13		#Open the file
	la $a0,file		#Use the entered directory		
	li $a1,1		#Set flag to read
	li $a2,0		#Ignore mode	
	syscall			#The result of opening the file is stored in $v0
	
	bltz $v0,main	#If the file failed to open, try again.
	add	$s6, $v0, $0	#Load $s7 with the file descriptor for later.
	
	j checkBOM		#Continue to check the Byte Order Marker

removeNull:
		
	lb $t6,file($s0)	#Load byte at index
	addi $s0,$s0,1	#Increment the index
	bnez $t6,removeNull	#Repeat until we reach the end of the string.
	beq $s0,$s1,openFile #If the length of the directory is equal to 32 characters skip to file load.
	addiu $s0,$s0,-2
	sb	$0,file($s0)

	j openFile		#Now attempt to open the file.
	
checkBOM:
	
	li $t2,0xEFBBBF		#The BOM byte for UTF-8 encoding
	li $v0,14 		#Prepare to read file 1 byte at a time
	add	$a0, $s6, $0	#Load file descriptor
	la $a1,bom		#We'll save the bytes in the char space
	li $a2,3		#We'll only accept 3 bytes in order to verify a BOM for a UTF-8 encoding.
	syscall
	bltz $v0,byteReadError	#If $v0 is negative, reading the byte failed.
	
	la $s2,bom		#Save the address to $s2
	lbu $t3,0($s2)	#load the byte from $s2 to $t3
	
	bne $t2,$t3,notUTF	#check if the read bytes are equivalent to the required BOM. Quit if not.
	j quit
	
processBytes:
	
	
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

quit:

	li $v0,10
	syscall
	
.data
	prompt: .asciiz "Enter the name/directory of the file encoded in UTF-8: \n"
	notUTFBOM: .asciiz "The entered file does not have a UTF-8 Byte Order Marking"
	byteError: .asciiz "There was an error reading in the next byte(s)"
	.align 2
	file: .space 32
	.align 2
	bom: .space 3
	.align 2
	char: .space 1
	