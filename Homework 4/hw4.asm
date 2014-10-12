#Homework 4
#Name: Andrew Rosiclair
#SBU ID: 109235970

.data
	wrongBase: .asciiz "\nInvalid base!\n"

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
	
	#STACK HEADER
	addi $sp,$sp,-4
	sw $a0,($sp)
	
	div $a0,$a1	#The number is greater than the base so divide it by the base
	mflo $a0
	jal printBase	#Recursively call with new quotient as number
	
	#RESTORE $a0
	lw $a0,($sp)
	addi $sp,$sp,4
	
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
	
	#Base case, empty strings
	beqz $a1,returnLenT
	beqz $a3,returnLenS
	
	#STACK HEADER
	addi $sp,$sp,-28
	sw $ra,($sp)
	sw $s0,4($sp)	#s0 is "match"
	sw $s1,8($sp)	#s1 saves $a1
	sw $s2,12($sp)	#s2 saves $a3
	sw $s3,16($sp)	#First int
	sw $s4,20($sp)	#Second int
	sw $s5,24($sp)	#Third int
	
	move $s1,$a1	#Save length args
	move $s2,$a3
	
	
	
	#CHECK IF LAST CHARS ARE EQUAL
	addi $t0,$a1,-1	#Set $t0 to index of the last char for string S
	add $t1,$t0,$a0	#Set $t1 to the last char's mem address.
	lb $t2,($t1)
	addi $t0,$a3,-1 #Set $t0 to index of the last char for string T
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
	jr $ra

returnLenS:

	move $v0,$a1
	jr $ra

#Recursively calls levenshteinDistance and uses the minimum function
minCall:
	
	#USE LEVENSHTEIN TO GET 3 DIFFERENT INTS $s3-$s5
	#$s3
	addi $a1,$a1,-1		#Decrement first string length as required by algorithm
	jal levenshteinDistance
	move $s3,$v0		#Save result into $s3
	addi $s1,$s1,1		#Increment result as required by algorithm
	
	#$s4
	move $a1,$s1		#Restore length values
	move $a3,$s2
	addi $a3,$a3,-1		#Decrement 2nd string length as required by algorithm
	jal levenshteinDistance
	move $s4,$v0		#Save result to $s4
	addi $s2,$s2,1		#Increment result as required by algorithm
	
	#$s5
	move $a1,$s1		#Restore length values
	move $a3,$s2 
	addi $a1,$a1,-1		#Decrement both lengths as required by the algorithm
	addi $a3,$a3,-1
	jal levenshteinDistance
	move $s5,$v0		#Save result to #s5
	add $s5,$s5,$s0		#Add the result with "match" as required by algorithm
	
	move $a0,$s3		#Move our ints into appropriate args
	move $a1,$s4
	move $a2,$s5
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
	addi $sp,$sp,28
	jr $ra		#Return to caller

#Compares three int args, returns the minimum
#Args:	$a0 = int 1
#		$a1 = int 2
#		$a2 = int 3
#Returns:	$v0 = The min integer
minimum:
	
	blt $a0,$a1,compIntOne	#Compare the first integer with the third if it's lesser
	j	compInt2			#Compare the second
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
	lb $t2,($t0)
	li $t3,'\n'
	beq $t2,$t3,done	#Check if the byte we just loaded was the newline character
	addi $t1,$t1,1		#Increment counter
	j strlenloop

done: 

	move $v0,$t1	#Return our counter
	jr $ra	
	
	
	