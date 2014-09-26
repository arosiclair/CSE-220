# Homework #2
# Name: Andrew Rosiclair
# SBU ID: 109235970

.text
.globl main
main:
	#-------------------------------------PROMPT THE USER----------------------------------------
	la $a0,prompt		#load and print main menu prompt.	
	li $v0,4
	syscall
	
	li $v0,5		#Read input integer
	syscall
	move $s5,$v0	#Save the option we chose for later.
	
	li $t1,1		#Possible input values
	li $t2,2
	li $t3,3
	bltz $v0,main 	#Restart if the integer is neg
	bgt $v0,$t3,main 	#Restart if the integer is to large
	beqz $v0,promptForFile	#Prompt for a UTF-8 file
	beq $v0,$t1,promptForFile	#Prompt for either UTF-16BE/LE
	beq $v0,$t2,promptForFile
	beq $v0,$t3,quit	#Exit the program
	
promptForFile:

	la $a0,filePrompt		#load and print main menu prompt.	
	li $v0,4
	syscall
	
	li $v0,8		#Read the input
	la $a0,file		#Save the directory in the "file" label
	li $a1,101		#Accepting max 100 characters for no particular reason.
	syscall			
	
	li $s0,0		#Use $s0 as a counter and $s1 as the number of max bytes for null removal process.
	li $s1,100
	j removeFileNull
	
promptForOut:
	
	la $a0,outPrompt	#Prompt user for an output file directory
	li $v0,4		
	syscall
	
	li $v0,8		#Read the directory
	la $a0,out		#Save it in our "out" label
	li $a1,101
	syscall
	
	li $s0,0		#Use $s0 as a counter and $s1 as the number of max bytes for null removal process.
	li $s1,100
	j removeOutNull
	
openFile:

	li $v0,13		#Open the file
	la $a0,file		#Use the entered file directory		
	li $a1,0		#Set flag to read
	li $a2,0		#Ignore mode	
	syscall			#The result of opening the file is stored in $v0
	bltz $v0,promptForFile		#If the file failed to open, try again.
	move $s6, $v0		#Load $s6 with the file descriptor for later.
	
	j checkBOM		#Continue to check the Byte Order Marker
	
	
	
openFileWrite:
	
	li $v0,13		#Open the file
	la $a0,out		#Use the entered output directory		
	li $a1,1		#Set flag to write
	li $a2,0		#Ignore mode	
	syscall			#The result of opening the file for writing is stored in $v0
	bltz $v0,promptForOut		#If the file failed to open, try again.
	move $s7, $v0		#Load $s7 with the output file descriptor for later.
	
	j addBOM		#Begin process by writing the appropriate BOM for UTF-16
	
	
removeFileNull:
	#Remove the null character at the end of the file directory.	
	lb $t6,file($s0)	#Load byte at index
	addi $s0,$s0,1		#Increment the index
	bnez $t6,removeFileNull	#Repeat until we reach the end of the string.
	beq $s0,$s1,openFile 	#If the length of the directory is equal to 100 characters skip to file load.
	addiu $s0,$s0,-2
	sb $0,file($s0)		#replace the last bit with 0.

	j openFile
	
removeOutNull:
	#Remove the null character at the end of the output directory.
	lb $t6,out($s0)	#Load byte at index
	addi $s0,$s0,1		#Increment the index
	bnez $t6,removeOutNull	#Repeat until we reach the end of the string.
	beq $s0,$s1,openFileWrite 	#If the length of the directory is equal to 100 characters skip to opening file for write.
	addiu $s0,$s0,-2
	sb $0,out($s0)		#replace the last bit with 0.

	j openFileWrite
checkBOM:
	
	li $t2,0xBFBBEF	#The BOM byte for UTF-8 encoding in little endian order
	li $v0,14 		#Prepare to read the BOM bytes
	move $a0, $s6	#Load file descriptor
	la $a1,bom		#We'll save the bytes in the bom space
	li $a2,3		#We'll accept 3 bytes in order to verify a BOM for a UTF-8 encoding.
	syscall
	bltz $v0,byteReadError	#If $v0 is negative, reading the byte failed.
	
	la $s2,bom		#Save the address to $s2
	lw $t3,0($s2)	#load the BOM word from $s2 to $t3
	
	bne $t2,$t3,notUTF	#check if the read bytes are equivalent to the required BOM. Quit if not.
	
	bnez $s5,promptForOut	#Prompt for an output directory if option 2 or 3 was selected.
	j processFirstByte
	
addBOM:

	li $t0,1	#Check if the user wants to encode in LE or BE
	beq $s5,$t0,addBEBOM
	
	li $t0,0xFF
	la $t1,BOMBuffer	#Save address of buffer 
	sb $t0,($t1)	#store the BOM value into the address of buffer
	
	li $v0,15		#Load the LE BOM by default
	la $a0,($s7)	#Load output file descriptor
	la $a1,($t1)	#Load byte from address of our BOM buffer
	li $a2,1		#The BOM is 1 bytes.
	syscall
	bltz $v0,byteWriteError	#Quit if there is a problem.
	
	li $t0,0xFE		#Save the second BOM byte
	sb $t0,($t1)	#Store the byte in address of buffer.
	syscall
	bltz $v0,byteWriteError	#Quit if there is a problem.
	
	j processFirstByte

addBEBOM:

	li $t0,0xFE		#Save the first BOM byte
	la $t1,BOMBuffer	#Save address of buffer 
	sb $t0,($t1)	#store the BOM value into the address of buffer
	
	li $v0,15		#Write the LE BOM by default
	la $a0,($s7)	#Load output file descriptor
	la $a1,($t1)	#Load byte from address of our BOM buffer
	li $a2,1		#The BOM is 1 bytes.
	syscall
	bltz $v0,byteWriteError	#Quit if there is a problem.
	
	li $t0,0xFF		#Save the second BOM byte
	sb $t0,($t1)	#Store the byte in address of buffer.
	syscall
	bltz $v0,byteWriteError	#Quit if there is a problem.
	
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
	
	bnez $s5,convert1B	#If we wanted to encode the file into an output, branch.
	j processFirstByte	#Read the next byte.
	
processMoreBytes:

	li $t5,0x00000006		#Possible Byte headers for Unicode bytes.
	li $t6,0x0000000E
	li $t7,0x0000001E
	srl $t4,$t3,5		#Shift the bits to leave just the first 3 bits
	beq $t4,$t5,process2B	#branch and process based on how many 1's the byte header has
	srl $t4,$t3,4
	beq $t4,$t6,process3B 
	srl $t4,$t3,3
	beq $t4,$t7,process4B
	
	j byteReadError		#If we haven't matched the header bits with an expected result we quit the program with an error messsage
	
process2B:
	
	li $v0,14		#Read the second byte
	move $a0,$s6
	la $a1,char		#Next byte is stored at our char address.
	li $a2,1
	syscall
	bltz $v0,byteReadError
	
	la $t5,char
	lbu $t4,0($t5)	#Load the second byte into $t4
	andi $t3,$t3,0x0000001F	#Clear the header bits for the first byte
	andi $t4,$t4,0x0000003F	#Clear the header bits for the second byte
	
	move $t3,$s0		#Save these bytes for conversion
	move $t4,$s1
	
	li $v0,4		#Print the label and the hex representations for each byte.
	la $a0,twoBytes
	syscall
	li $v0,34		#Print first byte in hex
	la $a0,($t3)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,34		#Print second byte in hex
	la $a0,($t4)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	
	sll $t3,$t3,6		#Shift the first byte's bits left to make room for the second's
	or $t3,$t3,$t4		#insert the second set of bits into the first and save to $t3
	
	la $a0,cpHeader		#Print the Codepoint header and the hex value
	syscall
	li $v0,34
	la $a0,($t3)
	syscall
	
	bnez $s5,convert2B
	j processFirstByte	#Return to process the next char.
	
process3B:

	li $v0,14		#Read the second byte
	move $a0,$s6
	la $a1,char		#Next byte is stored at our char address.
	li $a2,1
	syscall
	bltz $v0,byteReadError
	la $t5,char
	lbu $t4,0($t5)	#Load the second byte into $t4
	
	li $v0,14		#Read the third byte
	move $a0,$s6
	la $a1,char		#Next byte is third at our char address.
	li $a2,1
	syscall
	bltz $v0,byteReadError
	la $t5,char
	lbu $t5,0($t5)	#Load the third byte into $t5
	
	andi $t3,$t3,0x0000001F	#Clear the header bits for the first byte
	andi $t4,$t4,0x0000003F	#Clear the header bits for the second byte
	andi $t5,$t5,0x0000003F	#Clear the header bits for the third byte
	
	move $s0,$t3	#Save these bytes for conversion
	move $s1,$t4
	move $s2,$t5
	
	li $v0,4		#Print the label and the hex representations for each byte.
	la $a0,threeBytes
	syscall
	li $v0,34		#Print first byte in hex
	la $a0,($t3)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,34		#Print second byte in hex
	la $a0,($t4)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,34		#Print second third in hex
	la $a0,($t5)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	
	sll $t3,$t3,12		#Shift the first byte's bits left to make room for the second and third's
	sll $t4,$t4,6		#Shift the second byte's bits left to make room for the third's
	or $t3,$t3,$t4		#insert the second set of bits into the first and save to $t3
	or $t3,$t3,$t5		#insert the third set of bits into the first and seconds and save to $t3
	
	la $a0,cpHeader		#Print the Codepoint header and the hex value
	syscall
	li $v0,34
	la $a0,($t3)
	syscall
	
	j processFirstByte	#Return to process the next char.
	
process4B:

	li $v0,14		#Read the second byte
	move $a0,$s6
	la $a1,char		#Next byte is stored at our char address.
	li $a2,1
	syscall
	bltz $v0,byteReadError
	la $t5,char
	lbu $t4,0($t5)	#Load the second byte into $t4
	
	li $v0,14		#Read the third byte
	move $a0,$s6
	la $a1,char		#Next byte is third at our char address.
	li $a2,1
	syscall
	bltz $v0,byteReadError
	la $t5,char
	lbu $t5,0($t5)	#Load the third byte into $t5
	
	li $v0,14		#Read the fourth byte
	move $a0,$s6
	la $a1,char		#Next byte is stored at our char address.
	li $a2,1
	syscall
	bltz $v0,byteReadError
	la $t6,char
	lbu $t6,0($t6)	#Load the fourth byte into $t6
	
	andi $t3,$t3,0x0000001F	#Clear the header bits for the first byte
	andi $t4,$t4,0x0000003F	#Clear the header bits for the second byte
	andi $t5,$t5,0x0000003F	#Clear the header bits for the third byte
	andi $t6,$t6,0x0000003F	#Clear the header bits for the fourth byte
	
	li $v0,4		#Print the label and the hex representations for each byte.
	la $a0,threeBytes
	syscall
	li $v0,34		#Print first byte in hex
	la $a0,($t3)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,34		#Print second byte in hex
	la $a0,($t4)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,34		#Print second third in hex
	la $a0,($t5)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,34		#Print second fourth in hex
	la $a0,($t6)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	
	sll $t3,$t3,18		#Shift the first byte's bits left to make room for the second, third and fourth's
	sll $t4,$t4,12		#Shift the second byte's bits left to make room for the third's
	sll $t5,$t5,6		#Shift the third byte's bits left to make room for the fouth's
	or $t3,$t3,$t4		#insert the second set of bits into the first and save to $t3
	or $t3,$t3,$t5		#insert the third set of bits into the first and seconds and save to $t3
	or $t3,$t3,$t6		#insert the fourth set of bits into the first, second's and thirds and save to $t3
	
	la $a0,cpHeader		#Print the Codepoint header and the hex value
	syscall
	li $v0,34
	la $a0,($t3)
	syscall
	
	bnez $s5,convert4B
	j processFirstByte	#Return to process the next char.
	
convert1B:

	li $t0,1	#Check if the user wants to encode in LE or BE
	beq $s5,$t0,convert1BBE
	
	la $t0,sixteen1	#Load $t0 with the address of our UTF16 byte buffer
	sb $t3,($t0)	#Store the codepoint value into the address of our UTF16 byte buffer, $t0
	
	li $v0,15	#We are writing a code unit with only 1 unicode byte in LE so we'll insert that byte first
	la $a0,($s7)
	la $a1,($t0)
	li $a2,1
	syscall
	bltz $v0,byteWriteError
	
	la $a1,($0)	#We'll add a 0 byte to make it UTF-16LE
	syscall
	
	j processFirstByte	#Go back and process the next char
	
convert1BBE:
	la $t0,sixteen1
	sb $0,($t0)
	li $v0,15	#We are writing a code unit with only 1 unicode byte in BE so we'll insert a 0 byte first
	la $a0,($s7)
	la $a1,($t0)
	li $a2,1
	syscall
	bltz $v0,byteWriteError
	
	sb $t3,($t0)	#Store the codepoint value into the address of our UTF16 byte buffer, $t0
	syscall
	bltz $v0,byteWriteError
	
	j processFirstByte	#Go back and process the next char
	
convert2B:
	
	li $t0,1	#Check if the user wants to encode in LE or BE
	beq $s5,$t0,convert2BBE
	
	la $t0,sixteen1
	sb $s1,($t0)
	li $v0,15	#We are writing a code unit with only 2 unicode bytes in LE so we'll insert the last byte first
	la $a0,($s7)
	la $a1,($t0)
	li $a2,1
	syscall		#Write the second byte first
	bltz $v0,byteWriteError
	sb $s1,($t0)	
	syscall		#Write the first byte last.
	bltz $v0,byteWriteError
	
	la $a1,($0)
	syscall		#Write the 0 byte afterwards.
	
	j processFirstByte	#Jump back to process the next unicode char
	
convert2BBE:

	la $t0,sixteen1
	sb $0,($t0)
	li $v0,15	#We are writing a code unit with only 2 unicode bytes in BE so we'll insert a 0 byte first
	la $a0,($s7)
	la $a1,($t0)
	li $a2,1
	syscall
	bltz $v0,byteWriteError
	
	#then we'll write the UTF-8 bytes in reverse order
	sb $s0,($t0)
	syscall		#Write the first bite first.
	bltz $v0,byteWriteError
	sb $s1,($t0)
	syscall		#Write the second byte
	bltz $v0,byteWriteError
	
	j processFirstByte #Jump back to process the next unicode char.

convert3B:

	#Create surrogate pairs
	li $t0,0x10000
	sub $t4,$t3,$t0		#subtract 0x10000 from our UTF-8 codepoint
	srl $t1,$t4,10		#Shift the result 10 bits to the right. Store it in $t1 as the higher bits.
	andi $t0,$t3,0x3FF	#AND the subtracted codepoint with 0x3FF and save it in $t0 as the lower bits.
	
	andi $t1,$t1,0xD800	#To create the first of the surrogate pair, AND the higher bits with 0xD800.
	andi $t0,$t0,0xDF21	#To create the second of the pair, AND the lower bits with 0xDF21.
	
	#This pair is 2 bytes each. Total = 4 bytes. So we have to write 4 bytes.
	andi $t3,$t1,0x000000FF	#Clear unwanted bits
	andi $t4,$t1,0x0000FF00
	andi $t5,$t0,0x000000FF
	andi $t6,$t0,0x0000FF00
	
	li $t0,1	#Check if the user wants to encode in LE or BE
	beq $s5,$t0,convert3BBE
	
	la $t0,sixteen1
	sb $t3,($t0)
	li $v0,15	#We are writing a code unit with only 2 unicode bytes in BE so we'll write the lesser byte of the first surrogate
	la $a0,($s7)
	la $a1,($t0)
	li $a2,1
	syscall
	bltz $v0,byteWriteError
	
	sb $t4,($t0)	#write the higher byte of the first surrogate
	syscall
	bltz $v0,byteWriteError
	sb $t5,($t0)	#Write lesser byte of the second surrogate
	syscall	
	bltz $v0,byteWriteError	
	sb $t6,($t0)	#Write the higher byte of the second surrogate
	syscall
	bltz $v0,byteWriteError
	
	la $a1,($0)		#Write in a 0 since this is LE
	syscall
	bltz $v0,byteWriteError
	
	j processFirstByte #Jump back to process the next unicode char.
	
convert3BBE:

	la $t0,sixteen1
	sb $0,($t0)
	li $v0,15	#We're writing in BE so insert a 0 first.
	la $a0,($s7)
	la $a1,($t0)
	li $a2,1
	syscall
	bltz $v0,byteWriteError
	
	sb $t4,($t0)	#Print the higher byte of the first surrogate
	syscall
	bltz $v0,byteWriteError
	sb $t3,($t0)	#Print lesser byte of the first surrogate
	syscall
	bltz $v0,byteWriteError
	sb $t6,($t0)	#Print the higher byte of the second surrogate
	syscall
	bltz $v0,byteWriteError
	sb $t5,($t0)	#Print lesser byte of the second surrogate
	syscall
	bltz $v0,byteWriteError
	
	j processFirstByte #Jump back to process the next unicode char.

convert4B:

	j convert3B	#Converting a 4 Byte UTF-8 code point is the same process as a 3 byte code point because they are both larger than 2 bytes.
	
byteReadError:

	li $v0,4
	la $a0,byteError
	syscall
	j quit
	
byteWriteError:

	li $v0,4
	la $a0,writeError
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
	la $a0,finishWrite
	syscall
	j main

quit:

	li $v0,10
	syscall
	
.data
	prompt: .asciiz "\nMAIN MENU\n0) Read a UTF-8 file and print code points\n1) Convert a UTF-8 file to UTF-16BE\n2) Convert a UTF-8 file to UTF-16LE\n3) Quit\n"
	filePrompt: .asciiz "Enter the name/directory of the file encoded in UTF-8: \n"
	outPrompt: .asciiz "Enter the name/directory of the file you want to create and encode in UTF-16: \n"
	oneByte: .asciiz "\n1 byte:\t"
	twoBytes: .asciiz "\n2 bytes:"
	threeBytes: .asciiz "\n3 bytes:"
	tab: .asciiz "\t"
	cpHeader: .asciiz "U+"
	notUTFBOM: .asciiz "\nThe entered file does not have a UTF-8 Byte Order Marking"
	byteError: .asciiz "\nThere was an error reading in the next byte(s)"
	writeError: .asciiz "\nThere was an error writing the next byte(s)"
	finishStr: .asciiz "\nFinshed reading the file."
	finishWrite: .asciiz "\nFinished writing new if selected. Returning to main menu...\n"
	.align 2
	file: .space 100
	.align 2
	out: .space 100
	.align 2
	bom: .space 3
	.align 2
	char: .space 1
	.align 2
	BOMBuffer: .space 1
	.align 2
	sixteen1: .space 1
	.align 2
	sixteen2: .space 1
	
