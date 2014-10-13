#Homework 4
#Name: Andrew Rosiclair
#SBU ID: 109235970

.data
	wrongBase: .asciiz "\nInvalid base!\n"
	.align 2
	m1: .space 100
	.align 2
	m2: .space 100
	.align 2
	result: .space 100
	.align 0
	ii: .byte 0
	.align 0
	jj: .byte 0
	.align 0
	kk: .byte 0
.text

#Prints the remainder of a dividend and divisor
# Args: $a0 = dividend
#		$a1 = divisor
# Returns:	$v0 = remainder
remainder:
	
	bge $a0,$a1,remainderElse	#If the dividend is greater, branch
	la $v0,($a0)		#The dividend is less than the divisor so return it
	jr $ra			#Return to caller
remainderElse:
	sub $a0,$a0,$a1	#We set a new dividend to the difference between the old and the divisor
	j remainder	#Call remainder again until the dividend is less than the divisor

#Prints a number in different bases
#Args:	$a0 = Number to convert
#		$a1 = Base to convert it to
#Returns: N/A
printBase:

	#STACK HEADER
	addi $sp,$sp,-8
	sw $ra,($sp)
	sw $s0,4($sp)
	
	li $t0,2
	li $t1,9
	blt $a1,$t0,invalidBase	#Print an error message if base is out of range 
	bgt $a1,$t1,invalidBase

	bgt $a0,$a1,printBaseIf	#Branch if the number is greater than the base
	
	j printBaseElse	
invalidBase:
	li $v0,4
	la $a0,wrongBase
	syscall
	
	jr $ra		#Return to caller
printBaseIf:
	move $s0,$a0	#Save $a0
	
	div $a0,$a1	#The number is greater than the base so divide it by the base
	mflo $a0
	jal printBase	#Recursively call with new quotient as number
	
	move $a0,$s0	#Restore $a0
	
	j printBaseElse #After the above returns, continue to the rest of the function	
printBaseElse:
	jal remainder
	#Print the returned value
	move $a0,$v0
	li $v0,1
	syscall
	
	#RESTORE REGISTERS AND $SP
	lw $ra,($sp)
	lw $s0,4($sp)
	addi $sp,$sp,8
	jr $ra 		#Return to caller

#Computes the edit distance between two strings. The edit distance
#means the minimum number of single character changes to make the
#two strings the same.
#Args:	$a0 = String S
#		$a1 = Length of String S
#		$a2 = String T
#		$a3 = Length of String T
#Returns:	$v0 = Integer distance
levenshteinDistance:
	
	#STACK HEADER
	addi $sp,$sp,-36
	sw $ra,($sp)
	sw $s0,4($sp)	#s0 is "match"
	sw $s1,8($sp)	#s1 saves $a0
	sw $s2,12($sp)	#s2 saves $a1
	sw $s3,16($sp)	#s3 saves $a2
	sw $s4,20($sp)	#s4 saves $a3
	sw $s5,24($sp)	#First int
	sw $s6,28($sp)	#Second int
	sw $s7,32($sp)	#Third int
	
	
	move $s1,$a0	#Save args
	move $s2,$a1
	move $s3,$a2
	move $s4,$a3
	
	#Base case, empty strings
	beqz $a1,returnLenT
	beqz $a3,returnLenS
	
	
	#CHECK IF LAST CHARS ARE EQUAL
	addi $t0,$a1,-2	#Set $t0 to index of the last char for string S
	add $t1,$t0,$a0	#Set $t1 to the last char's mem address.
	lb $t2,($t1)
	addi $t0,$a3,-2 #Set $t0 to index of the last char for string T
	add $t1,$t0,$a2	#Set $t1 to the last char's mem address
	lb $t3,($t1)
	
	bne $t2,$t3,notEqual
	li $s0,0	#Set "match" to 0
	
	j minCall
notEqual:	
	li $s0,1	#Set "match" ($s0) to 1
	j minCall	
returnLenT:
	move $v0,$a3	
	#RESTORE REGISTERS AND $SP
	lw $ra,($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	lw $s3,16($sp)
	lw $s4,20($sp)
	lw $s5,24($sp)
	lw $s6,28($sp)
	lw $s7,32($sp)
	addi $sp,$sp,36
	jr $ra		#Return to caller
returnLenS:
	move $v0,$a1
	#RESTORE REGISTERS AND $SP
	lw $ra,($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	lw $s3,16($sp)
	lw $s4,20($sp)
	lw $s5,24($sp)
	lw $s6,28($sp)
	lw $s7,32($sp)
	addi $sp,$sp,36
	jr $ra		#Return to caller
#Recursively calls levenshteinDistance and uses the minimum function
minCall:
	#USE LEVENSHTEIN TO GET 3 DIFFERENT INTS $s5-$s7
	#$s5
	addi $a1,$a1,-1		#Decrement first string length as required by algorithm
	jal levenshteinDistance
	move $s5,$v0		#Save result into $s5
	addi $s5,$s5,1		#Increment result as required by algorithm
	
	#$s6
	move $a0,$s1		#Restore args
	move $a1,$s2
	move $a2,$s3
	move $a3,$s4
	addi $a3,$a3,-1		#Decrement 2nd string length as required by algorithm
	jal levenshteinDistance
	move $s6,$v0		#Save result to $s6
	addi $s6,$s6,1		#Increment result as required by algorithm
	
	#$s7
	move $a0,$s1		#Restore args
	move $a1,$s2
	move $a2,$s3
	move $a3,$s4
	addi $a1,$a1,-1		#Decrement both lengths as required by the algorithm
	addi $a3,$a3,-1
	jal levenshteinDistance
	move $s7,$v0		#Save result to #s5
	add $s7,$s7,$s0		#Add the result with "match" as required by algorithm
	
	
	move $a0,$s5		#Move our ints into appropriate args
	move $a1,$s6
	move $a2,$s7
	jal minimum			#Call minmum
	#v0 now has the return value
	
	#RESTORE REGISTERS AND $SP
	lw $ra,($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	lw $s3,16($sp)
	lw $s4,20($sp)
	lw $s5,24($sp)
	lw $s6,28($sp)
	lw $s7,32($sp)
	addi $sp,$sp,36
	jr $ra		#Return to caller

#Compares three int args, returns the minimum
#Args:	$a0 = int 1
#		$a1 = int 2
#		$a2 = int 3
#Returns:	$v0 = The min integer
minimum:
	
	blt $a0,$a1,compIntOne	#Compare the first integer with the third if it's lesser
	j	compIntTwo			#Compare the second with the third
compIntOne:
	blt $a0,$a2,returnIntOne	#int 1 is the least
	j returnIntThree			#int 3 is the least
compIntTwo:
	blt $a1,$a2,returnIntTwo	#int 2 is the least
	j returnIntThree			#int 3 is the least
returnIntOne:
	move $v0,$a0
	jr $ra
returnIntTwo:
	move $v0,$a1
	jr $ra
returnIntThree:
	move $v0,$a2
	jr $ra

#Computes the length of a string
#Args: 	$a0 = string
#Returns:	$v0 = length of the string
strlen:

	li $t1,0		#We use $t1 as a counter.
	j strlenloop	#Start a loop to count the length	
strlenloop:
	add $t0,$t1,$a0	#$t0 is the effective memory address of the counter
	addi $t1,$t1,1		#Increment counter
	lb $t2,($t0)
	li $t3,'\n'
	beq $t2,$t3,end	#Check if the byte we just loaded was the newline character
	j strlenloop
end: 
	move $v0,$t1	#Return our counter
	jr $ra

#Prompts the user for a 100 character input and verfies 
#that it is 100 characters
#Args:	$a0 = Memory address of char array
#		$a1 = Memory address of a prompt
#		$a2 = Memory address of error prompt
#Returns:	N/A
rd100Chars:	
	
	#STACK HEADER
	addi $sp,$sp,-16
	sw $ra,($sp)
	sw $s0,4($sp)
	sw $s1,8($sp)
	sw $s2,12($sp)
	
	
	move $s0,$a0	#Save args
	move $s1,$a1
	move $s2,$a2
	
	j printPrompt
printPrompt:
	li $v0,4		#Print prompt
	la $a0,($s1)
	syscall
	li $v0,8		#Read input, max 100 characters
	la $a0,($s0)
	li $a1,101
	syscall
	
	#Check last char
	addi $t0,$s0,99	#Effective mem address of last possible char
	lb $t1,($t0)
	beqz $t1,invalidInput	#There's nothing in the last char so we assume it is not 100 chars
	
	#RESTORE REGISTERS AND $SP
	lw $ra,($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	addi $sp,$sp,16
	jr $ra			#Return to caller
invalidInput:
	li $v0,4	#Print the error prompt
	la $a0,($s2)
	syscall
	
	j printPrompt #Reprompt for string and try again.

#Prints the values stored in arr as a 5x5 matrix of integers
#stored in row major order.
#Args:	$a0 = Memory address of the array
#Returns:	N/A

printIntMatrix:

	addi $sp,$sp,-20
	sw $ra,($sp)
	sw $s0,4($sp)
	sw $s1,8($sp)
	sw $s2,12($sp)
	sw $s3,16($sp)
	
	move $s0,$a0	#Save the arg
	li $s1,0		#$s1 will be used as an index
	li $s2,0		#$s2 will be used as a column counter
	li $s3,0		#$s3 will be used as a row counter
	
	j matrixLoop
matrixLoop:
	li $t0,5
	beq $s2,$t0,nextRow	#Check if we just finished a row

	add $t0,$s1,$s0		#$t0 has effective mem address of the next word.
	lw $t1,($t0)		#$t1 has the next word
	li $v0,1			#Print the word
	la $a0,($t1)
	syscall
	li $v0,11			#Print a space
	li $a0,' '
	syscall
	
	addi $s2,$s2,1		#Increment our column counter
	addi $s1,$s1,4		#Move our index over to the next word
	j matrixLoop
nextRow:
	li $s2,0	#Reset column counter
	addi $s3,$s3,1	#Increment row counter
	
	li $t0,5
	beq $s3,$t0,donePrinting	#Check if we just finished the last row
	
	li $v0,11	#Advance to a new line
	li $a0,'\n'
	syscall
	
	j matrixLoop	#Continue with the loop
donePrinting:
	#RESTORE REGISTERS AND $SP
	lw $ra,($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	lw $s3,16($sp)
	addi $sp,$sp,20
	jr $ra			#Return to caller

#Multiplies two given 2D array arguments. Stores the result in another
#given argument
#Args:	$a0 = Address of first array to multiply
#		$a1 = Address of second array to multiply
#		$a2 = Address of where to save the result
#Returns:	$v0 = N/A
matrixMult:
	
	
	sb $0,ii	#Set all of our index vars to 0
	sb $0,jj
	sb $0,kk
	
	j multiply	#jump to the recursive portion
multiply:
	li $t0,5
	lb $t1,ii
	bge $t1,$t0,return	#Check if index i is greater than or equal to 5
	
	lb $t1,jj
	blt $t1,$t0,ifJLess5	#branch  to this if statement
	
	#Execute if J is greater than or equal to 5
	sb $0,jj		#Set j to 0
	lb $t1,ii
	addi $t1,$t1,1	#Increment i
	sb $t1,ii
	
	j multiply	#Recursive call	
return:
	jr $ra	#return
ifJLess5:
	li $t0,5
	lb $t1,kk
	blt $t1,$t0,ifKLess5
	
	#Execute if K is greater than or equal to 5
	sb $0,kk		#Set k to 0
	lb $t0,jj		#Increment j
	addi $t0,$t0,1
	sb $t0,jj		
	
	j multiply
ifKLess5:
	#Calculate A[i][k] mem address
	lb $t0,ii
	li $t1,5
	mul $t2,$t0,$t1	#Multiply i by # of columns which is always 5
	lb $t3,kk
	add $t2,$t2,$t3	#Add the product with k
	li $t1,4
	mul $t2,$t2,$t1	#Multiply that sum by the size of a word, 4 bytes
	add $t2,$t2,$a0	#Set $t2 to the effective memory address of the entry A[i][k]
	
	#Calculate B[k][j] mem address
	lb $t0,kk
	li $t1,5
	mul $t4,$t0,$t1	#multiply k by # of columns
	lb $t3,jj
	add $t4,$t4,$t3	#add the product with j
	li $t1,4
	mul $t4,$t4,$t1 #Multiply the sum by 4 bytes
	add $t4,$t4,$a1	#Set $t4 to the effective mem address of entry B[k][j]
	
	#Calculate result[i][j] mem address
	lb $t0,ii
	li $t1,5
	mul $t6,$t0,$t1	#Multiply i with # of columns
	lb $t3,jj
	add $t6,$t6,$t3	#add the product with j
	li $t1,4
	mul $t6,$t6,$t1	#Multiply the sum by 4 bytes
	add $t6,$t6,$a2	#Set $t6 to effective mem address of entry result[i][j]
	
	lw $t1,($t2)	#Load entry A[i][k] into $t1
	lw $t3,($t4)	#Load entry B[k] into $t3
	mul $t0,$t1,$t3	#$t0 is the product of entries A[i][k] and B[k][j]
	lw $t1,($t6)	#Load entry result[i][j] into $t1
	addu $t1,$t1,$t0	#$t1 now holds the sum of its entry and the product
	sw $t1,($t6)	#Insert the new entry into result[i][j]
	
	lb $t0,kk		#Increment k
	addi $t0,$t0,1
	sb $t0,kk
	
	j multiply		#recursive call
	